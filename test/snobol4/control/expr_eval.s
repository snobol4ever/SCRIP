                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Push_α
proc_LBL__Push_α:
                        .global          proc_LBL__Push_α
                        .global          proc_LBL__Push_β
                        .global          proc_LBL__Push_γ
                        .global          proc_LBL__Push_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              [rsp + 2536], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__Push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n10_var_α
n0_goto_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n11_lit_string_α
n1_goto_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n12_var_α
n2_goto_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n11_lit_string_α
n3_goto_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   n13_call_α
n4_goto_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_goto_α:
                                                                                        jmp   n11_lit_string_α
n5_goto_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
n6_goto_α:
                                                                                        jmp   n14_call_α
n6_goto_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
n7_goto_α:
                                                                                        jmp   n11_lit_string_α
n7_goto_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
n8_goto_α:
                                                                                        jmp   n15_var_α
n8_goto_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
n9_goto_α:
                                                                                        jmp   n16_lit_string_α
n9_goto_β:
                                                                                        jmp   proc_LBL__Push_ω
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                                                                                        jmp   n17_lit_integer_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_string_α:
                        mov              qword ptr [rbp + 1616], 1
                        mov              rax, qword ptr [rip + .Lx150_0]
                        mov              qword ptr [rbp + 1624], rax
                                                                                        jmp   n19_call_α
.Lx150_0:
                        .quad            .Lx150_0_s
.Lx150_0_s:
                        .string          "PAT$0"
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 496], rax
                        mov              qword ptr [rbp + 504], rdx
                                                                                        jmp   n20_var_α
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx153_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx153_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx153_6]
                        lea              rdx, [rip + .Lx153_7]
                                                                                        jmp   rax
.Lx153_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx153_2
.Lx153_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx153_2
.Lx153_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx153_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx153_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx153_3]
                        lea              rdx, [rip + .Lx153_4]
                                                                                        jmp   rax
.Lx153_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx153_2
.Lx153_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx153_2
.Lx153_1:
                        call             rt_faildescr@PLT
.Lx153_2:
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                        cmp              eax, 99
                                                                                        je    n23_call_α
                                                                                        jmp   n22_assign_α
n13_call_β:
                                                                                        jmp   n23_call_α
.Lx153_0:
                        .quad            .Lx153_0_s
.Lx153_0_s:
                        .string          "Pop"
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx155_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx155_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx155_6]
                        lea              rdx, [rip + .Lx155_7]
                                                                                        jmp   rax
.Lx155_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx155_2
.Lx155_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx155_2
.Lx155_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx155_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx155_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx155_3]
                        lea              rdx, [rip + .Lx155_4]
                                                                                        jmp   rax
.Lx155_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx155_2
.Lx155_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx155_2
.Lx155_1:
                        call             rt_faildescr@PLT
.Lx155_2:
                        mov              qword ptr [rbp + 1104], rax
                        mov              qword ptr [rbp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n25_call_α
                                                                                        jmp   n24_assign_α
n14_call_β:
                                                                                        jmp   n25_call_α
.Lx155_0:
                        .quad            .Lx155_0_s
.Lx155_0_s:
                        .string          "Pop"
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx156_0]
                        call             NV_GET_fn@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx156_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__Push_γ
.Lx156_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n26_assign_α
.Lx156_0:
                        .quad            .Lx156_0_s
.Lx156_0_s:
                        .string          "INPUT"
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx157_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n27_assign_α
.Lx157_0:
                        .quad            .Lx157_0_s
.Lx157_0_s:
                        .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        mov              qword ptr [rbp + 160], 6
                        mov              rax, qword ptr [rip + .Lx158_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n28_subscript_α
.Lx158_0:
                        .quad            0
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n18_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                                                                                        jmp   n29_var_α
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:
                        mov              rax, qword ptr [rbp + 1616]
                        mov              qword ptr [rbp + 1584], rax
                        mov              rax, qword ptr [rbp + 1624]
                        mov              qword ptr [rbp + 1592], rax
                        .section         .rodata
.Lrkfn161:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn161]
                        lea              rsi, [rbp + 1584]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1568], rax
                        mov              qword ptr [rbp + 1576], rdx
                        cmp              eax, 99
                                                                                        je    n32_lit_string_α
                                                                                        jmp   n31_assign_α
n19_call_β:
                                                                                        jmp   n32_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n20_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n33_lit_integer_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   n34_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n22_assign_α:
                        mov              rax, qword ptr [rbp + 768]
                        mov              rdx, qword ptr [rbp + 776]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n23_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx166_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx166_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx166_6]
                        lea              rdx, [rip + .Lx166_7]
                                                                                        jmp   rax
.Lx166_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx166_2
.Lx166_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx166_2
.Lx166_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx166_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx166_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx166_3]
                        lea              rdx, [rip + .Lx166_4]
                                                                                        jmp   rax
.Lx166_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx166_2
.Lx166_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx166_2
.Lx166_1:
                        call             rt_faildescr@PLT
.Lx166_2:
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n37_lit_string_α
                                                                                        jmp   n36_assign_α
n23_call_β:
                                                                                        jmp   n37_lit_string_α
.Lx166_0:
                        .quad            .Lx166_0_s
.Lx166_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_α:
                        mov              rax, qword ptr [rbp + 1104]
                        mov              rdx, qword ptr [rbp + 1112]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n25_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n25_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx169_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx169_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx169_6]
                        lea              rdx, [rip + .Lx169_7]
                                                                                        jmp   rax
.Lx169_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx169_2
.Lx169_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx169_2
.Lx169_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx169_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx169_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx169_3]
                        lea              rdx, [rip + .Lx169_4]
                                                                                        jmp   rax
.Lx169_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx169_2
.Lx169_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx169_2
.Lx169_1:
                        call             rt_faildescr@PLT
.Lx169_2:
                        mov              qword ptr [rbp + 1136], rax
                        mov              qword ptr [rbp + 1144], rdx
                        cmp              eax, 99
                                                                                        je    n39_call_α
                                                                                        jmp   n38_assign_α
n25_call_β:
                                                                                        jmp   n39_call_α
.Lx169_0:
                        .quad            .Lx169_0_s
.Lx169_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052624], rax
                        mov              qword ptr [1879052632], rdx
                                                                                        jmp   n40_var_α
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx171_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n15_var_α
.Lx171_0:
                        .quad            .Lx171_0_s
.Lx171_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n28_subscript_α:
                        mov              rdi, qword ptr [rbp + 144]
                        mov              rsi, qword ptr [rbp + 152]
                        mov              rdx, qword ptr [rbp + 160]
                        mov              rcx, qword ptr [rbp + 168]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n18_var_α
                        mov              qword ptr [rbp + 176], rax
                        mov              qword ptr [rbp + 184], rdx
                                                                                        jmp   n41_var_α
#-----------------------------------------------------------------------------------------------------------------------
n29_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n42_lit_integer_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n43_call_α
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:
                        mov              rax, qword ptr [rbp + 1568]
                        mov              rdx, qword ptr [rbp + 1576]
                        mov              qword ptr [1879052464], rax
                        mov              qword ptr [1879052472], rdx
                                                                                        jmp   n32_lit_string_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_string_α:
                        mov              qword ptr [rbp + 1680], 1
                        mov              rax, qword ptr [rip + .Lx176_0]
                        mov              qword ptr [rbp + 1688], rax
                                                                                        jmp   n45_call_α
.Lx176_0:
                        .quad            .Lx176_0_s
.Lx176_0_s:
                        .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_integer_α:
                        mov              qword ptr [rbp + 528], 6
                        mov              rax, qword ptr [rip + .Lx177_0]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n46_subscript_α
.Lx177_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:
                        mov              qword ptr [rbp + 624], 6
                        mov              rax, qword ptr [rip + .Lx178_0]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n47_subscript_α
.Lx178_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n35_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n36_assign_α:
                        mov              rax, qword ptr [rbp + 800]
                        mov              rdx, qword ptr [rbp + 808]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n37_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_string_α:
                        mov              qword ptr [rbp + 832], 1
                        mov              rax, qword ptr [rip + .Lx182_0]
                        mov              qword ptr [rbp + 840], rax
                                                                                        jmp   n48_call_α
.Lx182_0:
                        .quad            .Lx182_0_s
.Lx182_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n38_assign_α:
                        mov              rax, qword ptr [rbp + 1136]
                        mov              rdx, qword ptr [rbp + 1144]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n39_call_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n39_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx185_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx185_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx185_6]
                        lea              rdx, [rip + .Lx185_7]
                                                                                        jmp   rax
.Lx185_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx185_2
.Lx185_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx185_2
.Lx185_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx185_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx185_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx185_3]
                        lea              rdx, [rip + .Lx185_4]
                                                                                        jmp   rax
.Lx185_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx185_2
.Lx185_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx185_2
.Lx185_1:
                        call             rt_faildescr@PLT
.Lx185_2:
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n50_lit_string_α
                                                                                        jmp   n49_assign_α
n39_call_β:
                                                                                        jmp   n50_lit_string_α
.Lx185_0:
                        .quad            .Lx185_0_s
.Lx185_0_s:
                        .string          "Pop"
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n40_var_α:
                        mov              rax, qword ptr [1879052624]
                        mov              rdx, qword ptr [1879052632]
                        mov              qword ptr [rbp + 2448], rax
                        mov              qword ptr [rbp + 2456], rdx
                                                                                        jmp   n51_match_head_α
#-----------------------------------------------------------------------------------------------------------------------
n41_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                                                                                        jmp   n52_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rbp + 336], 6
                        mov              rax, qword ptr [rip + .Lx188_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n53_subscript_α
.Lx188_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n43_call_α:
                        mov              rax, qword ptr [rbp + 448]
                        mov              qword ptr [rbp + 416], rax
                        mov              rax, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 424], rax
                        .section         .rodata
.Lrkfn190:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn190]
                        lea              rsi, [rbp + 416]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n44_lit_string_α
                                                                                        jmp   n54_var_α
n43_call_β:
                                                                                        jmp   n44_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_string_α:
                        mov              qword ptr [rbp + 32], 1
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n55_call_α
.Lx191_0:
                        .quad            .Lx191_0_s
.Lx191_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n45_call_α:
                        mov              rax, qword ptr [rbp + 1680]
                        mov              qword ptr [rbp + 1648], rax
                        mov              rax, qword ptr [rbp + 1688]
                        mov              qword ptr [rbp + 1656], rax
                        .section         .rodata
.Lrkfn193:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn193]
                        lea              rsi, [rbp + 1648]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1632], rax
                        mov              qword ptr [rbp + 1640], rdx
                        cmp              eax, 99
                                                                                        je    n57_lit_string_α
                                                                                        jmp   n56_assign_α
n45_call_β:
                                                                                        jmp   n57_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n46_subscript_α:
                        mov              rdi, qword ptr [rbp + 512]
                        mov              rsi, qword ptr [rbp + 520]
                        mov              rdx, qword ptr [rbp + 528]
                        mov              rcx, qword ptr [rbp + 536]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n21_var_α
                        mov              qword ptr [rbp + 544], rax
                        mov              qword ptr [rbp + 552], rdx
                                                                                        jmp   n58_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n47_subscript_α:
                        mov              rdi, qword ptr [rbp + 608]
                        mov              rsi, qword ptr [rbp + 616]
                        mov              rdx, qword ptr [rbp + 624]
                        mov              rcx, qword ptr [rbp + 632]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n35_op14_α
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   n59_var_α
#-----------------------------------------------------------------------------------------------------------------------
n48_call_α:
                        mov              rax, qword ptr [rbp + 832]
                        mov              qword ptr [rbp + 864], rax
                        mov              rax, qword ptr [rbp + 840]
                        mov              qword ptr [rbp + 872], rax
                        .section         .rodata
.Lrkfn197:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn197]
                        lea              rsi, [rbp + 864]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n61_lit_string_α
                                                                                        jmp   n60_call_α
n48_call_β:
                                                                                        jmp   n61_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n49_assign_α:
                        mov              rax, qword ptr [rbp + 1168]
                        mov              rdx, qword ptr [rbp + 1176]
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n50_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:
                        mov              qword ptr [rbp + 1200], 1
                        mov              rax, qword ptr [rip + .Lx199_0]
                        mov              qword ptr [rbp + 1208], rax
                                                                                        jmp   n62_call_α
.Lx199_0:
                        .quad            .Lx199_0_s
.Lx199_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n51_match_head_α:
                        mov              qword ptr [rbp + 2376], rbp
                        mov              rdi, qword ptr [rbp + 2448]
                        mov              rsi, qword ptr [rbp + 2456]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rbp + 2368], r12
                        mov              qword ptr [rbp + 2352], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 2344], rax
                        mov              dword ptr [rbp + 2336], 0
.Lx201_0:
                        mov              r14d, dword ptr [rbp + 2336]
                                                                                        jmp   n63_match_sequence_α
n51_match_head_β:
                        add              dword ptr [rbp + 2336], 1
                        mov              eax, dword ptr [rbp + 2336]
                        cmp              eax, r15d
                                                                                        jg    .Lx201_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx201_1
                                                                                        jmp   .Lx201_0
.Lx201_1:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        mov              r12, qword ptr [rbp + 2368]
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n16_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_integer_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n64_subscript_α
.Lx202_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n53_subscript_α:
                        mov              rdi, qword ptr [rbp + 320]
                        mov              rsi, qword ptr [rbp + 328]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n30_var_α
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   n65_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n54_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   n66_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n55_call_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 64], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 72], rax
                        .section         .rodata
.Lrkfn206:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn206]
                        lea              rsi, [rbp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 48], rax
                        mov              qword ptr [rbp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n67_op14_α
                                                                                        jmp   n35_op14_α
n55_call_β:
                                                                                        jmp   n67_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_α:
                        mov              rax, qword ptr [rbp + 1632]
                        mov              rdx, qword ptr [rbp + 1640]
                        mov              qword ptr [1879052480], rax
                        mov              qword ptr [1879052488], rdx
                                                                                        jmp   n57_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_string_α:
                        mov              qword ptr [rbp + 1744], 1
                        mov              rax, qword ptr [rip + .Lx208_0]
                        mov              qword ptr [rbp + 1752], rax
                                                                                        jmp   n68_call_α
.Lx208_0:
                        .quad            .Lx208_0_s
.Lx208_0_s:
                        .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n58_deref_α:
                        mov              rdi, qword ptr [rbp + 544]
                        mov              rsi, qword ptr [rbp + 552]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n21_var_α
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                                                                                        jmp   n69_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n59_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                                                                                        jmp   n70_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n60_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx212_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx212_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx212_6]
                        lea              rdx, [rip + .Lx212_7]
                                                                                        jmp   rax
.Lx212_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx212_2
.Lx212_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx212_2
.Lx212_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx212_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx212_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx212_3]
                        lea              rdx, [rip + .Lx212_4]
                                                                                        jmp   rax
.Lx212_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx212_2
.Lx212_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx212_2
.Lx212_1:
                        call             rt_faildescr@PLT
.Lx212_2:
                        mov              qword ptr [rbp + 896], rax
                        mov              qword ptr [rbp + 904], rdx
                        cmp              eax, 99
                                                                                        je    n61_lit_string_α
                                                                                        jmp   n71_var_α
n60_call_β:
                                                                                        jmp   n61_lit_string_α
.Lx212_0:
                        .quad            .Lx212_0_s
.Lx212_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:
                        mov              qword ptr [rbp + 1088], 1
                        mov              rax, qword ptr [rip + .Lx213_0]
                        mov              qword ptr [rbp + 1096], rax
                                                                                        jmp   n72_call_α
.Lx213_0:
                        .quad            .Lx213_0_s
.Lx213_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n62_call_α:
                        mov              rax, qword ptr [rbp + 1200]
                        mov              qword ptr [rbp + 1232], rax
                        mov              rax, qword ptr [rbp + 1208]
                        mov              qword ptr [rbp + 1240], rax
                        .section         .rodata
.Lrkfn215:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn215]
                        lea              rsi, [rbp + 1232]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1216], rax
                        mov              qword ptr [rbp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n74_lit_string_α
                                                                                        jmp   n73_call_α
n62_call_β:
                                                                                        jmp   n74_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n63_match_sequence_α:
                        mov              dword ptr [rbp + 2384], r14d
                                                                                        jmp   n76_lit_integer_α
n63_match_sequence_as:
                                                                                        jmp   n75_match_release_α
n63_match_sequence_β:
                                                                                        jmp   n80_match_rpos_β
n63_match_sequence_af:
                                                                                        jmp   n51_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n64_subscript_α:
                        mov              rdi, qword ptr [rbp + 208]
                        mov              rsi, qword ptr [rbp + 216]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n18_var_α
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n81_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n65_deref_α:
                        mov              rdi, qword ptr [rbp + 352]
                        mov              rsi, qword ptr [rbp + 360]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n30_var_α
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n82_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n66_assign_var_α:
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        mov              rdx, qword ptr [rbp + 464]
                        mov              rcx, qword ptr [rbp + 472]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n44_lit_string_α
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   n44_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n67_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n68_call_α:
                        mov              rax, qword ptr [rbp + 1744]
                        mov              qword ptr [rbp + 1712], rax
                        mov              rax, qword ptr [rbp + 1752]
                        mov              qword ptr [rbp + 1720], rax
                        .section         .rodata
.Lrkfn224:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn224]
                        lea              rsi, [rbp + 1712]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1696], rax
                        mov              qword ptr [rbp + 1704], rdx
                        cmp              eax, 99
                                                                                        je    n84_lit_string_α
                                                                                        jmp   n83_assign_α
n68_call_β:
                                                                                        jmp   n84_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n69_subscript_α:
                        mov              rdi, qword ptr [rbp + 496]
                        mov              rsi, qword ptr [rbp + 504]
                        mov              rdx, qword ptr [rbp + 560]
                        mov              rcx, qword ptr [rbp + 568]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n21_var_α
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n85_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_integer_α:
                        mov              qword ptr [rbp + 688], 6
                        mov              rax, qword ptr [rip + .Lx226_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   n86_subscript_α
.Lx226_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n71_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   n87_var_α
#-----------------------------------------------------------------------------------------------------------------------
n72_call_α:
                        mov              rax, qword ptr [rbp + 1088]
                        mov              qword ptr [rbp + 1056], rax
                        mov              rax, qword ptr [rbp + 1096]
                        mov              qword ptr [rbp + 1064], rax
                        .section         .rodata
.Lrkfn229:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn229]
                        lea              rsi, [rbp + 1056]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    n44_lit_string_α
                                                                                        jmp   n88_assign_α
n72_call_β:
                                                                                        jmp   n44_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx231_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx231_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx231_6]
                        lea              rdx, [rip + .Lx231_7]
                                                                                        jmp   rax
.Lx231_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx231_2
.Lx231_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx231_2
.Lx231_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx231_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx231_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx231_3]
                        lea              rdx, [rip + .Lx231_4]
                                                                                        jmp   rax
.Lx231_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx231_2
.Lx231_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx231_2
.Lx231_1:
                        call             rt_faildescr@PLT
.Lx231_2:
                        mov              qword ptr [rbp + 1264], rax
                        mov              qword ptr [rbp + 1272], rdx
                        cmp              eax, 99
                                                                                        je    n74_lit_string_α
                                                                                        jmp   n89_var_α
n73_call_β:
                                                                                        jmp   n74_lit_string_α
.Lx231_0:
                        .quad            .Lx231_0_s
.Lx231_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_string_α:
                        mov              qword ptr [rbp + 1552], 1
                        mov              rax, qword ptr [rip + .Lx232_0]
                        mov              qword ptr [rbp + 1560], rax
                                                                                        jmp   n90_call_α
.Lx232_0:
                        .quad            .Lx232_0_s
.Lx232_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n75_match_release_α:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 2400]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx234_1:
                        test             rax, rax
                                                                                        je    .Lx234_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx234_3]
                        lea              rdx, [rip + .Lx234_4]
                                                                                        jmp   rax
.Lx234_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx234_1
.Lx234_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx234_1
.Lx234_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + 2368]
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n91_call_α
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_integer_α:
                        mov              qword ptr [rbp + 2400], 6
                        mov              rax, qword ptr [rip + .Lx235_0]
                        mov              qword ptr [rbp + 2408], rax
                                                                                        jmp   n77_match_pos_α
n76_lit_integer_β:
                                                                                        jmp   n51_match_head_β
.Lx235_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n77_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n51_match_head_β
                                                                                        jmp   n78_match_defer_α
n77_match_pos_β:
                                                                                        jmp   n51_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n78_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx237_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx237_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx237_10
.Lx237_9:
                        xor              eax, eax
.Lx237_10:
                        test             rax, rax
                                                                                        jz    .Lx237_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx237_4]
                        lea              rdx, [rip + .Lx237_5]
                                                                                        jmp   rax
.Lx237_4:
                                                                                        jmp   n79_lit_integer_α
.Lx237_5:
                                                                                        jmp   n51_match_head_β
.Lx237_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx237_2:
                        test             rax, rax
                                                                                        je    .Lx237_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx237_7]
                        lea              rdx, [rip + .Lx237_8]
                                                                                        jmp   rax
.Lx237_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx237_2
.Lx237_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx237_2
.Lx237_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n51_match_head_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx237_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n79_lit_integer_α
.Lx237_6:
                        add              rsp, 16
                                                                                        jmp   n51_match_head_β
n78_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_integer_α:
                        mov              qword ptr [rbp + 2432], 6
                        mov              rax, qword ptr [rip + .Lx238_0]
                        mov              qword ptr [rbp + 2440], rax
                                                                                        jmp   n80_match_rpos_α
n79_lit_integer_β:
                                                                                        jmp   n78_match_defer_β
.Lx238_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n80_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n78_match_defer_β
                                                                                        jmp   n75_match_release_α
n80_match_rpos_β:
                                                                                        jmp   n78_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n81_deref_α:
                        mov              rdi, qword ptr [rbp + 240]
                        mov              rsi, qword ptr [rbp + 248]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n18_var_α
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n92_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n82_subscript_α:
                        mov              rdi, qword ptr [rbp + 304]
                        mov              rsi, qword ptr [rbp + 312]
                        mov              rdx, qword ptr [rbp + 368]
                        mov              rcx, qword ptr [rbp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n30_var_α
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                                                                                        jmp   n93_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_α:
                        mov              rax, qword ptr [rbp + 1696]
                        mov              rdx, qword ptr [rbp + 1704]
                        mov              qword ptr [1879052496], rax
                        mov              qword ptr [1879052504], rdx
                                                                                        jmp   n84_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_string_α:
                        mov              qword ptr [rbp + 1808], 1
                        mov              rax, qword ptr [rip + .Lx243_0]
                        mov              qword ptr [rbp + 1816], rax
                                                                                        jmp   n94_call_α
.Lx243_0:
                        .quad            .Lx243_0_s
.Lx243_0_s:
                        .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n85_deref_α:
                        mov              rdi, qword ptr [rbp + 576]
                        mov              rsi, qword ptr [rbp + 584]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n21_var_α
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n95_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n86_subscript_α:
                        mov              rdi, qword ptr [rbp + 672]
                        mov              rsi, qword ptr [rbp + 680]
                        mov              rdx, qword ptr [rbp + 688]
                        mov              rcx, qword ptr [rbp + 696]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n35_op14_α
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                                                                                        jmp   n96_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n87_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                                                                                        jmp   n97_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n88_assign_α:
                        mov              rax, qword ptr [rbp + 1040]
                        mov              rdx, qword ptr [rbp + 1048]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n44_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n89_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rbp + 1408], rax
                        mov              qword ptr [rbp + 1416], rdx
                                                                                        jmp   n98_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n90_call_α:
                        mov              rax, qword ptr [rbp + 1552]
                        mov              qword ptr [rbp + 1520], rax
                        mov              rax, qword ptr [rbp + 1560]
                        mov              qword ptr [rbp + 1528], rax
                        .section         .rodata
.Lrkfn250:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn250]
                        lea              rsi, [rbp + 1520]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1504], rax
                        mov              qword ptr [rbp + 1512], rdx
                        cmp              eax, 99
                                                                                        je    n44_lit_string_α
                                                                                        jmp   n99_assign_α
n90_call_β:
                                                                                        jmp   n44_lit_string_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n91_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx252_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx252_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx252_6]
                        lea              rdx, [rip + .Lx252_7]
                                                                                        jmp   rax
.Lx252_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx252_2
.Lx252_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx252_2
.Lx252_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx252_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx252_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx252_3]
                        lea              rdx, [rip + .Lx252_4]
                                                                                        jmp   rax
.Lx252_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx252_2
.Lx252_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx252_2
.Lx252_1:
                        call             rt_faildescr@PLT
.Lx252_2:
                        mov              qword ptr [rbp + 2464], rax
                        mov              qword ptr [rbp + 2472], rdx
                        cmp              eax, 99
                                                                                        je    n15_var_α
                                                                                        jmp   n100_assign_α
n91_call_β:
                                                                                        jmp   n15_var_α
.Lx252_0:
                        .quad            .Lx252_0_s
.Lx252_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_integer_α:
                        mov              qword ptr [rbp + 272], 6
                        mov              rax, qword ptr [rip + .Lx253_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n101_binop_α
.Lx253_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n93_assign_α:
                        mov              rax, qword ptr [rbp + 384]
                        mov              rdx, qword ptr [rbp + 392]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n30_var_α
#-----------------------------------------------------------------------------------------------------------------------
n94_call_α:
                        mov              rax, qword ptr [rbp + 1808]
                        mov              qword ptr [rbp + 1776], rax
                        mov              rax, qword ptr [rbp + 1816]
                        mov              qword ptr [rbp + 1784], rax
                        .section         .rodata
.Lrkfn256:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn256]
                        lea              rsi, [rbp + 1776]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1760], rax
                        mov              qword ptr [rbp + 1768], rdx
                        cmp              eax, 99
                                                                                        je    n103_lit_string_α
                                                                                        jmp   n102_assign_α
n94_call_β:
                                                                                        jmp   n103_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n95_assign_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              rdx, qword ptr [rbp + 600]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n21_var_α
#-----------------------------------------------------------------------------------------------------------------------
n96_deref_α:
                        mov              rdi, qword ptr [rbp + 704]
                        mov              rsi, qword ptr [rbp + 712]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n35_op14_α
                        mov              qword ptr [rbp + 720], rax
                        mov              qword ptr [rbp + 728], rdx
                                                                                        jmp   n104_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n97_binop_α:
                        mov              rdi, qword ptr [rbp + 992]
                        mov              rsi, qword ptr [rbp + 1000]
                        mov              rdx, qword ptr [rbp + 1008]
                        mov              rcx, qword ptr [rbp + 1016]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 976], rax
                        mov              qword ptr [rbp + 984], rdx
                                                                                        jmp   n105_call_α
#-----------------------------------------------------------------------------------------------------------------------
n98_lit_string_α:
                        mov              qword ptr [rbp + 1424], 1
                        mov              rax, qword ptr [rip + .Lx260_0]
                        mov              qword ptr [rbp + 1432], rax
                                                                                        jmp   n106_binop_α
.Lx260_0:
                        .quad            .Lx260_0_s
.Lx260_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n99_assign_α:
                        mov              rax, qword ptr [rbp + 1504]
                        mov              rdx, qword ptr [rbp + 1512]
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n44_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n100_assign_α:
                        mov              rsi, qword ptr [rbp + 2464]
                        mov              rdx, qword ptr [rbp + 2472]
                        mov              rdi, qword ptr [rip + .Lx262_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n15_var_α
.Lx262_0:
                        .quad            .Lx262_0_s
.Lx262_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n101_binop_α:
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 100
                                                                                        je    .Lx263_0
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 6
                                                                                        jne   .Lx263_2
.Lx263_1:
                        mov              rax, qword ptr [rbp + 264]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rbp + 192], 6
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n107_assign_var_α
.Lx263_0:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 0
                        lea              r9, [rbp + 192]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n107_assign_var_α
.Lx263_2:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 0
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n18_var_α
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   n107_assign_var_α
n101_binop_β:
                                                                                        jmp   n18_var_α
#-----------------------------------------------------------------------------------------------------------------------
n102_assign_α:
                        mov              rax, qword ptr [rbp + 1760]
                        mov              rdx, qword ptr [rbp + 1768]
                        mov              qword ptr [1879052512], rax
                        mov              qword ptr [1879052520], rdx
                                                                                        jmp   n103_lit_string_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n103_lit_string_α:
                        mov              qword ptr [rbp + 1872], 1
                        mov              rax, qword ptr [rip + .Lx265_0]
                        mov              qword ptr [rbp + 1880], rax
                                                                                        jmp   n108_call_α
.Lx265_0:
                        .quad            .Lx265_0_s
.Lx265_0_s:
                        .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_integer_α:
                        mov              qword ptr [rbp + 736], 6
                        mov              rax, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rbp + 744], rax
                                                                                        jmp   n109_binop_α
.Lx266_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n105_call_α:
                        mov              rax, qword ptr [rbp + 976]
                        mov              qword ptr [rbp + 944], rax
                        mov              rax, qword ptr [rbp + 984]
                        mov              qword ptr [rbp + 952], rax
                        .section         .rodata
.Lrkfn268:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn268]
                        lea              rsi, [rbp + 944]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 928], rax
                        mov              qword ptr [rbp + 936], rdx
                        cmp              eax, 99
                                                                                        je    n61_lit_string_α
                                                                                        jmp   n110_assign_var_α
n105_call_β:
                                                                                        jmp   n61_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n106_binop_α:
                        mov              rdi, qword ptr [rbp + 1408]
                        mov              rsi, qword ptr [rbp + 1416]
                        mov              rdx, qword ptr [rbp + 1424]
                        mov              rcx, qword ptr [rbp + 1432]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                                                                                        jmp   n111_var_α
#-----------------------------------------------------------------------------------------------------------------------
n107_assign_var_α:
                        mov              rdi, qword ptr [rbp + 176]
                        mov              rsi, qword ptr [rbp + 184]
                        mov              rdx, qword ptr [rbp + 192]
                        mov              rcx, qword ptr [rbp + 200]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n18_var_α
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                                                                                        jmp   n18_var_α
#-----------------------------------------------------------------------------------------------------------------------
n108_call_α:
                        mov              rax, qword ptr [rbp + 1872]
                        mov              qword ptr [rbp + 1840], rax
                        mov              rax, qword ptr [rbp + 1880]
                        mov              qword ptr [rbp + 1848], rax
                        .section         .rodata
.Lrkfn272:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn272]
                        lea              rsi, [rbp + 1840]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1824], rax
                        mov              qword ptr [rbp + 1832], rdx
                        cmp              eax, 99
                                                                                        je    n113_lit_string_α
                                                                                        jmp   n112_assign_α
n108_call_β:
                                                                                        jmp   n113_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n109_binop_α:
                        mov              eax, dword ptr [rbp + 720]
                        cmp              eax, 100
                                                                                        je    .Lx273_0
                        mov              eax, dword ptr [rbp + 720]
                        cmp              eax, 6
                                                                                        jne   .Lx273_2
.Lx273_1:
                        mov              rax, qword ptr [rbp + 728]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 656], 6
                        mov              qword ptr [rbp + 664], rax
                                                                                        jmp   n114_assign_var_α
.Lx273_0:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        mov              r8d, 1
                        lea              r9, [rbp + 656]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n114_assign_var_α
.Lx273_2:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        mov              r8d, 1
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n35_op14_α
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n114_assign_var_α
n109_binop_β:
                                                                                        jmp   n35_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n110_assign_var_α:
                        mov              rdi, qword ptr [rbp + 896]
                        mov              rsi, qword ptr [rbp + 904]
                        mov              rdx, qword ptr [rbp + 928]
                        mov              rcx, qword ptr [rbp + 936]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n61_lit_string_α
                        mov              qword ptr [rbp + 1024], rax
                        mov              qword ptr [rbp + 1032], rdx
                                                                                        jmp   n61_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n111_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 1440], rax
                        mov              qword ptr [rbp + 1448], rdx
                                                                                        jmp   n115_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n112_assign_α:
                        mov              rax, qword ptr [rbp + 1824]
                        mov              rdx, qword ptr [rbp + 1832]
                        mov              qword ptr [1879052528], rax
                        mov              qword ptr [1879052536], rdx
                                                                                        jmp   n113_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n113_lit_string_α:
                        mov              qword ptr [rbp + 1936], 1
                        mov              rax, qword ptr [rip + .Lx277_0]
                        mov              qword ptr [rbp + 1944], rax
                                                                                        jmp   n116_call_α
.Lx277_0:
                        .quad            .Lx277_0_s
.Lx277_0_s:
                        .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n114_assign_var_α:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 656]
                        mov              rcx, qword ptr [rbp + 664]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n35_op14_α
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   n35_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n115_binop_α:
                        mov              rdi, qword ptr [rbp + 1392]
                        mov              rsi, qword ptr [rbp + 1400]
                        mov              rdx, qword ptr [rbp + 1440]
                        mov              rcx, qword ptr [rbp + 1448]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                                                                                        jmp   n117_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n116_call_α:
                        mov              rax, qword ptr [rbp + 1936]
                        mov              qword ptr [rbp + 1904], rax
                        mov              rax, qword ptr [rbp + 1944]
                        mov              qword ptr [rbp + 1912], rax
                        .section         .rodata
.Lrkfn281:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn281]
                        lea              rsi, [rbp + 1904]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1888], rax
                        mov              qword ptr [rbp + 1896], rdx
                        cmp              eax, 99
                                                                                        je    n119_lit_string_α
                                                                                        jmp   n118_assign_α
n116_call_β:
                                                                                        jmp   n119_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n117_lit_string_α:
                        mov              qword ptr [rbp + 1456], 1
                        mov              rax, qword ptr [rip + .Lx282_0]
                        mov              qword ptr [rbp + 1464], rax
                                                                                        jmp   n120_binop_α
.Lx282_0:
                        .quad            .Lx282_0_s
.Lx282_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n118_assign_α:
                        mov              rax, qword ptr [rbp + 1888]
                        mov              rdx, qword ptr [rbp + 1896]
                        mov              qword ptr [1879052544], rax
                        mov              qword ptr [1879052552], rdx
                                                                                        jmp   n119_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n119_lit_string_α:
                        mov              qword ptr [rbp + 2000], 1
                        mov              rax, qword ptr [rip + .Lx284_0]
                        mov              qword ptr [rbp + 2008], rax
                                                                                        jmp   n121_call_α
.Lx284_0:
                        .quad            .Lx284_0_s
.Lx284_0_s:
                        .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n120_binop_α:
                        mov              rdi, qword ptr [rbp + 1376]
                        mov              rsi, qword ptr [rbp + 1384]
                        mov              rdx, qword ptr [rbp + 1456]
                        mov              rcx, qword ptr [rbp + 1464]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1360], rax
                        mov              qword ptr [rbp + 1368], rdx
                                                                                        jmp   n122_var_α
#-----------------------------------------------------------------------------------------------------------------------
n121_call_α:
                        mov              rax, qword ptr [rbp + 2000]
                        mov              qword ptr [rbp + 1968], rax
                        mov              rax, qword ptr [rbp + 2008]
                        mov              qword ptr [rbp + 1976], rax
                        .section         .rodata
.Lrkfn287:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn287]
                        lea              rsi, [rbp + 1968]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1952], rax
                        mov              qword ptr [rbp + 1960], rdx
                        cmp              eax, 99
                                                                                        je    n124_lit_string_α
                                                                                        jmp   n123_assign_α
n121_call_β:
                                                                                        jmp   n124_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n122_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rbp + 1472], rax
                        mov              qword ptr [rbp + 1480], rdx
                                                                                        jmp   n125_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n123_assign_α:
                        mov              rax, qword ptr [rbp + 1952]
                        mov              rdx, qword ptr [rbp + 1960]
                        mov              qword ptr [1879052560], rax
                        mov              qword ptr [1879052568], rdx
                                                                                        jmp   n124_lit_string_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n124_lit_string_α:
                        mov              qword ptr [rbp + 2064], 1
                        mov              rax, qword ptr [rip + .Lx290_0]
                        mov              qword ptr [rbp + 2072], rax
                                                                                        jmp   n126_call_α
.Lx290_0:
                        .quad            .Lx290_0_s
.Lx290_0_s:
                        .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n125_binop_α:
                        mov              rdi, qword ptr [rbp + 1360]
                        mov              rsi, qword ptr [rbp + 1368]
                        mov              rdx, qword ptr [rbp + 1472]
                        mov              rcx, qword ptr [rbp + 1480]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1344], rax
                        mov              qword ptr [rbp + 1352], rdx
                                                                                        jmp   n127_call_α
#-----------------------------------------------------------------------------------------------------------------------
n126_call_α:
                        mov              rax, qword ptr [rbp + 2064]
                        mov              qword ptr [rbp + 2032], rax
                        mov              rax, qword ptr [rbp + 2072]
                        mov              qword ptr [rbp + 2040], rax
                        .section         .rodata
.Lrkfn293:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn293]
                        lea              rsi, [rbp + 2032]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2016], rax
                        mov              qword ptr [rbp + 2024], rdx
                        cmp              eax, 99
                                                                                        je    n129_lit_string_α
                                                                                        jmp   n128_assign_α
n126_call_β:
                                                                                        jmp   n129_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n127_call_α:
                        mov              rax, qword ptr [rbp + 1344]
                        mov              qword ptr [rbp + 1312], rax
                        mov              rax, qword ptr [rbp + 1352]
                        mov              qword ptr [rbp + 1320], rax
                        .section         .rodata
.Lrkfn295:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn295]
                        lea              rsi, [rbp + 1312]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1296], rax
                        mov              qword ptr [rbp + 1304], rdx
                        cmp              eax, 99
                                                                                        je    n74_lit_string_α
                                                                                        jmp   n130_assign_var_α
n127_call_β:
                                                                                        jmp   n74_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n128_assign_α:
                        mov              rax, qword ptr [rbp + 2016]
                        mov              rdx, qword ptr [rbp + 2024]
                        mov              qword ptr [1879052576], rax
                        mov              qword ptr [1879052584], rdx
                                                                                        jmp   n129_lit_string_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n129_lit_string_α:
                        mov              qword ptr [rbp + 2128], 1
                        mov              rax, qword ptr [rip + .Lx297_0]
                        mov              qword ptr [rbp + 2136], rax
                                                                                        jmp   n131_call_α
.Lx297_0:
                        .quad            .Lx297_0_s
.Lx297_0_s:
                        .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n130_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1264]
                        mov              rsi, qword ptr [rbp + 1272]
                        mov              rdx, qword ptr [rbp + 1296]
                        mov              rcx, qword ptr [rbp + 1304]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n74_lit_string_α
                        mov              qword ptr [rbp + 1488], rax
                        mov              qword ptr [rbp + 1496], rdx
                                                                                        jmp   n74_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n131_call_α:
                        mov              rax, qword ptr [rbp + 2128]
                        mov              qword ptr [rbp + 2096], rax
                        mov              rax, qword ptr [rbp + 2136]
                        mov              qword ptr [rbp + 2104], rax
                        .section         .rodata
.Lrkfn300:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn300]
                        lea              rsi, [rbp + 2096]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2080], rax
                        mov              qword ptr [rbp + 2088], rdx
                        cmp              eax, 99
                                                                                        je    n133_lit_string_α
                                                                                        jmp   n132_assign_α
n131_call_β:
                                                                                        jmp   n133_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n132_assign_α:
                        mov              rax, qword ptr [rbp + 2080]
                        mov              rdx, qword ptr [rbp + 2088]
                        mov              qword ptr [1879052592], rax
                        mov              qword ptr [1879052600], rdx
                                                                                        jmp   n133_lit_string_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n133_lit_string_α:
                        mov              qword ptr [rbp + 2192], 1
                        mov              rax, qword ptr [rip + .Lx302_0]
                        mov              qword ptr [rbp + 2200], rax
                                                                                        jmp   n134_call_α
.Lx302_0:
                        .quad            .Lx302_0_s
.Lx302_0_s:
                        .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n134_call_α:
                        mov              rax, qword ptr [rbp + 2192]
                        mov              qword ptr [rbp + 2160], rax
                        mov              rax, qword ptr [rbp + 2200]
                        mov              qword ptr [rbp + 2168], rax
                        .section         .rodata
.Lrkfn304:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn304]
                        lea              rsi, [rbp + 2160]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2144], rax
                        mov              qword ptr [rbp + 2152], rdx
                        cmp              eax, 99
                                                                                        je    n136_lit_string_α
                                                                                        jmp   n135_assign_α
n134_call_β:
                                                                                        jmp   n136_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n135_assign_α:
                        mov              rax, qword ptr [rbp + 2144]
                        mov              rdx, qword ptr [rbp + 2152]
                        mov              qword ptr [1879052608], rax
                        mov              qword ptr [1879052616], rdx
                                                                                        jmp   n136_lit_string_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n136_lit_string_α:
                        mov              qword ptr [rbp + 2272], 1
                        mov              rax, qword ptr [rip + .Lx306_0]
                        mov              qword ptr [rbp + 2280], rax
                                                                                        jmp   n137_lit_integer_α
.Lx306_0:
                        .quad            .Lx306_0_s
.Lx306_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n137_lit_integer_α:
                        mov              qword ptr [rbp + 2288], 6
                        mov              rax, qword ptr [rip + .Lx307_0]
                        mov              qword ptr [rbp + 2296], rax
                                                                                        jmp   n138_call_α
.Lx307_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n138_call_α:
                        mov              rax, qword ptr [rbp + 2272]
                        mov              qword ptr [rbp + 2224], rax
                        mov              rax, qword ptr [rbp + 2280]
                        mov              qword ptr [rbp + 2232], rax
                        mov              rax, qword ptr [rbp + 2288]
                        mov              qword ptr [rbp + 2240], rax
                        mov              rax, qword ptr [rbp + 2296]
                        mov              qword ptr [rbp + 2248], rax
                        .section         .rodata
.Lrkfn309:              .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn309]
                        lea              rsi, [rbp + 2224]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2208], rax
                        mov              qword ptr [rbp + 2216], rdx
                        cmp              eax, 99
                                                                                        je    n15_var_α
                                                                                        jmp   n15_var_α
n138_call_β:
                                                                                        jmp   n15_var_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2520]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_ω:
                        mov              rax, [rbp + 2528]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Pop_α
proc_LBL__Pop_α:
                        .global          proc_LBL__Pop_α
                        .global          proc_LBL__Pop_β
                        .global          proc_LBL__Pop_γ
                        .global          proc_LBL__Pop_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              [rsp + 2536], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__Pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n310_goto_α:
                                                                                        jmp   n320_var_α
n310_goto_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
n311_goto_α:
                                                                                        jmp   n321_var_α
n311_goto_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
n312_goto_α:
                                                                                        jmp   n322_lit_string_α
n312_goto_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
n313_goto_α:
                                                                                        jmp   n322_lit_string_α
n313_goto_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
n314_goto_α:
                                                                                        jmp   n323_call_α
n314_goto_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
n315_goto_α:
                                                                                        jmp   n322_lit_string_α
n315_goto_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
n316_goto_α:
                                                                                        jmp   n324_call_α
n316_goto_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
n317_goto_α:
                                                                                        jmp   n322_lit_string_α
n317_goto_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
n318_goto_α:
                                                                                        jmp   n325_var_α
n318_goto_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
n319_goto_α:
                                                                                        jmp   n326_lit_string_α
n319_goto_β:
                                                                                        jmp   proc_LBL__Pop_ω
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n320_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 496], rax
                        mov              qword ptr [rbp + 504], rdx
                                                                                        jmp   n327_var_α
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n321_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                                                                                        jmp   n329_lit_integer_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n322_lit_string_α:
                        mov              qword ptr [rbp + 1616], 1
                        mov              rax, qword ptr [rip + .Lx461_0]
                        mov              qword ptr [rbp + 1624], rax
                                                                                        jmp   n331_call_α
.Lx461_0:
                        .quad            .Lx461_0_s
.Lx461_0_s:
                        .string          "PAT$0"
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n323_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx463_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx463_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx463_6]
                        lea              rdx, [rip + .Lx463_7]
                                                                                        jmp   rax
.Lx463_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx463_2
.Lx463_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx463_2
.Lx463_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx463_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx463_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx463_3]
                        lea              rdx, [rip + .Lx463_4]
                                                                                        jmp   rax
.Lx463_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx463_2
.Lx463_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx463_2
.Lx463_1:
                        call             rt_faildescr@PLT
.Lx463_2:
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                        cmp              eax, 99
                                                                                        je    n333_call_α
                                                                                        jmp   n332_assign_α
n323_call_β:
                                                                                        jmp   n333_call_α
.Lx463_0:
                        .quad            .Lx463_0_s
.Lx463_0_s:
                        .string          "Pop"
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n324_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx465_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx465_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx465_6]
                        lea              rdx, [rip + .Lx465_7]
                                                                                        jmp   rax
.Lx465_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx465_2
.Lx465_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx465_2
.Lx465_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx465_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx465_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx465_3]
                        lea              rdx, [rip + .Lx465_4]
                                                                                        jmp   rax
.Lx465_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx465_2
.Lx465_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx465_2
.Lx465_1:
                        call             rt_faildescr@PLT
.Lx465_2:
                        mov              qword ptr [rbp + 1104], rax
                        mov              qword ptr [rbp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n335_call_α
                                                                                        jmp   n334_assign_α
n324_call_β:
                                                                                        jmp   n335_call_α
.Lx465_0:
                        .quad            .Lx465_0_s
.Lx465_0_s:
                        .string          "Pop"
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n325_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx466_0]
                        call             NV_GET_fn@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx466_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__Pop_γ
.Lx466_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n336_assign_α
.Lx466_0:
                        .quad            .Lx466_0_s
.Lx466_0_s:
                        .string          "INPUT"
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n326_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx467_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n337_assign_α
.Lx467_0:
                        .quad            .Lx467_0_s
.Lx467_0_s:
                        .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n327_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n338_lit_integer_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n328_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   n339_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n329_lit_integer_α:
                        mov              qword ptr [rbp + 160], 6
                        mov              rax, qword ptr [rip + .Lx470_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n341_subscript_α
.Lx470_0:
                        .quad            0
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n330_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                                                                                        jmp   n342_var_α
#-----------------------------------------------------------------------------------------------------------------------
n331_call_α:
                        mov              rax, qword ptr [rbp + 1616]
                        mov              qword ptr [rbp + 1584], rax
                        mov              rax, qword ptr [rbp + 1624]
                        mov              qword ptr [rbp + 1592], rax
                        .section         .rodata
.Lrkfn473:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn473]
                        lea              rsi, [rbp + 1584]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1568], rax
                        mov              qword ptr [rbp + 1576], rdx
                        cmp              eax, 99
                                                                                        je    n345_lit_string_α
                                                                                        jmp   n344_assign_α
n331_call_β:
                                                                                        jmp   n345_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n332_assign_α:
                        mov              rax, qword ptr [rbp + 768]
                        mov              rdx, qword ptr [rbp + 776]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n333_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n333_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx476_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx476_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx476_6]
                        lea              rdx, [rip + .Lx476_7]
                                                                                        jmp   rax
.Lx476_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx476_2
.Lx476_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx476_2
.Lx476_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx476_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx476_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx476_3]
                        lea              rdx, [rip + .Lx476_4]
                                                                                        jmp   rax
.Lx476_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx476_2
.Lx476_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx476_2
.Lx476_1:
                        call             rt_faildescr@PLT
.Lx476_2:
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n347_lit_string_α
                                                                                        jmp   n346_assign_α
n333_call_β:
                                                                                        jmp   n347_lit_string_α
.Lx476_0:
                        .quad            .Lx476_0_s
.Lx476_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n334_assign_α:
                        mov              rax, qword ptr [rbp + 1104]
                        mov              rdx, qword ptr [rbp + 1112]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n335_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n335_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx479_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx479_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx479_6]
                        lea              rdx, [rip + .Lx479_7]
                                                                                        jmp   rax
.Lx479_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx479_2
.Lx479_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx479_2
.Lx479_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx479_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx479_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx479_3]
                        lea              rdx, [rip + .Lx479_4]
                                                                                        jmp   rax
.Lx479_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx479_2
.Lx479_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx479_2
.Lx479_1:
                        call             rt_faildescr@PLT
.Lx479_2:
                        mov              qword ptr [rbp + 1136], rax
                        mov              qword ptr [rbp + 1144], rdx
                        cmp              eax, 99
                                                                                        je    n349_call_α
                                                                                        jmp   n348_assign_α
n335_call_β:
                                                                                        jmp   n349_call_α
.Lx479_0:
                        .quad            .Lx479_0_s
.Lx479_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n336_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052624], rax
                        mov              qword ptr [1879052632], rdx
                                                                                        jmp   n350_var_α
#-----------------------------------------------------------------------------------------------------------------------
n337_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx481_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n325_var_α
.Lx481_0:
                        .quad            .Lx481_0_s
.Lx481_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n338_lit_integer_α:
                        mov              qword ptr [rbp + 528], 6
                        mov              rax, qword ptr [rip + .Lx482_0]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n351_subscript_α
.Lx482_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n339_lit_integer_α:
                        mov              qword ptr [rbp + 624], 6
                        mov              rax, qword ptr [rip + .Lx483_0]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n352_subscript_α
.Lx483_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n340_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n341_subscript_α:
                        mov              rdi, qword ptr [rbp + 144]
                        mov              rsi, qword ptr [rbp + 152]
                        mov              rdx, qword ptr [rbp + 160]
                        mov              rcx, qword ptr [rbp + 168]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n330_var_α
                        mov              qword ptr [rbp + 176], rax
                        mov              qword ptr [rbp + 184], rdx
                                                                                        jmp   n353_var_α
#-----------------------------------------------------------------------------------------------------------------------
n342_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n354_lit_integer_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n343_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n355_call_α
#-----------------------------------------------------------------------------------------------------------------------
n344_assign_α:
                        mov              rax, qword ptr [rbp + 1568]
                        mov              rdx, qword ptr [rbp + 1576]
                        mov              qword ptr [1879052464], rax
                        mov              qword ptr [1879052472], rdx
                                                                                        jmp   n345_lit_string_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n345_lit_string_α:
                        mov              qword ptr [rbp + 1680], 1
                        mov              rax, qword ptr [rip + .Lx490_0]
                        mov              qword ptr [rbp + 1688], rax
                                                                                        jmp   n357_call_α
.Lx490_0:
                        .quad            .Lx490_0_s
.Lx490_0_s:
                        .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n346_assign_α:
                        mov              rax, qword ptr [rbp + 800]
                        mov              rdx, qword ptr [rbp + 808]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n347_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n347_lit_string_α:
                        mov              qword ptr [rbp + 832], 1
                        mov              rax, qword ptr [rip + .Lx492_0]
                        mov              qword ptr [rbp + 840], rax
                                                                                        jmp   n358_call_α
.Lx492_0:
                        .quad            .Lx492_0_s
.Lx492_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n348_assign_α:
                        mov              rax, qword ptr [rbp + 1136]
                        mov              rdx, qword ptr [rbp + 1144]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n349_call_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n349_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx495_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx495_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx495_6]
                        lea              rdx, [rip + .Lx495_7]
                                                                                        jmp   rax
.Lx495_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx495_2
.Lx495_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx495_2
.Lx495_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx495_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx495_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx495_3]
                        lea              rdx, [rip + .Lx495_4]
                                                                                        jmp   rax
.Lx495_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx495_2
.Lx495_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx495_2
.Lx495_1:
                        call             rt_faildescr@PLT
.Lx495_2:
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n360_lit_string_α
                                                                                        jmp   n359_assign_α
n349_call_β:
                                                                                        jmp   n360_lit_string_α
.Lx495_0:
                        .quad            .Lx495_0_s
.Lx495_0_s:
                        .string          "Pop"
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n350_var_α:
                        mov              rax, qword ptr [1879052624]
                        mov              rdx, qword ptr [1879052632]
                        mov              qword ptr [rbp + 2448], rax
                        mov              qword ptr [rbp + 2456], rdx
                                                                                        jmp   n361_match_head_α
#-----------------------------------------------------------------------------------------------------------------------
n351_subscript_α:
                        mov              rdi, qword ptr [rbp + 512]
                        mov              rsi, qword ptr [rbp + 520]
                        mov              rdx, qword ptr [rbp + 528]
                        mov              rcx, qword ptr [rbp + 536]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n328_var_α
                        mov              qword ptr [rbp + 544], rax
                        mov              qword ptr [rbp + 552], rdx
                                                                                        jmp   n362_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n352_subscript_α:
                        mov              rdi, qword ptr [rbp + 608]
                        mov              rsi, qword ptr [rbp + 616]
                        mov              rdx, qword ptr [rbp + 624]
                        mov              rcx, qword ptr [rbp + 632]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n340_op14_α
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   n363_var_α
#-----------------------------------------------------------------------------------------------------------------------
n353_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                                                                                        jmp   n364_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n354_lit_integer_α:
                        mov              qword ptr [rbp + 336], 6
                        mov              rax, qword ptr [rip + .Lx500_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n365_subscript_α
.Lx500_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n355_call_α:
                        mov              rax, qword ptr [rbp + 448]
                        mov              qword ptr [rbp + 416], rax
                        mov              rax, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 424], rax
                        .section         .rodata
.Lrkfn502:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn502]
                        lea              rsi, [rbp + 416]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n356_lit_string_α
                                                                                        jmp   n366_var_α
n355_call_β:
                                                                                        jmp   n356_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n356_lit_string_α:
                        mov              qword ptr [rbp + 32], 1
                        mov              rax, qword ptr [rip + .Lx503_0]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n367_call_α
.Lx503_0:
                        .quad            .Lx503_0_s
.Lx503_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n357_call_α:
                        mov              rax, qword ptr [rbp + 1680]
                        mov              qword ptr [rbp + 1648], rax
                        mov              rax, qword ptr [rbp + 1688]
                        mov              qword ptr [rbp + 1656], rax
                        .section         .rodata
.Lrkfn505:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn505]
                        lea              rsi, [rbp + 1648]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1632], rax
                        mov              qword ptr [rbp + 1640], rdx
                        cmp              eax, 99
                                                                                        je    n369_lit_string_α
                                                                                        jmp   n368_assign_α
n357_call_β:
                                                                                        jmp   n369_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n358_call_α:
                        mov              rax, qword ptr [rbp + 832]
                        mov              qword ptr [rbp + 864], rax
                        mov              rax, qword ptr [rbp + 840]
                        mov              qword ptr [rbp + 872], rax
                        .section         .rodata
.Lrkfn507:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn507]
                        lea              rsi, [rbp + 864]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n371_lit_string_α
                                                                                        jmp   n370_call_α
n358_call_β:
                                                                                        jmp   n371_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n359_assign_α:
                        mov              rax, qword ptr [rbp + 1168]
                        mov              rdx, qword ptr [rbp + 1176]
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n360_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n360_lit_string_α:
                        mov              qword ptr [rbp + 1200], 1
                        mov              rax, qword ptr [rip + .Lx509_0]
                        mov              qword ptr [rbp + 1208], rax
                                                                                        jmp   n372_call_α
.Lx509_0:
                        .quad            .Lx509_0_s
.Lx509_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n361_match_head_α:
                        mov              qword ptr [rbp + 2376], rbp
                        mov              rdi, qword ptr [rbp + 2448]
                        mov              rsi, qword ptr [rbp + 2456]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rbp + 2368], r12
                        mov              qword ptr [rbp + 2352], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 2344], rax
                        mov              dword ptr [rbp + 2336], 0
.Lx511_0:
                        mov              r14d, dword ptr [rbp + 2336]
                                                                                        jmp   n373_match_sequence_α
n361_match_head_β:
                        add              dword ptr [rbp + 2336], 1
                        mov              eax, dword ptr [rbp + 2336]
                        cmp              eax, r15d
                                                                                        jg    .Lx511_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx511_1
                                                                                        jmp   .Lx511_0
.Lx511_1:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        mov              r12, qword ptr [rbp + 2368]
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n326_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n362_deref_α:
                        mov              rdi, qword ptr [rbp + 544]
                        mov              rsi, qword ptr [rbp + 552]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n328_var_α
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                                                                                        jmp   n374_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n363_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                                                                                        jmp   n375_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n364_lit_integer_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx514_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n376_subscript_α
.Lx514_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n365_subscript_α:
                        mov              rdi, qword ptr [rbp + 320]
                        mov              rsi, qword ptr [rbp + 328]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n343_var_α
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   n377_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n366_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   n378_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n367_call_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 64], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 72], rax
                        .section         .rodata
.Lrkfn518:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn518]
                        lea              rsi, [rbp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 48], rax
                        mov              qword ptr [rbp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n379_op14_α
                                                                                        jmp   n340_op14_α
n367_call_β:
                                                                                        jmp   n379_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n368_assign_α:
                        mov              rax, qword ptr [rbp + 1632]
                        mov              rdx, qword ptr [rbp + 1640]
                        mov              qword ptr [1879052480], rax
                        mov              qword ptr [1879052488], rdx
                                                                                        jmp   n369_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n369_lit_string_α:
                        mov              qword ptr [rbp + 1744], 1
                        mov              rax, qword ptr [rip + .Lx520_0]
                        mov              qword ptr [rbp + 1752], rax
                                                                                        jmp   n380_call_α
.Lx520_0:
                        .quad            .Lx520_0_s
.Lx520_0_s:
                        .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n370_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx522_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx522_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx522_6]
                        lea              rdx, [rip + .Lx522_7]
                                                                                        jmp   rax
.Lx522_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx522_2
.Lx522_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx522_2
.Lx522_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx522_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx522_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx522_3]
                        lea              rdx, [rip + .Lx522_4]
                                                                                        jmp   rax
.Lx522_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx522_2
.Lx522_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx522_2
.Lx522_1:
                        call             rt_faildescr@PLT
.Lx522_2:
                        mov              qword ptr [rbp + 896], rax
                        mov              qword ptr [rbp + 904], rdx
                        cmp              eax, 99
                                                                                        je    n371_lit_string_α
                                                                                        jmp   n381_var_α
n370_call_β:
                                                                                        jmp   n371_lit_string_α
.Lx522_0:
                        .quad            .Lx522_0_s
.Lx522_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n371_lit_string_α:
                        mov              qword ptr [rbp + 1088], 1
                        mov              rax, qword ptr [rip + .Lx523_0]
                        mov              qword ptr [rbp + 1096], rax
                                                                                        jmp   n382_call_α
.Lx523_0:
                        .quad            .Lx523_0_s
.Lx523_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n372_call_α:
                        mov              rax, qword ptr [rbp + 1200]
                        mov              qword ptr [rbp + 1232], rax
                        mov              rax, qword ptr [rbp + 1208]
                        mov              qword ptr [rbp + 1240], rax
                        .section         .rodata
.Lrkfn525:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn525]
                        lea              rsi, [rbp + 1232]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1216], rax
                        mov              qword ptr [rbp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n384_lit_string_α
                                                                                        jmp   n383_call_α
n372_call_β:
                                                                                        jmp   n384_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n373_match_sequence_α:
                        mov              dword ptr [rbp + 2384], r14d
                                                                                        jmp   n386_lit_integer_α
n373_match_sequence_as:
                                                                                        jmp   n385_match_release_α
n373_match_sequence_β:
                                                                                        jmp   n390_match_rpos_β
n373_match_sequence_af:
                                                                                        jmp   n361_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n374_subscript_α:
                        mov              rdi, qword ptr [rbp + 496]
                        mov              rsi, qword ptr [rbp + 504]
                        mov              rdx, qword ptr [rbp + 560]
                        mov              rcx, qword ptr [rbp + 568]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n328_var_α
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n391_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n375_lit_integer_α:
                        mov              qword ptr [rbp + 688], 6
                        mov              rax, qword ptr [rip + .Lx529_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   n392_subscript_α
.Lx529_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n376_subscript_α:
                        mov              rdi, qword ptr [rbp + 208]
                        mov              rsi, qword ptr [rbp + 216]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n330_var_α
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n393_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n377_deref_α:
                        mov              rdi, qword ptr [rbp + 352]
                        mov              rsi, qword ptr [rbp + 360]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n343_var_α
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n394_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n378_assign_var_α:
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        mov              rdx, qword ptr [rbp + 464]
                        mov              rcx, qword ptr [rbp + 472]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n356_lit_string_α
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   n356_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n379_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n380_call_α:
                        mov              rax, qword ptr [rbp + 1744]
                        mov              qword ptr [rbp + 1712], rax
                        mov              rax, qword ptr [rbp + 1752]
                        mov              qword ptr [rbp + 1720], rax
                        .section         .rodata
.Lrkfn536:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn536]
                        lea              rsi, [rbp + 1712]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1696], rax
                        mov              qword ptr [rbp + 1704], rdx
                        cmp              eax, 99
                                                                                        je    n396_lit_string_α
                                                                                        jmp   n395_assign_α
n380_call_β:
                                                                                        jmp   n396_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n381_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   n397_var_α
#-----------------------------------------------------------------------------------------------------------------------
n382_call_α:
                        mov              rax, qword ptr [rbp + 1088]
                        mov              qword ptr [rbp + 1056], rax
                        mov              rax, qword ptr [rbp + 1096]
                        mov              qword ptr [rbp + 1064], rax
                        .section         .rodata
.Lrkfn539:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn539]
                        lea              rsi, [rbp + 1056]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    n356_lit_string_α
                                                                                        jmp   n398_assign_α
n382_call_β:
                                                                                        jmp   n356_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n383_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx541_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx541_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx541_6]
                        lea              rdx, [rip + .Lx541_7]
                                                                                        jmp   rax
.Lx541_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx541_2
.Lx541_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx541_2
.Lx541_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx541_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx541_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx541_3]
                        lea              rdx, [rip + .Lx541_4]
                                                                                        jmp   rax
.Lx541_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx541_2
.Lx541_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx541_2
.Lx541_1:
                        call             rt_faildescr@PLT
.Lx541_2:
                        mov              qword ptr [rbp + 1264], rax
                        mov              qword ptr [rbp + 1272], rdx
                        cmp              eax, 99
                                                                                        je    n384_lit_string_α
                                                                                        jmp   n399_var_α
n383_call_β:
                                                                                        jmp   n384_lit_string_α
.Lx541_0:
                        .quad            .Lx541_0_s
.Lx541_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n384_lit_string_α:
                        mov              qword ptr [rbp + 1552], 1
                        mov              rax, qword ptr [rip + .Lx542_0]
                        mov              qword ptr [rbp + 1560], rax
                                                                                        jmp   n400_call_α
.Lx542_0:
                        .quad            .Lx542_0_s
.Lx542_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n385_match_release_α:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 2400]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx544_1:
                        test             rax, rax
                                                                                        je    .Lx544_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx544_3]
                        lea              rdx, [rip + .Lx544_4]
                                                                                        jmp   rax
.Lx544_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx544_1
.Lx544_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx544_1
.Lx544_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + 2368]
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n401_call_α
#-----------------------------------------------------------------------------------------------------------------------
n386_lit_integer_α:
                        mov              qword ptr [rbp + 2400], 6
                        mov              rax, qword ptr [rip + .Lx545_0]
                        mov              qword ptr [rbp + 2408], rax
                                                                                        jmp   n387_match_pos_α
n386_lit_integer_β:
                                                                                        jmp   n361_match_head_β
.Lx545_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n387_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n361_match_head_β
                                                                                        jmp   n388_match_defer_α
n387_match_pos_β:
                                                                                        jmp   n361_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n388_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx547_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx547_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx547_10
.Lx547_9:
                        xor              eax, eax
.Lx547_10:
                        test             rax, rax
                                                                                        jz    .Lx547_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx547_4]
                        lea              rdx, [rip + .Lx547_5]
                                                                                        jmp   rax
.Lx547_4:
                                                                                        jmp   n389_lit_integer_α
.Lx547_5:
                                                                                        jmp   n361_match_head_β
.Lx547_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx547_2:
                        test             rax, rax
                                                                                        je    .Lx547_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx547_7]
                        lea              rdx, [rip + .Lx547_8]
                                                                                        jmp   rax
.Lx547_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx547_2
.Lx547_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx547_2
.Lx547_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n361_match_head_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx547_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n389_lit_integer_α
.Lx547_6:
                        add              rsp, 16
                                                                                        jmp   n361_match_head_β
n388_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n389_lit_integer_α:
                        mov              qword ptr [rbp + 2432], 6
                        mov              rax, qword ptr [rip + .Lx548_0]
                        mov              qword ptr [rbp + 2440], rax
                                                                                        jmp   n390_match_rpos_α
n389_lit_integer_β:
                                                                                        jmp   n388_match_defer_β
.Lx548_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n390_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n388_match_defer_β
                                                                                        jmp   n385_match_release_α
n390_match_rpos_β:
                                                                                        jmp   n388_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n391_deref_α:
                        mov              rdi, qword ptr [rbp + 576]
                        mov              rsi, qword ptr [rbp + 584]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n328_var_α
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n402_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n392_subscript_α:
                        mov              rdi, qword ptr [rbp + 672]
                        mov              rsi, qword ptr [rbp + 680]
                        mov              rdx, qword ptr [rbp + 688]
                        mov              rcx, qword ptr [rbp + 696]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n340_op14_α
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                                                                                        jmp   n403_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n393_deref_α:
                        mov              rdi, qword ptr [rbp + 240]
                        mov              rsi, qword ptr [rbp + 248]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n330_var_α
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n404_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n394_subscript_α:
                        mov              rdi, qword ptr [rbp + 304]
                        mov              rsi, qword ptr [rbp + 312]
                        mov              rdx, qword ptr [rbp + 368]
                        mov              rcx, qword ptr [rbp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n343_var_α
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                                                                                        jmp   n405_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n395_assign_α:
                        mov              rax, qword ptr [rbp + 1696]
                        mov              rdx, qword ptr [rbp + 1704]
                        mov              qword ptr [1879052496], rax
                        mov              qword ptr [1879052504], rdx
                                                                                        jmp   n396_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n396_lit_string_α:
                        mov              qword ptr [rbp + 1808], 1
                        mov              rax, qword ptr [rip + .Lx555_0]
                        mov              qword ptr [rbp + 1816], rax
                                                                                        jmp   n406_call_α
.Lx555_0:
                        .quad            .Lx555_0_s
.Lx555_0_s:
                        .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n397_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                                                                                        jmp   n407_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n398_assign_α:
                        mov              rax, qword ptr [rbp + 1040]
                        mov              rdx, qword ptr [rbp + 1048]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n356_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n399_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rbp + 1408], rax
                        mov              qword ptr [rbp + 1416], rdx
                                                                                        jmp   n408_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n400_call_α:
                        mov              rax, qword ptr [rbp + 1552]
                        mov              qword ptr [rbp + 1520], rax
                        mov              rax, qword ptr [rbp + 1560]
                        mov              qword ptr [rbp + 1528], rax
                        .section         .rodata
.Lrkfn560:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn560]
                        lea              rsi, [rbp + 1520]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1504], rax
                        mov              qword ptr [rbp + 1512], rdx
                        cmp              eax, 99
                                                                                        je    n356_lit_string_α
                                                                                        jmp   n409_assign_α
n400_call_β:
                                                                                        jmp   n356_lit_string_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n401_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx562_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx562_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx562_6]
                        lea              rdx, [rip + .Lx562_7]
                                                                                        jmp   rax
.Lx562_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx562_2
.Lx562_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx562_2
.Lx562_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx562_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx562_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx562_3]
                        lea              rdx, [rip + .Lx562_4]
                                                                                        jmp   rax
.Lx562_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx562_2
.Lx562_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx562_2
.Lx562_1:
                        call             rt_faildescr@PLT
.Lx562_2:
                        mov              qword ptr [rbp + 2464], rax
                        mov              qword ptr [rbp + 2472], rdx
                        cmp              eax, 99
                                                                                        je    n325_var_α
                                                                                        jmp   n410_assign_α
n401_call_β:
                                                                                        jmp   n325_var_α
.Lx562_0:
                        .quad            .Lx562_0_s
.Lx562_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n402_assign_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              rdx, qword ptr [rbp + 600]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n328_var_α
#-----------------------------------------------------------------------------------------------------------------------
n403_deref_α:
                        mov              rdi, qword ptr [rbp + 704]
                        mov              rsi, qword ptr [rbp + 712]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n340_op14_α
                        mov              qword ptr [rbp + 720], rax
                        mov              qword ptr [rbp + 728], rdx
                                                                                        jmp   n411_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n404_lit_integer_α:
                        mov              qword ptr [rbp + 272], 6
                        mov              rax, qword ptr [rip + .Lx565_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n412_binop_α
.Lx565_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n405_assign_α:
                        mov              rax, qword ptr [rbp + 384]
                        mov              rdx, qword ptr [rbp + 392]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n343_var_α
#-----------------------------------------------------------------------------------------------------------------------
n406_call_α:
                        mov              rax, qword ptr [rbp + 1808]
                        mov              qword ptr [rbp + 1776], rax
                        mov              rax, qword ptr [rbp + 1816]
                        mov              qword ptr [rbp + 1784], rax
                        .section         .rodata
.Lrkfn568:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn568]
                        lea              rsi, [rbp + 1776]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1760], rax
                        mov              qword ptr [rbp + 1768], rdx
                        cmp              eax, 99
                                                                                        je    n414_lit_string_α
                                                                                        jmp   n413_assign_α
n406_call_β:
                                                                                        jmp   n414_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n407_binop_α:
                        mov              rdi, qword ptr [rbp + 992]
                        mov              rsi, qword ptr [rbp + 1000]
                        mov              rdx, qword ptr [rbp + 1008]
                        mov              rcx, qword ptr [rbp + 1016]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 976], rax
                        mov              qword ptr [rbp + 984], rdx
                                                                                        jmp   n415_call_α
#-----------------------------------------------------------------------------------------------------------------------
n408_lit_string_α:
                        mov              qword ptr [rbp + 1424], 1
                        mov              rax, qword ptr [rip + .Lx570_0]
                        mov              qword ptr [rbp + 1432], rax
                                                                                        jmp   n416_binop_α
.Lx570_0:
                        .quad            .Lx570_0_s
.Lx570_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n409_assign_α:
                        mov              rax, qword ptr [rbp + 1504]
                        mov              rdx, qword ptr [rbp + 1512]
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n356_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n410_assign_α:
                        mov              rsi, qword ptr [rbp + 2464]
                        mov              rdx, qword ptr [rbp + 2472]
                        mov              rdi, qword ptr [rip + .Lx572_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n325_var_α
.Lx572_0:
                        .quad            .Lx572_0_s
.Lx572_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n411_lit_integer_α:
                        mov              qword ptr [rbp + 736], 6
                        mov              rax, qword ptr [rip + .Lx573_0]
                        mov              qword ptr [rbp + 744], rax
                                                                                        jmp   n417_binop_α
.Lx573_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n412_binop_α:
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 100
                                                                                        je    .Lx574_0
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 6
                                                                                        jne   .Lx574_2
.Lx574_1:
                        mov              rax, qword ptr [rbp + 264]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rbp + 192], 6
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n418_assign_var_α
.Lx574_0:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 0
                        lea              r9, [rbp + 192]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n418_assign_var_α
.Lx574_2:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 0
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n330_var_α
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   n418_assign_var_α
n412_binop_β:
                                                                                        jmp   n330_var_α
#-----------------------------------------------------------------------------------------------------------------------
n413_assign_α:
                        mov              rax, qword ptr [rbp + 1760]
                        mov              rdx, qword ptr [rbp + 1768]
                        mov              qword ptr [1879052512], rax
                        mov              qword ptr [1879052520], rdx
                                                                                        jmp   n414_lit_string_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n414_lit_string_α:
                        mov              qword ptr [rbp + 1872], 1
                        mov              rax, qword ptr [rip + .Lx576_0]
                        mov              qword ptr [rbp + 1880], rax
                                                                                        jmp   n419_call_α
.Lx576_0:
                        .quad            .Lx576_0_s
.Lx576_0_s:
                        .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n415_call_α:
                        mov              rax, qword ptr [rbp + 976]
                        mov              qword ptr [rbp + 944], rax
                        mov              rax, qword ptr [rbp + 984]
                        mov              qword ptr [rbp + 952], rax
                        .section         .rodata
.Lrkfn578:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn578]
                        lea              rsi, [rbp + 944]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 928], rax
                        mov              qword ptr [rbp + 936], rdx
                        cmp              eax, 99
                                                                                        je    n371_lit_string_α
                                                                                        jmp   n420_assign_var_α
n415_call_β:
                                                                                        jmp   n371_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n416_binop_α:
                        mov              rdi, qword ptr [rbp + 1408]
                        mov              rsi, qword ptr [rbp + 1416]
                        mov              rdx, qword ptr [rbp + 1424]
                        mov              rcx, qword ptr [rbp + 1432]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                                                                                        jmp   n421_var_α
#-----------------------------------------------------------------------------------------------------------------------
n417_binop_α:
                        mov              eax, dword ptr [rbp + 720]
                        cmp              eax, 100
                                                                                        je    .Lx580_0
                        mov              eax, dword ptr [rbp + 720]
                        cmp              eax, 6
                                                                                        jne   .Lx580_2
.Lx580_1:
                        mov              rax, qword ptr [rbp + 728]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 656], 6
                        mov              qword ptr [rbp + 664], rax
                                                                                        jmp   n422_assign_var_α
.Lx580_0:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        mov              r8d, 1
                        lea              r9, [rbp + 656]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n422_assign_var_α
.Lx580_2:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        mov              r8d, 1
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n340_op14_α
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n422_assign_var_α
n417_binop_β:
                                                                                        jmp   n340_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n418_assign_var_α:
                        mov              rdi, qword ptr [rbp + 176]
                        mov              rsi, qword ptr [rbp + 184]
                        mov              rdx, qword ptr [rbp + 192]
                        mov              rcx, qword ptr [rbp + 200]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n330_var_α
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                                                                                        jmp   n330_var_α
#-----------------------------------------------------------------------------------------------------------------------
n419_call_α:
                        mov              rax, qword ptr [rbp + 1872]
                        mov              qword ptr [rbp + 1840], rax
                        mov              rax, qword ptr [rbp + 1880]
                        mov              qword ptr [rbp + 1848], rax
                        .section         .rodata
.Lrkfn583:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn583]
                        lea              rsi, [rbp + 1840]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1824], rax
                        mov              qword ptr [rbp + 1832], rdx
                        cmp              eax, 99
                                                                                        je    n424_lit_string_α
                                                                                        jmp   n423_assign_α
n419_call_β:
                                                                                        jmp   n424_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n420_assign_var_α:
                        mov              rdi, qword ptr [rbp + 896]
                        mov              rsi, qword ptr [rbp + 904]
                        mov              rdx, qword ptr [rbp + 928]
                        mov              rcx, qword ptr [rbp + 936]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n371_lit_string_α
                        mov              qword ptr [rbp + 1024], rax
                        mov              qword ptr [rbp + 1032], rdx
                                                                                        jmp   n371_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n421_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 1440], rax
                        mov              qword ptr [rbp + 1448], rdx
                                                                                        jmp   n425_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n422_assign_var_α:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 656]
                        mov              rcx, qword ptr [rbp + 664]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n340_op14_α
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   n340_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n423_assign_α:
                        mov              rax, qword ptr [rbp + 1824]
                        mov              rdx, qword ptr [rbp + 1832]
                        mov              qword ptr [1879052528], rax
                        mov              qword ptr [1879052536], rdx
                                                                                        jmp   n424_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n424_lit_string_α:
                        mov              qword ptr [rbp + 1936], 1
                        mov              rax, qword ptr [rip + .Lx588_0]
                        mov              qword ptr [rbp + 1944], rax
                                                                                        jmp   n426_call_α
.Lx588_0:
                        .quad            .Lx588_0_s
.Lx588_0_s:
                        .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n425_binop_α:
                        mov              rdi, qword ptr [rbp + 1392]
                        mov              rsi, qword ptr [rbp + 1400]
                        mov              rdx, qword ptr [rbp + 1440]
                        mov              rcx, qword ptr [rbp + 1448]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                                                                                        jmp   n427_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n426_call_α:
                        mov              rax, qword ptr [rbp + 1936]
                        mov              qword ptr [rbp + 1904], rax
                        mov              rax, qword ptr [rbp + 1944]
                        mov              qword ptr [rbp + 1912], rax
                        .section         .rodata
.Lrkfn591:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn591]
                        lea              rsi, [rbp + 1904]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1888], rax
                        mov              qword ptr [rbp + 1896], rdx
                        cmp              eax, 99
                                                                                        je    n429_lit_string_α
                                                                                        jmp   n428_assign_α
n426_call_β:
                                                                                        jmp   n429_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n427_lit_string_α:
                        mov              qword ptr [rbp + 1456], 1
                        mov              rax, qword ptr [rip + .Lx592_0]
                        mov              qword ptr [rbp + 1464], rax
                                                                                        jmp   n430_binop_α
.Lx592_0:
                        .quad            .Lx592_0_s
.Lx592_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n428_assign_α:
                        mov              rax, qword ptr [rbp + 1888]
                        mov              rdx, qword ptr [rbp + 1896]
                        mov              qword ptr [1879052544], rax
                        mov              qword ptr [1879052552], rdx
                                                                                        jmp   n429_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n429_lit_string_α:
                        mov              qword ptr [rbp + 2000], 1
                        mov              rax, qword ptr [rip + .Lx594_0]
                        mov              qword ptr [rbp + 2008], rax
                                                                                        jmp   n431_call_α
.Lx594_0:
                        .quad            .Lx594_0_s
.Lx594_0_s:
                        .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n430_binop_α:
                        mov              rdi, qword ptr [rbp + 1376]
                        mov              rsi, qword ptr [rbp + 1384]
                        mov              rdx, qword ptr [rbp + 1456]
                        mov              rcx, qword ptr [rbp + 1464]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1360], rax
                        mov              qword ptr [rbp + 1368], rdx
                                                                                        jmp   n432_var_α
#-----------------------------------------------------------------------------------------------------------------------
n431_call_α:
                        mov              rax, qword ptr [rbp + 2000]
                        mov              qword ptr [rbp + 1968], rax
                        mov              rax, qword ptr [rbp + 2008]
                        mov              qword ptr [rbp + 1976], rax
                        .section         .rodata
.Lrkfn597:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn597]
                        lea              rsi, [rbp + 1968]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1952], rax
                        mov              qword ptr [rbp + 1960], rdx
                        cmp              eax, 99
                                                                                        je    n434_lit_string_α
                                                                                        jmp   n433_assign_α
n431_call_β:
                                                                                        jmp   n434_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n432_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rbp + 1472], rax
                        mov              qword ptr [rbp + 1480], rdx
                                                                                        jmp   n435_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n433_assign_α:
                        mov              rax, qword ptr [rbp + 1952]
                        mov              rdx, qword ptr [rbp + 1960]
                        mov              qword ptr [1879052560], rax
                        mov              qword ptr [1879052568], rdx
                                                                                        jmp   n434_lit_string_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n434_lit_string_α:
                        mov              qword ptr [rbp + 2064], 1
                        mov              rax, qword ptr [rip + .Lx600_0]
                        mov              qword ptr [rbp + 2072], rax
                                                                                        jmp   n436_call_α
.Lx600_0:
                        .quad            .Lx600_0_s
.Lx600_0_s:
                        .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n435_binop_α:
                        mov              rdi, qword ptr [rbp + 1360]
                        mov              rsi, qword ptr [rbp + 1368]
                        mov              rdx, qword ptr [rbp + 1472]
                        mov              rcx, qword ptr [rbp + 1480]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1344], rax
                        mov              qword ptr [rbp + 1352], rdx
                                                                                        jmp   n437_call_α
#-----------------------------------------------------------------------------------------------------------------------
n436_call_α:
                        mov              rax, qword ptr [rbp + 2064]
                        mov              qword ptr [rbp + 2032], rax
                        mov              rax, qword ptr [rbp + 2072]
                        mov              qword ptr [rbp + 2040], rax
                        .section         .rodata
.Lrkfn603:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn603]
                        lea              rsi, [rbp + 2032]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2016], rax
                        mov              qword ptr [rbp + 2024], rdx
                        cmp              eax, 99
                                                                                        je    n439_lit_string_α
                                                                                        jmp   n438_assign_α
n436_call_β:
                                                                                        jmp   n439_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n437_call_α:
                        mov              rax, qword ptr [rbp + 1344]
                        mov              qword ptr [rbp + 1312], rax
                        mov              rax, qword ptr [rbp + 1352]
                        mov              qword ptr [rbp + 1320], rax
                        .section         .rodata
.Lrkfn605:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn605]
                        lea              rsi, [rbp + 1312]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1296], rax
                        mov              qword ptr [rbp + 1304], rdx
                        cmp              eax, 99
                                                                                        je    n384_lit_string_α
                                                                                        jmp   n440_assign_var_α
n437_call_β:
                                                                                        jmp   n384_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n438_assign_α:
                        mov              rax, qword ptr [rbp + 2016]
                        mov              rdx, qword ptr [rbp + 2024]
                        mov              qword ptr [1879052576], rax
                        mov              qword ptr [1879052584], rdx
                                                                                        jmp   n439_lit_string_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n439_lit_string_α:
                        mov              qword ptr [rbp + 2128], 1
                        mov              rax, qword ptr [rip + .Lx607_0]
                        mov              qword ptr [rbp + 2136], rax
                                                                                        jmp   n441_call_α
.Lx607_0:
                        .quad            .Lx607_0_s
.Lx607_0_s:
                        .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n440_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1264]
                        mov              rsi, qword ptr [rbp + 1272]
                        mov              rdx, qword ptr [rbp + 1296]
                        mov              rcx, qword ptr [rbp + 1304]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n384_lit_string_α
                        mov              qword ptr [rbp + 1488], rax
                        mov              qword ptr [rbp + 1496], rdx
                                                                                        jmp   n384_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n441_call_α:
                        mov              rax, qword ptr [rbp + 2128]
                        mov              qword ptr [rbp + 2096], rax
                        mov              rax, qword ptr [rbp + 2136]
                        mov              qword ptr [rbp + 2104], rax
                        .section         .rodata
.Lrkfn610:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn610]
                        lea              rsi, [rbp + 2096]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2080], rax
                        mov              qword ptr [rbp + 2088], rdx
                        cmp              eax, 99
                                                                                        je    n443_lit_string_α
                                                                                        jmp   n442_assign_α
n441_call_β:
                                                                                        jmp   n443_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n442_assign_α:
                        mov              rax, qword ptr [rbp + 2080]
                        mov              rdx, qword ptr [rbp + 2088]
                        mov              qword ptr [1879052592], rax
                        mov              qword ptr [1879052600], rdx
                                                                                        jmp   n443_lit_string_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n443_lit_string_α:
                        mov              qword ptr [rbp + 2192], 1
                        mov              rax, qword ptr [rip + .Lx612_0]
                        mov              qword ptr [rbp + 2200], rax
                                                                                        jmp   n444_call_α
.Lx612_0:
                        .quad            .Lx612_0_s
.Lx612_0_s:
                        .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n444_call_α:
                        mov              rax, qword ptr [rbp + 2192]
                        mov              qword ptr [rbp + 2160], rax
                        mov              rax, qword ptr [rbp + 2200]
                        mov              qword ptr [rbp + 2168], rax
                        .section         .rodata
.Lrkfn614:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn614]
                        lea              rsi, [rbp + 2160]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2144], rax
                        mov              qword ptr [rbp + 2152], rdx
                        cmp              eax, 99
                                                                                        je    n446_lit_string_α
                                                                                        jmp   n445_assign_α
n444_call_β:
                                                                                        jmp   n446_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n445_assign_α:
                        mov              rax, qword ptr [rbp + 2144]
                        mov              rdx, qword ptr [rbp + 2152]
                        mov              qword ptr [1879052608], rax
                        mov              qword ptr [1879052616], rdx
                                                                                        jmp   n446_lit_string_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n446_lit_string_α:
                        mov              qword ptr [rbp + 2272], 1
                        mov              rax, qword ptr [rip + .Lx616_0]
                        mov              qword ptr [rbp + 2280], rax
                                                                                        jmp   n447_lit_integer_α
.Lx616_0:
                        .quad            .Lx616_0_s
.Lx616_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n447_lit_integer_α:
                        mov              qword ptr [rbp + 2288], 6
                        mov              rax, qword ptr [rip + .Lx617_0]
                        mov              qword ptr [rbp + 2296], rax
                                                                                        jmp   n448_call_α
.Lx617_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n448_call_α:
                        mov              rax, qword ptr [rbp + 2272]
                        mov              qword ptr [rbp + 2224], rax
                        mov              rax, qword ptr [rbp + 2280]
                        mov              qword ptr [rbp + 2232], rax
                        mov              rax, qword ptr [rbp + 2288]
                        mov              qword ptr [rbp + 2240], rax
                        mov              rax, qword ptr [rbp + 2296]
                        mov              qword ptr [rbp + 2248], rax
                        .section         .rodata
.Lrkfn619:              .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn619]
                        lea              rsi, [rbp + 2224]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2208], rax
                        mov              qword ptr [rbp + 2216], rdx
                        cmp              eax, 99
                                                                                        je    n325_var_α
                                                                                        jmp   n325_var_α
n448_call_β:
                                                                                        jmp   n325_var_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2520]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_ω:
                        mov              rax, [rbp + 2528]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Unary_α
proc_LBL__Unary_α:
                        .global          proc_LBL__Unary_α
                        .global          proc_LBL__Unary_β
                        .global          proc_LBL__Unary_γ
                        .global          proc_LBL__Unary_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              [rsp + 2536], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__Unary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n620_goto_α:
                                                                                        jmp   n630_call_α
n620_goto_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
n621_goto_α:
                                                                                        jmp   n631_var_α
n621_goto_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
n622_goto_α:
                                                                                        jmp   n632_lit_string_α
n622_goto_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
n623_goto_α:
                                                                                        jmp   n633_var_α
n623_goto_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
n624_goto_α:
                                                                                        jmp   n632_lit_string_α
n624_goto_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
n625_goto_α:
                                                                                        jmp   n632_lit_string_α
n625_goto_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
n626_goto_α:
                                                                                        jmp   n634_call_α
n626_goto_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
n627_goto_α:
                                                                                        jmp   n632_lit_string_α
n627_goto_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
n628_goto_α:
                                                                                        jmp   n635_var_α
n628_goto_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
n629_goto_α:
                                                                                        jmp   n636_lit_string_α
n629_goto_β:
                                                                                        jmp   proc_LBL__Unary_ω
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n630_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx770_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx770_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx770_6]
                        lea              rdx, [rip + .Lx770_7]
                                                                                        jmp   rax
.Lx770_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx770_2
.Lx770_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx770_2
.Lx770_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx770_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx770_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx770_3]
                        lea              rdx, [rip + .Lx770_4]
                                                                                        jmp   rax
.Lx770_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx770_2
.Lx770_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx770_2
.Lx770_1:
                        call             rt_faildescr@PLT
.Lx770_2:
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                        cmp              eax, 99
                                                                                        je    n638_call_α
                                                                                        jmp   n637_assign_α
n630_call_β:
                                                                                        jmp   n638_call_α
.Lx770_0:
                        .quad            .Lx770_0_s
.Lx770_0_s:
                        .string          "Pop"
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n631_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                                                                                        jmp   n639_lit_integer_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n632_lit_string_α:
                        mov              qword ptr [rbp + 1616], 1
                        mov              rax, qword ptr [rip + .Lx772_0]
                        mov              qword ptr [rbp + 1624], rax
                                                                                        jmp   n641_call_α
.Lx772_0:
                        .quad            .Lx772_0_s
.Lx772_0_s:
                        .string          "PAT$0"
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n633_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 496], rax
                        mov              qword ptr [rbp + 504], rdx
                                                                                        jmp   n642_var_α
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n634_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx775_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx775_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx775_6]
                        lea              rdx, [rip + .Lx775_7]
                                                                                        jmp   rax
.Lx775_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx775_2
.Lx775_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx775_2
.Lx775_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx775_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx775_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx775_3]
                        lea              rdx, [rip + .Lx775_4]
                                                                                        jmp   rax
.Lx775_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx775_2
.Lx775_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx775_2
.Lx775_1:
                        call             rt_faildescr@PLT
.Lx775_2:
                        mov              qword ptr [rbp + 1104], rax
                        mov              qword ptr [rbp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n645_call_α
                                                                                        jmp   n644_assign_α
n634_call_β:
                                                                                        jmp   n645_call_α
.Lx775_0:
                        .quad            .Lx775_0_s
.Lx775_0_s:
                        .string          "Pop"
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n635_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx776_0]
                        call             NV_GET_fn@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx776_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__Unary_γ
.Lx776_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n646_assign_α
.Lx776_0:
                        .quad            .Lx776_0_s
.Lx776_0_s:
                        .string          "INPUT"
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n636_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx777_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n647_assign_α
.Lx777_0:
                        .quad            .Lx777_0_s
.Lx777_0_s:
                        .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n637_assign_α:
                        mov              rax, qword ptr [rbp + 768]
                        mov              rdx, qword ptr [rbp + 776]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n638_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n638_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx780_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx780_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx780_6]
                        lea              rdx, [rip + .Lx780_7]
                                                                                        jmp   rax
.Lx780_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx780_2
.Lx780_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx780_2
.Lx780_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx780_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx780_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx780_3]
                        lea              rdx, [rip + .Lx780_4]
                                                                                        jmp   rax
.Lx780_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx780_2
.Lx780_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx780_2
.Lx780_1:
                        call             rt_faildescr@PLT
.Lx780_2:
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n649_lit_string_α
                                                                                        jmp   n648_assign_α
n638_call_β:
                                                                                        jmp   n649_lit_string_α
.Lx780_0:
                        .quad            .Lx780_0_s
.Lx780_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n639_lit_integer_α:
                        mov              qword ptr [rbp + 160], 6
                        mov              rax, qword ptr [rip + .Lx781_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n650_subscript_α
.Lx781_0:
                        .quad            0
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n640_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                                                                                        jmp   n651_var_α
#-----------------------------------------------------------------------------------------------------------------------
n641_call_α:
                        mov              rax, qword ptr [rbp + 1616]
                        mov              qword ptr [rbp + 1584], rax
                        mov              rax, qword ptr [rbp + 1624]
                        mov              qword ptr [rbp + 1592], rax
                        .section         .rodata
.Lrkfn784:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn784]
                        lea              rsi, [rbp + 1584]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1568], rax
                        mov              qword ptr [rbp + 1576], rdx
                        cmp              eax, 99
                                                                                        je    n654_lit_string_α
                                                                                        jmp   n653_assign_α
n641_call_β:
                                                                                        jmp   n654_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n642_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n655_lit_integer_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n643_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   n656_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n644_assign_α:
                        mov              rax, qword ptr [rbp + 1104]
                        mov              rdx, qword ptr [rbp + 1112]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n645_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n645_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx789_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx789_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx789_6]
                        lea              rdx, [rip + .Lx789_7]
                                                                                        jmp   rax
.Lx789_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx789_2
.Lx789_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx789_2
.Lx789_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx789_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx789_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx789_3]
                        lea              rdx, [rip + .Lx789_4]
                                                                                        jmp   rax
.Lx789_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx789_2
.Lx789_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx789_2
.Lx789_1:
                        call             rt_faildescr@PLT
.Lx789_2:
                        mov              qword ptr [rbp + 1136], rax
                        mov              qword ptr [rbp + 1144], rdx
                        cmp              eax, 99
                                                                                        je    n659_call_α
                                                                                        jmp   n658_assign_α
n645_call_β:
                                                                                        jmp   n659_call_α
.Lx789_0:
                        .quad            .Lx789_0_s
.Lx789_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n646_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052624], rax
                        mov              qword ptr [1879052632], rdx
                                                                                        jmp   n660_var_α
#-----------------------------------------------------------------------------------------------------------------------
n647_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx791_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n635_var_α
.Lx791_0:
                        .quad            .Lx791_0_s
.Lx791_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n648_assign_α:
                        mov              rax, qword ptr [rbp + 800]
                        mov              rdx, qword ptr [rbp + 808]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n649_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n649_lit_string_α:
                        mov              qword ptr [rbp + 832], 1
                        mov              rax, qword ptr [rip + .Lx793_0]
                        mov              qword ptr [rbp + 840], rax
                                                                                        jmp   n661_call_α
.Lx793_0:
                        .quad            .Lx793_0_s
.Lx793_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n650_subscript_α:
                        mov              rdi, qword ptr [rbp + 144]
                        mov              rsi, qword ptr [rbp + 152]
                        mov              rdx, qword ptr [rbp + 160]
                        mov              rcx, qword ptr [rbp + 168]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n640_var_α
                        mov              qword ptr [rbp + 176], rax
                        mov              qword ptr [rbp + 184], rdx
                                                                                        jmp   n662_var_α
#-----------------------------------------------------------------------------------------------------------------------
n651_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n663_lit_integer_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n652_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n664_call_α
#-----------------------------------------------------------------------------------------------------------------------
n653_assign_α:
                        mov              rax, qword ptr [rbp + 1568]
                        mov              rdx, qword ptr [rbp + 1576]
                        mov              qword ptr [1879052464], rax
                        mov              qword ptr [1879052472], rdx
                                                                                        jmp   n654_lit_string_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n654_lit_string_α:
                        mov              qword ptr [rbp + 1680], 1
                        mov              rax, qword ptr [rip + .Lx798_0]
                        mov              qword ptr [rbp + 1688], rax
                                                                                        jmp   n666_call_α
.Lx798_0:
                        .quad            .Lx798_0_s
.Lx798_0_s:
                        .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n655_lit_integer_α:
                        mov              qword ptr [rbp + 528], 6
                        mov              rax, qword ptr [rip + .Lx799_0]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n667_subscript_α
.Lx799_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n656_lit_integer_α:
                        mov              qword ptr [rbp + 624], 6
                        mov              rax, qword ptr [rip + .Lx800_0]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n668_subscript_α
.Lx800_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n657_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n658_assign_α:
                        mov              rax, qword ptr [rbp + 1136]
                        mov              rdx, qword ptr [rbp + 1144]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n659_call_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n659_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx805_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx805_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx805_6]
                        lea              rdx, [rip + .Lx805_7]
                                                                                        jmp   rax
.Lx805_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx805_2
.Lx805_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx805_2
.Lx805_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx805_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx805_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx805_3]
                        lea              rdx, [rip + .Lx805_4]
                                                                                        jmp   rax
.Lx805_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx805_2
.Lx805_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx805_2
.Lx805_1:
                        call             rt_faildescr@PLT
.Lx805_2:
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n670_lit_string_α
                                                                                        jmp   n669_assign_α
n659_call_β:
                                                                                        jmp   n670_lit_string_α
.Lx805_0:
                        .quad            .Lx805_0_s
.Lx805_0_s:
                        .string          "Pop"
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n660_var_α:
                        mov              rax, qword ptr [1879052624]
                        mov              rdx, qword ptr [1879052632]
                        mov              qword ptr [rbp + 2448], rax
                        mov              qword ptr [rbp + 2456], rdx
                                                                                        jmp   n671_match_head_α
#-----------------------------------------------------------------------------------------------------------------------
n661_call_α:
                        mov              rax, qword ptr [rbp + 832]
                        mov              qword ptr [rbp + 864], rax
                        mov              rax, qword ptr [rbp + 840]
                        mov              qword ptr [rbp + 872], rax
                        .section         .rodata
.Lrkfn808:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn808]
                        lea              rsi, [rbp + 864]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n673_lit_string_α
                                                                                        jmp   n672_call_α
n661_call_β:
                                                                                        jmp   n673_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n662_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                                                                                        jmp   n674_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n663_lit_integer_α:
                        mov              qword ptr [rbp + 336], 6
                        mov              rax, qword ptr [rip + .Lx810_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n675_subscript_α
.Lx810_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n664_call_α:
                        mov              rax, qword ptr [rbp + 448]
                        mov              qword ptr [rbp + 416], rax
                        mov              rax, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 424], rax
                        .section         .rodata
.Lrkfn812:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn812]
                        lea              rsi, [rbp + 416]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n665_lit_string_α
                                                                                        jmp   n676_var_α
n664_call_β:
                                                                                        jmp   n665_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n665_lit_string_α:
                        mov              qword ptr [rbp + 32], 1
                        mov              rax, qword ptr [rip + .Lx813_0]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n677_call_α
.Lx813_0:
                        .quad            .Lx813_0_s
.Lx813_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n666_call_α:
                        mov              rax, qword ptr [rbp + 1680]
                        mov              qword ptr [rbp + 1648], rax
                        mov              rax, qword ptr [rbp + 1688]
                        mov              qword ptr [rbp + 1656], rax
                        .section         .rodata
.Lrkfn815:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn815]
                        lea              rsi, [rbp + 1648]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1632], rax
                        mov              qword ptr [rbp + 1640], rdx
                        cmp              eax, 99
                                                                                        je    n679_lit_string_α
                                                                                        jmp   n678_assign_α
n666_call_β:
                                                                                        jmp   n679_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n667_subscript_α:
                        mov              rdi, qword ptr [rbp + 512]
                        mov              rsi, qword ptr [rbp + 520]
                        mov              rdx, qword ptr [rbp + 528]
                        mov              rcx, qword ptr [rbp + 536]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n643_var_α
                        mov              qword ptr [rbp + 544], rax
                        mov              qword ptr [rbp + 552], rdx
                                                                                        jmp   n680_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n668_subscript_α:
                        mov              rdi, qword ptr [rbp + 608]
                        mov              rsi, qword ptr [rbp + 616]
                        mov              rdx, qword ptr [rbp + 624]
                        mov              rcx, qword ptr [rbp + 632]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n657_op14_α
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   n681_var_α
#-----------------------------------------------------------------------------------------------------------------------
n669_assign_α:
                        mov              rax, qword ptr [rbp + 1168]
                        mov              rdx, qword ptr [rbp + 1176]
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n670_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n670_lit_string_α:
                        mov              qword ptr [rbp + 1200], 1
                        mov              rax, qword ptr [rip + .Lx819_0]
                        mov              qword ptr [rbp + 1208], rax
                                                                                        jmp   n682_call_α
.Lx819_0:
                        .quad            .Lx819_0_s
.Lx819_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n671_match_head_α:
                        mov              qword ptr [rbp + 2376], rbp
                        mov              rdi, qword ptr [rbp + 2448]
                        mov              rsi, qword ptr [rbp + 2456]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rbp + 2368], r12
                        mov              qword ptr [rbp + 2352], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 2344], rax
                        mov              dword ptr [rbp + 2336], 0
.Lx821_0:
                        mov              r14d, dword ptr [rbp + 2336]
                                                                                        jmp   n683_match_sequence_α
n671_match_head_β:
                        add              dword ptr [rbp + 2336], 1
                        mov              eax, dword ptr [rbp + 2336]
                        cmp              eax, r15d
                                                                                        jg    .Lx821_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx821_1
                                                                                        jmp   .Lx821_0
.Lx821_1:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        mov              r12, qword ptr [rbp + 2368]
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n636_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n672_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx823_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx823_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx823_6]
                        lea              rdx, [rip + .Lx823_7]
                                                                                        jmp   rax
.Lx823_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx823_2
.Lx823_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx823_2
.Lx823_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx823_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx823_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx823_3]
                        lea              rdx, [rip + .Lx823_4]
                                                                                        jmp   rax
.Lx823_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx823_2
.Lx823_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx823_2
.Lx823_1:
                        call             rt_faildescr@PLT
.Lx823_2:
                        mov              qword ptr [rbp + 896], rax
                        mov              qword ptr [rbp + 904], rdx
                        cmp              eax, 99
                                                                                        je    n673_lit_string_α
                                                                                        jmp   n684_var_α
n672_call_β:
                                                                                        jmp   n673_lit_string_α
.Lx823_0:
                        .quad            .Lx823_0_s
.Lx823_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n673_lit_string_α:
                        mov              qword ptr [rbp + 1088], 1
                        mov              rax, qword ptr [rip + .Lx824_0]
                        mov              qword ptr [rbp + 1096], rax
                                                                                        jmp   n685_call_α
.Lx824_0:
                        .quad            .Lx824_0_s
.Lx824_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n674_lit_integer_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx825_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n686_subscript_α
.Lx825_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n675_subscript_α:
                        mov              rdi, qword ptr [rbp + 320]
                        mov              rsi, qword ptr [rbp + 328]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n652_var_α
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   n687_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n676_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   n688_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n677_call_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 64], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 72], rax
                        .section         .rodata
.Lrkfn829:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn829]
                        lea              rsi, [rbp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 48], rax
                        mov              qword ptr [rbp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n689_op14_α
                                                                                        jmp   n657_op14_α
n677_call_β:
                                                                                        jmp   n689_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n678_assign_α:
                        mov              rax, qword ptr [rbp + 1632]
                        mov              rdx, qword ptr [rbp + 1640]
                        mov              qword ptr [1879052480], rax
                        mov              qword ptr [1879052488], rdx
                                                                                        jmp   n679_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n679_lit_string_α:
                        mov              qword ptr [rbp + 1744], 1
                        mov              rax, qword ptr [rip + .Lx831_0]
                        mov              qword ptr [rbp + 1752], rax
                                                                                        jmp   n690_call_α
.Lx831_0:
                        .quad            .Lx831_0_s
.Lx831_0_s:
                        .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n680_deref_α:
                        mov              rdi, qword ptr [rbp + 544]
                        mov              rsi, qword ptr [rbp + 552]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n643_var_α
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                                                                                        jmp   n691_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n681_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                                                                                        jmp   n692_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n682_call_α:
                        mov              rax, qword ptr [rbp + 1200]
                        mov              qword ptr [rbp + 1232], rax
                        mov              rax, qword ptr [rbp + 1208]
                        mov              qword ptr [rbp + 1240], rax
                        .section         .rodata
.Lrkfn835:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn835]
                        lea              rsi, [rbp + 1232]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1216], rax
                        mov              qword ptr [rbp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n694_lit_string_α
                                                                                        jmp   n693_call_α
n682_call_β:
                                                                                        jmp   n694_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n683_match_sequence_α:
                        mov              dword ptr [rbp + 2384], r14d
                                                                                        jmp   n696_lit_integer_α
n683_match_sequence_as:
                                                                                        jmp   n695_match_release_α
n683_match_sequence_β:
                                                                                        jmp   n700_match_rpos_β
n683_match_sequence_af:
                                                                                        jmp   n671_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n684_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   n701_var_α
#-----------------------------------------------------------------------------------------------------------------------
n685_call_α:
                        mov              rax, qword ptr [rbp + 1088]
                        mov              qword ptr [rbp + 1056], rax
                        mov              rax, qword ptr [rbp + 1096]
                        mov              qword ptr [rbp + 1064], rax
                        .section         .rodata
.Lrkfn840:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn840]
                        lea              rsi, [rbp + 1056]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    n665_lit_string_α
                                                                                        jmp   n702_assign_α
n685_call_β:
                                                                                        jmp   n665_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n686_subscript_α:
                        mov              rdi, qword ptr [rbp + 208]
                        mov              rsi, qword ptr [rbp + 216]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n640_var_α
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n703_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n687_deref_α:
                        mov              rdi, qword ptr [rbp + 352]
                        mov              rsi, qword ptr [rbp + 360]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n652_var_α
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n704_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n688_assign_var_α:
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        mov              rdx, qword ptr [rbp + 464]
                        mov              rcx, qword ptr [rbp + 472]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n665_lit_string_α
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   n665_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n689_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n690_call_α:
                        mov              rax, qword ptr [rbp + 1744]
                        mov              qword ptr [rbp + 1712], rax
                        mov              rax, qword ptr [rbp + 1752]
                        mov              qword ptr [rbp + 1720], rax
                        .section         .rodata
.Lrkfn847:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn847]
                        lea              rsi, [rbp + 1712]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1696], rax
                        mov              qword ptr [rbp + 1704], rdx
                        cmp              eax, 99
                                                                                        je    n706_lit_string_α
                                                                                        jmp   n705_assign_α
n690_call_β:
                                                                                        jmp   n706_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n691_subscript_α:
                        mov              rdi, qword ptr [rbp + 496]
                        mov              rsi, qword ptr [rbp + 504]
                        mov              rdx, qword ptr [rbp + 560]
                        mov              rcx, qword ptr [rbp + 568]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n643_var_α
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n707_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n692_lit_integer_α:
                        mov              qword ptr [rbp + 688], 6
                        mov              rax, qword ptr [rip + .Lx849_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   n708_subscript_α
.Lx849_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n693_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx851_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx851_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx851_6]
                        lea              rdx, [rip + .Lx851_7]
                                                                                        jmp   rax
.Lx851_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx851_2
.Lx851_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx851_2
.Lx851_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx851_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx851_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx851_3]
                        lea              rdx, [rip + .Lx851_4]
                                                                                        jmp   rax
.Lx851_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx851_2
.Lx851_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx851_2
.Lx851_1:
                        call             rt_faildescr@PLT
.Lx851_2:
                        mov              qword ptr [rbp + 1264], rax
                        mov              qword ptr [rbp + 1272], rdx
                        cmp              eax, 99
                                                                                        je    n694_lit_string_α
                                                                                        jmp   n709_var_α
n693_call_β:
                                                                                        jmp   n694_lit_string_α
.Lx851_0:
                        .quad            .Lx851_0_s
.Lx851_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n694_lit_string_α:
                        mov              qword ptr [rbp + 1552], 1
                        mov              rax, qword ptr [rip + .Lx852_0]
                        mov              qword ptr [rbp + 1560], rax
                                                                                        jmp   n710_call_α
.Lx852_0:
                        .quad            .Lx852_0_s
.Lx852_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n695_match_release_α:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 2400]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx854_1:
                        test             rax, rax
                                                                                        je    .Lx854_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx854_3]
                        lea              rdx, [rip + .Lx854_4]
                                                                                        jmp   rax
.Lx854_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx854_1
.Lx854_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx854_1
.Lx854_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + 2368]
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n711_call_α
#-----------------------------------------------------------------------------------------------------------------------
n696_lit_integer_α:
                        mov              qword ptr [rbp + 2400], 6
                        mov              rax, qword ptr [rip + .Lx855_0]
                        mov              qword ptr [rbp + 2408], rax
                                                                                        jmp   n697_match_pos_α
n696_lit_integer_β:
                                                                                        jmp   n671_match_head_β
.Lx855_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n697_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n671_match_head_β
                                                                                        jmp   n698_match_defer_α
n697_match_pos_β:
                                                                                        jmp   n671_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n698_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx857_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx857_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx857_10
.Lx857_9:
                        xor              eax, eax
.Lx857_10:
                        test             rax, rax
                                                                                        jz    .Lx857_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx857_4]
                        lea              rdx, [rip + .Lx857_5]
                                                                                        jmp   rax
.Lx857_4:
                                                                                        jmp   n699_lit_integer_α
.Lx857_5:
                                                                                        jmp   n671_match_head_β
.Lx857_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx857_2:
                        test             rax, rax
                                                                                        je    .Lx857_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx857_7]
                        lea              rdx, [rip + .Lx857_8]
                                                                                        jmp   rax
.Lx857_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx857_2
.Lx857_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx857_2
.Lx857_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n671_match_head_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx857_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n699_lit_integer_α
.Lx857_6:
                        add              rsp, 16
                                                                                        jmp   n671_match_head_β
n698_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n699_lit_integer_α:
                        mov              qword ptr [rbp + 2432], 6
                        mov              rax, qword ptr [rip + .Lx858_0]
                        mov              qword ptr [rbp + 2440], rax
                                                                                        jmp   n700_match_rpos_α
n699_lit_integer_β:
                                                                                        jmp   n698_match_defer_β
.Lx858_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n700_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n698_match_defer_β
                                                                                        jmp   n695_match_release_α
n700_match_rpos_β:
                                                                                        jmp   n698_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n701_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                                                                                        jmp   n712_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n702_assign_α:
                        mov              rax, qword ptr [rbp + 1040]
                        mov              rdx, qword ptr [rbp + 1048]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n665_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n703_deref_α:
                        mov              rdi, qword ptr [rbp + 240]
                        mov              rsi, qword ptr [rbp + 248]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n640_var_α
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n713_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n704_subscript_α:
                        mov              rdi, qword ptr [rbp + 304]
                        mov              rsi, qword ptr [rbp + 312]
                        mov              rdx, qword ptr [rbp + 368]
                        mov              rcx, qword ptr [rbp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n652_var_α
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                                                                                        jmp   n714_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n705_assign_α:
                        mov              rax, qword ptr [rbp + 1696]
                        mov              rdx, qword ptr [rbp + 1704]
                        mov              qword ptr [1879052496], rax
                        mov              qword ptr [1879052504], rdx
                                                                                        jmp   n706_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n706_lit_string_α:
                        mov              qword ptr [rbp + 1808], 1
                        mov              rax, qword ptr [rip + .Lx865_0]
                        mov              qword ptr [rbp + 1816], rax
                                                                                        jmp   n715_call_α
.Lx865_0:
                        .quad            .Lx865_0_s
.Lx865_0_s:
                        .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n707_deref_α:
                        mov              rdi, qword ptr [rbp + 576]
                        mov              rsi, qword ptr [rbp + 584]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n643_var_α
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n716_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n708_subscript_α:
                        mov              rdi, qword ptr [rbp + 672]
                        mov              rsi, qword ptr [rbp + 680]
                        mov              rdx, qword ptr [rbp + 688]
                        mov              rcx, qword ptr [rbp + 696]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n657_op14_α
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                                                                                        jmp   n717_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n709_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rbp + 1408], rax
                        mov              qword ptr [rbp + 1416], rdx
                                                                                        jmp   n718_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n710_call_α:
                        mov              rax, qword ptr [rbp + 1552]
                        mov              qword ptr [rbp + 1520], rax
                        mov              rax, qword ptr [rbp + 1560]
                        mov              qword ptr [rbp + 1528], rax
                        .section         .rodata
.Lrkfn870:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn870]
                        lea              rsi, [rbp + 1520]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1504], rax
                        mov              qword ptr [rbp + 1512], rdx
                        cmp              eax, 99
                                                                                        je    n665_lit_string_α
                                                                                        jmp   n719_assign_α
n710_call_β:
                                                                                        jmp   n665_lit_string_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n711_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx872_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx872_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx872_6]
                        lea              rdx, [rip + .Lx872_7]
                                                                                        jmp   rax
.Lx872_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx872_2
.Lx872_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx872_2
.Lx872_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx872_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx872_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx872_3]
                        lea              rdx, [rip + .Lx872_4]
                                                                                        jmp   rax
.Lx872_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx872_2
.Lx872_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx872_2
.Lx872_1:
                        call             rt_faildescr@PLT
.Lx872_2:
                        mov              qword ptr [rbp + 2464], rax
                        mov              qword ptr [rbp + 2472], rdx
                        cmp              eax, 99
                                                                                        je    n635_var_α
                                                                                        jmp   n720_assign_α
n711_call_β:
                                                                                        jmp   n635_var_α
.Lx872_0:
                        .quad            .Lx872_0_s
.Lx872_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n712_binop_α:
                        mov              rdi, qword ptr [rbp + 992]
                        mov              rsi, qword ptr [rbp + 1000]
                        mov              rdx, qword ptr [rbp + 1008]
                        mov              rcx, qword ptr [rbp + 1016]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 976], rax
                        mov              qword ptr [rbp + 984], rdx
                                                                                        jmp   n721_call_α
#-----------------------------------------------------------------------------------------------------------------------
n713_lit_integer_α:
                        mov              qword ptr [rbp + 272], 6
                        mov              rax, qword ptr [rip + .Lx874_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n722_binop_α
.Lx874_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n714_assign_α:
                        mov              rax, qword ptr [rbp + 384]
                        mov              rdx, qword ptr [rbp + 392]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n652_var_α
#-----------------------------------------------------------------------------------------------------------------------
n715_call_α:
                        mov              rax, qword ptr [rbp + 1808]
                        mov              qword ptr [rbp + 1776], rax
                        mov              rax, qword ptr [rbp + 1816]
                        mov              qword ptr [rbp + 1784], rax
                        .section         .rodata
.Lrkfn877:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn877]
                        lea              rsi, [rbp + 1776]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1760], rax
                        mov              qword ptr [rbp + 1768], rdx
                        cmp              eax, 99
                                                                                        je    n724_lit_string_α
                                                                                        jmp   n723_assign_α
n715_call_β:
                                                                                        jmp   n724_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n716_assign_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              rdx, qword ptr [rbp + 600]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n643_var_α
#-----------------------------------------------------------------------------------------------------------------------
n717_deref_α:
                        mov              rdi, qword ptr [rbp + 704]
                        mov              rsi, qword ptr [rbp + 712]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n657_op14_α
                        mov              qword ptr [rbp + 720], rax
                        mov              qword ptr [rbp + 728], rdx
                                                                                        jmp   n725_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n718_lit_string_α:
                        mov              qword ptr [rbp + 1424], 1
                        mov              rax, qword ptr [rip + .Lx880_0]
                        mov              qword ptr [rbp + 1432], rax
                                                                                        jmp   n726_binop_α
.Lx880_0:
                        .quad            .Lx880_0_s
.Lx880_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n719_assign_α:
                        mov              rax, qword ptr [rbp + 1504]
                        mov              rdx, qword ptr [rbp + 1512]
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n665_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n720_assign_α:
                        mov              rsi, qword ptr [rbp + 2464]
                        mov              rdx, qword ptr [rbp + 2472]
                        mov              rdi, qword ptr [rip + .Lx882_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n635_var_α
.Lx882_0:
                        .quad            .Lx882_0_s
.Lx882_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n721_call_α:
                        mov              rax, qword ptr [rbp + 976]
                        mov              qword ptr [rbp + 944], rax
                        mov              rax, qword ptr [rbp + 984]
                        mov              qword ptr [rbp + 952], rax
                        .section         .rodata
.Lrkfn884:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn884]
                        lea              rsi, [rbp + 944]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 928], rax
                        mov              qword ptr [rbp + 936], rdx
                        cmp              eax, 99
                                                                                        je    n673_lit_string_α
                                                                                        jmp   n727_assign_var_α
n721_call_β:
                                                                                        jmp   n673_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n722_binop_α:
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 100
                                                                                        je    .Lx885_0
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 6
                                                                                        jne   .Lx885_2
.Lx885_1:
                        mov              rax, qword ptr [rbp + 264]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rbp + 192], 6
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n728_assign_var_α
.Lx885_0:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 0
                        lea              r9, [rbp + 192]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n728_assign_var_α
.Lx885_2:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 0
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n640_var_α
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   n728_assign_var_α
n722_binop_β:
                                                                                        jmp   n640_var_α
#-----------------------------------------------------------------------------------------------------------------------
n723_assign_α:
                        mov              rax, qword ptr [rbp + 1760]
                        mov              rdx, qword ptr [rbp + 1768]
                        mov              qword ptr [1879052512], rax
                        mov              qword ptr [1879052520], rdx
                                                                                        jmp   n724_lit_string_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n724_lit_string_α:
                        mov              qword ptr [rbp + 1872], 1
                        mov              rax, qword ptr [rip + .Lx887_0]
                        mov              qword ptr [rbp + 1880], rax
                                                                                        jmp   n729_call_α
.Lx887_0:
                        .quad            .Lx887_0_s
.Lx887_0_s:
                        .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n725_lit_integer_α:
                        mov              qword ptr [rbp + 736], 6
                        mov              rax, qword ptr [rip + .Lx888_0]
                        mov              qword ptr [rbp + 744], rax
                                                                                        jmp   n730_binop_α
.Lx888_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n726_binop_α:
                        mov              rdi, qword ptr [rbp + 1408]
                        mov              rsi, qword ptr [rbp + 1416]
                        mov              rdx, qword ptr [rbp + 1424]
                        mov              rcx, qword ptr [rbp + 1432]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                                                                                        jmp   n731_var_α
#-----------------------------------------------------------------------------------------------------------------------
n727_assign_var_α:
                        mov              rdi, qword ptr [rbp + 896]
                        mov              rsi, qword ptr [rbp + 904]
                        mov              rdx, qword ptr [rbp + 928]
                        mov              rcx, qword ptr [rbp + 936]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n673_lit_string_α
                        mov              qword ptr [rbp + 1024], rax
                        mov              qword ptr [rbp + 1032], rdx
                                                                                        jmp   n673_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n728_assign_var_α:
                        mov              rdi, qword ptr [rbp + 176]
                        mov              rsi, qword ptr [rbp + 184]
                        mov              rdx, qword ptr [rbp + 192]
                        mov              rcx, qword ptr [rbp + 200]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n640_var_α
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                                                                                        jmp   n640_var_α
#-----------------------------------------------------------------------------------------------------------------------
n729_call_α:
                        mov              rax, qword ptr [rbp + 1872]
                        mov              qword ptr [rbp + 1840], rax
                        mov              rax, qword ptr [rbp + 1880]
                        mov              qword ptr [rbp + 1848], rax
                        .section         .rodata
.Lrkfn893:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn893]
                        lea              rsi, [rbp + 1840]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1824], rax
                        mov              qword ptr [rbp + 1832], rdx
                        cmp              eax, 99
                                                                                        je    n733_lit_string_α
                                                                                        jmp   n732_assign_α
n729_call_β:
                                                                                        jmp   n733_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n730_binop_α:
                        mov              eax, dword ptr [rbp + 720]
                        cmp              eax, 100
                                                                                        je    .Lx894_0
                        mov              eax, dword ptr [rbp + 720]
                        cmp              eax, 6
                                                                                        jne   .Lx894_2
.Lx894_1:
                        mov              rax, qword ptr [rbp + 728]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 656], 6
                        mov              qword ptr [rbp + 664], rax
                                                                                        jmp   n734_assign_var_α
.Lx894_0:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        mov              r8d, 1
                        lea              r9, [rbp + 656]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n734_assign_var_α
.Lx894_2:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        mov              r8d, 1
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n657_op14_α
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n734_assign_var_α
n730_binop_β:
                                                                                        jmp   n657_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n731_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 1440], rax
                        mov              qword ptr [rbp + 1448], rdx
                                                                                        jmp   n735_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n732_assign_α:
                        mov              rax, qword ptr [rbp + 1824]
                        mov              rdx, qword ptr [rbp + 1832]
                        mov              qword ptr [1879052528], rax
                        mov              qword ptr [1879052536], rdx
                                                                                        jmp   n733_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n733_lit_string_α:
                        mov              qword ptr [rbp + 1936], 1
                        mov              rax, qword ptr [rip + .Lx897_0]
                        mov              qword ptr [rbp + 1944], rax
                                                                                        jmp   n736_call_α
.Lx897_0:
                        .quad            .Lx897_0_s
.Lx897_0_s:
                        .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n734_assign_var_α:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 656]
                        mov              rcx, qword ptr [rbp + 664]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n657_op14_α
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   n657_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n735_binop_α:
                        mov              rdi, qword ptr [rbp + 1392]
                        mov              rsi, qword ptr [rbp + 1400]
                        mov              rdx, qword ptr [rbp + 1440]
                        mov              rcx, qword ptr [rbp + 1448]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                                                                                        jmp   n737_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n736_call_α:
                        mov              rax, qword ptr [rbp + 1936]
                        mov              qword ptr [rbp + 1904], rax
                        mov              rax, qword ptr [rbp + 1944]
                        mov              qword ptr [rbp + 1912], rax
                        .section         .rodata
.Lrkfn901:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn901]
                        lea              rsi, [rbp + 1904]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1888], rax
                        mov              qword ptr [rbp + 1896], rdx
                        cmp              eax, 99
                                                                                        je    n739_lit_string_α
                                                                                        jmp   n738_assign_α
n736_call_β:
                                                                                        jmp   n739_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n737_lit_string_α:
                        mov              qword ptr [rbp + 1456], 1
                        mov              rax, qword ptr [rip + .Lx902_0]
                        mov              qword ptr [rbp + 1464], rax
                                                                                        jmp   n740_binop_α
.Lx902_0:
                        .quad            .Lx902_0_s
.Lx902_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n738_assign_α:
                        mov              rax, qword ptr [rbp + 1888]
                        mov              rdx, qword ptr [rbp + 1896]
                        mov              qword ptr [1879052544], rax
                        mov              qword ptr [1879052552], rdx
                                                                                        jmp   n739_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n739_lit_string_α:
                        mov              qword ptr [rbp + 2000], 1
                        mov              rax, qword ptr [rip + .Lx904_0]
                        mov              qword ptr [rbp + 2008], rax
                                                                                        jmp   n741_call_α
.Lx904_0:
                        .quad            .Lx904_0_s
.Lx904_0_s:
                        .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n740_binop_α:
                        mov              rdi, qword ptr [rbp + 1376]
                        mov              rsi, qword ptr [rbp + 1384]
                        mov              rdx, qword ptr [rbp + 1456]
                        mov              rcx, qword ptr [rbp + 1464]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1360], rax
                        mov              qword ptr [rbp + 1368], rdx
                                                                                        jmp   n742_var_α
#-----------------------------------------------------------------------------------------------------------------------
n741_call_α:
                        mov              rax, qword ptr [rbp + 2000]
                        mov              qword ptr [rbp + 1968], rax
                        mov              rax, qword ptr [rbp + 2008]
                        mov              qword ptr [rbp + 1976], rax
                        .section         .rodata
.Lrkfn907:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn907]
                        lea              rsi, [rbp + 1968]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1952], rax
                        mov              qword ptr [rbp + 1960], rdx
                        cmp              eax, 99
                                                                                        je    n744_lit_string_α
                                                                                        jmp   n743_assign_α
n741_call_β:
                                                                                        jmp   n744_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n742_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rbp + 1472], rax
                        mov              qword ptr [rbp + 1480], rdx
                                                                                        jmp   n745_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n743_assign_α:
                        mov              rax, qword ptr [rbp + 1952]
                        mov              rdx, qword ptr [rbp + 1960]
                        mov              qword ptr [1879052560], rax
                        mov              qword ptr [1879052568], rdx
                                                                                        jmp   n744_lit_string_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n744_lit_string_α:
                        mov              qword ptr [rbp + 2064], 1
                        mov              rax, qword ptr [rip + .Lx910_0]
                        mov              qword ptr [rbp + 2072], rax
                                                                                        jmp   n746_call_α
.Lx910_0:
                        .quad            .Lx910_0_s
.Lx910_0_s:
                        .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n745_binop_α:
                        mov              rdi, qword ptr [rbp + 1360]
                        mov              rsi, qword ptr [rbp + 1368]
                        mov              rdx, qword ptr [rbp + 1472]
                        mov              rcx, qword ptr [rbp + 1480]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1344], rax
                        mov              qword ptr [rbp + 1352], rdx
                                                                                        jmp   n747_call_α
#-----------------------------------------------------------------------------------------------------------------------
n746_call_α:
                        mov              rax, qword ptr [rbp + 2064]
                        mov              qword ptr [rbp + 2032], rax
                        mov              rax, qword ptr [rbp + 2072]
                        mov              qword ptr [rbp + 2040], rax
                        .section         .rodata
.Lrkfn913:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn913]
                        lea              rsi, [rbp + 2032]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2016], rax
                        mov              qword ptr [rbp + 2024], rdx
                        cmp              eax, 99
                                                                                        je    n749_lit_string_α
                                                                                        jmp   n748_assign_α
n746_call_β:
                                                                                        jmp   n749_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n747_call_α:
                        mov              rax, qword ptr [rbp + 1344]
                        mov              qword ptr [rbp + 1312], rax
                        mov              rax, qword ptr [rbp + 1352]
                        mov              qword ptr [rbp + 1320], rax
                        .section         .rodata
.Lrkfn915:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn915]
                        lea              rsi, [rbp + 1312]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1296], rax
                        mov              qword ptr [rbp + 1304], rdx
                        cmp              eax, 99
                                                                                        je    n694_lit_string_α
                                                                                        jmp   n750_assign_var_α
n747_call_β:
                                                                                        jmp   n694_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n748_assign_α:
                        mov              rax, qword ptr [rbp + 2016]
                        mov              rdx, qword ptr [rbp + 2024]
                        mov              qword ptr [1879052576], rax
                        mov              qword ptr [1879052584], rdx
                                                                                        jmp   n749_lit_string_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n749_lit_string_α:
                        mov              qword ptr [rbp + 2128], 1
                        mov              rax, qword ptr [rip + .Lx917_0]
                        mov              qword ptr [rbp + 2136], rax
                                                                                        jmp   n751_call_α
.Lx917_0:
                        .quad            .Lx917_0_s
.Lx917_0_s:
                        .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n750_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1264]
                        mov              rsi, qword ptr [rbp + 1272]
                        mov              rdx, qword ptr [rbp + 1296]
                        mov              rcx, qword ptr [rbp + 1304]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n694_lit_string_α
                        mov              qword ptr [rbp + 1488], rax
                        mov              qword ptr [rbp + 1496], rdx
                                                                                        jmp   n694_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n751_call_α:
                        mov              rax, qword ptr [rbp + 2128]
                        mov              qword ptr [rbp + 2096], rax
                        mov              rax, qword ptr [rbp + 2136]
                        mov              qword ptr [rbp + 2104], rax
                        .section         .rodata
.Lrkfn920:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn920]
                        lea              rsi, [rbp + 2096]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2080], rax
                        mov              qword ptr [rbp + 2088], rdx
                        cmp              eax, 99
                                                                                        je    n753_lit_string_α
                                                                                        jmp   n752_assign_α
n751_call_β:
                                                                                        jmp   n753_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n752_assign_α:
                        mov              rax, qword ptr [rbp + 2080]
                        mov              rdx, qword ptr [rbp + 2088]
                        mov              qword ptr [1879052592], rax
                        mov              qword ptr [1879052600], rdx
                                                                                        jmp   n753_lit_string_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n753_lit_string_α:
                        mov              qword ptr [rbp + 2192], 1
                        mov              rax, qword ptr [rip + .Lx922_0]
                        mov              qword ptr [rbp + 2200], rax
                                                                                        jmp   n754_call_α
.Lx922_0:
                        .quad            .Lx922_0_s
.Lx922_0_s:
                        .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n754_call_α:
                        mov              rax, qword ptr [rbp + 2192]
                        mov              qword ptr [rbp + 2160], rax
                        mov              rax, qword ptr [rbp + 2200]
                        mov              qword ptr [rbp + 2168], rax
                        .section         .rodata
.Lrkfn924:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn924]
                        lea              rsi, [rbp + 2160]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2144], rax
                        mov              qword ptr [rbp + 2152], rdx
                        cmp              eax, 99
                                                                                        je    n756_lit_string_α
                                                                                        jmp   n755_assign_α
n754_call_β:
                                                                                        jmp   n756_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n755_assign_α:
                        mov              rax, qword ptr [rbp + 2144]
                        mov              rdx, qword ptr [rbp + 2152]
                        mov              qword ptr [1879052608], rax
                        mov              qword ptr [1879052616], rdx
                                                                                        jmp   n756_lit_string_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n756_lit_string_α:
                        mov              qword ptr [rbp + 2272], 1
                        mov              rax, qword ptr [rip + .Lx926_0]
                        mov              qword ptr [rbp + 2280], rax
                                                                                        jmp   n757_lit_integer_α
.Lx926_0:
                        .quad            .Lx926_0_s
.Lx926_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n757_lit_integer_α:
                        mov              qword ptr [rbp + 2288], 6
                        mov              rax, qword ptr [rip + .Lx927_0]
                        mov              qword ptr [rbp + 2296], rax
                                                                                        jmp   n758_call_α
.Lx927_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n758_call_α:
                        mov              rax, qword ptr [rbp + 2272]
                        mov              qword ptr [rbp + 2224], rax
                        mov              rax, qword ptr [rbp + 2280]
                        mov              qword ptr [rbp + 2232], rax
                        mov              rax, qword ptr [rbp + 2288]
                        mov              qword ptr [rbp + 2240], rax
                        mov              rax, qword ptr [rbp + 2296]
                        mov              qword ptr [rbp + 2248], rax
                        .section         .rodata
.Lrkfn929:              .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn929]
                        lea              rsi, [rbp + 2224]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2208], rax
                        mov              qword ptr [rbp + 2216], rdx
                        cmp              eax, 99
                                                                                        je    n635_var_α
                                                                                        jmp   n635_var_α
n758_call_β:
                                                                                        jmp   n635_var_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2520]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_ω:
                        mov              rax, [rbp + 2528]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Binary_α
proc_LBL__Binary_α:
                        .global          proc_LBL__Binary_α
                        .global          proc_LBL__Binary_β
                        .global          proc_LBL__Binary_γ
                        .global          proc_LBL__Binary_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              [rsp + 2536], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__Binary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n930_goto_α:
                                                                                        jmp   n940_call_α
n930_goto_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
n931_goto_α:
                                                                                        jmp   n941_var_α
n931_goto_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
n932_goto_α:
                                                                                        jmp   n942_lit_string_α
n932_goto_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
n933_goto_α:
                                                                                        jmp   n943_var_α
n933_goto_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
n934_goto_α:
                                                                                        jmp   n942_lit_string_α
n934_goto_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
n935_goto_α:
                                                                                        jmp   n944_call_α
n935_goto_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
n936_goto_α:
                                                                                        jmp   n942_lit_string_α
n936_goto_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
n937_goto_α:
                                                                                        jmp   n942_lit_string_α
n937_goto_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
n938_goto_α:
                                                                                        jmp   n945_var_α
n938_goto_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
n939_goto_α:
                                                                                        jmp   n946_lit_string_α
n939_goto_β:
                                                                                        jmp   proc_LBL__Binary_ω
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n940_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1080_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1080_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1080_6]
                        lea              rdx, [rip + .Lx1080_7]
                                                                                        jmp   rax
.Lx1080_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1080_2
.Lx1080_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1080_2
.Lx1080_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1080_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1080_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1080_3]
                        lea              rdx, [rip + .Lx1080_4]
                                                                                        jmp   rax
.Lx1080_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1080_2
.Lx1080_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1080_2
.Lx1080_1:
                        call             rt_faildescr@PLT
.Lx1080_2:
                        mov              qword ptr [rbp + 1104], rax
                        mov              qword ptr [rbp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n948_call_α
                                                                                        jmp   n947_assign_α
n940_call_β:
                                                                                        jmp   n948_call_α
.Lx1080_0:
                        .quad            .Lx1080_0_s
.Lx1080_0_s:
                        .string          "Pop"
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n941_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                                                                                        jmp   n949_lit_integer_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n942_lit_string_α:
                        mov              qword ptr [rbp + 1616], 1
                        mov              rax, qword ptr [rip + .Lx1082_0]
                        mov              qword ptr [rbp + 1624], rax
                                                                                        jmp   n951_call_α
.Lx1082_0:
                        .quad            .Lx1082_0_s
.Lx1082_0_s:
                        .string          "PAT$0"
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n943_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 496], rax
                        mov              qword ptr [rbp + 504], rdx
                                                                                        jmp   n952_var_α
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n944_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1085_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1085_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1085_6]
                        lea              rdx, [rip + .Lx1085_7]
                                                                                        jmp   rax
.Lx1085_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1085_2
.Lx1085_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1085_2
.Lx1085_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1085_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1085_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1085_3]
                        lea              rdx, [rip + .Lx1085_4]
                                                                                        jmp   rax
.Lx1085_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1085_2
.Lx1085_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1085_2
.Lx1085_1:
                        call             rt_faildescr@PLT
.Lx1085_2:
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                        cmp              eax, 99
                                                                                        je    n955_call_α
                                                                                        jmp   n954_assign_α
n944_call_β:
                                                                                        jmp   n955_call_α
.Lx1085_0:
                        .quad            .Lx1085_0_s
.Lx1085_0_s:
                        .string          "Pop"
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n945_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1086_0]
                        call             NV_GET_fn@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx1086_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__Binary_γ
.Lx1086_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n956_assign_α
.Lx1086_0:
                        .quad            .Lx1086_0_s
.Lx1086_0_s:
                        .string          "INPUT"
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n946_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx1087_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n957_assign_α
.Lx1087_0:
                        .quad            .Lx1087_0_s
.Lx1087_0_s:
                        .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n947_assign_α:
                        mov              rax, qword ptr [rbp + 1104]
                        mov              rdx, qword ptr [rbp + 1112]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n948_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n948_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1090_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1090_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1090_6]
                        lea              rdx, [rip + .Lx1090_7]
                                                                                        jmp   rax
.Lx1090_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1090_2
.Lx1090_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1090_2
.Lx1090_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1090_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1090_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1090_3]
                        lea              rdx, [rip + .Lx1090_4]
                                                                                        jmp   rax
.Lx1090_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1090_2
.Lx1090_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1090_2
.Lx1090_1:
                        call             rt_faildescr@PLT
.Lx1090_2:
                        mov              qword ptr [rbp + 1136], rax
                        mov              qword ptr [rbp + 1144], rdx
                        cmp              eax, 99
                                                                                        je    n959_call_α
                                                                                        jmp   n958_assign_α
n948_call_β:
                                                                                        jmp   n959_call_α
.Lx1090_0:
                        .quad            .Lx1090_0_s
.Lx1090_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n949_lit_integer_α:
                        mov              qword ptr [rbp + 160], 6
                        mov              rax, qword ptr [rip + .Lx1091_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n960_subscript_α
.Lx1091_0:
                        .quad            0
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n950_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                                                                                        jmp   n961_var_α
#-----------------------------------------------------------------------------------------------------------------------
n951_call_α:
                        mov              rax, qword ptr [rbp + 1616]
                        mov              qword ptr [rbp + 1584], rax
                        mov              rax, qword ptr [rbp + 1624]
                        mov              qword ptr [rbp + 1592], rax
                        .section         .rodata
.Lrkfn1094:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1094]
                        lea              rsi, [rbp + 1584]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1568], rax
                        mov              qword ptr [rbp + 1576], rdx
                        cmp              eax, 99
                                                                                        je    n964_lit_string_α
                                                                                        jmp   n963_assign_α
n951_call_β:
                                                                                        jmp   n964_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n952_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n965_lit_integer_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n953_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   n966_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n954_assign_α:
                        mov              rax, qword ptr [rbp + 768]
                        mov              rdx, qword ptr [rbp + 776]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n955_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n955_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1099_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1099_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1099_6]
                        lea              rdx, [rip + .Lx1099_7]
                                                                                        jmp   rax
.Lx1099_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1099_2
.Lx1099_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1099_2
.Lx1099_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1099_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1099_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1099_3]
                        lea              rdx, [rip + .Lx1099_4]
                                                                                        jmp   rax
.Lx1099_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1099_2
.Lx1099_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1099_2
.Lx1099_1:
                        call             rt_faildescr@PLT
.Lx1099_2:
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n969_lit_string_α
                                                                                        jmp   n968_assign_α
n955_call_β:
                                                                                        jmp   n969_lit_string_α
.Lx1099_0:
                        .quad            .Lx1099_0_s
.Lx1099_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n956_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052624], rax
                        mov              qword ptr [1879052632], rdx
                                                                                        jmp   n970_var_α
#-----------------------------------------------------------------------------------------------------------------------
n957_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1101_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n945_var_α
.Lx1101_0:
                        .quad            .Lx1101_0_s
.Lx1101_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n958_assign_α:
                        mov              rax, qword ptr [rbp + 1136]
                        mov              rdx, qword ptr [rbp + 1144]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n959_call_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n959_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1104_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1104_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1104_6]
                        lea              rdx, [rip + .Lx1104_7]
                                                                                        jmp   rax
.Lx1104_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1104_2
.Lx1104_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1104_2
.Lx1104_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1104_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1104_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1104_3]
                        lea              rdx, [rip + .Lx1104_4]
                                                                                        jmp   rax
.Lx1104_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1104_2
.Lx1104_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1104_2
.Lx1104_1:
                        call             rt_faildescr@PLT
.Lx1104_2:
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n972_lit_string_α
                                                                                        jmp   n971_assign_α
n959_call_β:
                                                                                        jmp   n972_lit_string_α
.Lx1104_0:
                        .quad            .Lx1104_0_s
.Lx1104_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n960_subscript_α:
                        mov              rdi, qword ptr [rbp + 144]
                        mov              rsi, qword ptr [rbp + 152]
                        mov              rdx, qword ptr [rbp + 160]
                        mov              rcx, qword ptr [rbp + 168]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n950_var_α
                        mov              qword ptr [rbp + 176], rax
                        mov              qword ptr [rbp + 184], rdx
                                                                                        jmp   n973_var_α
#-----------------------------------------------------------------------------------------------------------------------
n961_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n974_lit_integer_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n962_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n975_call_α
#-----------------------------------------------------------------------------------------------------------------------
n963_assign_α:
                        mov              rax, qword ptr [rbp + 1568]
                        mov              rdx, qword ptr [rbp + 1576]
                        mov              qword ptr [1879052464], rax
                        mov              qword ptr [1879052472], rdx
                                                                                        jmp   n964_lit_string_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n964_lit_string_α:
                        mov              qword ptr [rbp + 1680], 1
                        mov              rax, qword ptr [rip + .Lx1109_0]
                        mov              qword ptr [rbp + 1688], rax
                                                                                        jmp   n977_call_α
.Lx1109_0:
                        .quad            .Lx1109_0_s
.Lx1109_0_s:
                        .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n965_lit_integer_α:
                        mov              qword ptr [rbp + 528], 6
                        mov              rax, qword ptr [rip + .Lx1110_0]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n978_subscript_α
.Lx1110_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n966_lit_integer_α:
                        mov              qword ptr [rbp + 624], 6
                        mov              rax, qword ptr [rip + .Lx1111_0]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n979_subscript_α
.Lx1111_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n967_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n968_assign_α:
                        mov              rax, qword ptr [rbp + 800]
                        mov              rdx, qword ptr [rbp + 808]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n969_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n969_lit_string_α:
                        mov              qword ptr [rbp + 832], 1
                        mov              rax, qword ptr [rip + .Lx1115_0]
                        mov              qword ptr [rbp + 840], rax
                                                                                        jmp   n980_call_α
.Lx1115_0:
                        .quad            .Lx1115_0_s
.Lx1115_0_s:
                        .string          ""
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n970_var_α:
                        mov              rax, qword ptr [1879052624]
                        mov              rdx, qword ptr [1879052632]
                        mov              qword ptr [rbp + 2448], rax
                        mov              qword ptr [rbp + 2456], rdx
                                                                                        jmp   n981_match_head_α
#-----------------------------------------------------------------------------------------------------------------------
n971_assign_α:
                        mov              rax, qword ptr [rbp + 1168]
                        mov              rdx, qword ptr [rbp + 1176]
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n972_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n972_lit_string_α:
                        mov              qword ptr [rbp + 1200], 1
                        mov              rax, qword ptr [rip + .Lx1118_0]
                        mov              qword ptr [rbp + 1208], rax
                                                                                        jmp   n982_call_α
.Lx1118_0:
                        .quad            .Lx1118_0_s
.Lx1118_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n973_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                                                                                        jmp   n983_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n974_lit_integer_α:
                        mov              qword ptr [rbp + 336], 6
                        mov              rax, qword ptr [rip + .Lx1120_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n984_subscript_α
.Lx1120_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n975_call_α:
                        mov              rax, qword ptr [rbp + 448]
                        mov              qword ptr [rbp + 416], rax
                        mov              rax, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 424], rax
                        .section         .rodata
.Lrkfn1122:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1122]
                        lea              rsi, [rbp + 416]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n976_lit_string_α
                                                                                        jmp   n985_var_α
n975_call_β:
                                                                                        jmp   n976_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n976_lit_string_α:
                        mov              qword ptr [rbp + 32], 1
                        mov              rax, qword ptr [rip + .Lx1123_0]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n986_call_α
.Lx1123_0:
                        .quad            .Lx1123_0_s
.Lx1123_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n977_call_α:
                        mov              rax, qword ptr [rbp + 1680]
                        mov              qword ptr [rbp + 1648], rax
                        mov              rax, qword ptr [rbp + 1688]
                        mov              qword ptr [rbp + 1656], rax
                        .section         .rodata
.Lrkfn1125:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1125]
                        lea              rsi, [rbp + 1648]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1632], rax
                        mov              qword ptr [rbp + 1640], rdx
                        cmp              eax, 99
                                                                                        je    n988_lit_string_α
                                                                                        jmp   n987_assign_α
n977_call_β:
                                                                                        jmp   n988_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n978_subscript_α:
                        mov              rdi, qword ptr [rbp + 512]
                        mov              rsi, qword ptr [rbp + 520]
                        mov              rdx, qword ptr [rbp + 528]
                        mov              rcx, qword ptr [rbp + 536]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n953_var_α
                        mov              qword ptr [rbp + 544], rax
                        mov              qword ptr [rbp + 552], rdx
                                                                                        jmp   n989_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n979_subscript_α:
                        mov              rdi, qword ptr [rbp + 608]
                        mov              rsi, qword ptr [rbp + 616]
                        mov              rdx, qword ptr [rbp + 624]
                        mov              rcx, qword ptr [rbp + 632]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n967_op14_α
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   n990_var_α
#-----------------------------------------------------------------------------------------------------------------------
n980_call_α:
                        mov              rax, qword ptr [rbp + 832]
                        mov              qword ptr [rbp + 864], rax
                        mov              rax, qword ptr [rbp + 840]
                        mov              qword ptr [rbp + 872], rax
                        .section         .rodata
.Lrkfn1129:             .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1129]
                        lea              rsi, [rbp + 864]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n992_lit_string_α
                                                                                        jmp   n991_call_α
n980_call_β:
                                                                                        jmp   n992_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n981_match_head_α:
                        mov              qword ptr [rbp + 2376], rbp
                        mov              rdi, qword ptr [rbp + 2448]
                        mov              rsi, qword ptr [rbp + 2456]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rbp + 2368], r12
                        mov              qword ptr [rbp + 2352], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 2344], rax
                        mov              dword ptr [rbp + 2336], 0
.Lx1131_0:
                        mov              r14d, dword ptr [rbp + 2336]
                                                                                        jmp   n993_match_sequence_α
n981_match_head_β:
                        add              dword ptr [rbp + 2336], 1
                        mov              eax, dword ptr [rbp + 2336]
                        cmp              eax, r15d
                                                                                        jg    .Lx1131_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx1131_1
                                                                                        jmp   .Lx1131_0
.Lx1131_1:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        mov              r12, qword ptr [rbp + 2368]
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n946_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n982_call_α:
                        mov              rax, qword ptr [rbp + 1200]
                        mov              qword ptr [rbp + 1232], rax
                        mov              rax, qword ptr [rbp + 1208]
                        mov              qword ptr [rbp + 1240], rax
                        .section         .rodata
.Lrkfn1133:             .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1133]
                        lea              rsi, [rbp + 1232]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1216], rax
                        mov              qword ptr [rbp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n995_lit_string_α
                                                                                        jmp   n994_call_α
n982_call_β:
                                                                                        jmp   n995_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n983_lit_integer_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx1134_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n996_subscript_α
.Lx1134_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n984_subscript_α:
                        mov              rdi, qword ptr [rbp + 320]
                        mov              rsi, qword ptr [rbp + 328]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n962_var_α
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   n997_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n985_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   n998_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n986_call_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 64], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 72], rax
                        .section         .rodata
.Lrkfn1138:             .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1138]
                        lea              rsi, [rbp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 48], rax
                        mov              qword ptr [rbp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n999_op14_α
                                                                                        jmp   n967_op14_α
n986_call_β:
                                                                                        jmp   n999_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n987_assign_α:
                        mov              rax, qword ptr [rbp + 1632]
                        mov              rdx, qword ptr [rbp + 1640]
                        mov              qword ptr [1879052480], rax
                        mov              qword ptr [1879052488], rdx
                                                                                        jmp   n988_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n988_lit_string_α:
                        mov              qword ptr [rbp + 1744], 1
                        mov              rax, qword ptr [rip + .Lx1140_0]
                        mov              qword ptr [rbp + 1752], rax
                                                                                        jmp   n1000_call_α
.Lx1140_0:
                        .quad            .Lx1140_0_s
.Lx1140_0_s:
                        .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n989_deref_α:
                        mov              rdi, qword ptr [rbp + 544]
                        mov              rsi, qword ptr [rbp + 552]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n953_var_α
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                                                                                        jmp   n1001_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n990_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                                                                                        jmp   n1002_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n991_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx1144_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1144_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1144_6]
                        lea              rdx, [rip + .Lx1144_7]
                                                                                        jmp   rax
.Lx1144_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1144_2
.Lx1144_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1144_2
.Lx1144_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx1144_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1144_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1144_3]
                        lea              rdx, [rip + .Lx1144_4]
                                                                                        jmp   rax
.Lx1144_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1144_2
.Lx1144_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1144_2
.Lx1144_1:
                        call             rt_faildescr@PLT
.Lx1144_2:
                        mov              qword ptr [rbp + 896], rax
                        mov              qword ptr [rbp + 904], rdx
                        cmp              eax, 99
                                                                                        je    n992_lit_string_α
                                                                                        jmp   n1003_var_α
n991_call_β:
                                                                                        jmp   n992_lit_string_α
.Lx1144_0:
                        .quad            .Lx1144_0_s
.Lx1144_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n992_lit_string_α:
                        mov              qword ptr [rbp + 1088], 1
                        mov              rax, qword ptr [rip + .Lx1145_0]
                        mov              qword ptr [rbp + 1096], rax
                                                                                        jmp   n1004_call_α
.Lx1145_0:
                        .quad            .Lx1145_0_s
.Lx1145_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n993_match_sequence_α:
                        mov              dword ptr [rbp + 2384], r14d
                                                                                        jmp   n1006_lit_integer_α
n993_match_sequence_as:
                                                                                        jmp   n1005_match_release_α
n993_match_sequence_β:
                                                                                        jmp   n1010_match_rpos_β
n993_match_sequence_af:
                                                                                        jmp   n981_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n994_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx1149_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1149_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1149_6]
                        lea              rdx, [rip + .Lx1149_7]
                                                                                        jmp   rax
.Lx1149_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1149_2
.Lx1149_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1149_2
.Lx1149_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx1149_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1149_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1149_3]
                        lea              rdx, [rip + .Lx1149_4]
                                                                                        jmp   rax
.Lx1149_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1149_2
.Lx1149_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1149_2
.Lx1149_1:
                        call             rt_faildescr@PLT
.Lx1149_2:
                        mov              qword ptr [rbp + 1264], rax
                        mov              qword ptr [rbp + 1272], rdx
                        cmp              eax, 99
                                                                                        je    n995_lit_string_α
                                                                                        jmp   n1011_var_α
n994_call_β:
                                                                                        jmp   n995_lit_string_α
.Lx1149_0:
                        .quad            .Lx1149_0_s
.Lx1149_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n995_lit_string_α:
                        mov              qword ptr [rbp + 1552], 1
                        mov              rax, qword ptr [rip + .Lx1150_0]
                        mov              qword ptr [rbp + 1560], rax
                                                                                        jmp   n1012_call_α
.Lx1150_0:
                        .quad            .Lx1150_0_s
.Lx1150_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n996_subscript_α:
                        mov              rdi, qword ptr [rbp + 208]
                        mov              rsi, qword ptr [rbp + 216]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n950_var_α
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n1013_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n997_deref_α:
                        mov              rdi, qword ptr [rbp + 352]
                        mov              rsi, qword ptr [rbp + 360]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n962_var_α
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n1014_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n998_assign_var_α:
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        mov              rdx, qword ptr [rbp + 464]
                        mov              rcx, qword ptr [rbp + 472]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n976_lit_string_α
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   n976_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n999_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n1000_call_α:
                        mov              rax, qword ptr [rbp + 1744]
                        mov              qword ptr [rbp + 1712], rax
                        mov              rax, qword ptr [rbp + 1752]
                        mov              qword ptr [rbp + 1720], rax
                        .section         .rodata
.Lrkfn1157:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1157]
                        lea              rsi, [rbp + 1712]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1696], rax
                        mov              qword ptr [rbp + 1704], rdx
                        cmp              eax, 99
                                                                                        je    n1016_lit_string_α
                                                                                        jmp   n1015_assign_α
n1000_call_β:
                                                                                        jmp   n1016_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1001_subscript_α:
                        mov              rdi, qword ptr [rbp + 496]
                        mov              rsi, qword ptr [rbp + 504]
                        mov              rdx, qword ptr [rbp + 560]
                        mov              rcx, qword ptr [rbp + 568]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n953_var_α
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n1017_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n1002_lit_integer_α:
                        mov              qword ptr [rbp + 688], 6
                        mov              rax, qword ptr [rip + .Lx1159_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   n1018_subscript_α
.Lx1159_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1003_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   n1019_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1004_call_α:
                        mov              rax, qword ptr [rbp + 1088]
                        mov              qword ptr [rbp + 1056], rax
                        mov              rax, qword ptr [rbp + 1096]
                        mov              qword ptr [rbp + 1064], rax
                        .section         .rodata
.Lrkfn1162:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1162]
                        lea              rsi, [rbp + 1056]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    n976_lit_string_α
                                                                                        jmp   n1020_assign_α
n1004_call_β:
                                                                                        jmp   n976_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1005_match_release_α:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 2400]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx1164_1:
                        test             rax, rax
                                                                                        je    .Lx1164_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1164_3]
                        lea              rdx, [rip + .Lx1164_4]
                                                                                        jmp   rax
.Lx1164_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx1164_1
.Lx1164_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx1164_1
.Lx1164_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + 2368]
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n1021_call_α
#-----------------------------------------------------------------------------------------------------------------------
n1006_lit_integer_α:
                        mov              qword ptr [rbp + 2400], 6
                        mov              rax, qword ptr [rip + .Lx1165_0]
                        mov              qword ptr [rbp + 2408], rax
                                                                                        jmp   n1007_match_pos_α
n1006_lit_integer_β:
                                                                                        jmp   n981_match_head_β
.Lx1165_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1007_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n981_match_head_β
                                                                                        jmp   n1008_match_defer_α
n1007_match_pos_β:
                                                                                        jmp   n981_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n1008_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx1167_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1167_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1167_10
.Lx1167_9:
                        xor              eax, eax
.Lx1167_10:
                        test             rax, rax
                                                                                        jz    .Lx1167_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1167_4]
                        lea              rdx, [rip + .Lx1167_5]
                                                                                        jmp   rax
.Lx1167_4:
                                                                                        jmp   n1009_lit_integer_α
.Lx1167_5:
                                                                                        jmp   n981_match_head_β
.Lx1167_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1167_2:
                        test             rax, rax
                                                                                        je    .Lx1167_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1167_7]
                        lea              rdx, [rip + .Lx1167_8]
                                                                                        jmp   rax
.Lx1167_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1167_2
.Lx1167_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1167_2
.Lx1167_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n981_match_head_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1167_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1009_lit_integer_α
.Lx1167_6:
                        add              rsp, 16
                                                                                        jmp   n981_match_head_β
n1008_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1009_lit_integer_α:
                        mov              qword ptr [rbp + 2432], 6
                        mov              rax, qword ptr [rip + .Lx1168_0]
                        mov              qword ptr [rbp + 2440], rax
                                                                                        jmp   n1010_match_rpos_α
n1009_lit_integer_β:
                                                                                        jmp   n1008_match_defer_β
.Lx1168_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1010_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n1008_match_defer_β
                                                                                        jmp   n1005_match_release_α
n1010_match_rpos_β:
                                                                                        jmp   n1008_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n1011_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rbp + 1408], rax
                        mov              qword ptr [rbp + 1416], rdx
                                                                                        jmp   n1022_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1012_call_α:
                        mov              rax, qword ptr [rbp + 1552]
                        mov              qword ptr [rbp + 1520], rax
                        mov              rax, qword ptr [rbp + 1560]
                        mov              qword ptr [rbp + 1528], rax
                        .section         .rodata
.Lrkfn1172:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1172]
                        lea              rsi, [rbp + 1520]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1504], rax
                        mov              qword ptr [rbp + 1512], rdx
                        cmp              eax, 99
                                                                                        je    n976_lit_string_α
                                                                                        jmp   n1023_assign_α
n1012_call_β:
                                                                                        jmp   n976_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1013_deref_α:
                        mov              rdi, qword ptr [rbp + 240]
                        mov              rsi, qword ptr [rbp + 248]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n950_var_α
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n1024_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n1014_subscript_α:
                        mov              rdi, qword ptr [rbp + 304]
                        mov              rsi, qword ptr [rbp + 312]
                        mov              rdx, qword ptr [rbp + 368]
                        mov              rcx, qword ptr [rbp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n962_var_α
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                                                                                        jmp   n1025_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n1015_assign_α:
                        mov              rax, qword ptr [rbp + 1696]
                        mov              rdx, qword ptr [rbp + 1704]
                        mov              qword ptr [1879052496], rax
                        mov              qword ptr [1879052504], rdx
                                                                                        jmp   n1016_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n1016_lit_string_α:
                        mov              qword ptr [rbp + 1808], 1
                        mov              rax, qword ptr [rip + .Lx1176_0]
                        mov              qword ptr [rbp + 1816], rax
                                                                                        jmp   n1026_call_α
.Lx1176_0:
                        .quad            .Lx1176_0_s
.Lx1176_0_s:
                        .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n1017_deref_α:
                        mov              rdi, qword ptr [rbp + 576]
                        mov              rsi, qword ptr [rbp + 584]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n953_var_α
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n1027_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n1018_subscript_α:
                        mov              rdi, qword ptr [rbp + 672]
                        mov              rsi, qword ptr [rbp + 680]
                        mov              rdx, qword ptr [rbp + 688]
                        mov              rcx, qword ptr [rbp + 696]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n967_op14_α
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                                                                                        jmp   n1028_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n1019_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                                                                                        jmp   n1029_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n1020_assign_α:
                        mov              rax, qword ptr [rbp + 1040]
                        mov              rdx, qword ptr [rbp + 1048]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n976_lit_string_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n1021_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1182_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1182_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1182_6]
                        lea              rdx, [rip + .Lx1182_7]
                                                                                        jmp   rax
.Lx1182_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1182_2
.Lx1182_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1182_2
.Lx1182_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1182_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1182_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1182_3]
                        lea              rdx, [rip + .Lx1182_4]
                                                                                        jmp   rax
.Lx1182_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1182_2
.Lx1182_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1182_2
.Lx1182_1:
                        call             rt_faildescr@PLT
.Lx1182_2:
                        mov              qword ptr [rbp + 2464], rax
                        mov              qword ptr [rbp + 2472], rdx
                        cmp              eax, 99
                                                                                        je    n945_var_α
                                                                                        jmp   n1030_assign_α
n1021_call_β:
                                                                                        jmp   n945_var_α
.Lx1182_0:
                        .quad            .Lx1182_0_s
.Lx1182_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n1022_lit_string_α:
                        mov              qword ptr [rbp + 1424], 1
                        mov              rax, qword ptr [rip + .Lx1183_0]
                        mov              qword ptr [rbp + 1432], rax
                                                                                        jmp   n1031_binop_α
.Lx1183_0:
                        .quad            .Lx1183_0_s
.Lx1183_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n1023_assign_α:
                        mov              rax, qword ptr [rbp + 1504]
                        mov              rdx, qword ptr [rbp + 1512]
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n976_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1024_lit_integer_α:
                        mov              qword ptr [rbp + 272], 6
                        mov              rax, qword ptr [rip + .Lx1185_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n1032_binop_α
.Lx1185_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n1025_assign_α:
                        mov              rax, qword ptr [rbp + 384]
                        mov              rdx, qword ptr [rbp + 392]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n962_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1026_call_α:
                        mov              rax, qword ptr [rbp + 1808]
                        mov              qword ptr [rbp + 1776], rax
                        mov              rax, qword ptr [rbp + 1816]
                        mov              qword ptr [rbp + 1784], rax
                        .section         .rodata
.Lrkfn1188:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1188]
                        lea              rsi, [rbp + 1776]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1760], rax
                        mov              qword ptr [rbp + 1768], rdx
                        cmp              eax, 99
                                                                                        je    n1034_lit_string_α
                                                                                        jmp   n1033_assign_α
n1026_call_β:
                                                                                        jmp   n1034_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1027_assign_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              rdx, qword ptr [rbp + 600]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n953_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1028_deref_α:
                        mov              rdi, qword ptr [rbp + 704]
                        mov              rsi, qword ptr [rbp + 712]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n967_op14_α
                        mov              qword ptr [rbp + 720], rax
                        mov              qword ptr [rbp + 728], rdx
                                                                                        jmp   n1035_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n1029_binop_α:
                        mov              rdi, qword ptr [rbp + 992]
                        mov              rsi, qword ptr [rbp + 1000]
                        mov              rdx, qword ptr [rbp + 1008]
                        mov              rcx, qword ptr [rbp + 1016]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 976], rax
                        mov              qword ptr [rbp + 984], rdx
                                                                                        jmp   n1036_call_α
#-----------------------------------------------------------------------------------------------------------------------
n1030_assign_α:
                        mov              rsi, qword ptr [rbp + 2464]
                        mov              rdx, qword ptr [rbp + 2472]
                        mov              rdi, qword ptr [rip + .Lx1192_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n945_var_α
.Lx1192_0:
                        .quad            .Lx1192_0_s
.Lx1192_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n1031_binop_α:
                        mov              rdi, qword ptr [rbp + 1408]
                        mov              rsi, qword ptr [rbp + 1416]
                        mov              rdx, qword ptr [rbp + 1424]
                        mov              rcx, qword ptr [rbp + 1432]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                                                                                        jmp   n1037_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1032_binop_α:
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 100
                                                                                        je    .Lx1194_0
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 6
                                                                                        jne   .Lx1194_2
.Lx1194_1:
                        mov              rax, qword ptr [rbp + 264]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rbp + 192], 6
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n1038_assign_var_α
.Lx1194_0:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 0
                        lea              r9, [rbp + 192]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n1038_assign_var_α
.Lx1194_2:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 0
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n950_var_α
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   n1038_assign_var_α
n1032_binop_β:
                                                                                        jmp   n950_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1033_assign_α:
                        mov              rax, qword ptr [rbp + 1760]
                        mov              rdx, qword ptr [rbp + 1768]
                        mov              qword ptr [1879052512], rax
                        mov              qword ptr [1879052520], rdx
                                                                                        jmp   n1034_lit_string_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n1034_lit_string_α:
                        mov              qword ptr [rbp + 1872], 1
                        mov              rax, qword ptr [rip + .Lx1196_0]
                        mov              qword ptr [rbp + 1880], rax
                                                                                        jmp   n1039_call_α
.Lx1196_0:
                        .quad            .Lx1196_0_s
.Lx1196_0_s:
                        .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n1035_lit_integer_α:
                        mov              qword ptr [rbp + 736], 6
                        mov              rax, qword ptr [rip + .Lx1197_0]
                        mov              qword ptr [rbp + 744], rax
                                                                                        jmp   n1040_binop_α
.Lx1197_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n1036_call_α:
                        mov              rax, qword ptr [rbp + 976]
                        mov              qword ptr [rbp + 944], rax
                        mov              rax, qword ptr [rbp + 984]
                        mov              qword ptr [rbp + 952], rax
                        .section         .rodata
.Lrkfn1199:             .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1199]
                        lea              rsi, [rbp + 944]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 928], rax
                        mov              qword ptr [rbp + 936], rdx
                        cmp              eax, 99
                                                                                        je    n992_lit_string_α
                                                                                        jmp   n1041_assign_var_α
n1036_call_β:
                                                                                        jmp   n992_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1037_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 1440], rax
                        mov              qword ptr [rbp + 1448], rdx
                                                                                        jmp   n1042_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n1038_assign_var_α:
                        mov              rdi, qword ptr [rbp + 176]
                        mov              rsi, qword ptr [rbp + 184]
                        mov              rdx, qword ptr [rbp + 192]
                        mov              rcx, qword ptr [rbp + 200]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n950_var_α
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                                                                                        jmp   n950_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1039_call_α:
                        mov              rax, qword ptr [rbp + 1872]
                        mov              qword ptr [rbp + 1840], rax
                        mov              rax, qword ptr [rbp + 1880]
                        mov              qword ptr [rbp + 1848], rax
                        .section         .rodata
.Lrkfn1203:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1203]
                        lea              rsi, [rbp + 1840]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1824], rax
                        mov              qword ptr [rbp + 1832], rdx
                        cmp              eax, 99
                                                                                        je    n1044_lit_string_α
                                                                                        jmp   n1043_assign_α
n1039_call_β:
                                                                                        jmp   n1044_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1040_binop_α:
                        mov              eax, dword ptr [rbp + 720]
                        cmp              eax, 100
                                                                                        je    .Lx1204_0
                        mov              eax, dword ptr [rbp + 720]
                        cmp              eax, 6
                                                                                        jne   .Lx1204_2
.Lx1204_1:
                        mov              rax, qword ptr [rbp + 728]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 656], 6
                        mov              qword ptr [rbp + 664], rax
                                                                                        jmp   n1045_assign_var_α
.Lx1204_0:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        mov              r8d, 1
                        lea              r9, [rbp + 656]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n1045_assign_var_α
.Lx1204_2:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        mov              r8d, 1
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n967_op14_α
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n1045_assign_var_α
n1040_binop_β:
                                                                                        jmp   n967_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n1041_assign_var_α:
                        mov              rdi, qword ptr [rbp + 896]
                        mov              rsi, qword ptr [rbp + 904]
                        mov              rdx, qword ptr [rbp + 928]
                        mov              rcx, qword ptr [rbp + 936]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n992_lit_string_α
                        mov              qword ptr [rbp + 1024], rax
                        mov              qword ptr [rbp + 1032], rdx
                                                                                        jmp   n992_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1042_binop_α:
                        mov              rdi, qword ptr [rbp + 1392]
                        mov              rsi, qword ptr [rbp + 1400]
                        mov              rdx, qword ptr [rbp + 1440]
                        mov              rcx, qword ptr [rbp + 1448]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                                                                                        jmp   n1046_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1043_assign_α:
                        mov              rax, qword ptr [rbp + 1824]
                        mov              rdx, qword ptr [rbp + 1832]
                        mov              qword ptr [1879052528], rax
                        mov              qword ptr [1879052536], rdx
                                                                                        jmp   n1044_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n1044_lit_string_α:
                        mov              qword ptr [rbp + 1936], 1
                        mov              rax, qword ptr [rip + .Lx1208_0]
                        mov              qword ptr [rbp + 1944], rax
                                                                                        jmp   n1047_call_α
.Lx1208_0:
                        .quad            .Lx1208_0_s
.Lx1208_0_s:
                        .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n1045_assign_var_α:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 656]
                        mov              rcx, qword ptr [rbp + 664]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n967_op14_α
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   n967_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n1046_lit_string_α:
                        mov              qword ptr [rbp + 1456], 1
                        mov              rax, qword ptr [rip + .Lx1210_0]
                        mov              qword ptr [rbp + 1464], rax
                                                                                        jmp   n1048_binop_α
.Lx1210_0:
                        .quad            .Lx1210_0_s
.Lx1210_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n1047_call_α:
                        mov              rax, qword ptr [rbp + 1936]
                        mov              qword ptr [rbp + 1904], rax
                        mov              rax, qword ptr [rbp + 1944]
                        mov              qword ptr [rbp + 1912], rax
                        .section         .rodata
.Lrkfn1212:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1212]
                        lea              rsi, [rbp + 1904]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1888], rax
                        mov              qword ptr [rbp + 1896], rdx
                        cmp              eax, 99
                                                                                        je    n1050_lit_string_α
                                                                                        jmp   n1049_assign_α
n1047_call_β:
                                                                                        jmp   n1050_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1048_binop_α:
                        mov              rdi, qword ptr [rbp + 1376]
                        mov              rsi, qword ptr [rbp + 1384]
                        mov              rdx, qword ptr [rbp + 1456]
                        mov              rcx, qword ptr [rbp + 1464]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1360], rax
                        mov              qword ptr [rbp + 1368], rdx
                                                                                        jmp   n1051_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1049_assign_α:
                        mov              rax, qword ptr [rbp + 1888]
                        mov              rdx, qword ptr [rbp + 1896]
                        mov              qword ptr [1879052544], rax
                        mov              qword ptr [1879052552], rdx
                                                                                        jmp   n1050_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n1050_lit_string_α:
                        mov              qword ptr [rbp + 2000], 1
                        mov              rax, qword ptr [rip + .Lx1215_0]
                        mov              qword ptr [rbp + 2008], rax
                                                                                        jmp   n1052_call_α
.Lx1215_0:
                        .quad            .Lx1215_0_s
.Lx1215_0_s:
                        .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n1051_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rbp + 1472], rax
                        mov              qword ptr [rbp + 1480], rdx
                                                                                        jmp   n1053_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n1052_call_α:
                        mov              rax, qword ptr [rbp + 2000]
                        mov              qword ptr [rbp + 1968], rax
                        mov              rax, qword ptr [rbp + 2008]
                        mov              qword ptr [rbp + 1976], rax
                        .section         .rodata
.Lrkfn1218:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1218]
                        lea              rsi, [rbp + 1968]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1952], rax
                        mov              qword ptr [rbp + 1960], rdx
                        cmp              eax, 99
                                                                                        je    n1055_lit_string_α
                                                                                        jmp   n1054_assign_α
n1052_call_β:
                                                                                        jmp   n1055_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1053_binop_α:
                        mov              rdi, qword ptr [rbp + 1360]
                        mov              rsi, qword ptr [rbp + 1368]
                        mov              rdx, qword ptr [rbp + 1472]
                        mov              rcx, qword ptr [rbp + 1480]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1344], rax
                        mov              qword ptr [rbp + 1352], rdx
                                                                                        jmp   n1056_call_α
#-----------------------------------------------------------------------------------------------------------------------
n1054_assign_α:
                        mov              rax, qword ptr [rbp + 1952]
                        mov              rdx, qword ptr [rbp + 1960]
                        mov              qword ptr [1879052560], rax
                        mov              qword ptr [1879052568], rdx
                                                                                        jmp   n1055_lit_string_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n1055_lit_string_α:
                        mov              qword ptr [rbp + 2064], 1
                        mov              rax, qword ptr [rip + .Lx1221_0]
                        mov              qword ptr [rbp + 2072], rax
                                                                                        jmp   n1057_call_α
.Lx1221_0:
                        .quad            .Lx1221_0_s
.Lx1221_0_s:
                        .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n1056_call_α:
                        mov              rax, qword ptr [rbp + 1344]
                        mov              qword ptr [rbp + 1312], rax
                        mov              rax, qword ptr [rbp + 1352]
                        mov              qword ptr [rbp + 1320], rax
                        .section         .rodata
.Lrkfn1223:             .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1223]
                        lea              rsi, [rbp + 1312]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1296], rax
                        mov              qword ptr [rbp + 1304], rdx
                        cmp              eax, 99
                                                                                        je    n995_lit_string_α
                                                                                        jmp   n1058_assign_var_α
n1056_call_β:
                                                                                        jmp   n995_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1057_call_α:
                        mov              rax, qword ptr [rbp + 2064]
                        mov              qword ptr [rbp + 2032], rax
                        mov              rax, qword ptr [rbp + 2072]
                        mov              qword ptr [rbp + 2040], rax
                        .section         .rodata
.Lrkfn1225:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1225]
                        lea              rsi, [rbp + 2032]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2016], rax
                        mov              qword ptr [rbp + 2024], rdx
                        cmp              eax, 99
                                                                                        je    n1060_lit_string_α
                                                                                        jmp   n1059_assign_α
n1057_call_β:
                                                                                        jmp   n1060_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1058_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1264]
                        mov              rsi, qword ptr [rbp + 1272]
                        mov              rdx, qword ptr [rbp + 1296]
                        mov              rcx, qword ptr [rbp + 1304]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n995_lit_string_α
                        mov              qword ptr [rbp + 1488], rax
                        mov              qword ptr [rbp + 1496], rdx
                                                                                        jmp   n995_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1059_assign_α:
                        mov              rax, qword ptr [rbp + 2016]
                        mov              rdx, qword ptr [rbp + 2024]
                        mov              qword ptr [1879052576], rax
                        mov              qword ptr [1879052584], rdx
                                                                                        jmp   n1060_lit_string_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n1060_lit_string_α:
                        mov              qword ptr [rbp + 2128], 1
                        mov              rax, qword ptr [rip + .Lx1228_0]
                        mov              qword ptr [rbp + 2136], rax
                                                                                        jmp   n1061_call_α
.Lx1228_0:
                        .quad            .Lx1228_0_s
.Lx1228_0_s:
                        .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n1061_call_α:
                        mov              rax, qword ptr [rbp + 2128]
                        mov              qword ptr [rbp + 2096], rax
                        mov              rax, qword ptr [rbp + 2136]
                        mov              qword ptr [rbp + 2104], rax
                        .section         .rodata
.Lrkfn1230:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1230]
                        lea              rsi, [rbp + 2096]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2080], rax
                        mov              qword ptr [rbp + 2088], rdx
                        cmp              eax, 99
                                                                                        je    n1063_lit_string_α
                                                                                        jmp   n1062_assign_α
n1061_call_β:
                                                                                        jmp   n1063_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1062_assign_α:
                        mov              rax, qword ptr [rbp + 2080]
                        mov              rdx, qword ptr [rbp + 2088]
                        mov              qword ptr [1879052592], rax
                        mov              qword ptr [1879052600], rdx
                                                                                        jmp   n1063_lit_string_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n1063_lit_string_α:
                        mov              qword ptr [rbp + 2192], 1
                        mov              rax, qword ptr [rip + .Lx1232_0]
                        mov              qword ptr [rbp + 2200], rax
                                                                                        jmp   n1064_call_α
.Lx1232_0:
                        .quad            .Lx1232_0_s
.Lx1232_0_s:
                        .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n1064_call_α:
                        mov              rax, qword ptr [rbp + 2192]
                        mov              qword ptr [rbp + 2160], rax
                        mov              rax, qword ptr [rbp + 2200]
                        mov              qword ptr [rbp + 2168], rax
                        .section         .rodata
.Lrkfn1234:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1234]
                        lea              rsi, [rbp + 2160]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2144], rax
                        mov              qword ptr [rbp + 2152], rdx
                        cmp              eax, 99
                                                                                        je    n1066_lit_string_α
                                                                                        jmp   n1065_assign_α
n1064_call_β:
                                                                                        jmp   n1066_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1065_assign_α:
                        mov              rax, qword ptr [rbp + 2144]
                        mov              rdx, qword ptr [rbp + 2152]
                        mov              qword ptr [1879052608], rax
                        mov              qword ptr [1879052616], rdx
                                                                                        jmp   n1066_lit_string_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n1066_lit_string_α:
                        mov              qword ptr [rbp + 2272], 1
                        mov              rax, qword ptr [rip + .Lx1236_0]
                        mov              qword ptr [rbp + 2280], rax
                                                                                        jmp   n1067_lit_integer_α
.Lx1236_0:
                        .quad            .Lx1236_0_s
.Lx1236_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n1067_lit_integer_α:
                        mov              qword ptr [rbp + 2288], 6
                        mov              rax, qword ptr [rip + .Lx1237_0]
                        mov              qword ptr [rbp + 2296], rax
                                                                                        jmp   n1068_call_α
.Lx1237_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n1068_call_α:
                        mov              rax, qword ptr [rbp + 2272]
                        mov              qword ptr [rbp + 2224], rax
                        mov              rax, qword ptr [rbp + 2280]
                        mov              qword ptr [rbp + 2232], rax
                        mov              rax, qword ptr [rbp + 2288]
                        mov              qword ptr [rbp + 2240], rax
                        mov              rax, qword ptr [rbp + 2296]
                        mov              qword ptr [rbp + 2248], rax
                        .section         .rodata
.Lrkfn1239:             .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1239]
                        lea              rsi, [rbp + 2224]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2208], rax
                        mov              qword ptr [rbp + 2216], rdx
                        cmp              eax, 99
                                                                                        je    n945_var_α
                                                                                        jmp   n945_var_α
n1068_call_β:
                                                                                        jmp   n945_var_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2520]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_ω:
                        mov              rax, [rbp + 2528]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Push_α
proc_Push_α:
                        .global          proc_Push_α
                        .global          proc_Push_β
                        .global          proc_Push_γ
                        .global          proc_Push_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              [rsp + 2536], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2512
                        xor              eax, eax
                        rep stosb
proc_Push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n1240_op14_α:
                        mov              rdi, qword ptr [rbp + 2520]
                        mov              rsi, qword ptr [rbp + 2528]
                        lea              rdx, [rbp + 2544]
                        mov              rcx, qword ptr [rbp + 2536]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n1241_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n1241_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx1245_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx1245_1
.Lx1245_0:
                        .quad            .Lx1245_0_s
.Lx1245_0_s:
                        .string          "Push"
.Lx1245_1:
                                                                                        jmp   proc_Push_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_β:
                                                                                        jmp   proc_Push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2520]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_ω:
                        mov              rax, [rbp + 2528]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Pop_α
proc_Pop_α:
                        .global          proc_Pop_α
                        .global          proc_Pop_β
                        .global          proc_Pop_γ
                        .global          proc_Pop_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              [rsp + 2536], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2512
                        xor              eax, eax
                        rep stosb
proc_Pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n1246_op14_α:
                        mov              rdi, qword ptr [rbp + 2520]
                        mov              rsi, qword ptr [rbp + 2528]
                        lea              rdx, [rbp + 2544]
                        mov              rcx, qword ptr [rbp + 2536]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n1247_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n1247_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx1251_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx1251_1
.Lx1251_0:
                        .quad            .Lx1251_0_s
.Lx1251_0_s:
                        .string          "Pop"
.Lx1251_1:
                                                                                        jmp   proc_Pop_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_β:
                                                                                        jmp   proc_Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2520]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_ω:
                        mov              rax, [rbp + 2528]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Unary_α
proc_Unary_α:
                        .global          proc_Unary_α
                        .global          proc_Unary_β
                        .global          proc_Unary_γ
                        .global          proc_Unary_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              [rsp + 2536], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2512
                        xor              eax, eax
                        rep stosb
proc_Unary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n1252_op14_α:
                        mov              rdi, qword ptr [rbp + 2520]
                        mov              rsi, qword ptr [rbp + 2528]
                        lea              rdx, [rbp + 2544]
                        mov              rcx, qword ptr [rbp + 2536]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n1253_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n1253_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx1257_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx1257_1
.Lx1257_0:
                        .quad            .Lx1257_0_s
.Lx1257_0_s:
                        .string          "Unary"
.Lx1257_1:
                                                                                        jmp   proc_Unary_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_β:
                                                                                        jmp   proc_Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2520]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_ω:
                        mov              rax, [rbp + 2528]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Binary_α
proc_Binary_α:
                        .global          proc_Binary_α
                        .global          proc_Binary_β
                        .global          proc_Binary_γ
                        .global          proc_Binary_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              [rsp + 2536], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2512
                        xor              eax, eax
                        rep stosb
proc_Binary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n1258_op14_α:
                        mov              rdi, qword ptr [rbp + 2520]
                        mov              rsi, qword ptr [rbp + 2528]
                        lea              rdx, [rbp + 2544]
                        mov              rcx, qword ptr [rbp + 2536]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n1259_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n1259_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx1263_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx1263_1
.Lx1263_0:
                        .quad            .Lx1263_0_s
.Lx1263_0_s:
                        .string          "Binary"
.Lx1263_1:
                                                                                        jmp   proc_Binary_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_β:
                                                                                        jmp   proc_Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2520]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_ω:
                        mov              rax, [rbp + 2528]
                        lea              rsp, [rbp + 2544]
                        mov              rbp, [rbp + 2536]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
                        .global          proc_PAT$0_α
                        .global          proc_PAT$0_β
                        .global          proc_PAT$0_γ
                        .global          proc_PAT$0_ω
                        sub              rsp, 80
                        mov              [rsp + 56], rcx
                        mov              [rsp + 64], rdx
                        mov              [rsp + 72], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 32], 0
                        mov              qword ptr [rsp + 40], 0
                        mov              qword ptr [rbp + 48], r8
                        mov              dword ptr [rbp + 40], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
                        lea              rax, [rip + n1264_match_span_β]
                        mov              qword ptr [rbp + 32], rax
#-----------------------------------------------------------------------------------------------------------------------
n1264_match_span_α:
                        sub              rsp, 16
                        lea              rdi, [rip + .C0]
                        movsxd           rcx, r14d
.Lx1266_0:
                        cmp              ecx, r15d
                                                                                        jge   .Lx1266_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx1266_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx1266_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx1266_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx1266_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx1266_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx1266_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx1266_1
                        add              ecx, 1
                                                                                        jmp   .Lx1266_0
.Lx1266_1:
                        cmp              ecx, r14d
                                                                                        jg    .Lx1266_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
.Lx1266_240:
                        mov              dword ptr [rsp + 4], r14d
                        mov              r14d, ecx
                                                                                        jmp   proc_PAT$0_scanhit
n1264_match_span_β:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
proc_PAT$0_scanhit:
                        cmp              qword ptr [rbp + 48], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 40]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$0_γ
proc_PAT$0_scanfail:
                        cmp              qword ptr [rbp + 48], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 40]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 40], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$0_attempt
8:
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_β:
                                                                                        jmp   qword ptr [rbp + 32]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$0_res]
                        push             rax
                        mov              rax, [rbp + 56]
                        mov              rbp, [rbp + 72]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                        mov              rax, [rbp + 64]
                        lea              rsp, [rbp + 80]
                        mov              rbp, [rbp + 72]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$1_α
proc_PAT$1_α:
                        .global          proc_PAT$1_α
                        .global          proc_PAT$1_β
                        .global          proc_PAT$1_γ
                        .global          proc_PAT$1_ω
                        sub              rsp, 144
                        mov              [rsp + 120], rcx
                        mov              [rsp + 128], rdx
                        mov              [rsp + 136], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 96], 0
                        mov              qword ptr [rsp + 104], 0
                        mov              qword ptr [rbp + 112], r8
                        mov              dword ptr [rbp + 104], r14d
proc_PAT$1_attempt:
proc_PAT$1_α_body:
                        lea              rax, [rip + n1267_match_sequence_β]
                        mov              qword ptr [rbp + 96], rax
#-----------------------------------------------------------------------------------------------------------------------
n1267_match_sequence_α:
                        mov              dword ptr [rbp + 16], r14d
                                                                                        jmp   n1268_match_any_α
n1267_match_sequence_as:
                                                                                        jmp   proc_PAT$1_γ
n1267_match_sequence_β:
                                                                                        jmp   n1270_match_defer_β
n1267_match_sequence_af:
                                                                                        jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
n1268_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   proc_PAT$1_ω
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C1]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    proc_PAT$1_ω
                        add              r14d, 1
                                                                                        jmp   n1269_match_alternate_α
n1268_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
n1269_match_alternate_α:
                        mov              dword ptr [rbp + 48], r14d
                        mov              dword ptr [rbp + 52], 0
                                                                                        jmp   n1271_match_any_α
n1269_match_alternate_as:
                                                                                        jmp   n1270_match_defer_α
n1269_match_alternate_β:
                        mov              eax, dword ptr [rbp + 52]
                        cmp              eax, 0
                                                                                        je    n1271_match_any_β
                                                                                        jmp   n1272_match_defer_β
n1269_match_alternate_af:
                        add              dword ptr [rbp + 52], 1
                        mov              r14d, dword ptr [rbp + 48]
                        mov              eax, dword ptr [rbp + 52]
                        cmp              eax, 1
                                                                                        je    n1272_match_defer_α
                                                                                        jmp   n1268_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n1270_match_defer_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx1279_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1279_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1279_10
.Lx1279_9:
                        xor              eax, eax
.Lx1279_10:
                        test             rax, rax
                                                                                        jz    .Lx1279_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1279_4]
                        lea              rdx, [rip + .Lx1279_5]
                                                                                        jmp   rax
.Lx1279_4:
                                                                                        jmp   proc_PAT$1_γ
.Lx1279_5:
                                                                                        jmp   n1269_match_alternate_β
.Lx1279_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1279_2:
                        test             rax, rax
                                                                                        je    .Lx1279_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1279_7]
                        lea              rdx, [rip + .Lx1279_8]
                                                                                        jmp   rax
.Lx1279_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1279_2
.Lx1279_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1279_2
.Lx1279_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1269_match_alternate_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1279_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   proc_PAT$1_γ
.Lx1279_6:
                        add              rsp, 16
                                                                                        jmp   n1269_match_alternate_β
n1270_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1271_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   n1269_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43
                                                                                        je    .Lx1281_0
                        cmp              esi, 45
                                                                                        je    .Lx1281_0
                                                                                        jmp   n1269_match_alternate_af
.Lx1281_0:
                        add              r14d, 1
                                                                                        jmp   n1269_match_alternate_as
n1271_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   n1269_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n1272_match_defer_α:
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx1282_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1282_4]
                        lea              rdx, [rip + .Lx1282_5]
                                                                                        jmp   rax
.Lx1282_4:
                                                                                        jmp   n1269_match_alternate_as
.Lx1282_5:
                                                                                        jmp   n1269_match_alternate_af
.Lx1282_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1282_2:
                        test             rax, rax
                                                                                        je    .Lx1282_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1282_7]
                        lea              rdx, [rip + .Lx1282_8]
                                                                                        jmp   rax
.Lx1282_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1282_2
.Lx1282_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1282_2
.Lx1282_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1269_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1282_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1269_match_alternate_as
.Lx1282_6:
                        add              rsp, 16
                                                                                        jmp   n1269_match_alternate_af
n1272_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$1_scanhit:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 104]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$1_γ
proc_PAT$1_scanfail:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 104]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 104], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$1_attempt
8:
                                                                                        jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_β:
                                                                                        jmp   qword ptr [rbp + 96]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$1_res]
                        push             rax
                        mov              rax, [rbp + 120]
                        mov              rbp, [rbp + 136]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_ω:
                        mov              rax, [rbp + 128]
                        lea              rsp, [rbp + 144]
                        mov              rbp, [rbp + 136]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$2_α
proc_PAT$2_α:
                        .global          proc_PAT$2_α
                        .global          proc_PAT$2_β
                        .global          proc_PAT$2_γ
                        .global          proc_PAT$2_ω
                        sub              rsp, 272
                        mov              [rsp + 248], rcx
                        mov              [rsp + 256], rdx
                        mov              [rsp + 264], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 224], 0
                        mov              qword ptr [rsp + 232], 0
                        mov              qword ptr [rbp + 240], r8
                        mov              dword ptr [rbp + 232], r14d
proc_PAT$2_attempt:
proc_PAT$2_α_body:
                        lea              rax, [rip + n1283_match_alternate_β]
                        mov              qword ptr [rbp + 224], rax
#-----------------------------------------------------------------------------------------------------------------------
n1283_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        mov              dword ptr [rbp + 20], 0
                                                                                        jmp   n1284_match_sequence_α
n1283_match_alternate_as:
                                                                                        jmp   proc_PAT$2_γ
n1283_match_alternate_β:
                        mov              eax, dword ptr [rbp + 20]
                        cmp              eax, 0
                                                                                        je    n1284_match_sequence_β
                                                                                        jmp   n1285_match_sequence_β
n1283_match_alternate_af:
                        add              dword ptr [rbp + 20], 1
                        mov              r14d, dword ptr [rbp + 16]
                        mov              eax, dword ptr [rbp + 20]
                        cmp              eax, 1
                                                                                        je    n1285_match_sequence_α
                                                                                        jmp   proc_PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
n1284_match_sequence_α:
                        mov              dword ptr [rbp + 32], r14d
                                                                                        jmp   n1286_match_defer_α
n1284_match_sequence_as:
                                                                                        jmp   n1283_match_alternate_as
n1284_match_sequence_β:
                                                                                        jmp   n1289_match_alternate_β
n1284_match_sequence_af:
                                                                                        jmp   n1283_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n1285_match_sequence_α:
                        mov              dword ptr [rbp + 176], r14d
                                                                                        jmp   n1290_match_defer_α
n1285_match_sequence_as:
                                                                                        jmp   n1283_match_alternate_as
n1285_match_sequence_β:
                                                                                        jmp   n1291_match_defer_β
n1285_match_sequence_af:
                                                                                        jmp   n1283_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n1286_match_defer_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx1302_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1302_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1302_10
.Lx1302_9:
                        xor              eax, eax
.Lx1302_10:
                        test             rax, rax
                                                                                        jz    .Lx1302_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1302_4]
                        lea              rdx, [rip + .Lx1302_5]
                                                                                        jmp   rax
.Lx1302_4:
                                                                                        jmp   n1287_match_lit_α
.Lx1302_5:
                                                                                        jmp   n1283_match_alternate_af
.Lx1302_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1302_2:
                        test             rax, rax
                                                                                        je    .Lx1302_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1302_7]
                        lea              rdx, [rip + .Lx1302_8]
                                                                                        jmp   rax
.Lx1302_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1302_2
.Lx1302_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1302_2
.Lx1302_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1283_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1302_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1287_match_lit_α
.Lx1302_6:
                        add              rsp, 16
                                                                                        jmp   n1283_match_alternate_af
n1286_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1287_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n1286_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 46
                                                                                        jne   n1286_match_defer_β
                        add              r14d, 1
                                                                                        jmp   n1288_match_alternate_α
n1287_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n1286_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n1288_match_alternate_α:
                        mov              dword ptr [rbp + 80], r14d
                        mov              dword ptr [rbp + 84], 0
                                                                                        jmp   n1292_match_defer_α
n1288_match_alternate_as:
                                                                                        jmp   n1289_match_alternate_α
n1288_match_alternate_β:
                        mov              eax, dword ptr [rbp + 84]
                        cmp              eax, 0
                                                                                        je    n1292_match_defer_β
                                                                                        jmp   n1293_match_defer_β
n1288_match_alternate_af:
                        add              dword ptr [rbp + 84], 1
                        mov              r14d, dword ptr [rbp + 80]
                        mov              eax, dword ptr [rbp + 84]
                        cmp              eax, 1
                                                                                        je    n1293_match_defer_α
                                                                                        jmp   n1287_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n1289_match_alternate_α:
                        mov              dword ptr [rbp + 128], r14d
                        mov              dword ptr [rbp + 132], 0
                                                                                        jmp   n1294_match_defer_α
n1289_match_alternate_as:
                                                                                        jmp   n1283_match_alternate_as
n1289_match_alternate_β:
                        mov              eax, dword ptr [rbp + 132]
                        cmp              eax, 0
                                                                                        je    n1294_match_defer_β
                                                                                        jmp   n1295_match_defer_β
n1289_match_alternate_af:
                        add              dword ptr [rbp + 132], 1
                        mov              r14d, dword ptr [rbp + 128]
                        mov              eax, dword ptr [rbp + 132]
                        cmp              eax, 1
                                                                                        je    n1295_match_defer_α
                                                                                        jmp   n1288_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n1290_match_defer_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx1309_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1309_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1309_10
.Lx1309_9:
                        xor              eax, eax
.Lx1309_10:
                        test             rax, rax
                                                                                        jz    .Lx1309_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1309_4]
                        lea              rdx, [rip + .Lx1309_5]
                                                                                        jmp   rax
.Lx1309_4:
                                                                                        jmp   n1291_match_defer_α
.Lx1309_5:
                                                                                        jmp   n1283_match_alternate_af
.Lx1309_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1309_2:
                        test             rax, rax
                                                                                        je    .Lx1309_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1309_7]
                        lea              rdx, [rip + .Lx1309_8]
                                                                                        jmp   rax
.Lx1309_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1309_2
.Lx1309_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1309_2
.Lx1309_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1283_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1309_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1291_match_defer_α
.Lx1309_6:
                        add              rsp, 16
                                                                                        jmp   n1283_match_alternate_af
n1290_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1291_match_defer_α:
                        mov              rax, qword ptr [1879052480]
                        mov              rdx, qword ptr [1879052488]
                        cmp              eax, 3
                                                                                        jne   .Lx1310_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1310_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1310_10
.Lx1310_9:
                        xor              eax, eax
.Lx1310_10:
                        test             rax, rax
                                                                                        jz    .Lx1310_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1310_4]
                        lea              rdx, [rip + .Lx1310_5]
                                                                                        jmp   rax
.Lx1310_4:
                                                                                        jmp   n1283_match_alternate_as
.Lx1310_5:
                                                                                        jmp   n1290_match_defer_β
.Lx1310_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1310_2:
                        test             rax, rax
                                                                                        je    .Lx1310_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1310_7]
                        lea              rdx, [rip + .Lx1310_8]
                                                                                        jmp   rax
.Lx1310_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1310_2
.Lx1310_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1310_2
.Lx1310_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1290_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1310_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1283_match_alternate_as
.Lx1310_6:
                        add              rsp, 16
                                                                                        jmp   n1290_match_defer_β
n1291_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1292_match_defer_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx1311_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1311_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1311_10
.Lx1311_9:
                        xor              eax, eax
.Lx1311_10:
                        test             rax, rax
                                                                                        jz    .Lx1311_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1311_4]
                        lea              rdx, [rip + .Lx1311_5]
                                                                                        jmp   rax
.Lx1311_4:
                                                                                        jmp   n1288_match_alternate_as
.Lx1311_5:
                                                                                        jmp   n1288_match_alternate_af
.Lx1311_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1311_2:
                        test             rax, rax
                                                                                        je    .Lx1311_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1311_7]
                        lea              rdx, [rip + .Lx1311_8]
                                                                                        jmp   rax
.Lx1311_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1311_2
.Lx1311_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1311_2
.Lx1311_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1288_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1311_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1288_match_alternate_as
.Lx1311_6:
                        add              rsp, 16
                                                                                        jmp   n1288_match_alternate_af
n1292_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1293_match_defer_α:
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx1312_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1312_4]
                        lea              rdx, [rip + .Lx1312_5]
                                                                                        jmp   rax
.Lx1312_4:
                                                                                        jmp   n1288_match_alternate_as
.Lx1312_5:
                                                                                        jmp   n1288_match_alternate_af
.Lx1312_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1312_2:
                        test             rax, rax
                                                                                        je    .Lx1312_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1312_7]
                        lea              rdx, [rip + .Lx1312_8]
                                                                                        jmp   rax
.Lx1312_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1312_2
.Lx1312_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1312_2
.Lx1312_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1288_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1312_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1288_match_alternate_as
.Lx1312_6:
                        add              rsp, 16
                                                                                        jmp   n1288_match_alternate_af
n1293_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1294_match_defer_α:
                        mov              rax, qword ptr [1879052480]
                        mov              rdx, qword ptr [1879052488]
                        cmp              eax, 3
                                                                                        jne   .Lx1313_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1313_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1313_10
.Lx1313_9:
                        xor              eax, eax
.Lx1313_10:
                        test             rax, rax
                                                                                        jz    .Lx1313_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1313_4]
                        lea              rdx, [rip + .Lx1313_5]
                                                                                        jmp   rax
.Lx1313_4:
                                                                                        jmp   n1289_match_alternate_as
.Lx1313_5:
                                                                                        jmp   n1289_match_alternate_af
.Lx1313_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1313_2:
                        test             rax, rax
                                                                                        je    .Lx1313_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1313_7]
                        lea              rdx, [rip + .Lx1313_8]
                                                                                        jmp   rax
.Lx1313_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1313_2
.Lx1313_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1313_2
.Lx1313_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1289_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1313_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1289_match_alternate_as
.Lx1313_6:
                        add              rsp, 16
                                                                                        jmp   n1289_match_alternate_af
n1294_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1295_match_defer_α:
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx1314_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1314_4]
                        lea              rdx, [rip + .Lx1314_5]
                                                                                        jmp   rax
.Lx1314_4:
                                                                                        jmp   n1289_match_alternate_as
.Lx1314_5:
                                                                                        jmp   n1289_match_alternate_af
.Lx1314_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1314_2:
                        test             rax, rax
                                                                                        je    .Lx1314_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1314_7]
                        lea              rdx, [rip + .Lx1314_8]
                                                                                        jmp   rax
.Lx1314_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1314_2
.Lx1314_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1314_2
.Lx1314_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1289_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1314_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1289_match_alternate_as
.Lx1314_6:
                        add              rsp, 16
                                                                                        jmp   n1289_match_alternate_af
n1295_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$2_scanhit:
                        cmp              qword ptr [rbp + 240], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 232]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$2_γ
proc_PAT$2_scanfail:
                        cmp              qword ptr [rbp + 240], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 232]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 232], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$2_attempt
8:
                                                                                        jmp   proc_PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_β:
                                                                                        jmp   qword ptr [rbp + 224]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$2_res]
                        push             rax
                        mov              rax, [rbp + 248]
                        mov              rbp, [rbp + 264]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_ω:
                        mov              rax, [rbp + 256]
                        lea              rsp, [rbp + 272]
                        mov              rbp, [rbp + 264]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$3_α
proc_PAT$3_α:
                        .global          proc_PAT$3_α
                        .global          proc_PAT$3_β
                        .global          proc_PAT$3_γ
                        .global          proc_PAT$3_ω
                        sub              rsp, 128
                        mov              [rsp + 104], rcx
                        mov              [rsp + 112], rdx
                        mov              [rsp + 120], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rbp + 96], r8
                        mov              dword ptr [rbp + 88], r14d
proc_PAT$3_attempt:
proc_PAT$3_α_body:
                        lea              rax, [rip + n1317_match_assign_cond_β]
                        mov              qword ptr [rbp + 80], rax
#-----------------------------------------------------------------------------------------------------------------------
n1315_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n1316_match_any_α
n1315_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n1316_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx1321_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
.Lx1321_240:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43
                                                                                        je    .Lx1321_0
                        cmp              esi, 45
                                                                                        je    .Lx1321_0
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
.Lx1321_0:
                        add              r14d, 1
                                                                                        jmp   n1317_match_assign_cond_α
n1316_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n1317_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S4]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   proc_PAT$3_scanhit
n1317_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n1316_match_any_β
proc_PAT$3_scanhit:
                        cmp              qword ptr [rbp + 96], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 88]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$3_γ
proc_PAT$3_scanfail:
                        cmp              qword ptr [rbp + 96], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 88]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 88], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$3_attempt
8:
                                                                                        jmp   proc_PAT$3_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_β:
                                                                                        jmp   qword ptr [rbp + 80]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$3_res]
                        push             rax
                        mov              rax, [rbp + 104]
                        mov              rbp, [rbp + 120]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_ω:
                        mov              rax, [rbp + 112]
                        lea              rsp, [rbp + 128]
                        mov              rbp, [rbp + 120]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$4_α
proc_PAT$4_α:
                        .global          proc_PAT$4_α
                        .global          proc_PAT$4_β
                        .global          proc_PAT$4_γ
                        .global          proc_PAT$4_ω
                        sub              rsp, 128
                        mov              [rsp + 104], rcx
                        mov              [rsp + 112], rdx
                        mov              [rsp + 120], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rbp + 96], r8
                        mov              dword ptr [rbp + 88], r14d
proc_PAT$4_attempt:
proc_PAT$4_α_body:
                        lea              rax, [rip + n1326_match_assign_cond_β]
                        mov              qword ptr [rbp + 80], rax
#-----------------------------------------------------------------------------------------------------------------------
n1324_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n1325_match_any_α
n1324_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n1325_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx1330_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
.Lx1330_240:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 42
                                                                                        je    .Lx1330_0
                        cmp              esi, 47
                                                                                        je    .Lx1330_0
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
.Lx1330_0:
                        add              r14d, 1
                                                                                        jmp   n1326_match_assign_cond_α
n1325_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n1326_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S4]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   proc_PAT$4_scanhit
n1326_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n1325_match_any_β
proc_PAT$4_scanhit:
                        cmp              qword ptr [rbp + 96], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 88]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$4_γ
proc_PAT$4_scanfail:
                        cmp              qword ptr [rbp + 96], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 88]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 88], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$4_attempt
8:
                                                                                        jmp   proc_PAT$4_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_β:
                                                                                        jmp   qword ptr [rbp + 80]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$4_res]
                        push             rax
                        mov              rax, [rbp + 104]
                        mov              rbp, [rbp + 120]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_ω:
                        mov              rax, [rbp + 112]
                        lea              rsp, [rbp + 128]
                        mov              rbp, [rbp + 120]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$5_α
proc_PAT$5_α:
                        .global          proc_PAT$5_α
                        .global          proc_PAT$5_β
                        .global          proc_PAT$5_γ
                        .global          proc_PAT$5_ω
                        sub              rsp, 176
                        mov              [rsp + 152], rcx
                        mov              [rsp + 160], rdx
                        mov              [rsp + 168], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 128], 0
                        mov              qword ptr [rsp + 136], 0
                        mov              qword ptr [rbp + 144], r8
                        mov              dword ptr [rbp + 136], r14d
proc_PAT$5_attempt:
proc_PAT$5_α_body:
                        lea              rax, [rip + n1335_match_assign_cond_β]
                        mov              qword ptr [rbp + 128], rax
#-----------------------------------------------------------------------------------------------------------------------
n1333_match_assign_save_α:
                        lea              rdi, [rbp + 48]
                        mov              esi, r14d
                        call             rt_cap_push@PLT
                                                                                        jmp   n1334_match_alternate_α
n1333_match_assign_save_β:
                        lea              rdi, [rbp + 48]
                        call             rt_cap_pop@PLT
                                                                                        jmp   proc_PAT$5_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n1334_match_alternate_α:
                        mov              dword ptr [rbp + 80], r14d
                        mov              dword ptr [rbp + 84], 0
                                                                                        jmp   n1336_match_defer_α
n1334_match_alternate_as:
                                                                                        jmp   n1335_match_assign_cond_α
n1334_match_alternate_β:
                        mov              eax, dword ptr [rbp + 84]
                        cmp              eax, 0
                                                                                        je    n1336_match_defer_β
                                                                                        jmp   n1337_match_defer_β
n1334_match_alternate_af:
                        add              dword ptr [rbp + 84], 1
                        mov              r14d, dword ptr [rbp + 80]
                        mov              eax, dword ptr [rbp + 84]
                        cmp              eax, 1
                                                                                        je    n1337_match_defer_α
                                                                                        jmp   n1333_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n1335_match_assign_cond_α:
                        lea              rdi, [rbp + 48]
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S4]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   proc_PAT$5_scanhit
n1335_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n1334_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n1336_match_defer_α:
                        mov              rax, qword ptr [1879052496]
                        mov              rdx, qword ptr [1879052504]
                        cmp              eax, 3
                                                                                        jne   .Lx1344_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1344_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1344_10
.Lx1344_9:
                        xor              eax, eax
.Lx1344_10:
                        test             rax, rax
                                                                                        jz    .Lx1344_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1344_4]
                        lea              rdx, [rip + .Lx1344_5]
                                                                                        jmp   rax
.Lx1344_4:
                                                                                        jmp   n1334_match_alternate_as
.Lx1344_5:
                                                                                        jmp   n1334_match_alternate_af
.Lx1344_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1344_2:
                        test             rax, rax
                                                                                        je    .Lx1344_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1344_7]
                        lea              rdx, [rip + .Lx1344_8]
                                                                                        jmp   rax
.Lx1344_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1344_2
.Lx1344_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1344_2
.Lx1344_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1334_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1344_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1334_match_alternate_as
.Lx1344_6:
                        add              rsp, 16
                                                                                        jmp   n1334_match_alternate_af
n1336_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1337_match_defer_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx1345_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1345_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1345_10
.Lx1345_9:
                        xor              eax, eax
.Lx1345_10:
                        test             rax, rax
                                                                                        jz    .Lx1345_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1345_4]
                        lea              rdx, [rip + .Lx1345_5]
                                                                                        jmp   rax
.Lx1345_4:
                                                                                        jmp   n1334_match_alternate_as
.Lx1345_5:
                                                                                        jmp   n1334_match_alternate_af
.Lx1345_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1345_2:
                        test             rax, rax
                                                                                        je    .Lx1345_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1345_7]
                        lea              rdx, [rip + .Lx1345_8]
                                                                                        jmp   rax
.Lx1345_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1345_2
.Lx1345_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1345_2
.Lx1345_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1334_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1345_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1334_match_alternate_as
.Lx1345_6:
                        add              rsp, 16
                                                                                        jmp   n1334_match_alternate_af
n1337_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$5_scanhit:
                        cmp              qword ptr [rbp + 144], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 136]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$5_γ
proc_PAT$5_scanfail:
                        cmp              qword ptr [rbp + 144], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 136]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 136], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$5_attempt
8:
                                                                                        jmp   proc_PAT$5_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_β:
                                                                                        jmp   qword ptr [rbp + 128]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$5_res]
                        push             rax
                        mov              rax, [rbp + 152]
                        mov              rbp, [rbp + 168]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_ω:
                        mov              rax, [rbp + 160]
                        lea              rsp, [rbp + 176]
                        mov              rbp, [rbp + 168]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$6_α
proc_PAT$6_α:
                        .global          proc_PAT$6_α
                        .global          proc_PAT$6_β
                        .global          proc_PAT$6_γ
                        .global          proc_PAT$6_ω
                        sub              rsp, 144
                        mov              [rsp + 120], rcx
                        mov              [rsp + 128], rdx
                        mov              [rsp + 136], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 96], 0
                        mov              qword ptr [rsp + 104], 0
                        mov              qword ptr [rbp + 112], r8
                        mov              dword ptr [rbp + 104], r14d
proc_PAT$6_attempt:
proc_PAT$6_α_body:
                        lea              rax, [rip + n1346_match_alternate_β]
                        mov              qword ptr [rbp + 96], rax
#-----------------------------------------------------------------------------------------------------------------------
n1346_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        mov              dword ptr [rbp + 20], 0
                                                                                        jmp   n1347_match_defer_α
n1346_match_alternate_as:
                                                                                        jmp   proc_PAT$6_γ
n1346_match_alternate_β:
                        mov              eax, dword ptr [rbp + 20]
                        cmp              eax, 0
                                                                                        je    n1347_match_defer_β
                                                                                        jmp   n1348_match_sequence_β
n1346_match_alternate_af:
                        add              dword ptr [rbp + 20], 1
                        mov              r14d, dword ptr [rbp + 16]
                        mov              eax, dword ptr [rbp + 20]
                        cmp              eax, 1
                                                                                        je    n1348_match_sequence_α
                                                                                        jmp   proc_PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
n1347_match_defer_α:
                        mov              rax, qword ptr [1879052544]
                        mov              rdx, qword ptr [1879052552]
                        cmp              eax, 3
                                                                                        jne   .Lx1354_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1354_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1354_10
.Lx1354_9:
                        xor              eax, eax
.Lx1354_10:
                        test             rax, rax
                                                                                        jz    .Lx1354_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1354_4]
                        lea              rdx, [rip + .Lx1354_5]
                                                                                        jmp   rax
.Lx1354_4:
                                                                                        jmp   n1346_match_alternate_as
.Lx1354_5:
                                                                                        jmp   n1346_match_alternate_af
.Lx1354_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S6]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1354_2:
                        test             rax, rax
                                                                                        je    .Lx1354_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1354_7]
                        lea              rdx, [rip + .Lx1354_8]
                                                                                        jmp   rax
.Lx1354_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1354_2
.Lx1354_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1354_2
.Lx1354_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1346_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1354_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1346_match_alternate_as
.Lx1354_6:
                        add              rsp, 16
                                                                                        jmp   n1346_match_alternate_af
n1347_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1348_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n1349_match_lit_α
n1348_match_sequence_as:
                                                                                        jmp   n1346_match_alternate_as
n1348_match_sequence_β:
                                                                                        jmp   n1351_match_lit_β
n1348_match_sequence_af:
                                                                                        jmp   n1346_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n1349_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n1346_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 40
                                                                                        jne   n1346_match_alternate_af
                        add              r14d, 1
                                                                                        jmp   n1350_match_defer_α
n1349_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n1346_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n1350_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx1359_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1359_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1359_10
.Lx1359_9:
                        xor              eax, eax
.Lx1359_10:
                        test             rax, rax
                                                                                        jz    .Lx1359_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1359_4]
                        lea              rdx, [rip + .Lx1359_5]
                                                                                        jmp   rax
.Lx1359_4:
                                                                                        jmp   n1351_match_lit_α
.Lx1359_5:
                                                                                        jmp   n1349_match_lit_β
.Lx1359_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1359_2:
                        test             rax, rax
                                                                                        je    .Lx1359_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1359_7]
                        lea              rdx, [rip + .Lx1359_8]
                                                                                        jmp   rax
.Lx1359_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1359_2
.Lx1359_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1359_2
.Lx1359_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1349_match_lit_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1359_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1351_match_lit_α
.Lx1359_6:
                        add              rsp, 16
                                                                                        jmp   n1349_match_lit_β
n1350_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1351_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n1350_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 41
                                                                                        jne   n1350_match_defer_β
                        add              r14d, 1
                                                                                        jmp   n1346_match_alternate_as
n1351_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n1350_match_defer_β
proc_PAT$6_scanhit:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 104]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$6_γ
proc_PAT$6_scanfail:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 104]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 104], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$6_attempt
8:
                                                                                        jmp   proc_PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_β:
                                                                                        jmp   qword ptr [rbp + 96]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$6_res]
                        push             rax
                        mov              rax, [rbp + 120]
                        mov              rbp, [rbp + 136]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_ω:
                        mov              rax, [rbp + 128]
                        lea              rsp, [rbp + 144]
                        mov              rbp, [rbp + 136]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$7_α
proc_PAT$7_α:
                        .global          proc_PAT$7_α
                        .global          proc_PAT$7_β
                        .global          proc_PAT$7_γ
                        .global          proc_PAT$7_ω
                        sub              rsp, 208
                        mov              [rsp + 184], rcx
                        mov              [rsp + 192], rdx
                        mov              [rsp + 200], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 96], 0
                        mov              qword ptr [rsp + 104], 0
                        mov              qword ptr [rsp + 160], 0
                        mov              qword ptr [rsp + 168], 0
                        mov              qword ptr [rbp + 176], r8
                        mov              dword ptr [rbp + 168], r14d
proc_PAT$7_attempt:
proc_PAT$7_α_body:
                        lea              rax, [rip + n1362_match_alternate_β]
                        mov              qword ptr [rbp + 160], rax
#-----------------------------------------------------------------------------------------------------------------------
n1362_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        mov              dword ptr [rbp + 20], 0
                                                                                        jmp   n1363_match_sequence_α
n1362_match_alternate_as:
                                                                                        jmp   proc_PAT$7_γ
n1362_match_alternate_β:
                        mov              eax, dword ptr [rbp + 20]
                        cmp              eax, 0
                                                                                        je    n1363_match_sequence_β
                                                                                        jmp   n1364_match_defer_β
n1362_match_alternate_af:
                        add              dword ptr [rbp + 20], 1
                        mov              r14d, dword ptr [rbp + 16]
                        mov              eax, dword ptr [rbp + 20]
                        cmp              eax, 1
                                                                                        je    n1364_match_defer_α
                                                                                        jmp   proc_PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
n1363_match_sequence_α:
                        mov              dword ptr [rbp + 32], r14d
                                                                                        jmp   n1365_match_defer_α
n1363_match_sequence_as:
                                                                                        jmp   n1362_match_alternate_as
n1363_match_sequence_β:
                                                                                        jmp   n1367_match_assign_cond_β
n1363_match_sequence_af:
                                                                                        jmp   n1362_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n1364_match_defer_α:
                        mov              rax, qword ptr [1879052560]
                        mov              rdx, qword ptr [1879052568]
                        cmp              eax, 3
                                                                                        jne   .Lx1373_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1373_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1373_10
.Lx1373_9:
                        xor              eax, eax
.Lx1373_10:
                        test             rax, rax
                                                                                        jz    .Lx1373_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1373_4]
                        lea              rdx, [rip + .Lx1373_5]
                                                                                        jmp   rax
.Lx1373_4:
                                                                                        jmp   n1362_match_alternate_as
.Lx1373_5:
                                                                                        jmp   n1362_match_alternate_af
.Lx1373_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S7]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1373_2:
                        test             rax, rax
                                                                                        je    .Lx1373_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1373_7]
                        lea              rdx, [rip + .Lx1373_8]
                                                                                        jmp   rax
.Lx1373_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1373_2
.Lx1373_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1373_2
.Lx1373_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1362_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1373_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1362_match_alternate_as
.Lx1373_6:
                        add              rsp, 16
                                                                                        jmp   n1362_match_alternate_af
n1364_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1365_match_defer_α:
                        mov              rax, qword ptr [1879052512]
                        mov              rdx, qword ptr [1879052520]
                        cmp              eax, 3
                                                                                        jne   .Lx1374_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1374_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1374_10
.Lx1374_9:
                        xor              eax, eax
.Lx1374_10:
                        test             rax, rax
                                                                                        jz    .Lx1374_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1374_4]
                        lea              rdx, [rip + .Lx1374_5]
                                                                                        jmp   rax
.Lx1374_4:
                                                                                        jmp   n1366_match_assign_save_α
.Lx1374_5:
                                                                                        jmp   n1362_match_alternate_af
.Lx1374_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1374_2:
                        test             rax, rax
                                                                                        je    .Lx1374_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1374_7]
                        lea              rdx, [rip + .Lx1374_8]
                                                                                        jmp   rax
.Lx1374_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1374_2
.Lx1374_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1374_2
.Lx1374_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1362_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1374_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1366_match_assign_save_α
.Lx1374_6:
                        add              rsp, 16
                                                                                        jmp   n1362_match_alternate_af
n1365_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1366_match_assign_save_α:
                        lea              rdi, [rbp + 96]
                        mov              esi, r14d
                        call             rt_cap_push@PLT
                                                                                        jmp   n1368_match_defer_α
n1366_match_assign_save_β:
                        lea              rdi, [rbp + 96]
                        call             rt_cap_pop@PLT
                                                                                        jmp   n1365_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n1367_match_assign_cond_α:
                        lea              rdi, [rbp + 96]
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n1362_match_alternate_as
n1367_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n1368_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n1368_match_defer_α:
                        mov              rax, qword ptr [1879052576]
                        mov              rdx, qword ptr [1879052584]
                        cmp              eax, 3
                                                                                        jne   .Lx1379_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1379_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1379_10
.Lx1379_9:
                        xor              eax, eax
.Lx1379_10:
                        test             rax, rax
                                                                                        jz    .Lx1379_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1379_4]
                        lea              rdx, [rip + .Lx1379_5]
                                                                                        jmp   rax
.Lx1379_4:
                                                                                        jmp   n1367_match_assign_cond_α
.Lx1379_5:
                                                                                        jmp   n1366_match_assign_save_β
.Lx1379_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S10]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1379_2:
                        test             rax, rax
                                                                                        je    .Lx1379_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1379_7]
                        lea              rdx, [rip + .Lx1379_8]
                                                                                        jmp   rax
.Lx1379_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1379_2
.Lx1379_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1379_2
.Lx1379_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1366_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1379_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1367_match_assign_cond_α
.Lx1379_6:
                        add              rsp, 16
                                                                                        jmp   n1366_match_assign_save_β
n1368_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$7_scanhit:
                        cmp              qword ptr [rbp + 176], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 168]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$7_γ
proc_PAT$7_scanfail:
                        cmp              qword ptr [rbp + 176], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 168]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 168], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$7_attempt
8:
                                                                                        jmp   proc_PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_β:
                                                                                        jmp   qword ptr [rbp + 160]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$7_res]
                        push             rax
                        mov              rax, [rbp + 184]
                        mov              rbp, [rbp + 200]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_ω:
                        mov              rax, [rbp + 192]
                        lea              rsp, [rbp + 208]
                        mov              rbp, [rbp + 200]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$8_α
proc_PAT$8_α:
                        .global          proc_PAT$8_α
                        .global          proc_PAT$8_β
                        .global          proc_PAT$8_γ
                        .global          proc_PAT$8_ω
                        sub              rsp, 224
                        mov              [rsp + 200], rcx
                        mov              [rsp + 208], rdx
                        mov              [rsp + 216], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 112], 0
                        mov              qword ptr [rsp + 120], 0
                        mov              qword ptr [rsp + 176], 0
                        mov              qword ptr [rsp + 184], 0
                        mov              qword ptr [rbp + 192], r8
                        mov              dword ptr [rbp + 184], r14d
proc_PAT$8_attempt:
proc_PAT$8_α_body:
                        lea              rax, [rip + n1380_match_alternate_β]
                        mov              qword ptr [rbp + 176], rax
#-----------------------------------------------------------------------------------------------------------------------
n1380_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        mov              dword ptr [rbp + 20], 0
                                                                                        jmp   n1381_match_sequence_α
n1380_match_alternate_as:
                                                                                        jmp   proc_PAT$8_γ
n1380_match_alternate_β:
                        mov              eax, dword ptr [rbp + 20]
                        cmp              eax, 0
                                                                                        je    n1381_match_sequence_β
                                                                                        jmp   n1382_match_defer_β
n1380_match_alternate_af:
                        add              dword ptr [rbp + 20], 1
                        mov              r14d, dword ptr [rbp + 16]
                        mov              eax, dword ptr [rbp + 20]
                        cmp              eax, 1
                                                                                        je    n1382_match_defer_α
                                                                                        jmp   proc_PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
n1381_match_sequence_α:
                        mov              dword ptr [rbp + 32], r14d
                                                                                        jmp   n1383_match_defer_α
n1381_match_sequence_as:
                                                                                        jmp   n1380_match_alternate_as
n1381_match_sequence_β:
                                                                                        jmp   n1386_match_assign_cond_β
n1381_match_sequence_af:
                                                                                        jmp   n1380_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n1382_match_defer_α:
                        mov              rax, qword ptr [1879052576]
                        mov              rdx, qword ptr [1879052584]
                        cmp              eax, 3
                                                                                        jne   .Lx1392_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1392_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1392_10
.Lx1392_9:
                        xor              eax, eax
.Lx1392_10:
                        test             rax, rax
                                                                                        jz    .Lx1392_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1392_4]
                        lea              rdx, [rip + .Lx1392_5]
                                                                                        jmp   rax
.Lx1392_4:
                                                                                        jmp   n1380_match_alternate_as
.Lx1392_5:
                                                                                        jmp   n1380_match_alternate_af
.Lx1392_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S10]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1392_2:
                        test             rax, rax
                                                                                        je    .Lx1392_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1392_7]
                        lea              rdx, [rip + .Lx1392_8]
                                                                                        jmp   rax
.Lx1392_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1392_2
.Lx1392_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1392_2
.Lx1392_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1380_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1392_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1380_match_alternate_as
.Lx1392_6:
                        add              rsp, 16
                                                                                        jmp   n1380_match_alternate_af
n1382_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1383_match_defer_α:
                        mov              rax, qword ptr [1879052576]
                        mov              rdx, qword ptr [1879052584]
                        cmp              eax, 3
                                                                                        jne   .Lx1393_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1393_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1393_10
.Lx1393_9:
                        xor              eax, eax
.Lx1393_10:
                        test             rax, rax
                                                                                        jz    .Lx1393_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1393_4]
                        lea              rdx, [rip + .Lx1393_5]
                                                                                        jmp   rax
.Lx1393_4:
                                                                                        jmp   n1384_match_defer_α
.Lx1393_5:
                                                                                        jmp   n1380_match_alternate_af
.Lx1393_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S10]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1393_2:
                        test             rax, rax
                                                                                        je    .Lx1393_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1393_7]
                        lea              rdx, [rip + .Lx1393_8]
                                                                                        jmp   rax
.Lx1393_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1393_2
.Lx1393_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1393_2
.Lx1393_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1380_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1393_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1384_match_defer_α
.Lx1393_6:
                        add              rsp, 16
                                                                                        jmp   n1380_match_alternate_af
n1383_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1384_match_defer_α:
                        mov              rax, qword ptr [1879052528]
                        mov              rdx, qword ptr [1879052536]
                        cmp              eax, 3
                                                                                        jne   .Lx1394_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1394_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1394_10
.Lx1394_9:
                        xor              eax, eax
.Lx1394_10:
                        test             rax, rax
                                                                                        jz    .Lx1394_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1394_4]
                        lea              rdx, [rip + .Lx1394_5]
                                                                                        jmp   rax
.Lx1394_4:
                                                                                        jmp   n1385_match_assign_save_α
.Lx1394_5:
                                                                                        jmp   n1383_match_defer_β
.Lx1394_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S11]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1394_2:
                        test             rax, rax
                                                                                        je    .Lx1394_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1394_7]
                        lea              rdx, [rip + .Lx1394_8]
                                                                                        jmp   rax
.Lx1394_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1394_2
.Lx1394_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1394_2
.Lx1394_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1383_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1394_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1385_match_assign_save_α
.Lx1394_6:
                        add              rsp, 16
                                                                                        jmp   n1383_match_defer_β
n1384_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1385_match_assign_save_α:
                        lea              rdi, [rbp + 112]
                        mov              esi, r14d
                        call             rt_cap_push@PLT
                                                                                        jmp   n1387_match_defer_α
n1385_match_assign_save_β:
                        lea              rdi, [rbp + 112]
                        call             rt_cap_pop@PLT
                                                                                        jmp   n1384_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n1386_match_assign_cond_α:
                        lea              rdi, [rbp + 112]
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S12]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n1380_match_alternate_as
n1386_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n1387_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n1387_match_defer_α:
                        mov              rax, qword ptr [1879052592]
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 3
                                                                                        jne   .Lx1399_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1399_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1399_10
.Lx1399_9:
                        xor              eax, eax
.Lx1399_10:
                        test             rax, rax
                                                                                        jz    .Lx1399_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1399_4]
                        lea              rdx, [rip + .Lx1399_5]
                                                                                        jmp   rax
.Lx1399_4:
                                                                                        jmp   n1386_match_assign_cond_α
.Lx1399_5:
                                                                                        jmp   n1385_match_assign_save_β
.Lx1399_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S13]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1399_2:
                        test             rax, rax
                                                                                        je    .Lx1399_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1399_7]
                        lea              rdx, [rip + .Lx1399_8]
                                                                                        jmp   rax
.Lx1399_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1399_2
.Lx1399_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1399_2
.Lx1399_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1385_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1399_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1386_match_assign_cond_α
.Lx1399_6:
                        add              rsp, 16
                                                                                        jmp   n1385_match_assign_save_β
n1387_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$8_scanhit:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 184]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$8_γ
proc_PAT$8_scanfail:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 184]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 184], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$8_attempt
8:
                                                                                        jmp   proc_PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_β:
                                                                                        jmp   qword ptr [rbp + 176]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$8_res]
                        push             rax
                        mov              rax, [rbp + 200]
                        mov              rbp, [rbp + 216]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_ω:
                        mov              rax, [rbp + 208]
                        lea              rsp, [rbp + 224]
                        mov              rbp, [rbp + 216]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$9_α
proc_PAT$9_α:
                        .global          proc_PAT$9_α
                        .global          proc_PAT$9_β
                        .global          proc_PAT$9_γ
                        .global          proc_PAT$9_ω
                        sub              rsp, 224
                        mov              [rsp + 200], rcx
                        mov              [rsp + 208], rdx
                        mov              [rsp + 216], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 112], 0
                        mov              qword ptr [rsp + 120], 0
                        mov              qword ptr [rsp + 176], 0
                        mov              qword ptr [rsp + 184], 0
                        mov              qword ptr [rbp + 192], r8
                        mov              dword ptr [rbp + 184], r14d
proc_PAT$9_attempt:
proc_PAT$9_α_body:
                        lea              rax, [rip + n1400_match_alternate_β]
                        mov              qword ptr [rbp + 176], rax
#-----------------------------------------------------------------------------------------------------------------------
n1400_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        mov              dword ptr [rbp + 20], 0
                                                                                        jmp   n1401_match_sequence_α
n1400_match_alternate_as:
                                                                                        jmp   proc_PAT$9_γ
n1400_match_alternate_β:
                        mov              eax, dword ptr [rbp + 20]
                        cmp              eax, 0
                                                                                        je    n1401_match_sequence_β
                                                                                        jmp   n1402_match_defer_β
n1400_match_alternate_af:
                        add              dword ptr [rbp + 20], 1
                        mov              r14d, dword ptr [rbp + 16]
                        mov              eax, dword ptr [rbp + 20]
                        cmp              eax, 1
                                                                                        je    n1402_match_defer_α
                                                                                        jmp   proc_PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
n1401_match_sequence_α:
                        mov              dword ptr [rbp + 32], r14d
                                                                                        jmp   n1403_match_defer_α
n1401_match_sequence_as:
                                                                                        jmp   n1400_match_alternate_as
n1401_match_sequence_β:
                                                                                        jmp   n1406_match_assign_cond_β
n1401_match_sequence_af:
                                                                                        jmp   n1400_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n1402_match_defer_α:
                        mov              rax, qword ptr [1879052592]
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 3
                                                                                        jne   .Lx1412_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1412_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1412_10
.Lx1412_9:
                        xor              eax, eax
.Lx1412_10:
                        test             rax, rax
                                                                                        jz    .Lx1412_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1412_4]
                        lea              rdx, [rip + .Lx1412_5]
                                                                                        jmp   rax
.Lx1412_4:
                                                                                        jmp   n1400_match_alternate_as
.Lx1412_5:
                                                                                        jmp   n1400_match_alternate_af
.Lx1412_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S13]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1412_2:
                        test             rax, rax
                                                                                        je    .Lx1412_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1412_7]
                        lea              rdx, [rip + .Lx1412_8]
                                                                                        jmp   rax
.Lx1412_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1412_2
.Lx1412_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1412_2
.Lx1412_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1400_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1412_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1400_match_alternate_as
.Lx1412_6:
                        add              rsp, 16
                                                                                        jmp   n1400_match_alternate_af
n1402_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1403_match_defer_α:
                        mov              rax, qword ptr [1879052592]
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 3
                                                                                        jne   .Lx1413_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1413_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1413_10
.Lx1413_9:
                        xor              eax, eax
.Lx1413_10:
                        test             rax, rax
                                                                                        jz    .Lx1413_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1413_4]
                        lea              rdx, [rip + .Lx1413_5]
                                                                                        jmp   rax
.Lx1413_4:
                                                                                        jmp   n1404_match_defer_α
.Lx1413_5:
                                                                                        jmp   n1400_match_alternate_af
.Lx1413_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S13]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1413_2:
                        test             rax, rax
                                                                                        je    .Lx1413_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1413_7]
                        lea              rdx, [rip + .Lx1413_8]
                                                                                        jmp   rax
.Lx1413_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1413_2
.Lx1413_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1413_2
.Lx1413_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1400_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1413_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1404_match_defer_α
.Lx1413_6:
                        add              rsp, 16
                                                                                        jmp   n1400_match_alternate_af
n1403_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1404_match_defer_α:
                        mov              rax, qword ptr [1879052512]
                        mov              rdx, qword ptr [1879052520]
                        cmp              eax, 3
                                                                                        jne   .Lx1414_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1414_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1414_10
.Lx1414_9:
                        xor              eax, eax
.Lx1414_10:
                        test             rax, rax
                                                                                        jz    .Lx1414_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1414_4]
                        lea              rdx, [rip + .Lx1414_5]
                                                                                        jmp   rax
.Lx1414_4:
                                                                                        jmp   n1405_match_assign_save_α
.Lx1414_5:
                                                                                        jmp   n1403_match_defer_β
.Lx1414_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1414_2:
                        test             rax, rax
                                                                                        je    .Lx1414_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1414_7]
                        lea              rdx, [rip + .Lx1414_8]
                                                                                        jmp   rax
.Lx1414_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1414_2
.Lx1414_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1414_2
.Lx1414_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1403_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1414_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1405_match_assign_save_α
.Lx1414_6:
                        add              rsp, 16
                                                                                        jmp   n1403_match_defer_β
n1404_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1405_match_assign_save_α:
                        lea              rdi, [rbp + 112]
                        mov              esi, r14d
                        call             rt_cap_push@PLT
                                                                                        jmp   n1407_match_defer_α
n1405_match_assign_save_β:
                        lea              rdi, [rbp + 112]
                        call             rt_cap_pop@PLT
                                                                                        jmp   n1404_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n1406_match_assign_cond_α:
                        lea              rdi, [rbp + 112]
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S12]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n1400_match_alternate_as
n1406_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n1407_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n1407_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx1419_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1419_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1419_10
.Lx1419_9:
                        xor              eax, eax
.Lx1419_10:
                        test             rax, rax
                                                                                        jz    .Lx1419_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1419_4]
                        lea              rdx, [rip + .Lx1419_5]
                                                                                        jmp   rax
.Lx1419_4:
                                                                                        jmp   n1406_match_assign_cond_α
.Lx1419_5:
                                                                                        jmp   n1405_match_assign_save_β
.Lx1419_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1419_2:
                        test             rax, rax
                                                                                        je    .Lx1419_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1419_7]
                        lea              rdx, [rip + .Lx1419_8]
                                                                                        jmp   rax
.Lx1419_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1419_2
.Lx1419_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1419_2
.Lx1419_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1405_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1419_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1406_match_assign_cond_α
.Lx1419_6:
                        add              rsp, 16
                                                                                        jmp   n1405_match_assign_save_β
n1407_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$9_scanhit:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 184]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$9_γ
proc_PAT$9_scanfail:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 184]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 184], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$9_attempt
8:
                                                                                        jmp   proc_PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_β:
                                                                                        jmp   qword ptr [rbp + 176]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$9_res]
                        push             rax
                        mov              rax, [rbp + 200]
                        mov              rbp, [rbp + 216]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_ω:
                        mov              rax, [rbp + 208]
                        lea              rsp, [rbp + 224]
                        mov              rbp, [rbp + 216]
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__Push"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__Push_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2512
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__Pop"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__Pop_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 2512
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__Unary"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_LBL__Unary_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 2512
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__Binary"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_LBL__Binary_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 2512
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname4:       .string          "Push"
.Lstartup_pp4_0:        .string          "x"
                        .align           8
.Lstartup_pnames4:
                        .quad            .Lstartup_pp4_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + .Lstartup_pnames4]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + proc_Push_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname5:       .string          "Pop"
                        .align           8
.Lstartup_pnames5:
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + .Lstartup_pnames5]
                        mov              edx, 0
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + proc_Pop_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname6:       .string          "Unary"
.Lstartup_pp6_0:        .string          "arg"
.Lstartup_pp6_1:        .string          "op"
                        .align           8
.Lstartup_pnames6:
                        .quad            .Lstartup_pp6_0
                        .quad            .Lstartup_pp6_1
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + .Lstartup_pnames6]
                        mov              edx, 2
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + proc_Unary_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname7:       .string          "Binary"
.Lstartup_pp7_0:        .string          "op"
.Lstartup_pp7_1:        .string          "left"
.Lstartup_pp7_2:        .string          "right"
                        .align           8
.Lstartup_pnames7:
                        .quad            .Lstartup_pp7_0
                        .quad            .Lstartup_pp7_1
                        .quad            .Lstartup_pp7_2
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + .Lstartup_pnames7]
                        mov              edx, 3
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + proc_Binary_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 3
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname8:       .string          "PAT$0"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname8]
                        lea              rsi, [rip + proc_PAT$0_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 48
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname9:       .string          "PAT$1"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + proc_PAT$1_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 112
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname10:      .string          "PAT$2"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + proc_PAT$2_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 240
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname11:      .string          "PAT$3"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + proc_PAT$3_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 96
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname12:      .string          "PAT$4"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname12]
                        lea              rsi, [rip + proc_PAT$4_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 96
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname13:      .string          "PAT$5"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname13]
                        lea              rsi, [rip + proc_PAT$5_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 144
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname14:      .string          "PAT$6"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname14]
                        lea              rsi, [rip + proc_PAT$6_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 112
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname15:      .string          "PAT$7"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname15]
                        lea              rsi, [rip + proc_PAT$7_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname16:      .string          "PAT$8"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname16]
                        lea              rsi, [rip + proc_PAT$8_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 192
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname17:      .string          "PAT$9"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname17]
                        lea              rsi, [rip + proc_PAT$9_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 192
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "Push"
.Lgvan1:                .string          "x"
.Lgvan2:                .string          "Pop"
.Lgvan3:                .string          "Unary"
.Lgvan4:                .string          "arg"
.Lgvan5:                .string          "op"
.Lgvan6:                .string          "Binary"
.Lgvan7:                .string          "left"
.Lgvan8:                .string          "right"
.Lgvan9:                .string          "stk"
.Lgvan10:               .string          "dummy"
.Lgvan11:               .string          "integer"
.Lgvan12:               .string          "exponent"
.Lgvan13:               .string          "real"
.Lgvan14:               .string          "addop"
.Lgvan15:               .string          "mulop"
.Lgvan16:               .string          "constant"
.Lgvan17:               .string          "primary"
.Lgvan18:               .string          "factor"
.Lgvan19:               .string          "term"
.Lgvan20:               .string          "expr"
.Lgvan21:               .string          "line"
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
                        .quad            .Lgvan15
                        .quad            .Lgvan16
                        .quad            .Lgvan17
                        .quad            .Lgvan18
                        .quad            .Lgvan19
                        .quad            .Lgvan20
                        .quad            .Lgvan21
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 22
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 22
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
                        sub              rsp, 2520
                        mov              rdi, rsp
                        mov              ecx, 2520
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 2512], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#          DEFINE('Push(x)')
#          stk      =  TABLE()                       :(PushEnd)
#-----------------------------------------------------------------------------------------------------------------------
n1420_call_α:
                        .section         .rodata
.Lrkfn1562:             .string          "TABLE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1562]
                        lea              rsi, [rbp + 128]
                        mov              edx, 0
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 112], rax
                        mov              qword ptr [rbp + 120], rdx
                        cmp              eax, 99
                                                                                        je    n1432_lit_string_α
                                                                                        jmp   n1431_assign_α
n1420_call_β:
                                                                                        jmp   n1432_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1421_goto_α:
                                                                                        jmp   n1433_var_α
n1421_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n1422_goto_α:
                                                                                        jmp   n1432_lit_string_α
n1422_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n1423_goto_α:
                                                                                        jmp   n1434_var_α
n1423_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n1424_goto_α:
                                                                                        jmp   n1432_lit_string_α
n1424_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n1425_goto_α:
                                                                                        jmp   n1435_call_α
n1425_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n1426_goto_α:
                                                                                        jmp   n1432_lit_string_α
n1426_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n1427_goto_α:
                                                                                        jmp   n1436_call_α
n1427_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n1428_goto_α:
                                                                                        jmp   n1432_lit_string_α
n1428_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n1429_goto_α:
                                                                                        jmp   n1437_var_α
n1429_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n1430_goto_α:
                                                                                        jmp   n1438_lit_string_α
n1430_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n1431_assign_α:
                        mov              rax, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        mov              qword ptr [1879052432], rax
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n1432_lit_string_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n1432_lit_string_α:
                        mov              qword ptr [rbp + 1616], 1
                        mov              rax, qword ptr [rip + .Lx1574_0]
                        mov              qword ptr [rbp + 1624], rax
                                                                                        jmp   n1439_call_α
.Lx1574_0:
                        .quad            .Lx1574_0_s
.Lx1574_0_s:
                        .string          "PAT$0"
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n1433_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                                                                                        jmp   n1440_lit_integer_α
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n1434_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 496], rax
                        mov              qword ptr [rbp + 504], rdx
                                                                                        jmp   n1442_var_α
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n1435_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1578_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1578_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1578_6]
                        lea              rdx, [rip + .Lx1578_7]
                                                                                        jmp   rax
.Lx1578_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1578_2
.Lx1578_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1578_2
.Lx1578_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1578_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1578_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1578_3]
                        lea              rdx, [rip + .Lx1578_4]
                                                                                        jmp   rax
.Lx1578_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1578_2
.Lx1578_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1578_2
.Lx1578_1:
                        call             rt_faildescr@PLT
.Lx1578_2:
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                        cmp              eax, 99
                                                                                        je    n1445_call_α
                                                                                        jmp   n1444_assign_α
n1435_call_β:
                                                                                        jmp   n1445_call_α
.Lx1578_0:
                        .quad            .Lx1578_0_s
.Lx1578_0_s:
                        .string          "Pop"
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n1436_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1580_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1580_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1580_6]
                        lea              rdx, [rip + .Lx1580_7]
                                                                                        jmp   rax
.Lx1580_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1580_2
.Lx1580_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1580_2
.Lx1580_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1580_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1580_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1580_3]
                        lea              rdx, [rip + .Lx1580_4]
                                                                                        jmp   rax
.Lx1580_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1580_2
.Lx1580_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1580_2
.Lx1580_1:
                        call             rt_faildescr@PLT
.Lx1580_2:
                        mov              qword ptr [rbp + 1104], rax
                        mov              qword ptr [rbp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n1447_call_α
                                                                                        jmp   n1446_assign_α
n1436_call_β:
                                                                                        jmp   n1447_call_α
.Lx1580_0:
                        .quad            .Lx1580_0_s
.Lx1580_0_s:
                        .string          "Pop"
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n1437_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1581_0]
                        call             NV_GET_fn@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx1581_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx1581_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n1448_assign_α
.Lx1581_0:
                        .quad            .Lx1581_0_s
.Lx1581_0_s:
                        .string          "INPUT"
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n1438_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx1582_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n1449_assign_α
.Lx1582_0:
                        .quad            .Lx1582_0_s
.Lx1582_0_s:
                        .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n1439_call_α:
                        mov              rax, qword ptr [rbp + 1616]
                        mov              qword ptr [rbp + 1584], rax
                        mov              rax, qword ptr [rbp + 1624]
                        mov              qword ptr [rbp + 1592], rax
                        .section         .rodata
.Lrkfn1584:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1584]
                        lea              rsi, [rbp + 1584]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1568], rax
                        mov              qword ptr [rbp + 1576], rdx
                        cmp              eax, 99
                                                                                        je    n1451_lit_string_α
                                                                                        jmp   n1450_assign_α
n1439_call_β:
                                                                                        jmp   n1451_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1440_lit_integer_α:
                        mov              qword ptr [rbp + 160], 6
                        mov              rax, qword ptr [rip + .Lx1585_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n1452_subscript_α
.Lx1585_0:
                        .quad            0
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n1441_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                                                                                        jmp   n1453_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1442_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n1455_lit_integer_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n1443_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   n1456_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n1444_assign_α:
                        mov              rax, qword ptr [rbp + 768]
                        mov              rdx, qword ptr [rbp + 776]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n1445_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n1445_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1591_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1591_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1591_6]
                        lea              rdx, [rip + .Lx1591_7]
                                                                                        jmp   rax
.Lx1591_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1591_2
.Lx1591_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1591_2
.Lx1591_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1591_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1591_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1591_3]
                        lea              rdx, [rip + .Lx1591_4]
                                                                                        jmp   rax
.Lx1591_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1591_2
.Lx1591_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1591_2
.Lx1591_1:
                        call             rt_faildescr@PLT
.Lx1591_2:
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n1459_lit_string_α
                                                                                        jmp   n1458_assign_α
n1445_call_β:
                                                                                        jmp   n1459_lit_string_α
.Lx1591_0:
                        .quad            .Lx1591_0_s
.Lx1591_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n1446_assign_α:
                        mov              rax, qword ptr [rbp + 1104]
                        mov              rdx, qword ptr [rbp + 1112]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n1447_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n1447_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1594_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1594_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1594_6]
                        lea              rdx, [rip + .Lx1594_7]
                                                                                        jmp   rax
.Lx1594_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1594_2
.Lx1594_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1594_2
.Lx1594_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1594_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1594_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1594_3]
                        lea              rdx, [rip + .Lx1594_4]
                                                                                        jmp   rax
.Lx1594_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1594_2
.Lx1594_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1594_2
.Lx1594_1:
                        call             rt_faildescr@PLT
.Lx1594_2:
                        mov              qword ptr [rbp + 1136], rax
                        mov              qword ptr [rbp + 1144], rdx
                        cmp              eax, 99
                                                                                        je    n1461_call_α
                                                                                        jmp   n1460_assign_α
n1447_call_β:
                                                                                        jmp   n1461_call_α
.Lx1594_0:
                        .quad            .Lx1594_0_s
.Lx1594_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n1448_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052624], rax
                        mov              qword ptr [1879052632], rdx
                                                                                        jmp   n1462_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1449_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1596_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n1437_var_α
.Lx1596_0:
                        .quad            .Lx1596_0_s
.Lx1596_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n1450_assign_α:
                        mov              rax, qword ptr [rbp + 1568]
                        mov              rdx, qword ptr [rbp + 1576]
                        mov              qword ptr [1879052464], rax
                        mov              qword ptr [1879052472], rdx
                                                                                        jmp   n1451_lit_string_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n1451_lit_string_α:
                        mov              qword ptr [rbp + 1680], 1
                        mov              rax, qword ptr [rip + .Lx1598_0]
                        mov              qword ptr [rbp + 1688], rax
                                                                                        jmp   n1463_call_α
.Lx1598_0:
                        .quad            .Lx1598_0_s
.Lx1598_0_s:
                        .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n1452_subscript_α:
                        mov              rdi, qword ptr [rbp + 144]
                        mov              rsi, qword ptr [rbp + 152]
                        mov              rdx, qword ptr [rbp + 160]
                        mov              rcx, qword ptr [rbp + 168]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n1441_var_α
                        mov              qword ptr [rbp + 176], rax
                        mov              qword ptr [rbp + 184], rdx
                                                                                        jmp   n1464_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1453_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n1465_lit_integer_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n1454_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n1466_call_α
#-----------------------------------------------------------------------------------------------------------------------
n1455_lit_integer_α:
                        mov              qword ptr [rbp + 528], 6
                        mov              rax, qword ptr [rip + .Lx1602_0]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n1468_subscript_α
.Lx1602_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1456_lit_integer_α:
                        mov              qword ptr [rbp + 624], 6
                        mov              rax, qword ptr [rip + .Lx1603_0]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n1469_subscript_α
.Lx1603_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1457_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n1458_assign_α:
                        mov              rax, qword ptr [rbp + 800]
                        mov              rdx, qword ptr [rbp + 808]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n1459_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n1459_lit_string_α:
                        mov              qword ptr [rbp + 832], 1
                        mov              rax, qword ptr [rip + .Lx1607_0]
                        mov              qword ptr [rbp + 840], rax
                                                                                        jmp   n1470_call_α
.Lx1607_0:
                        .quad            .Lx1607_0_s
.Lx1607_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n1460_assign_α:
                        mov              rax, qword ptr [rbp + 1136]
                        mov              rdx, qword ptr [rbp + 1144]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n1461_call_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n1461_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1610_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1610_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1610_6]
                        lea              rdx, [rip + .Lx1610_7]
                                                                                        jmp   rax
.Lx1610_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1610_2
.Lx1610_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1610_2
.Lx1610_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1610_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1610_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1610_3]
                        lea              rdx, [rip + .Lx1610_4]
                                                                                        jmp   rax
.Lx1610_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1610_2
.Lx1610_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1610_2
.Lx1610_1:
                        call             rt_faildescr@PLT
.Lx1610_2:
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n1472_lit_string_α
                                                                                        jmp   n1471_assign_α
n1461_call_β:
                                                                                        jmp   n1472_lit_string_α
.Lx1610_0:
                        .quad            .Lx1610_0_s
.Lx1610_0_s:
                        .string          "Pop"
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n1462_var_α:
                        mov              rax, qword ptr [1879052624]
                        mov              rdx, qword ptr [1879052632]
                        mov              qword ptr [rbp + 2448], rax
                        mov              qword ptr [rbp + 2456], rdx
                                                                                        jmp   n1473_match_head_α
#-----------------------------------------------------------------------------------------------------------------------
n1463_call_α:
                        mov              rax, qword ptr [rbp + 1680]
                        mov              qword ptr [rbp + 1648], rax
                        mov              rax, qword ptr [rbp + 1688]
                        mov              qword ptr [rbp + 1656], rax
                        .section         .rodata
.Lrkfn1613:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1613]
                        lea              rsi, [rbp + 1648]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1632], rax
                        mov              qword ptr [rbp + 1640], rdx
                        cmp              eax, 99
                                                                                        je    n1475_lit_string_α
                                                                                        jmp   n1474_assign_α
n1463_call_β:
                                                                                        jmp   n1475_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1464_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                                                                                        jmp   n1476_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n1465_lit_integer_α:
                        mov              qword ptr [rbp + 336], 6
                        mov              rax, qword ptr [rip + .Lx1615_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n1477_subscript_α
.Lx1615_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1466_call_α:
                        mov              rax, qword ptr [rbp + 448]
                        mov              qword ptr [rbp + 416], rax
                        mov              rax, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 424], rax
                        .section         .rodata
.Lrkfn1617:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1617]
                        lea              rsi, [rbp + 416]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n1467_lit_string_α
                                                                                        jmp   n1478_var_α
n1466_call_β:
                                                                                        jmp   n1467_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1467_lit_string_α:
                        mov              qword ptr [rbp + 32], 1
                        mov              rax, qword ptr [rip + .Lx1618_0]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n1479_call_α
.Lx1618_0:
                        .quad            .Lx1618_0_s
.Lx1618_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n1468_subscript_α:
                        mov              rdi, qword ptr [rbp + 512]
                        mov              rsi, qword ptr [rbp + 520]
                        mov              rdx, qword ptr [rbp + 528]
                        mov              rcx, qword ptr [rbp + 536]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n1443_var_α
                        mov              qword ptr [rbp + 544], rax
                        mov              qword ptr [rbp + 552], rdx
                                                                                        jmp   n1480_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n1469_subscript_α:
                        mov              rdi, qword ptr [rbp + 608]
                        mov              rsi, qword ptr [rbp + 616]
                        mov              rdx, qword ptr [rbp + 624]
                        mov              rcx, qword ptr [rbp + 632]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n1457_op14_α
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   n1481_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1470_call_α:
                        mov              rax, qword ptr [rbp + 832]
                        mov              qword ptr [rbp + 864], rax
                        mov              rax, qword ptr [rbp + 840]
                        mov              qword ptr [rbp + 872], rax
                        .section         .rodata
.Lrkfn1622:             .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1622]
                        lea              rsi, [rbp + 864]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n1483_lit_string_α
                                                                                        jmp   n1482_call_α
n1470_call_β:
                                                                                        jmp   n1483_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1471_assign_α:
                        mov              rax, qword ptr [rbp + 1168]
                        mov              rdx, qword ptr [rbp + 1176]
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n1472_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n1472_lit_string_α:
                        mov              qword ptr [rbp + 1200], 1
                        mov              rax, qword ptr [rip + .Lx1624_0]
                        mov              qword ptr [rbp + 1208], rax
                                                                                        jmp   n1484_call_α
.Lx1624_0:
                        .quad            .Lx1624_0_s
.Lx1624_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n1473_match_head_α:
                        mov              qword ptr [rbp + 2376], rbp
                        mov              rdi, qword ptr [rbp + 2448]
                        mov              rsi, qword ptr [rbp + 2456]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rbp + 2368], r12
                        mov              qword ptr [rbp + 2352], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 2344], rax
                        mov              dword ptr [rbp + 2336], 0
.Lx1626_0:
                        mov              r14d, dword ptr [rbp + 2336]
                                                                                        jmp   n1485_match_sequence_α
n1473_match_head_β:
                        add              dword ptr [rbp + 2336], 1
                        mov              eax, dword ptr [rbp + 2336]
                        cmp              eax, r15d
                                                                                        jg    .Lx1626_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx1626_1
                                                                                        jmp   .Lx1626_0
.Lx1626_1:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        mov              r12, qword ptr [rbp + 2368]
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n1438_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1474_assign_α:
                        mov              rax, qword ptr [rbp + 1632]
                        mov              rdx, qword ptr [rbp + 1640]
                        mov              qword ptr [1879052480], rax
                        mov              qword ptr [1879052488], rdx
                                                                                        jmp   n1475_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n1475_lit_string_α:
                        mov              qword ptr [rbp + 1744], 1
                        mov              rax, qword ptr [rip + .Lx1628_0]
                        mov              qword ptr [rbp + 1752], rax
                                                                                        jmp   n1486_call_α
.Lx1628_0:
                        .quad            .Lx1628_0_s
.Lx1628_0_s:
                        .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n1476_lit_integer_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx1629_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n1487_subscript_α
.Lx1629_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1477_subscript_α:
                        mov              rdi, qword ptr [rbp + 320]
                        mov              rsi, qword ptr [rbp + 328]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n1454_var_α
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   n1488_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n1478_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   n1489_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1479_call_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 64], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 72], rax
                        .section         .rodata
.Lrkfn1633:             .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1633]
                        lea              rsi, [rbp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 48], rax
                        mov              qword ptr [rbp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n1490_op14_α
                                                                                        jmp   n1457_op14_α
n1479_call_β:
                                                                                        jmp   n1490_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n1480_deref_α:
                        mov              rdi, qword ptr [rbp + 544]
                        mov              rsi, qword ptr [rbp + 552]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n1443_var_α
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                                                                                        jmp   n1491_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n1481_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                                                                                        jmp   n1492_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n1482_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx1637_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1637_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1637_6]
                        lea              rdx, [rip + .Lx1637_7]
                                                                                        jmp   rax
.Lx1637_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1637_2
.Lx1637_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1637_2
.Lx1637_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx1637_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1637_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1637_3]
                        lea              rdx, [rip + .Lx1637_4]
                                                                                        jmp   rax
.Lx1637_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1637_2
.Lx1637_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1637_2
.Lx1637_1:
                        call             rt_faildescr@PLT
.Lx1637_2:
                        mov              qword ptr [rbp + 896], rax
                        mov              qword ptr [rbp + 904], rdx
                        cmp              eax, 99
                                                                                        je    n1483_lit_string_α
                                                                                        jmp   n1493_var_α
n1482_call_β:
                                                                                        jmp   n1483_lit_string_α
.Lx1637_0:
                        .quad            .Lx1637_0_s
.Lx1637_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n1483_lit_string_α:
                        mov              qword ptr [rbp + 1088], 1
                        mov              rax, qword ptr [rip + .Lx1638_0]
                        mov              qword ptr [rbp + 1096], rax
                                                                                        jmp   n1494_call_α
.Lx1638_0:
                        .quad            .Lx1638_0_s
.Lx1638_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n1484_call_α:
                        mov              rax, qword ptr [rbp + 1200]
                        mov              qword ptr [rbp + 1232], rax
                        mov              rax, qword ptr [rbp + 1208]
                        mov              qword ptr [rbp + 1240], rax
                        .section         .rodata
.Lrkfn1640:             .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1640]
                        lea              rsi, [rbp + 1232]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1216], rax
                        mov              qword ptr [rbp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n1496_lit_string_α
                                                                                        jmp   n1495_call_α
n1484_call_β:
                                                                                        jmp   n1496_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1485_match_sequence_α:
                        mov              dword ptr [rbp + 2384], r14d
                                                                                        jmp   n1498_lit_integer_α
n1485_match_sequence_as:
                                                                                        jmp   n1497_match_release_α
n1485_match_sequence_β:
                                                                                        jmp   n1502_match_rpos_β
n1485_match_sequence_af:
                                                                                        jmp   n1473_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n1486_call_α:
                        mov              rax, qword ptr [rbp + 1744]
                        mov              qword ptr [rbp + 1712], rax
                        mov              rax, qword ptr [rbp + 1752]
                        mov              qword ptr [rbp + 1720], rax
                        .section         .rodata
.Lrkfn1644:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1644]
                        lea              rsi, [rbp + 1712]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1696], rax
                        mov              qword ptr [rbp + 1704], rdx
                        cmp              eax, 99
                                                                                        je    n1504_lit_string_α
                                                                                        jmp   n1503_assign_α
n1486_call_β:
                                                                                        jmp   n1504_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1487_subscript_α:
                        mov              rdi, qword ptr [rbp + 208]
                        mov              rsi, qword ptr [rbp + 216]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n1441_var_α
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n1505_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n1488_deref_α:
                        mov              rdi, qword ptr [rbp + 352]
                        mov              rsi, qword ptr [rbp + 360]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n1454_var_α
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n1506_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n1489_assign_var_α:
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        mov              rdx, qword ptr [rbp + 464]
                        mov              rcx, qword ptr [rbp + 472]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n1467_lit_string_α
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   n1467_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1490_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n1491_subscript_α:
                        mov              rdi, qword ptr [rbp + 496]
                        mov              rsi, qword ptr [rbp + 504]
                        mov              rdx, qword ptr [rbp + 560]
                        mov              rcx, qword ptr [rbp + 568]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n1443_var_α
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n1507_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n1492_lit_integer_α:
                        mov              qword ptr [rbp + 688], 6
                        mov              rax, qword ptr [rip + .Lx1651_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   n1508_subscript_α
.Lx1651_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1493_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   n1509_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1494_call_α:
                        mov              rax, qword ptr [rbp + 1088]
                        mov              qword ptr [rbp + 1056], rax
                        mov              rax, qword ptr [rbp + 1096]
                        mov              qword ptr [rbp + 1064], rax
                        .section         .rodata
.Lrkfn1654:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1654]
                        lea              rsi, [rbp + 1056]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    n1467_lit_string_α
                                                                                        jmp   n1510_assign_α
n1494_call_β:
                                                                                        jmp   n1467_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1495_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx1656_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1656_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1656_6]
                        lea              rdx, [rip + .Lx1656_7]
                                                                                        jmp   rax
.Lx1656_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1656_2
.Lx1656_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1656_2
.Lx1656_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx1656_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1656_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1656_3]
                        lea              rdx, [rip + .Lx1656_4]
                                                                                        jmp   rax
.Lx1656_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1656_2
.Lx1656_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1656_2
.Lx1656_1:
                        call             rt_faildescr@PLT
.Lx1656_2:
                        mov              qword ptr [rbp + 1264], rax
                        mov              qword ptr [rbp + 1272], rdx
                        cmp              eax, 99
                                                                                        je    n1496_lit_string_α
                                                                                        jmp   n1511_var_α
n1495_call_β:
                                                                                        jmp   n1496_lit_string_α
.Lx1656_0:
                        .quad            .Lx1656_0_s
.Lx1656_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n1496_lit_string_α:
                        mov              qword ptr [rbp + 1552], 1
                        mov              rax, qword ptr [rip + .Lx1657_0]
                        mov              qword ptr [rbp + 1560], rax
                                                                                        jmp   n1512_call_α
.Lx1657_0:
                        .quad            .Lx1657_0_s
.Lx1657_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n1497_match_release_α:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 2400]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx1659_1:
                        test             rax, rax
                                                                                        je    .Lx1659_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1659_3]
                        lea              rdx, [rip + .Lx1659_4]
                                                                                        jmp   rax
.Lx1659_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx1659_1
.Lx1659_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx1659_1
.Lx1659_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + 2368]
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n1513_call_α
#-----------------------------------------------------------------------------------------------------------------------
n1498_lit_integer_α:
                        mov              qword ptr [rbp + 2400], 6
                        mov              rax, qword ptr [rip + .Lx1660_0]
                        mov              qword ptr [rbp + 2408], rax
                                                                                        jmp   n1499_match_pos_α
n1498_lit_integer_β:
                                                                                        jmp   n1473_match_head_β
.Lx1660_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1499_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n1473_match_head_β
                                                                                        jmp   n1500_match_defer_α
n1499_match_pos_β:
                                                                                        jmp   n1473_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n1500_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx1662_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx1662_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx1662_10
.Lx1662_9:
                        xor              eax, eax
.Lx1662_10:
                        test             rax, rax
                                                                                        jz    .Lx1662_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx1662_4]
                        lea              rdx, [rip + .Lx1662_5]
                                                                                        jmp   rax
.Lx1662_4:
                                                                                        jmp   n1501_lit_integer_α
.Lx1662_5:
                                                                                        jmp   n1473_match_head_β
.Lx1662_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx1662_2:
                        test             rax, rax
                                                                                        je    .Lx1662_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1662_7]
                        lea              rdx, [rip + .Lx1662_8]
                                                                                        jmp   rax
.Lx1662_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1662_2
.Lx1662_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx1662_2
.Lx1662_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n1473_match_head_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx1662_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n1501_lit_integer_α
.Lx1662_6:
                        add              rsp, 16
                                                                                        jmp   n1473_match_head_β
n1500_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1501_lit_integer_α:
                        mov              qword ptr [rbp + 2432], 6
                        mov              rax, qword ptr [rip + .Lx1663_0]
                        mov              qword ptr [rbp + 2440], rax
                                                                                        jmp   n1502_match_rpos_α
n1501_lit_integer_β:
                                                                                        jmp   n1500_match_defer_β
.Lx1663_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1502_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n1500_match_defer_β
                                                                                        jmp   n1497_match_release_α
n1502_match_rpos_β:
                                                                                        jmp   n1500_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n1503_assign_α:
                        mov              rax, qword ptr [rbp + 1696]
                        mov              rdx, qword ptr [rbp + 1704]
                        mov              qword ptr [1879052496], rax
                        mov              qword ptr [1879052504], rdx
                                                                                        jmp   n1504_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n1504_lit_string_α:
                        mov              qword ptr [rbp + 1808], 1
                        mov              rax, qword ptr [rip + .Lx1666_0]
                        mov              qword ptr [rbp + 1816], rax
                                                                                        jmp   n1514_call_α
.Lx1666_0:
                        .quad            .Lx1666_0_s
.Lx1666_0_s:
                        .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n1505_deref_α:
                        mov              rdi, qword ptr [rbp + 240]
                        mov              rsi, qword ptr [rbp + 248]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n1441_var_α
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n1515_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n1506_subscript_α:
                        mov              rdi, qword ptr [rbp + 304]
                        mov              rsi, qword ptr [rbp + 312]
                        mov              rdx, qword ptr [rbp + 368]
                        mov              rcx, qword ptr [rbp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n1454_var_α
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                                                                                        jmp   n1516_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n1507_deref_α:
                        mov              rdi, qword ptr [rbp + 576]
                        mov              rsi, qword ptr [rbp + 584]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n1443_var_α
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n1517_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n1508_subscript_α:
                        mov              rdi, qword ptr [rbp + 672]
                        mov              rsi, qword ptr [rbp + 680]
                        mov              rdx, qword ptr [rbp + 688]
                        mov              rcx, qword ptr [rbp + 696]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n1457_op14_α
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                                                                                        jmp   n1518_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n1509_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                                                                                        jmp   n1519_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n1510_assign_α:
                        mov              rax, qword ptr [rbp + 1040]
                        mov              rdx, qword ptr [rbp + 1048]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n1467_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1511_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rbp + 1408], rax
                        mov              qword ptr [rbp + 1416], rdx
                                                                                        jmp   n1520_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1512_call_α:
                        mov              rax, qword ptr [rbp + 1552]
                        mov              qword ptr [rbp + 1520], rax
                        mov              rax, qword ptr [rbp + 1560]
                        mov              qword ptr [rbp + 1528], rax
                        .section         .rodata
.Lrkfn1675:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1675]
                        lea              rsi, [rbp + 1520]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1504], rax
                        mov              qword ptr [rbp + 1512], rdx
                        cmp              eax, 99
                                                                                        je    n1467_lit_string_α
                                                                                        jmp   n1521_assign_α
n1512_call_β:
                                                                                        jmp   n1467_lit_string_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n1513_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1677_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1677_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1677_6]
                        lea              rdx, [rip + .Lx1677_7]
                                                                                        jmp   rax
.Lx1677_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1677_2
.Lx1677_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1677_2
.Lx1677_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx1677_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx1677_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1677_3]
                        lea              rdx, [rip + .Lx1677_4]
                                                                                        jmp   rax
.Lx1677_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx1677_2
.Lx1677_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx1677_2
.Lx1677_1:
                        call             rt_faildescr@PLT
.Lx1677_2:
                        mov              qword ptr [rbp + 2464], rax
                        mov              qword ptr [rbp + 2472], rdx
                        cmp              eax, 99
                                                                                        je    n1437_var_α
                                                                                        jmp   n1522_assign_α
n1513_call_β:
                                                                                        jmp   n1437_var_α
.Lx1677_0:
                        .quad            .Lx1677_0_s
.Lx1677_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n1514_call_α:
                        mov              rax, qword ptr [rbp + 1808]
                        mov              qword ptr [rbp + 1776], rax
                        mov              rax, qword ptr [rbp + 1816]
                        mov              qword ptr [rbp + 1784], rax
                        .section         .rodata
.Lrkfn1679:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1679]
                        lea              rsi, [rbp + 1776]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1760], rax
                        mov              qword ptr [rbp + 1768], rdx
                        cmp              eax, 99
                                                                                        je    n1524_lit_string_α
                                                                                        jmp   n1523_assign_α
n1514_call_β:
                                                                                        jmp   n1524_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1515_lit_integer_α:
                        mov              qword ptr [rbp + 272], 6
                        mov              rax, qword ptr [rip + .Lx1680_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n1525_binop_α
.Lx1680_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n1516_assign_α:
                        mov              rax, qword ptr [rbp + 384]
                        mov              rdx, qword ptr [rbp + 392]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n1454_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1517_assign_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              rdx, qword ptr [rbp + 600]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n1443_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1518_deref_α:
                        mov              rdi, qword ptr [rbp + 704]
                        mov              rsi, qword ptr [rbp + 712]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n1457_op14_α
                        mov              qword ptr [rbp + 720], rax
                        mov              qword ptr [rbp + 728], rdx
                                                                                        jmp   n1526_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n1519_binop_α:
                        mov              rdi, qword ptr [rbp + 992]
                        mov              rsi, qword ptr [rbp + 1000]
                        mov              rdx, qword ptr [rbp + 1008]
                        mov              rcx, qword ptr [rbp + 1016]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 976], rax
                        mov              qword ptr [rbp + 984], rdx
                                                                                        jmp   n1527_call_α
#-----------------------------------------------------------------------------------------------------------------------
n1520_lit_string_α:
                        mov              qword ptr [rbp + 1424], 1
                        mov              rax, qword ptr [rip + .Lx1685_0]
                        mov              qword ptr [rbp + 1432], rax
                                                                                        jmp   n1528_binop_α
.Lx1685_0:
                        .quad            .Lx1685_0_s
.Lx1685_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n1521_assign_α:
                        mov              rax, qword ptr [rbp + 1504]
                        mov              rdx, qword ptr [rbp + 1512]
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n1467_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1522_assign_α:
                        mov              rsi, qword ptr [rbp + 2464]
                        mov              rdx, qword ptr [rbp + 2472]
                        mov              rdi, qword ptr [rip + .Lx1687_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n1437_var_α
.Lx1687_0:
                        .quad            .Lx1687_0_s
.Lx1687_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n1523_assign_α:
                        mov              rax, qword ptr [rbp + 1760]
                        mov              rdx, qword ptr [rbp + 1768]
                        mov              qword ptr [1879052512], rax
                        mov              qword ptr [1879052520], rdx
                                                                                        jmp   n1524_lit_string_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n1524_lit_string_α:
                        mov              qword ptr [rbp + 1872], 1
                        mov              rax, qword ptr [rip + .Lx1689_0]
                        mov              qword ptr [rbp + 1880], rax
                                                                                        jmp   n1529_call_α
.Lx1689_0:
                        .quad            .Lx1689_0_s
.Lx1689_0_s:
                        .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n1525_binop_α:
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 100
                                                                                        je    .Lx1690_0
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 6
                                                                                        jne   .Lx1690_2
.Lx1690_1:
                        mov              rax, qword ptr [rbp + 264]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rbp + 192], 6
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n1530_assign_var_α
.Lx1690_0:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 0
                        lea              r9, [rbp + 192]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n1530_assign_var_α
.Lx1690_2:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 0
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n1441_var_α
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   n1530_assign_var_α
n1525_binop_β:
                                                                                        jmp   n1441_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1526_lit_integer_α:
                        mov              qword ptr [rbp + 736], 6
                        mov              rax, qword ptr [rip + .Lx1691_0]
                        mov              qword ptr [rbp + 744], rax
                                                                                        jmp   n1531_binop_α
.Lx1691_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n1527_call_α:
                        mov              rax, qword ptr [rbp + 976]
                        mov              qword ptr [rbp + 944], rax
                        mov              rax, qword ptr [rbp + 984]
                        mov              qword ptr [rbp + 952], rax
                        .section         .rodata
.Lrkfn1693:             .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1693]
                        lea              rsi, [rbp + 944]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 928], rax
                        mov              qword ptr [rbp + 936], rdx
                        cmp              eax, 99
                                                                                        je    n1483_lit_string_α
                                                                                        jmp   n1532_assign_var_α
n1527_call_β:
                                                                                        jmp   n1483_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1528_binop_α:
                        mov              rdi, qword ptr [rbp + 1408]
                        mov              rsi, qword ptr [rbp + 1416]
                        mov              rdx, qword ptr [rbp + 1424]
                        mov              rcx, qword ptr [rbp + 1432]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                                                                                        jmp   n1533_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1529_call_α:
                        mov              rax, qword ptr [rbp + 1872]
                        mov              qword ptr [rbp + 1840], rax
                        mov              rax, qword ptr [rbp + 1880]
                        mov              qword ptr [rbp + 1848], rax
                        .section         .rodata
.Lrkfn1696:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1696]
                        lea              rsi, [rbp + 1840]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1824], rax
                        mov              qword ptr [rbp + 1832], rdx
                        cmp              eax, 99
                                                                                        je    n1535_lit_string_α
                                                                                        jmp   n1534_assign_α
n1529_call_β:
                                                                                        jmp   n1535_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1530_assign_var_α:
                        mov              rdi, qword ptr [rbp + 176]
                        mov              rsi, qword ptr [rbp + 184]
                        mov              rdx, qword ptr [rbp + 192]
                        mov              rcx, qword ptr [rbp + 200]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n1441_var_α
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                                                                                        jmp   n1441_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1531_binop_α:
                        mov              eax, dword ptr [rbp + 720]
                        cmp              eax, 100
                                                                                        je    .Lx1698_0
                        mov              eax, dword ptr [rbp + 720]
                        cmp              eax, 6
                                                                                        jne   .Lx1698_2
.Lx1698_1:
                        mov              rax, qword ptr [rbp + 728]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 656], 6
                        mov              qword ptr [rbp + 664], rax
                                                                                        jmp   n1536_assign_var_α
.Lx1698_0:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        mov              r8d, 1
                        lea              r9, [rbp + 656]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n1536_assign_var_α
.Lx1698_2:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        mov              r8d, 1
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n1457_op14_α
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n1536_assign_var_α
n1531_binop_β:
                                                                                        jmp   n1457_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n1532_assign_var_α:
                        mov              rdi, qword ptr [rbp + 896]
                        mov              rsi, qword ptr [rbp + 904]
                        mov              rdx, qword ptr [rbp + 928]
                        mov              rcx, qword ptr [rbp + 936]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n1483_lit_string_α
                        mov              qword ptr [rbp + 1024], rax
                        mov              qword ptr [rbp + 1032], rdx
                                                                                        jmp   n1483_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1533_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 1440], rax
                        mov              qword ptr [rbp + 1448], rdx
                                                                                        jmp   n1537_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n1534_assign_α:
                        mov              rax, qword ptr [rbp + 1824]
                        mov              rdx, qword ptr [rbp + 1832]
                        mov              qword ptr [1879052528], rax
                        mov              qword ptr [1879052536], rdx
                                                                                        jmp   n1535_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n1535_lit_string_α:
                        mov              qword ptr [rbp + 1936], 1
                        mov              rax, qword ptr [rip + .Lx1702_0]
                        mov              qword ptr [rbp + 1944], rax
                                                                                        jmp   n1538_call_α
.Lx1702_0:
                        .quad            .Lx1702_0_s
.Lx1702_0_s:
                        .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n1536_assign_var_α:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 656]
                        mov              rcx, qword ptr [rbp + 664]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n1457_op14_α
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   n1457_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n1537_binop_α:
                        mov              rdi, qword ptr [rbp + 1392]
                        mov              rsi, qword ptr [rbp + 1400]
                        mov              rdx, qword ptr [rbp + 1440]
                        mov              rcx, qword ptr [rbp + 1448]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                                                                                        jmp   n1539_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1538_call_α:
                        mov              rax, qword ptr [rbp + 1936]
                        mov              qword ptr [rbp + 1904], rax
                        mov              rax, qword ptr [rbp + 1944]
                        mov              qword ptr [rbp + 1912], rax
                        .section         .rodata
.Lrkfn1706:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1706]
                        lea              rsi, [rbp + 1904]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1888], rax
                        mov              qword ptr [rbp + 1896], rdx
                        cmp              eax, 99
                                                                                        je    n1541_lit_string_α
                                                                                        jmp   n1540_assign_α
n1538_call_β:
                                                                                        jmp   n1541_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1539_lit_string_α:
                        mov              qword ptr [rbp + 1456], 1
                        mov              rax, qword ptr [rip + .Lx1707_0]
                        mov              qword ptr [rbp + 1464], rax
                                                                                        jmp   n1542_binop_α
.Lx1707_0:
                        .quad            .Lx1707_0_s
.Lx1707_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n1540_assign_α:
                        mov              rax, qword ptr [rbp + 1888]
                        mov              rdx, qword ptr [rbp + 1896]
                        mov              qword ptr [1879052544], rax
                        mov              qword ptr [1879052552], rdx
                                                                                        jmp   n1541_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n1541_lit_string_α:
                        mov              qword ptr [rbp + 2000], 1
                        mov              rax, qword ptr [rip + .Lx1709_0]
                        mov              qword ptr [rbp + 2008], rax
                                                                                        jmp   n1543_call_α
.Lx1709_0:
                        .quad            .Lx1709_0_s
.Lx1709_0_s:
                        .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n1542_binop_α:
                        mov              rdi, qword ptr [rbp + 1376]
                        mov              rsi, qword ptr [rbp + 1384]
                        mov              rdx, qword ptr [rbp + 1456]
                        mov              rcx, qword ptr [rbp + 1464]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1360], rax
                        mov              qword ptr [rbp + 1368], rdx
                                                                                        jmp   n1544_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1543_call_α:
                        mov              rax, qword ptr [rbp + 2000]
                        mov              qword ptr [rbp + 1968], rax
                        mov              rax, qword ptr [rbp + 2008]
                        mov              qword ptr [rbp + 1976], rax
                        .section         .rodata
.Lrkfn1712:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1712]
                        lea              rsi, [rbp + 1968]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1952], rax
                        mov              qword ptr [rbp + 1960], rdx
                        cmp              eax, 99
                                                                                        je    n1546_lit_string_α
                                                                                        jmp   n1545_assign_α
n1543_call_β:
                                                                                        jmp   n1546_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1544_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rbp + 1472], rax
                        mov              qword ptr [rbp + 1480], rdx
                                                                                        jmp   n1547_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n1545_assign_α:
                        mov              rax, qword ptr [rbp + 1952]
                        mov              rdx, qword ptr [rbp + 1960]
                        mov              qword ptr [1879052560], rax
                        mov              qword ptr [1879052568], rdx
                                                                                        jmp   n1546_lit_string_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n1546_lit_string_α:
                        mov              qword ptr [rbp + 2064], 1
                        mov              rax, qword ptr [rip + .Lx1715_0]
                        mov              qword ptr [rbp + 2072], rax
                                                                                        jmp   n1548_call_α
.Lx1715_0:
                        .quad            .Lx1715_0_s
.Lx1715_0_s:
                        .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n1547_binop_α:
                        mov              rdi, qword ptr [rbp + 1360]
                        mov              rsi, qword ptr [rbp + 1368]
                        mov              rdx, qword ptr [rbp + 1472]
                        mov              rcx, qword ptr [rbp + 1480]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1344], rax
                        mov              qword ptr [rbp + 1352], rdx
                                                                                        jmp   n1549_call_α
#-----------------------------------------------------------------------------------------------------------------------
n1548_call_α:
                        mov              rax, qword ptr [rbp + 2064]
                        mov              qword ptr [rbp + 2032], rax
                        mov              rax, qword ptr [rbp + 2072]
                        mov              qword ptr [rbp + 2040], rax
                        .section         .rodata
.Lrkfn1718:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1718]
                        lea              rsi, [rbp + 2032]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2016], rax
                        mov              qword ptr [rbp + 2024], rdx
                        cmp              eax, 99
                                                                                        je    n1551_lit_string_α
                                                                                        jmp   n1550_assign_α
n1548_call_β:
                                                                                        jmp   n1551_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1549_call_α:
                        mov              rax, qword ptr [rbp + 1344]
                        mov              qword ptr [rbp + 1312], rax
                        mov              rax, qword ptr [rbp + 1352]
                        mov              qword ptr [rbp + 1320], rax
                        .section         .rodata
.Lrkfn1720:             .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1720]
                        lea              rsi, [rbp + 1312]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1296], rax
                        mov              qword ptr [rbp + 1304], rdx
                        cmp              eax, 99
                                                                                        je    n1496_lit_string_α
                                                                                        jmp   n1552_assign_var_α
n1549_call_β:
                                                                                        jmp   n1496_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1550_assign_α:
                        mov              rax, qword ptr [rbp + 2016]
                        mov              rdx, qword ptr [rbp + 2024]
                        mov              qword ptr [1879052576], rax
                        mov              qword ptr [1879052584], rdx
                                                                                        jmp   n1551_lit_string_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n1551_lit_string_α:
                        mov              qword ptr [rbp + 2128], 1
                        mov              rax, qword ptr [rip + .Lx1722_0]
                        mov              qword ptr [rbp + 2136], rax
                                                                                        jmp   n1553_call_α
.Lx1722_0:
                        .quad            .Lx1722_0_s
.Lx1722_0_s:
                        .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n1552_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1264]
                        mov              rsi, qword ptr [rbp + 1272]
                        mov              rdx, qword ptr [rbp + 1296]
                        mov              rcx, qword ptr [rbp + 1304]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n1496_lit_string_α
                        mov              qword ptr [rbp + 1488], rax
                        mov              qword ptr [rbp + 1496], rdx
                                                                                        jmp   n1496_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1553_call_α:
                        mov              rax, qword ptr [rbp + 2128]
                        mov              qword ptr [rbp + 2096], rax
                        mov              rax, qword ptr [rbp + 2136]
                        mov              qword ptr [rbp + 2104], rax
                        .section         .rodata
.Lrkfn1725:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1725]
                        lea              rsi, [rbp + 2096]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2080], rax
                        mov              qword ptr [rbp + 2088], rdx
                        cmp              eax, 99
                                                                                        je    n1555_lit_string_α
                                                                                        jmp   n1554_assign_α
n1553_call_β:
                                                                                        jmp   n1555_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1554_assign_α:
                        mov              rax, qword ptr [rbp + 2080]
                        mov              rdx, qword ptr [rbp + 2088]
                        mov              qword ptr [1879052592], rax
                        mov              qword ptr [1879052600], rdx
                                                                                        jmp   n1555_lit_string_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n1555_lit_string_α:
                        mov              qword ptr [rbp + 2192], 1
                        mov              rax, qword ptr [rip + .Lx1727_0]
                        mov              qword ptr [rbp + 2200], rax
                                                                                        jmp   n1556_call_α
.Lx1727_0:
                        .quad            .Lx1727_0_s
.Lx1727_0_s:
                        .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n1556_call_α:
                        mov              rax, qword ptr [rbp + 2192]
                        mov              qword ptr [rbp + 2160], rax
                        mov              rax, qword ptr [rbp + 2200]
                        mov              qword ptr [rbp + 2168], rax
                        .section         .rodata
.Lrkfn1729:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1729]
                        lea              rsi, [rbp + 2160]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2144], rax
                        mov              qword ptr [rbp + 2152], rdx
                        cmp              eax, 99
                                                                                        je    n1558_lit_string_α
                                                                                        jmp   n1557_assign_α
n1556_call_β:
                                                                                        jmp   n1558_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1557_assign_α:
                        mov              rax, qword ptr [rbp + 2144]
                        mov              rdx, qword ptr [rbp + 2152]
                        mov              qword ptr [1879052608], rax
                        mov              qword ptr [1879052616], rdx
                                                                                        jmp   n1558_lit_string_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n1558_lit_string_α:
                        mov              qword ptr [rbp + 2272], 1
                        mov              rax, qword ptr [rip + .Lx1731_0]
                        mov              qword ptr [rbp + 2280], rax
                                                                                        jmp   n1559_lit_integer_α
.Lx1731_0:
                        .quad            .Lx1731_0_s
.Lx1731_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n1559_lit_integer_α:
                        mov              qword ptr [rbp + 2288], 6
                        mov              rax, qword ptr [rip + .Lx1732_0]
                        mov              qword ptr [rbp + 2296], rax
                                                                                        jmp   n1560_call_α
.Lx1732_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n1560_call_α:
                        mov              rax, qword ptr [rbp + 2272]
                        mov              qword ptr [rbp + 2224], rax
                        mov              rax, qword ptr [rbp + 2280]
                        mov              qword ptr [rbp + 2232], rax
                        mov              rax, qword ptr [rbp + 2288]
                        mov              qword ptr [rbp + 2240], rax
                        mov              rax, qword ptr [rbp + 2296]
                        mov              qword ptr [rbp + 2248], rax
                        .section         .rodata
.Lrkfn1734:             .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1734]
                        lea              rsi, [rbp + 2224]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2208], rax
                        mov              qword ptr [rbp + 2216], rdx
                        cmp              eax, 99
                                                                                        je    n1437_var_α
                                                                                        jmp   n1437_var_α
n1560_call_β:
                                                                                        jmp   n1437_var_α
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 2512]
                        add              rsp, 2520
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 2512]
                        add              rsp, 2520
                        ret
                        .section         .rodata
.S0:                    .string          "expr"
.S1:                    .string          "integer"
.S2:                    .string          "epsilon"
.S3:                    .string          "exponent"
.S4:                    .string          "*Push"
.S5:                    .string          "real"
.S6:                    .string          "constant"
.S7:                    .string          "primary"
.S8:                    .string          "addop"
.S9:                    .string          "*Unary"
.S10:                   .string          "factor"
.S11:                   .string          "mulop"
.S12:                   .string          "*Binary"
.S13:                   .string          "term"
                        .text
                        .section         .rodata
.C0:
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.C1:
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .text
                        .section         .note.GNU-stack,"",@progbits
