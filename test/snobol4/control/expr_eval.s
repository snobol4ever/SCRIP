  .intel_syntax noprefix
  .text
  .globl proc_Push_α
proc_Push_α:
#=======================================================================================================================
    .global proc_Push_α
    .global proc_Push_β
    .global proc_Push_γ
    .global proc_Push_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 5744], rax
 pop rsi
proc_Push_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n3_α
 xchain0_n1_β:
 jmp xchain0_n2_α
.Lx2_0:
 .quad 0
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n5_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n3_α:
 mov rdi, qword ptr [r12 + 144]
 mov rsi, qword ptr [r12 + 152]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n6_α
 xchain0_n3_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain0_n7_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xchain0_n8_α
 xchain0_n5_β:
 jmp xchain0_n9_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n10_α
 xchain0_n6_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n11_α
 xchain0_n7_β:
 jmp xchain0_n5_α
.Lx8_0:
 .quad 0
 xchain0_n8_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+672] -> [zr+640]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 648], rax
  .section .rodata
  .Lrkfn10: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn10]
 lea rsi, [r12 + 640]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n12_α
 xchain0_n8_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp proc_Push_ω
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string ""
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 304], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n2_α
.Lx12_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain0_n11_α:
 mov rdi, qword ptr [r12 + 480]
 mov rsi, qword ptr [r12 + 488]
 mov rdx, qword ptr [r12 + 512]
 mov rcx, qword ptr [r12 + 520]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n9_α
 xchain0_n13_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+64]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 72], rax
  .section .rodata
  .Lrkfn16: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn16]
 lea rsi, [r12 + 64]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je proc_Push_ω
 jmp proc_Push_γ
 xchain0_n13_β:
 jmp proc_Push_ω
# IR_SUBSCRIPT x[i] variable
 xchain0_n14_α:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 304]
 mov rcx, qword ptr [r12 + 312]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp xchain0_n2_α
# IR_DEREF variable -> value
 xchain0_n15_α:
 mov rdi, qword ptr [r12 + 544]
 mov rsi, qword ptr [r12 + 552]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp xchain0_n5_α
# IR_ASSIGN_VAR
 xchain0_n16_α:
 mov rdi, qword ptr [r12 + 624]
 mov rsi, qword ptr [r12 + 632]
 mov rdx, qword ptr [r12 + 704]
 mov rcx, qword ptr [r12 + 712]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n9_α
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp xchain0_n9_α
 xchain0_n16_β:
 jmp xchain0_n9_α
# IR_DEREF variable -> value
 xchain0_n17_α:
 mov rdi, qword ptr [r12 + 336]
 mov rsi, qword ptr [r12 + 344]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n2_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n18_α:
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 mov rdx, qword ptr [r12 + 576]
 mov rcx, qword ptr [r12 + 584]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [r12 + 384], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n2_α
.Lx22_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [r12 + 592]
 mov rdx, qword ptr [r12 + 600]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain0_n5_α
 xchain0_n20_β:
 jmp xchain0_n5_α
 xchain0_n21_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 368]
 cmp eax, 100
 je .Lx24_0
 mov eax, dword ptr [r12 + 368]
 cmp eax, 6
 jne .Lx24_2
.Lx24_1:
 mov rax, qword ptr [r12 + 376]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 240], 6
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n22_α
.Lx24_0:
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 mov rdx, qword ptr [r12 + 384]
 mov rcx, qword ptr [r12 + 392]
 mov r8d, 0
 lea r9, [r12 + 240]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx24_3
.Lx24_2:
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 mov rdx, qword ptr [r12 + 384]
 mov rcx, qword ptr [r12 + 392]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
.Lx24_3:
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n2_α
# IR_ASSIGN_VAR
 xchain0_n22_α:
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n2_α
 xchain0_n22_β:
 jmp xchain0_n2_α
proc_Push_β:
jmp proc_Push_ω
proc_Push_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 5744]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_Push_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_Pop_α
proc_Pop_α:
#=======================================================================================================================
    .global proc_Pop_α
    .global proc_Pop_β
    .global proc_Pop_γ
    .global proc_Pop_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 5744], rax
 pop rsi
proc_Pop_α_body:
# IR_VAR
 xchain26_n0_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xchain26_n1_α
 xchain26_n0_β:
 jmp xchain26_n2_α
# IR_VAR
 xchain26_n1_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp xchain26_n3_α
 xchain26_n1_β:
 jmp xchain26_n2_α
# IR_VAR
 xchain26_n2_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 jmp xchain26_n4_α
 xchain26_n2_β:
 jmp proc_Pop_γ
# IR_LIT_INTEGER
 xchain26_n3_α:
 mov qword ptr [r12 + 832], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain26_n5_α
 xchain26_n3_β:
 jmp xchain26_n2_α
.Lx30_0:
 .quad 0
# IR_LIT_INTEGER
 xchain26_n4_α:
 mov qword ptr [r12 + 992], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 1000], rax
 jmp xchain26_n6_α
 xchain26_n4_β:
 jmp proc_Pop_γ
.Lx31_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain26_n5_α:
 mov rdi, qword ptr [r12 + 800]
 mov rsi, qword ptr [r12 + 808]
 mov rdx, qword ptr [r12 + 832]
 mov rcx, qword ptr [r12 + 840]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain26_n7_α
 xchain26_n5_β:
 jmp xchain26_n2_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n6_α:
 mov rdi, qword ptr [r12 + 960]
 mov rsi, qword ptr [r12 + 968]
 mov rdx, qword ptr [r12 + 992]
 mov rcx, qword ptr [r12 + 1000]
 call rt_subscript_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 jmp xchain26_n8_α
 xchain26_n6_β:
 jmp proc_Pop_γ
# IR_DEREF variable -> value
 xchain26_n7_α:
 mov rdi, qword ptr [r12 + 864]
 mov rsi, qword ptr [r12 + 872]
 call rt_deref@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp xchain26_n9_α
 xchain26_n7_β:
 jmp xchain26_n2_α
# IR_VAR
 xchain26_n8_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 1088], rax
 mov qword ptr [r12 + 1096], rdx
 jmp xchain26_n10_α
 xchain26_n8_β:
 jmp proc_Pop_γ
# IR_SUBSCRIPT x[i] variable
 xchain26_n9_α:
 mov rdi, qword ptr [r12 + 768]
 mov rsi, qword ptr [r12 + 776]
 mov rdx, qword ptr [r12 + 896]
 mov rcx, qword ptr [r12 + 904]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp xchain26_n11_α
 xchain26_n9_β:
 jmp xchain26_n2_α
# IR_LIT_INTEGER
 xchain26_n10_α:
 mov qword ptr [r12 + 1120], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain26_n12_α
 xchain26_n10_β:
 jmp proc_Pop_γ
.Lx37_0:
 .quad 0
# IR_DEREF variable -> value
 xchain26_n11_α:
 mov rdi, qword ptr [r12 + 912]
 mov rsi, qword ptr [r12 + 920]
 call rt_deref@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 jmp xchain26_n13_α
 xchain26_n11_β:
 jmp xchain26_n2_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n12_α:
 mov rdi, qword ptr [r12 + 1088]
 mov rsi, qword ptr [r12 + 1096]
 mov rdx, qword ptr [r12 + 1120]
 mov rcx, qword ptr [r12 + 1128]
 call rt_subscript_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 jmp xchain26_n14_α
 xchain26_n12_β:
 jmp proc_Pop_γ
# IR_ASSIGN gva
 xchain26_n13_α:
 mov rax, qword ptr [r12 + 944]
 mov rdx, qword ptr [r12 + 952]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 jmp xchain26_n2_α
 xchain26_n13_β:
 jmp xchain26_n2_α
# IR_DEREF variable -> value
 xchain26_n14_α:
 mov rdi, qword ptr [r12 + 1152]
 mov rsi, qword ptr [r12 + 1160]
 call rt_deref@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 jmp xchain26_n15_α
 xchain26_n14_β:
 jmp proc_Pop_γ
# IR_LIT_INTEGER
 xchain26_n15_α:
 mov qword ptr [r12 + 1200], 6
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 1208], rax
 jmp xchain26_n16_α
 xchain26_n15_β:
 jmp proc_Pop_γ
.Lx42_0:
 .quad 1
 xchain26_n16_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1184]
 cmp eax, 100
 je .Lx43_0
 mov eax, dword ptr [r12 + 1184]
 cmp eax, 6
 jne .Lx43_2
.Lx43_1:
 mov rax, qword ptr [r12 + 1192]
 mov rcx, 1
 sub rax, rcx
 mov qword ptr [r12 + 1056], 6
 mov qword ptr [r12 + 1064], rax
 jmp xchain26_n17_α
.Lx43_0:
 mov rdi, qword ptr [r12 + 1184]
 mov rsi, qword ptr [r12 + 1192]
 mov rdx, qword ptr [r12 + 1200]
 mov rcx, qword ptr [r12 + 1208]
 mov r8d, 1
 lea r9, [r12 + 1056]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx43_3
.Lx43_2:
 mov rdi, qword ptr [r12 + 1184]
 mov rsi, qword ptr [r12 + 1192]
 mov rdx, qword ptr [r12 + 1200]
 mov rcx, qword ptr [r12 + 1208]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
.Lx43_3:
 jmp xchain26_n17_α
 xchain26_n16_β:
 jmp proc_Pop_γ
# IR_ASSIGN_VAR
 xchain26_n17_α:
 mov rdi, qword ptr [r12 + 1024]
 mov rsi, qword ptr [r12 + 1032]
 mov rdx, qword ptr [r12 + 1056]
 mov rcx, qword ptr [r12 + 1064]
 call rt_assign_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 jmp proc_Pop_γ
 xchain26_n17_β:
 jmp proc_Pop_γ
proc_Pop_β:
jmp proc_Pop_ω
proc_Pop_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 5744]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_Pop_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_Unary_α
proc_Unary_α:
#=======================================================================================================================
    .global proc_Unary_α
    .global proc_Unary_β
    .global proc_Unary_γ
    .global proc_Unary_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 5744], rax
 pop rsi
proc_Unary_α_body:
 xchain45_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx47_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx47_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx47_2
.Lx47_1:
 call rt_faildescr@PLT
.Lx47_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 cmp eax, 99
 je xchain45_n2_α
 jmp xchain45_n1_α
 xchain45_n0_β:
 jmp xchain45_n2_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain45_n1_α:
 mov rax, qword ptr [r12 + 1264]
 mov rdx, qword ptr [r12 + 1272]
 mov qword ptr [rbx + 64], rax
 mov qword ptr [rbx + 72], rdx
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 jmp xchain45_n2_α
 xchain45_n1_β:
 jmp xchain45_n2_α
 xchain45_n2_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx50_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx50_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx50_2
.Lx50_1:
 call rt_faildescr@PLT
.Lx50_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 cmp eax, 99
 je xchain45_n4_α
 jmp xchain45_n3_α
 xchain45_n2_β:
 jmp xchain45_n4_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain45_n3_α:
 mov rax, qword ptr [r12 + 1312]
 mov rdx, qword ptr [r12 + 1320]
 mov qword ptr [rbx + 80], rax
 mov qword ptr [rbx + 88], rdx
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 jmp xchain45_n4_α
 xchain45_n3_β:
 jmp xchain45_n4_α
# IR_LIT_STRING
 xchain45_n4_α:
 mov qword ptr [r12 + 1344], 1
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 1352], rax
 jmp xchain45_n5_α
 xchain45_n4_β:
 jmp xchain45_n7_α
.Lx52_0:
 .quad .Lx52_0_s
.Lx52_0_s:
 .string ""
 xchain45_n5_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1344] -> [zr+1392]
 mov rax, qword ptr [r12 + 1344]
 mov qword ptr [r12 + 1392], rax
 mov rax, qword ptr [r12 + 1352]
 mov qword ptr [r12 + 1400], rax
  .section .rodata
  .Lrkfn54: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn54]
 lea rsi, [r12 + 1392]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1376], rax
 mov qword ptr [r12 + 1384], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n6_α
 xchain45_n5_β:
 jmp xchain45_n7_α
 xchain45_n6_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx56_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx56_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx56_2
.Lx56_1:
 call rt_faildescr@PLT
.Lx56_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 1424], rax
 mov qword ptr [r12 + 1432], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n8_α
 xchain45_n6_β:
 jmp xchain45_n7_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "Push"
# IR_LIT_STRING
 xchain45_n7_α:
 mov qword ptr [r12 + 1680], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 1688], rax
 jmp xchain45_n9_α
 xchain45_n7_β:
 jmp xchain45_n12_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "dummy"
# IR_VAR
 xchain45_n8_α:
 mov rax, qword ptr [rbx + 80]
 mov rdx, qword ptr [rbx + 88]
 mov qword ptr [r12 + 1536], rax
 mov qword ptr [r12 + 1544], rdx
 jmp xchain45_n10_α
 xchain45_n8_β:
 jmp xchain45_n7_α
 xchain45_n9_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1680] -> [zr+1648]
 mov rax, qword ptr [r12 + 1680]
 mov qword ptr [r12 + 1648], rax
 mov rax, qword ptr [r12 + 1688]
 mov qword ptr [r12 + 1656], rax
  .section .rodata
  .Lrkfn60: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn60]
 lea rsi, [r12 + 1648]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1632], rax
 mov qword ptr [r12 + 1640], rdx
 cmp eax, 99
 je xchain45_n12_α
 jmp xchain45_n11_α
 xchain45_n9_β:
 jmp xchain45_n12_α
# IR_VAR
 xchain45_n10_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1568], rax
 mov qword ptr [r12 + 1576], rdx
 jmp xchain45_n13_α
 xchain45_n10_β:
 jmp xchain45_n7_α
# IR_ASSIGN gva
 xchain45_n11_α:
 mov rax, qword ptr [r12 + 1632]
 mov rdx, qword ptr [r12 + 1640]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 1616], rax
 mov qword ptr [r12 + 1624], rdx
 jmp xchain45_n12_α
 xchain45_n11_β:
 jmp xchain45_n12_α
# IR_LIT_STRING
 xchain45_n12_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain45_n14_α
 xchain45_n12_β:
 jmp proc_Unary_ω
.Lx63_0:
 .quad .Lx63_0_s
.Lx63_0_s:
 .string ""
 xchain45_n13_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 1536]
 mov rsi, qword ptr [r12 + 1544]
 mov rdx, qword ptr [r12 + 1568]
 mov rcx, qword ptr [r12 + 1576]
 call str_concat_d@PLT
 mov qword ptr [r12 + 1504], rax
 mov qword ptr [r12 + 1512], rdx
 jmp xchain45_n15_α
 xchain45_n13_β:
 jmp xchain45_n7_α
 xchain45_n14_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+64]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 72], rax
  .section .rodata
  .Lrkfn66: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn66]
 lea rsi, [r12 + 64]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je proc_Unary_ω
 jmp proc_Unary_γ
 xchain45_n14_β:
 jmp proc_Unary_ω
 xchain45_n15_α:
# BOX IR_CALL EVAL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1504] -> [zr+1472]
 mov rax, qword ptr [r12 + 1504]
 mov qword ptr [r12 + 1472], rax
 mov rax, qword ptr [r12 + 1512]
 mov qword ptr [r12 + 1480], rax
  .section .rodata
  .Lrkfn68: .string "EVAL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn68]
 lea rsi, [r12 + 1472]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1456], rax
 mov qword ptr [r12 + 1464], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n16_α
 xchain45_n15_β:
 jmp xchain45_n7_α
# IR_ASSIGN_VAR
 xchain45_n16_α:
 mov rdi, qword ptr [r12 + 1424]
 mov rsi, qword ptr [r12 + 1432]
 mov rdx, qword ptr [r12 + 1456]
 mov rcx, qword ptr [r12 + 1464]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain45_n7_α
 mov qword ptr [r12 + 1600], rax
 mov qword ptr [r12 + 1608], rdx
 jmp xchain45_n7_α
 xchain45_n16_β:
 jmp xchain45_n7_α
proc_Unary_β:
jmp proc_Unary_ω
proc_Unary_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 5744]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_Unary_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_Binary_α
proc_Binary_α:
#=======================================================================================================================
    .global proc_Binary_α
    .global proc_Binary_β
    .global proc_Binary_γ
    .global proc_Binary_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 5744], rax
 pop rsi
proc_Binary_α_body:
 xchain70_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx72_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx72_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx72_2
.Lx72_1:
 call rt_faildescr@PLT
.Lx72_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 1728], rax
 mov qword ptr [r12 + 1736], rdx
 cmp eax, 99
 je xchain70_n2_α
 jmp xchain70_n1_α
 xchain70_n0_β:
 jmp xchain70_n2_α
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain70_n1_α:
 mov rax, qword ptr [r12 + 1728]
 mov rdx, qword ptr [r12 + 1736]
 mov qword ptr [rbx + 128], rax
 mov qword ptr [rbx + 136], rdx
 mov qword ptr [r12 + 1712], rax
 mov qword ptr [r12 + 1720], rdx
 jmp xchain70_n2_α
 xchain70_n1_β:
 jmp xchain70_n2_α
 xchain70_n2_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx75_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx75_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx75_2
.Lx75_1:
 call rt_faildescr@PLT
.Lx75_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 1776], rax
 mov qword ptr [r12 + 1784], rdx
 cmp eax, 99
 je xchain70_n4_α
 jmp xchain70_n3_α
 xchain70_n2_β:
 jmp xchain70_n4_α
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain70_n3_α:
 mov rax, qword ptr [r12 + 1776]
 mov rdx, qword ptr [r12 + 1784]
 mov qword ptr [rbx + 80], rax
 mov qword ptr [rbx + 88], rdx
 mov qword ptr [r12 + 1760], rax
 mov qword ptr [r12 + 1768], rdx
 jmp xchain70_n4_α
 xchain70_n3_β:
 jmp xchain70_n4_α
 xchain70_n4_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx78_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx78_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx78_2
.Lx78_1:
 call rt_faildescr@PLT
.Lx78_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 1824], rax
 mov qword ptr [r12 + 1832], rdx
 cmp eax, 99
 je xchain70_n6_α
 jmp xchain70_n5_α
 xchain70_n4_β:
 jmp xchain70_n6_α
.Lx78_0:
 .quad .Lx78_0_s
.Lx78_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain70_n5_α:
 mov rax, qword ptr [r12 + 1824]
 mov rdx, qword ptr [r12 + 1832]
 mov qword ptr [rbx + 112], rax
 mov qword ptr [rbx + 120], rdx
 mov qword ptr [r12 + 1808], rax
 mov qword ptr [r12 + 1816], rdx
 jmp xchain70_n6_α
 xchain70_n5_β:
 jmp xchain70_n6_α
# IR_LIT_STRING
 xchain70_n6_α:
 mov qword ptr [r12 + 1856], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [r12 + 1864], rax
 jmp xchain70_n7_α
 xchain70_n6_β:
 jmp xchain70_n9_α
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string ""
 xchain70_n7_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1856] -> [zr+1904]
 mov rax, qword ptr [r12 + 1856]
 mov qword ptr [r12 + 1904], rax
 mov rax, qword ptr [r12 + 1864]
 mov qword ptr [r12 + 1912], rax
  .section .rodata
  .Lrkfn82: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn82]
 lea rsi, [r12 + 1904]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1888], rax
 mov qword ptr [r12 + 1896], rdx
 cmp eax, 99
 je xchain70_n9_α
 jmp xchain70_n8_α
 xchain70_n7_β:
 jmp xchain70_n9_α
 xchain70_n8_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx84_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx84_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx84_2
.Lx84_1:
 call rt_faildescr@PLT
.Lx84_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 1936], rax
 mov qword ptr [r12 + 1944], rdx
 cmp eax, 99
 je xchain70_n9_α
 jmp xchain70_n10_α
 xchain70_n8_β:
 jmp xchain70_n9_α
.Lx84_0:
 .quad .Lx84_0_s
.Lx84_0_s:
 .string "Push"
# IR_LIT_STRING
 xchain70_n9_α:
 mov qword ptr [r12 + 2384], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [r12 + 2392], rax
 jmp xchain70_n11_α
 xchain70_n9_β:
 jmp xchain70_n14_α
.Lx85_0:
 .quad .Lx85_0_s
.Lx85_0_s:
 .string "dummy"
# IR_VAR
 xchain70_n10_α:
 mov rax, qword ptr [rbx + 112]
 mov rdx, qword ptr [rbx + 120]
 mov qword ptr [r12 + 2144], rax
 mov qword ptr [r12 + 2152], rdx
 jmp xchain70_n12_α
 xchain70_n10_β:
 jmp xchain70_n9_α
 xchain70_n11_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2384] -> [zr+2352]
 mov rax, qword ptr [r12 + 2384]
 mov qword ptr [r12 + 2352], rax
 mov rax, qword ptr [r12 + 2392]
 mov qword ptr [r12 + 2360], rax
  .section .rodata
  .Lrkfn88: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn88]
 lea rsi, [r12 + 2352]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2336], rax
 mov qword ptr [r12 + 2344], rdx
 cmp eax, 99
 je xchain70_n14_α
 jmp xchain70_n13_α
 xchain70_n11_β:
 jmp xchain70_n14_α
# IR_LIT_STRING
 xchain70_n12_α:
 mov qword ptr [r12 + 2176], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [r12 + 2184], rax
 jmp xchain70_n15_α
 xchain70_n12_β:
 jmp xchain70_n9_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string " "
# IR_ASSIGN gva
 xchain70_n13_α:
 mov rax, qword ptr [r12 + 2336]
 mov rdx, qword ptr [r12 + 2344]
 mov qword ptr [rbx + 96], rax
 mov qword ptr [rbx + 104], rdx
 mov qword ptr [r12 + 2320], rax
 mov qword ptr [r12 + 2328], rdx
 jmp xchain70_n14_α
 xchain70_n13_β:
 jmp xchain70_n14_α
# IR_LIT_STRING
 xchain70_n14_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx91_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain70_n16_α
 xchain70_n14_β:
 jmp proc_Binary_ω
.Lx91_0:
 .quad .Lx91_0_s
.Lx91_0_s:
 .string ""
 xchain70_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2144]
 mov rsi, qword ptr [r12 + 2152]
 mov rdx, qword ptr [r12 + 2176]
 mov rcx, qword ptr [r12 + 2184]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2112], rax
 mov qword ptr [r12 + 2120], rdx
 jmp xchain70_n17_α
 xchain70_n15_β:
 jmp xchain70_n9_α
 xchain70_n16_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+64]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 72], rax
  .section .rodata
  .Lrkfn94: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn94]
 lea rsi, [r12 + 64]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je proc_Binary_ω
 jmp proc_Binary_γ
 xchain70_n16_β:
 jmp proc_Binary_ω
# IR_VAR
 xchain70_n17_α:
 mov rax, qword ptr [rbx + 80]
 mov rdx, qword ptr [rbx + 88]
 mov qword ptr [r12 + 2208], rax
 mov qword ptr [r12 + 2216], rdx
 jmp xchain70_n18_α
 xchain70_n17_β:
 jmp xchain70_n9_α
 xchain70_n18_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2112]
 mov rsi, qword ptr [r12 + 2120]
 mov rdx, qword ptr [r12 + 2208]
 mov rcx, qword ptr [r12 + 2216]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2080], rax
 mov qword ptr [r12 + 2088], rdx
 jmp xchain70_n19_α
 xchain70_n18_β:
 jmp xchain70_n9_α
# IR_LIT_STRING
 xchain70_n19_α:
 mov qword ptr [r12 + 2240], 1
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [r12 + 2248], rax
 jmp xchain70_n20_α
 xchain70_n19_β:
 jmp xchain70_n9_α
.Lx97_0:
 .quad .Lx97_0_s
.Lx97_0_s:
 .string " "
 xchain70_n20_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2080]
 mov rsi, qword ptr [r12 + 2088]
 mov rdx, qword ptr [r12 + 2240]
 mov rcx, qword ptr [r12 + 2248]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2048], rax
 mov qword ptr [r12 + 2056], rdx
 jmp xchain70_n21_α
 xchain70_n20_β:
 jmp xchain70_n9_α
# IR_VAR
 xchain70_n21_α:
 mov rax, qword ptr [rbx + 128]
 mov rdx, qword ptr [rbx + 136]
 mov qword ptr [r12 + 2272], rax
 mov qword ptr [r12 + 2280], rdx
 jmp xchain70_n22_α
 xchain70_n21_β:
 jmp xchain70_n9_α
 xchain70_n22_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2048]
 mov rsi, qword ptr [r12 + 2056]
 mov rdx, qword ptr [r12 + 2272]
 mov rcx, qword ptr [r12 + 2280]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2016], rax
 mov qword ptr [r12 + 2024], rdx
 jmp xchain70_n23_α
 xchain70_n22_β:
 jmp xchain70_n9_α
 xchain70_n23_α:
# BOX IR_CALL EVAL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2016] -> [zr+1984]
 mov rax, qword ptr [r12 + 2016]
 mov qword ptr [r12 + 1984], rax
 mov rax, qword ptr [r12 + 2024]
 mov qword ptr [r12 + 1992], rax
  .section .rodata
  .Lrkfn102: .string "EVAL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn102]
 lea rsi, [r12 + 1984]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1968], rax
 mov qword ptr [r12 + 1976], rdx
 cmp eax, 99
 je xchain70_n9_α
 jmp xchain70_n24_α
 xchain70_n23_β:
 jmp xchain70_n9_α
# IR_ASSIGN_VAR
 xchain70_n24_α:
 mov rdi, qword ptr [r12 + 1936]
 mov rsi, qword ptr [r12 + 1944]
 mov rdx, qword ptr [r12 + 1968]
 mov rcx, qword ptr [r12 + 1976]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain70_n9_α
 mov qword ptr [r12 + 2304], rax
 mov qword ptr [r12 + 2312], rdx
 jmp xchain70_n9_α
 xchain70_n24_β:
 jmp xchain70_n9_α
proc_Binary_β:
jmp proc_Binary_ω
proc_Binary_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 5744]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_Binary_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$0_α
proc_EXPR$0_α:
#=======================================================================================================================
    .global proc_EXPR$0_α
    .global proc_EXPR$0_β
    .global proc_EXPR$0_γ
    .global proc_EXPR$0_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$0_α_body:
 xchain104_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx106_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx106_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx106_2
.Lx106_1:
 call rt_faildescr@PLT
.Lx106_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$0_ω
 jmp xchain104_n1_α
 xchain104_n0_β:
 jmp proc_EXPR$0_ω
.Lx106_0:
 .quad .Lx106_0_s
.Lx106_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain104_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 352], rax
 mov qword ptr [rbx + 360], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$0_γ
 xchain104_n1_β:
 jmp proc_EXPR$0_ω
proc_EXPR$0_β:
jmp proc_EXPR$0_ω
proc_EXPR$0_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$0_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$1_α
proc_EXPR$1_α:
#=======================================================================================================================
    .global proc_EXPR$1_α
    .global proc_EXPR$1_β
    .global proc_EXPR$1_γ
    .global proc_EXPR$1_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$1_α_body:
 xchain108_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx110_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx110_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx110_2
.Lx110_1:
 call rt_faildescr@PLT
.Lx110_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$1_ω
 jmp xchain108_n1_α
 xchain108_n0_β:
 jmp proc_EXPR$1_ω
.Lx110_0:
 .quad .Lx110_0_s
.Lx110_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain108_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 368], rax
 mov qword ptr [rbx + 376], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$1_γ
 xchain108_n1_β:
 jmp proc_EXPR$1_ω
proc_EXPR$1_β:
jmp proc_EXPR$1_ω
proc_EXPR$1_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$1_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$2_α
proc_EXPR$2_α:
#=======================================================================================================================
    .global proc_EXPR$2_α
    .global proc_EXPR$2_β
    .global proc_EXPR$2_γ
    .global proc_EXPR$2_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$2_α_body:
 xchain112_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx114_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx114_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx114_2
.Lx114_1:
 call rt_faildescr@PLT
.Lx114_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$2_ω
 jmp xchain112_n1_α
 xchain112_n0_β:
 jmp proc_EXPR$2_ω
.Lx114_0:
 .quad .Lx114_0_s
.Lx114_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain112_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 384], rax
 mov qword ptr [rbx + 392], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$2_γ
 xchain112_n1_β:
 jmp proc_EXPR$2_ω
proc_EXPR$2_β:
jmp proc_EXPR$2_ω
proc_EXPR$2_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$2_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$3_α
proc_EXPR$3_α:
#=======================================================================================================================
    .global proc_EXPR$3_α
    .global proc_EXPR$3_β
    .global proc_EXPR$3_γ
    .global proc_EXPR$3_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$3_α_body:
 xchain116_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx118_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx118_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx118_2
.Lx118_1:
 call rt_faildescr@PLT
.Lx118_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$3_ω
 jmp xchain116_n1_α
 xchain116_n0_β:
 jmp proc_EXPR$3_ω
.Lx118_0:
 .quad .Lx118_0_s
.Lx118_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain116_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 400], rax
 mov qword ptr [rbx + 408], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$3_γ
 xchain116_n1_β:
 jmp proc_EXPR$3_ω
proc_EXPR$3_β:
jmp proc_EXPR$3_ω
proc_EXPR$3_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$3_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$4_α
proc_EXPR$4_α:
#=======================================================================================================================
    .global proc_EXPR$4_α
    .global proc_EXPR$4_β
    .global proc_EXPR$4_γ
    .global proc_EXPR$4_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$4_α_body:
# IR_VAR
 xchain120_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain120_n1_α
 xchain120_n0_β:
 jmp proc_EXPR$4_ω
# IR_ASSIGN gva
 xchain120_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 416], rax
 mov qword ptr [rbx + 424], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$4_γ
 xchain120_n1_β:
 jmp proc_EXPR$4_ω
proc_EXPR$4_β:
jmp proc_EXPR$4_ω
proc_EXPR$4_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$4_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$5_α
proc_EXPR$5_α:
#=======================================================================================================================
    .global proc_EXPR$5_α
    .global proc_EXPR$5_β
    .global proc_EXPR$5_γ
    .global proc_EXPR$5_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$5_α_body:
# IR_VAR
 xchain123_n0_α:
 mov rax, qword ptr [rbx + 272]
 mov rdx, qword ptr [rbx + 280]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain123_n1_α
 xchain123_n0_β:
 jmp proc_EXPR$5_ω
# IR_ASSIGN gva
 xchain123_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 432], rax
 mov qword ptr [rbx + 440], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$5_γ
 xchain123_n1_β:
 jmp proc_EXPR$5_ω
proc_EXPR$5_β:
jmp proc_EXPR$5_ω
proc_EXPR$5_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$5_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$6_α
proc_EXPR$6_α:
#=======================================================================================================================
    .global proc_EXPR$6_α
    .global proc_EXPR$6_β
    .global proc_EXPR$6_γ
    .global proc_EXPR$6_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$6_α_body:
# IR_VAR
 xchain126_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain126_n1_α
 xchain126_n0_β:
 jmp proc_EXPR$6_ω
# IR_ASSIGN gva
 xchain126_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 448], rax
 mov qword ptr [rbx + 456], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$6_γ
 xchain126_n1_β:
 jmp proc_EXPR$6_ω
proc_EXPR$6_β:
jmp proc_EXPR$6_ω
proc_EXPR$6_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$6_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$7_α
proc_EXPR$7_α:
#=======================================================================================================================
    .global proc_EXPR$7_α
    .global proc_EXPR$7_β
    .global proc_EXPR$7_γ
    .global proc_EXPR$7_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$7_α_body:
 xchain129_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx131_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx131_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx131_2
.Lx131_1:
 call rt_faildescr@PLT
.Lx131_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$7_ω
 jmp xchain129_n1_α
 xchain129_n0_β:
 jmp proc_EXPR$7_ω
.Lx131_0:
 .quad .Lx131_0_s
.Lx131_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain129_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 464], rax
 mov qword ptr [rbx + 472], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$7_γ
 xchain129_n1_β:
 jmp proc_EXPR$7_ω
proc_EXPR$7_β:
jmp proc_EXPR$7_ω
proc_EXPR$7_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$7_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$8_α
proc_EXPR$8_α:
#=======================================================================================================================
    .global proc_EXPR$8_α
    .global proc_EXPR$8_β
    .global proc_EXPR$8_γ
    .global proc_EXPR$8_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$8_α_body:
# IR_VAR
 xchain133_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain133_n1_α
 xchain133_n0_β:
 jmp proc_EXPR$8_ω
# IR_ASSIGN gva
 xchain133_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 480], rax
 mov qword ptr [rbx + 488], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$8_γ
 xchain133_n1_β:
 jmp proc_EXPR$8_ω
proc_EXPR$8_β:
jmp proc_EXPR$8_ω
proc_EXPR$8_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$8_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$9_α
proc_EXPR$9_α:
#=======================================================================================================================
    .global proc_EXPR$9_α
    .global proc_EXPR$9_β
    .global proc_EXPR$9_γ
    .global proc_EXPR$9_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$9_α_body:
# IR_VAR
 xchain136_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain136_n1_α
 xchain136_n0_β:
 jmp proc_EXPR$9_ω
# IR_ASSIGN gva
 xchain136_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 496], rax
 mov qword ptr [rbx + 504], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$9_γ
 xchain136_n1_β:
 jmp proc_EXPR$9_ω
proc_EXPR$9_β:
jmp proc_EXPR$9_ω
proc_EXPR$9_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$9_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$10_α
proc_EXPR$10_α:
#=======================================================================================================================
    .global proc_EXPR$10_α
    .global proc_EXPR$10_β
    .global proc_EXPR$10_γ
    .global proc_EXPR$10_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$10_α_body:
# IR_VAR
 xchain139_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain139_n1_α
 xchain139_n0_β:
 jmp proc_EXPR$10_ω
# IR_ASSIGN gva
 xchain139_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 512], rax
 mov qword ptr [rbx + 520], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$10_γ
 xchain139_n1_β:
 jmp proc_EXPR$10_ω
proc_EXPR$10_β:
jmp proc_EXPR$10_ω
proc_EXPR$10_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$10_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$11_α
proc_EXPR$11_α:
#=======================================================================================================================
    .global proc_EXPR$11_α
    .global proc_EXPR$11_β
    .global proc_EXPR$11_γ
    .global proc_EXPR$11_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$11_α_body:
 xchain142_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx144_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx144_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx144_2
.Lx144_1:
 call rt_faildescr@PLT
.Lx144_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$11_ω
 jmp xchain142_n1_α
 xchain142_n0_β:
 jmp proc_EXPR$11_ω
.Lx144_0:
 .quad .Lx144_0_s
.Lx144_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain142_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 528], rax
 mov qword ptr [rbx + 536], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$11_γ
 xchain142_n1_β:
 jmp proc_EXPR$11_ω
proc_EXPR$11_β:
jmp proc_EXPR$11_ω
proc_EXPR$11_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$11_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$12_α
proc_EXPR$12_α:
#=======================================================================================================================
    .global proc_EXPR$12_α
    .global proc_EXPR$12_β
    .global proc_EXPR$12_γ
    .global proc_EXPR$12_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$12_α_body:
# IR_VAR
 xchain146_n0_α:
 mov rax, qword ptr [rbx + 320]
 mov rdx, qword ptr [rbx + 328]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain146_n1_α
 xchain146_n0_β:
 jmp proc_EXPR$12_ω
# IR_ASSIGN gva
 xchain146_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 544], rax
 mov qword ptr [rbx + 552], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$12_γ
 xchain146_n1_β:
 jmp proc_EXPR$12_ω
proc_EXPR$12_β:
jmp proc_EXPR$12_ω
proc_EXPR$12_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$12_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$13_α
proc_EXPR$13_α:
#=======================================================================================================================
    .global proc_EXPR$13_α
    .global proc_EXPR$13_β
    .global proc_EXPR$13_γ
    .global proc_EXPR$13_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$13_α_body:
# IR_VAR
 xchain149_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain149_n1_α
 xchain149_n0_β:
 jmp proc_EXPR$13_ω
# IR_ASSIGN gva
 xchain149_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 560], rax
 mov qword ptr [rbx + 568], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$13_γ
 xchain149_n1_β:
 jmp proc_EXPR$13_ω
proc_EXPR$13_β:
jmp proc_EXPR$13_ω
proc_EXPR$13_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$13_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$14_α
proc_EXPR$14_α:
#=======================================================================================================================
    .global proc_EXPR$14_α
    .global proc_EXPR$14_β
    .global proc_EXPR$14_γ
    .global proc_EXPR$14_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$14_α_body:
 xchain152_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx154_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx154_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx154_2
.Lx154_1:
 call rt_faildescr@PLT
.Lx154_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$14_ω
 jmp xchain152_n1_α
 xchain152_n0_β:
 jmp proc_EXPR$14_ω
.Lx154_0:
 .quad .Lx154_0_s
.Lx154_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain152_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 576], rax
 mov qword ptr [rbx + 584], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$14_γ
 xchain152_n1_β:
 jmp proc_EXPR$14_ω
proc_EXPR$14_β:
jmp proc_EXPR$14_ω
proc_EXPR$14_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$14_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$15_α
proc_EXPR$15_α:
#=======================================================================================================================
    .global proc_EXPR$15_α
    .global proc_EXPR$15_β
    .global proc_EXPR$15_γ
    .global proc_EXPR$15_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$15_α_body:
 xchain156_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx158_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx158_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx158_2
.Lx158_1:
 call rt_faildescr@PLT
.Lx158_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$15_ω
 jmp xchain156_n1_α
 xchain156_n0_β:
 jmp proc_EXPR$15_ω
.Lx158_0:
 .quad .Lx158_0_s
.Lx158_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain156_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 592], rax
 mov qword ptr [rbx + 600], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$15_γ
 xchain156_n1_β:
 jmp proc_EXPR$15_ω
proc_EXPR$15_β:
jmp proc_EXPR$15_ω
proc_EXPR$15_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$15_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$16_α
proc_EXPR$16_α:
#=======================================================================================================================
    .global proc_EXPR$16_α
    .global proc_EXPR$16_β
    .global proc_EXPR$16_γ
    .global proc_EXPR$16_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$16_α_body:
 xchain160_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx162_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx162_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx162_2
.Lx162_1:
 call rt_faildescr@PLT
.Lx162_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$16_ω
 jmp xchain160_n1_α
 xchain160_n0_β:
 jmp proc_EXPR$16_ω
.Lx162_0:
 .quad .Lx162_0_s
.Lx162_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain160_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 608], rax
 mov qword ptr [rbx + 616], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$16_γ
 xchain160_n1_β:
 jmp proc_EXPR$16_ω
proc_EXPR$16_β:
jmp proc_EXPR$16_ω
proc_EXPR$16_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$16_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$17_α
proc_EXPR$17_α:
#=======================================================================================================================
    .global proc_EXPR$17_α
    .global proc_EXPR$17_β
    .global proc_EXPR$17_γ
    .global proc_EXPR$17_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$17_α_body:
 xchain164_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx166_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx166_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx166_2
.Lx166_1:
 call rt_faildescr@PLT
.Lx166_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$17_ω
 jmp xchain164_n1_α
 xchain164_n0_β:
 jmp proc_EXPR$17_ω
.Lx166_0:
 .quad .Lx166_0_s
.Lx166_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain164_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 624], rax
 mov qword ptr [rbx + 632], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$17_γ
 xchain164_n1_β:
 jmp proc_EXPR$17_ω
proc_EXPR$17_β:
jmp proc_EXPR$17_ω
proc_EXPR$17_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$17_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$18_α
proc_EXPR$18_α:
#=======================================================================================================================
    .global proc_EXPR$18_α
    .global proc_EXPR$18_β
    .global proc_EXPR$18_γ
    .global proc_EXPR$18_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$18_α_body:
# IR_VAR
 xchain168_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain168_n1_α
 xchain168_n0_β:
 jmp proc_EXPR$18_ω
# IR_ASSIGN gva
 xchain168_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 640], rax
 mov qword ptr [rbx + 648], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$18_γ
 xchain168_n1_β:
 jmp proc_EXPR$18_ω
proc_EXPR$18_β:
jmp proc_EXPR$18_ω
proc_EXPR$18_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$18_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$19_α
proc_EXPR$19_α:
#=======================================================================================================================
    .global proc_EXPR$19_α
    .global proc_EXPR$19_β
    .global proc_EXPR$19_γ
    .global proc_EXPR$19_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$19_α_body:
# IR_VAR
 xchain171_n0_α:
 mov rax, qword ptr [rbx + 272]
 mov rdx, qword ptr [rbx + 280]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain171_n1_α
 xchain171_n0_β:
 jmp proc_EXPR$19_ω
# IR_ASSIGN gva
 xchain171_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 656], rax
 mov qword ptr [rbx + 664], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$19_γ
 xchain171_n1_β:
 jmp proc_EXPR$19_ω
proc_EXPR$19_β:
jmp proc_EXPR$19_ω
proc_EXPR$19_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$19_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$20_α
proc_EXPR$20_α:
#=======================================================================================================================
    .global proc_EXPR$20_α
    .global proc_EXPR$20_β
    .global proc_EXPR$20_γ
    .global proc_EXPR$20_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$20_α_body:
# IR_VAR
 xchain174_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain174_n1_α
 xchain174_n0_β:
 jmp proc_EXPR$20_ω
# IR_ASSIGN gva
 xchain174_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 672], rax
 mov qword ptr [rbx + 680], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$20_γ
 xchain174_n1_β:
 jmp proc_EXPR$20_ω
proc_EXPR$20_β:
jmp proc_EXPR$20_ω
proc_EXPR$20_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$20_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$21_α
proc_EXPR$21_α:
#=======================================================================================================================
    .global proc_EXPR$21_α
    .global proc_EXPR$21_β
    .global proc_EXPR$21_γ
    .global proc_EXPR$21_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$21_α_body:
 xchain177_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx179_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx179_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx179_2
.Lx179_1:
 call rt_faildescr@PLT
.Lx179_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$21_ω
 jmp xchain177_n1_α
 xchain177_n0_β:
 jmp proc_EXPR$21_ω
.Lx179_0:
 .quad .Lx179_0_s
.Lx179_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain177_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 688], rax
 mov qword ptr [rbx + 696], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$21_γ
 xchain177_n1_β:
 jmp proc_EXPR$21_ω
proc_EXPR$21_β:
jmp proc_EXPR$21_ω
proc_EXPR$21_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$21_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$22_α
proc_EXPR$22_α:
#=======================================================================================================================
    .global proc_EXPR$22_α
    .global proc_EXPR$22_β
    .global proc_EXPR$22_γ
    .global proc_EXPR$22_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$22_α_body:
# IR_VAR
 xchain181_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain181_n1_α
 xchain181_n0_β:
 jmp proc_EXPR$22_ω
# IR_ASSIGN gva
 xchain181_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 704], rax
 mov qword ptr [rbx + 712], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$22_γ
 xchain181_n1_β:
 jmp proc_EXPR$22_ω
proc_EXPR$22_β:
jmp proc_EXPR$22_ω
proc_EXPR$22_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$22_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$23_α
proc_EXPR$23_α:
#=======================================================================================================================
    .global proc_EXPR$23_α
    .global proc_EXPR$23_β
    .global proc_EXPR$23_γ
    .global proc_EXPR$23_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$23_α_body:
# IR_VAR
 xchain184_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain184_n1_α
 xchain184_n0_β:
 jmp proc_EXPR$23_ω
# IR_ASSIGN gva
 xchain184_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 720], rax
 mov qword ptr [rbx + 728], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$23_γ
 xchain184_n1_β:
 jmp proc_EXPR$23_ω
proc_EXPR$23_β:
jmp proc_EXPR$23_ω
proc_EXPR$23_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$23_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$24_α
proc_EXPR$24_α:
#=======================================================================================================================
    .global proc_EXPR$24_α
    .global proc_EXPR$24_β
    .global proc_EXPR$24_γ
    .global proc_EXPR$24_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$24_α_body:
# IR_VAR
 xchain187_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain187_n1_α
 xchain187_n0_β:
 jmp proc_EXPR$24_ω
# IR_ASSIGN gva
 xchain187_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 736], rax
 mov qword ptr [rbx + 744], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$24_γ
 xchain187_n1_β:
 jmp proc_EXPR$24_ω
proc_EXPR$24_β:
jmp proc_EXPR$24_ω
proc_EXPR$24_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$24_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$25_α
proc_EXPR$25_α:
#=======================================================================================================================
    .global proc_EXPR$25_α
    .global proc_EXPR$25_β
    .global proc_EXPR$25_γ
    .global proc_EXPR$25_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$25_α_body:
 xchain190_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx192_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx192_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx192_2
.Lx192_1:
 call rt_faildescr@PLT
.Lx192_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$25_ω
 jmp xchain190_n1_α
 xchain190_n0_β:
 jmp proc_EXPR$25_ω
.Lx192_0:
 .quad .Lx192_0_s
.Lx192_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain190_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 752], rax
 mov qword ptr [rbx + 760], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$25_γ
 xchain190_n1_β:
 jmp proc_EXPR$25_ω
proc_EXPR$25_β:
jmp proc_EXPR$25_ω
proc_EXPR$25_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$25_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$26_α
proc_EXPR$26_α:
#=======================================================================================================================
    .global proc_EXPR$26_α
    .global proc_EXPR$26_β
    .global proc_EXPR$26_γ
    .global proc_EXPR$26_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$26_α_body:
# IR_VAR
 xchain194_n0_α:
 mov rax, qword ptr [rbx + 320]
 mov rdx, qword ptr [rbx + 328]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain194_n1_α
 xchain194_n0_β:
 jmp proc_EXPR$26_ω
# IR_ASSIGN gva
 xchain194_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 768], rax
 mov qword ptr [rbx + 776], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$26_γ
 xchain194_n1_β:
 jmp proc_EXPR$26_ω
proc_EXPR$26_β:
jmp proc_EXPR$26_ω
proc_EXPR$26_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$26_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$27_α
proc_EXPR$27_α:
#=======================================================================================================================
    .global proc_EXPR$27_α
    .global proc_EXPR$27_β
    .global proc_EXPR$27_γ
    .global proc_EXPR$27_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$27_α_body:
# IR_VAR
 xchain197_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain197_n1_α
 xchain197_n0_β:
 jmp proc_EXPR$27_ω
# IR_ASSIGN gva
 xchain197_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 784], rax
 mov qword ptr [rbx + 792], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$27_γ
 xchain197_n1_β:
 jmp proc_EXPR$27_ω
proc_EXPR$27_β:
jmp proc_EXPR$27_ω
proc_EXPR$27_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$27_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$28_α
proc_EXPR$28_α:
#=======================================================================================================================
    .global proc_EXPR$28_α
    .global proc_EXPR$28_β
    .global proc_EXPR$28_γ
    .global proc_EXPR$28_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$28_α_body:
 xchain200_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx202_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx202_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx202_2
.Lx202_1:
 call rt_faildescr@PLT
.Lx202_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$28_ω
 jmp xchain200_n1_α
 xchain200_n0_β:
 jmp proc_EXPR$28_ω
.Lx202_0:
 .quad .Lx202_0_s
.Lx202_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain200_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 800], rax
 mov qword ptr [rbx + 808], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$28_γ
 xchain200_n1_β:
 jmp proc_EXPR$28_ω
proc_EXPR$28_β:
jmp proc_EXPR$28_ω
proc_EXPR$28_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$28_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$29_α
proc_EXPR$29_α:
#=======================================================================================================================
    .global proc_EXPR$29_α
    .global proc_EXPR$29_β
    .global proc_EXPR$29_γ
    .global proc_EXPR$29_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$29_α_body:
 xchain204_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx206_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx206_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx206_2
.Lx206_1:
 call rt_faildescr@PLT
.Lx206_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$29_ω
 jmp xchain204_n1_α
 xchain204_n0_β:
 jmp proc_EXPR$29_ω
.Lx206_0:
 .quad .Lx206_0_s
.Lx206_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain204_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 816], rax
 mov qword ptr [rbx + 824], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$29_γ
 xchain204_n1_β:
 jmp proc_EXPR$29_ω
proc_EXPR$29_β:
jmp proc_EXPR$29_ω
proc_EXPR$29_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$29_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$30_α
proc_EXPR$30_α:
#=======================================================================================================================
    .global proc_EXPR$30_α
    .global proc_EXPR$30_β
    .global proc_EXPR$30_γ
    .global proc_EXPR$30_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$30_α_body:
 xchain208_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx210_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx210_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx210_2
.Lx210_1:
 call rt_faildescr@PLT
.Lx210_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$30_ω
 jmp xchain208_n1_α
 xchain208_n0_β:
 jmp proc_EXPR$30_ω
.Lx210_0:
 .quad .Lx210_0_s
.Lx210_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain208_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 832], rax
 mov qword ptr [rbx + 840], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$30_γ
 xchain208_n1_β:
 jmp proc_EXPR$30_ω
proc_EXPR$30_β:
jmp proc_EXPR$30_ω
proc_EXPR$30_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$30_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$31_α
proc_EXPR$31_α:
#=======================================================================================================================
    .global proc_EXPR$31_α
    .global proc_EXPR$31_β
    .global proc_EXPR$31_γ
    .global proc_EXPR$31_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$31_α_body:
 xchain212_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx214_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx214_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx214_2
.Lx214_1:
 call rt_faildescr@PLT
.Lx214_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$31_ω
 jmp xchain212_n1_α
 xchain212_n0_β:
 jmp proc_EXPR$31_ω
.Lx214_0:
 .quad .Lx214_0_s
.Lx214_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain212_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 848], rax
 mov qword ptr [rbx + 856], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$31_γ
 xchain212_n1_β:
 jmp proc_EXPR$31_ω
proc_EXPR$31_β:
jmp proc_EXPR$31_ω
proc_EXPR$31_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$31_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$32_α
proc_EXPR$32_α:
#=======================================================================================================================
    .global proc_EXPR$32_α
    .global proc_EXPR$32_β
    .global proc_EXPR$32_γ
    .global proc_EXPR$32_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$32_α_body:
# IR_VAR
 xchain216_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain216_n1_α
 xchain216_n0_β:
 jmp proc_EXPR$32_ω
# IR_ASSIGN gva
 xchain216_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 864], rax
 mov qword ptr [rbx + 872], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$32_γ
 xchain216_n1_β:
 jmp proc_EXPR$32_ω
proc_EXPR$32_β:
jmp proc_EXPR$32_ω
proc_EXPR$32_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$32_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$33_α
proc_EXPR$33_α:
#=======================================================================================================================
    .global proc_EXPR$33_α
    .global proc_EXPR$33_β
    .global proc_EXPR$33_γ
    .global proc_EXPR$33_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$33_α_body:
# IR_VAR
 xchain219_n0_α:
 mov rax, qword ptr [rbx + 272]
 mov rdx, qword ptr [rbx + 280]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain219_n1_α
 xchain219_n0_β:
 jmp proc_EXPR$33_ω
# IR_ASSIGN gva
 xchain219_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 880], rax
 mov qword ptr [rbx + 888], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$33_γ
 xchain219_n1_β:
 jmp proc_EXPR$33_ω
proc_EXPR$33_β:
jmp proc_EXPR$33_ω
proc_EXPR$33_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$33_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$34_α
proc_EXPR$34_α:
#=======================================================================================================================
    .global proc_EXPR$34_α
    .global proc_EXPR$34_β
    .global proc_EXPR$34_γ
    .global proc_EXPR$34_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$34_α_body:
# IR_VAR
 xchain222_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain222_n1_α
 xchain222_n0_β:
 jmp proc_EXPR$34_ω
# IR_ASSIGN gva
 xchain222_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 896], rax
 mov qword ptr [rbx + 904], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$34_γ
 xchain222_n1_β:
 jmp proc_EXPR$34_ω
proc_EXPR$34_β:
jmp proc_EXPR$34_ω
proc_EXPR$34_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$34_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$35_α
proc_EXPR$35_α:
#=======================================================================================================================
    .global proc_EXPR$35_α
    .global proc_EXPR$35_β
    .global proc_EXPR$35_γ
    .global proc_EXPR$35_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$35_α_body:
 xchain225_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx227_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx227_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx227_2
.Lx227_1:
 call rt_faildescr@PLT
.Lx227_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$35_ω
 jmp xchain225_n1_α
 xchain225_n0_β:
 jmp proc_EXPR$35_ω
.Lx227_0:
 .quad .Lx227_0_s
.Lx227_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain225_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 912], rax
 mov qword ptr [rbx + 920], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$35_γ
 xchain225_n1_β:
 jmp proc_EXPR$35_ω
proc_EXPR$35_β:
jmp proc_EXPR$35_ω
proc_EXPR$35_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$35_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$36_α
proc_EXPR$36_α:
#=======================================================================================================================
    .global proc_EXPR$36_α
    .global proc_EXPR$36_β
    .global proc_EXPR$36_γ
    .global proc_EXPR$36_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$36_α_body:
# IR_VAR
 xchain229_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain229_n1_α
 xchain229_n0_β:
 jmp proc_EXPR$36_ω
# IR_ASSIGN gva
 xchain229_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 928], rax
 mov qword ptr [rbx + 936], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$36_γ
 xchain229_n1_β:
 jmp proc_EXPR$36_ω
proc_EXPR$36_β:
jmp proc_EXPR$36_ω
proc_EXPR$36_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$36_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$37_α
proc_EXPR$37_α:
#=======================================================================================================================
    .global proc_EXPR$37_α
    .global proc_EXPR$37_β
    .global proc_EXPR$37_γ
    .global proc_EXPR$37_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$37_α_body:
# IR_VAR
 xchain232_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain232_n1_α
 xchain232_n0_β:
 jmp proc_EXPR$37_ω
# IR_ASSIGN gva
 xchain232_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 944], rax
 mov qword ptr [rbx + 952], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$37_γ
 xchain232_n1_β:
 jmp proc_EXPR$37_ω
proc_EXPR$37_β:
jmp proc_EXPR$37_ω
proc_EXPR$37_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$37_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$38_α
proc_EXPR$38_α:
#=======================================================================================================================
    .global proc_EXPR$38_α
    .global proc_EXPR$38_β
    .global proc_EXPR$38_γ
    .global proc_EXPR$38_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$38_α_body:
# IR_VAR
 xchain235_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain235_n1_α
 xchain235_n0_β:
 jmp proc_EXPR$38_ω
# IR_ASSIGN gva
 xchain235_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 960], rax
 mov qword ptr [rbx + 968], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$38_γ
 xchain235_n1_β:
 jmp proc_EXPR$38_ω
proc_EXPR$38_β:
jmp proc_EXPR$38_ω
proc_EXPR$38_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$38_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$39_α
proc_EXPR$39_α:
#=======================================================================================================================
    .global proc_EXPR$39_α
    .global proc_EXPR$39_β
    .global proc_EXPR$39_γ
    .global proc_EXPR$39_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$39_α_body:
 xchain238_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx240_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx240_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx240_2
.Lx240_1:
 call rt_faildescr@PLT
.Lx240_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$39_ω
 jmp xchain238_n1_α
 xchain238_n0_β:
 jmp proc_EXPR$39_ω
.Lx240_0:
 .quad .Lx240_0_s
.Lx240_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain238_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 976], rax
 mov qword ptr [rbx + 984], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$39_γ
 xchain238_n1_β:
 jmp proc_EXPR$39_ω
proc_EXPR$39_β:
jmp proc_EXPR$39_ω
proc_EXPR$39_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$39_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$40_α
proc_EXPR$40_α:
#=======================================================================================================================
    .global proc_EXPR$40_α
    .global proc_EXPR$40_β
    .global proc_EXPR$40_γ
    .global proc_EXPR$40_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$40_α_body:
# IR_VAR
 xchain242_n0_α:
 mov rax, qword ptr [rbx + 320]
 mov rdx, qword ptr [rbx + 328]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain242_n1_α
 xchain242_n0_β:
 jmp proc_EXPR$40_ω
# IR_ASSIGN gva
 xchain242_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 992], rax
 mov qword ptr [rbx + 1000], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$40_γ
 xchain242_n1_β:
 jmp proc_EXPR$40_ω
proc_EXPR$40_β:
jmp proc_EXPR$40_ω
proc_EXPR$40_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$40_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$41_α
proc_EXPR$41_α:
#=======================================================================================================================
    .global proc_EXPR$41_α
    .global proc_EXPR$41_β
    .global proc_EXPR$41_γ
    .global proc_EXPR$41_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$41_α_body:
# IR_VAR
 xchain245_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain245_n1_α
 xchain245_n0_β:
 jmp proc_EXPR$41_ω
# IR_ASSIGN gva
 xchain245_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1008], rax
 mov qword ptr [rbx + 1016], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$41_γ
 xchain245_n1_β:
 jmp proc_EXPR$41_ω
proc_EXPR$41_β:
jmp proc_EXPR$41_ω
proc_EXPR$41_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$41_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$42_α
proc_EXPR$42_α:
#=======================================================================================================================
    .global proc_EXPR$42_α
    .global proc_EXPR$42_β
    .global proc_EXPR$42_γ
    .global proc_EXPR$42_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$42_α_body:
 xchain248_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx250_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx250_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx250_2
.Lx250_1:
 call rt_faildescr@PLT
.Lx250_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$42_ω
 jmp xchain248_n1_α
 xchain248_n0_β:
 jmp proc_EXPR$42_ω
.Lx250_0:
 .quad .Lx250_0_s
.Lx250_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain248_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1024], rax
 mov qword ptr [rbx + 1032], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$42_γ
 xchain248_n1_β:
 jmp proc_EXPR$42_ω
proc_EXPR$42_β:
jmp proc_EXPR$42_ω
proc_EXPR$42_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$42_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$43_α
proc_EXPR$43_α:
#=======================================================================================================================
    .global proc_EXPR$43_α
    .global proc_EXPR$43_β
    .global proc_EXPR$43_γ
    .global proc_EXPR$43_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$43_α_body:
 xchain252_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx254_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx254_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx254_2
.Lx254_1:
 call rt_faildescr@PLT
.Lx254_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$43_ω
 jmp xchain252_n1_α
 xchain252_n0_β:
 jmp proc_EXPR$43_ω
.Lx254_0:
 .quad .Lx254_0_s
.Lx254_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain252_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1040], rax
 mov qword ptr [rbx + 1048], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$43_γ
 xchain252_n1_β:
 jmp proc_EXPR$43_ω
proc_EXPR$43_β:
jmp proc_EXPR$43_ω
proc_EXPR$43_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$43_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$44_α
proc_EXPR$44_α:
#=======================================================================================================================
    .global proc_EXPR$44_α
    .global proc_EXPR$44_β
    .global proc_EXPR$44_γ
    .global proc_EXPR$44_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$44_α_body:
 xchain256_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx258_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx258_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx258_2
.Lx258_1:
 call rt_faildescr@PLT
.Lx258_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$44_ω
 jmp xchain256_n1_α
 xchain256_n0_β:
 jmp proc_EXPR$44_ω
.Lx258_0:
 .quad .Lx258_0_s
.Lx258_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain256_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1056], rax
 mov qword ptr [rbx + 1064], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$44_γ
 xchain256_n1_β:
 jmp proc_EXPR$44_ω
proc_EXPR$44_β:
jmp proc_EXPR$44_ω
proc_EXPR$44_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$44_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$45_α
proc_EXPR$45_α:
#=======================================================================================================================
    .global proc_EXPR$45_α
    .global proc_EXPR$45_β
    .global proc_EXPR$45_γ
    .global proc_EXPR$45_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$45_α_body:
 xchain260_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx262_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx262_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx262_2
.Lx262_1:
 call rt_faildescr@PLT
.Lx262_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$45_ω
 jmp xchain260_n1_α
 xchain260_n0_β:
 jmp proc_EXPR$45_ω
.Lx262_0:
 .quad .Lx262_0_s
.Lx262_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain260_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1072], rax
 mov qword ptr [rbx + 1080], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$45_γ
 xchain260_n1_β:
 jmp proc_EXPR$45_ω
proc_EXPR$45_β:
jmp proc_EXPR$45_ω
proc_EXPR$45_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$45_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$46_α
proc_EXPR$46_α:
#=======================================================================================================================
    .global proc_EXPR$46_α
    .global proc_EXPR$46_β
    .global proc_EXPR$46_γ
    .global proc_EXPR$46_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$46_α_body:
# IR_VAR
 xchain264_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain264_n1_α
 xchain264_n0_β:
 jmp proc_EXPR$46_ω
# IR_ASSIGN gva
 xchain264_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1088], rax
 mov qword ptr [rbx + 1096], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$46_γ
 xchain264_n1_β:
 jmp proc_EXPR$46_ω
proc_EXPR$46_β:
jmp proc_EXPR$46_ω
proc_EXPR$46_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$46_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$47_α
proc_EXPR$47_α:
#=======================================================================================================================
    .global proc_EXPR$47_α
    .global proc_EXPR$47_β
    .global proc_EXPR$47_γ
    .global proc_EXPR$47_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$47_α_body:
# IR_VAR
 xchain267_n0_α:
 mov rax, qword ptr [rbx + 272]
 mov rdx, qword ptr [rbx + 280]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain267_n1_α
 xchain267_n0_β:
 jmp proc_EXPR$47_ω
# IR_ASSIGN gva
 xchain267_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1104], rax
 mov qword ptr [rbx + 1112], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$47_γ
 xchain267_n1_β:
 jmp proc_EXPR$47_ω
proc_EXPR$47_β:
jmp proc_EXPR$47_ω
proc_EXPR$47_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$47_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$48_α
proc_EXPR$48_α:
#=======================================================================================================================
    .global proc_EXPR$48_α
    .global proc_EXPR$48_β
    .global proc_EXPR$48_γ
    .global proc_EXPR$48_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$48_α_body:
# IR_VAR
 xchain270_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain270_n1_α
 xchain270_n0_β:
 jmp proc_EXPR$48_ω
# IR_ASSIGN gva
 xchain270_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1120], rax
 mov qword ptr [rbx + 1128], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$48_γ
 xchain270_n1_β:
 jmp proc_EXPR$48_ω
proc_EXPR$48_β:
jmp proc_EXPR$48_ω
proc_EXPR$48_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$48_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$49_α
proc_EXPR$49_α:
#=======================================================================================================================
    .global proc_EXPR$49_α
    .global proc_EXPR$49_β
    .global proc_EXPR$49_γ
    .global proc_EXPR$49_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$49_α_body:
 xchain273_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx275_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx275_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx275_2
.Lx275_1:
 call rt_faildescr@PLT
.Lx275_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$49_ω
 jmp xchain273_n1_α
 xchain273_n0_β:
 jmp proc_EXPR$49_ω
.Lx275_0:
 .quad .Lx275_0_s
.Lx275_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain273_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1136], rax
 mov qword ptr [rbx + 1144], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$49_γ
 xchain273_n1_β:
 jmp proc_EXPR$49_ω
proc_EXPR$49_β:
jmp proc_EXPR$49_ω
proc_EXPR$49_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$49_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$50_α
proc_EXPR$50_α:
#=======================================================================================================================
    .global proc_EXPR$50_α
    .global proc_EXPR$50_β
    .global proc_EXPR$50_γ
    .global proc_EXPR$50_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$50_α_body:
# IR_VAR
 xchain277_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain277_n1_α
 xchain277_n0_β:
 jmp proc_EXPR$50_ω
# IR_ASSIGN gva
 xchain277_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1152], rax
 mov qword ptr [rbx + 1160], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$50_γ
 xchain277_n1_β:
 jmp proc_EXPR$50_ω
proc_EXPR$50_β:
jmp proc_EXPR$50_ω
proc_EXPR$50_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$50_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$51_α
proc_EXPR$51_α:
#=======================================================================================================================
    .global proc_EXPR$51_α
    .global proc_EXPR$51_β
    .global proc_EXPR$51_γ
    .global proc_EXPR$51_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$51_α_body:
# IR_VAR
 xchain280_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain280_n1_α
 xchain280_n0_β:
 jmp proc_EXPR$51_ω
# IR_ASSIGN gva
 xchain280_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1168], rax
 mov qword ptr [rbx + 1176], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$51_γ
 xchain280_n1_β:
 jmp proc_EXPR$51_ω
proc_EXPR$51_β:
jmp proc_EXPR$51_ω
proc_EXPR$51_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$51_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$52_α
proc_EXPR$52_α:
#=======================================================================================================================
    .global proc_EXPR$52_α
    .global proc_EXPR$52_β
    .global proc_EXPR$52_γ
    .global proc_EXPR$52_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$52_α_body:
# IR_VAR
 xchain283_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain283_n1_α
 xchain283_n0_β:
 jmp proc_EXPR$52_ω
# IR_ASSIGN gva
 xchain283_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1184], rax
 mov qword ptr [rbx + 1192], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$52_γ
 xchain283_n1_β:
 jmp proc_EXPR$52_ω
proc_EXPR$52_β:
jmp proc_EXPR$52_ω
proc_EXPR$52_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$52_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$53_α
proc_EXPR$53_α:
#=======================================================================================================================
    .global proc_EXPR$53_α
    .global proc_EXPR$53_β
    .global proc_EXPR$53_γ
    .global proc_EXPR$53_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$53_α_body:
 xchain286_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx288_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx288_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx288_2
.Lx288_1:
 call rt_faildescr@PLT
.Lx288_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$53_ω
 jmp xchain286_n1_α
 xchain286_n0_β:
 jmp proc_EXPR$53_ω
.Lx288_0:
 .quad .Lx288_0_s
.Lx288_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain286_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1200], rax
 mov qword ptr [rbx + 1208], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$53_γ
 xchain286_n1_β:
 jmp proc_EXPR$53_ω
proc_EXPR$53_β:
jmp proc_EXPR$53_ω
proc_EXPR$53_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$53_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$54_α
proc_EXPR$54_α:
#=======================================================================================================================
    .global proc_EXPR$54_α
    .global proc_EXPR$54_β
    .global proc_EXPR$54_γ
    .global proc_EXPR$54_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$54_α_body:
# IR_VAR
 xchain290_n0_α:
 mov rax, qword ptr [rbx + 320]
 mov rdx, qword ptr [rbx + 328]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain290_n1_α
 xchain290_n0_β:
 jmp proc_EXPR$54_ω
# IR_ASSIGN gva
 xchain290_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1216], rax
 mov qword ptr [rbx + 1224], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$54_γ
 xchain290_n1_β:
 jmp proc_EXPR$54_ω
proc_EXPR$54_β:
jmp proc_EXPR$54_ω
proc_EXPR$54_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$54_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$55_α
proc_EXPR$55_α:
#=======================================================================================================================
    .global proc_EXPR$55_α
    .global proc_EXPR$55_β
    .global proc_EXPR$55_γ
    .global proc_EXPR$55_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$55_α_body:
# IR_VAR
 xchain293_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain293_n1_α
 xchain293_n0_β:
 jmp proc_EXPR$55_ω
# IR_ASSIGN gva
 xchain293_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1232], rax
 mov qword ptr [rbx + 1240], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$55_γ
 xchain293_n1_β:
 jmp proc_EXPR$55_ω
proc_EXPR$55_β:
jmp proc_EXPR$55_ω
proc_EXPR$55_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$55_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$56_α
proc_EXPR$56_α:
#=======================================================================================================================
    .global proc_EXPR$56_α
    .global proc_EXPR$56_β
    .global proc_EXPR$56_γ
    .global proc_EXPR$56_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$56_α_body:
 xchain296_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx298_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx298_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx298_2
.Lx298_1:
 call rt_faildescr@PLT
.Lx298_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$56_ω
 jmp xchain296_n1_α
 xchain296_n0_β:
 jmp proc_EXPR$56_ω
.Lx298_0:
 .quad .Lx298_0_s
.Lx298_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain296_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1248], rax
 mov qword ptr [rbx + 1256], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$56_γ
 xchain296_n1_β:
 jmp proc_EXPR$56_ω
proc_EXPR$56_β:
jmp proc_EXPR$56_ω
proc_EXPR$56_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$56_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$57_α
proc_EXPR$57_α:
#=======================================================================================================================
    .global proc_EXPR$57_α
    .global proc_EXPR$57_β
    .global proc_EXPR$57_γ
    .global proc_EXPR$57_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$57_α_body:
 xchain300_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx302_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx302_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx302_2
.Lx302_1:
 call rt_faildescr@PLT
.Lx302_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$57_ω
 jmp xchain300_n1_α
 xchain300_n0_β:
 jmp proc_EXPR$57_ω
.Lx302_0:
 .quad .Lx302_0_s
.Lx302_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain300_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1264], rax
 mov qword ptr [rbx + 1272], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$57_γ
 xchain300_n1_β:
 jmp proc_EXPR$57_ω
proc_EXPR$57_β:
jmp proc_EXPR$57_ω
proc_EXPR$57_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$57_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$58_α
proc_EXPR$58_α:
#=======================================================================================================================
    .global proc_EXPR$58_α
    .global proc_EXPR$58_β
    .global proc_EXPR$58_γ
    .global proc_EXPR$58_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$58_α_body:
 xchain304_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx306_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx306_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx306_2
.Lx306_1:
 call rt_faildescr@PLT
.Lx306_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$58_ω
 jmp xchain304_n1_α
 xchain304_n0_β:
 jmp proc_EXPR$58_ω
.Lx306_0:
 .quad .Lx306_0_s
.Lx306_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain304_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1280], rax
 mov qword ptr [rbx + 1288], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$58_γ
 xchain304_n1_β:
 jmp proc_EXPR$58_ω
proc_EXPR$58_β:
jmp proc_EXPR$58_ω
proc_EXPR$58_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$58_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$59_α
proc_EXPR$59_α:
#=======================================================================================================================
    .global proc_EXPR$59_α
    .global proc_EXPR$59_β
    .global proc_EXPR$59_γ
    .global proc_EXPR$59_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$59_α_body:
 xchain308_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx310_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx310_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx310_2
.Lx310_1:
 call rt_faildescr@PLT
.Lx310_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$59_ω
 jmp xchain308_n1_α
 xchain308_n0_β:
 jmp proc_EXPR$59_ω
.Lx310_0:
 .quad .Lx310_0_s
.Lx310_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain308_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1296], rax
 mov qword ptr [rbx + 1304], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$59_γ
 xchain308_n1_β:
 jmp proc_EXPR$59_ω
proc_EXPR$59_β:
jmp proc_EXPR$59_ω
proc_EXPR$59_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$59_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$60_α
proc_EXPR$60_α:
#=======================================================================================================================
    .global proc_EXPR$60_α
    .global proc_EXPR$60_β
    .global proc_EXPR$60_γ
    .global proc_EXPR$60_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$60_α_body:
# IR_VAR
 xchain312_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain312_n1_α
 xchain312_n0_β:
 jmp proc_EXPR$60_ω
# IR_ASSIGN gva
 xchain312_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1312], rax
 mov qword ptr [rbx + 1320], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$60_γ
 xchain312_n1_β:
 jmp proc_EXPR$60_ω
proc_EXPR$60_β:
jmp proc_EXPR$60_ω
proc_EXPR$60_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$60_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$61_α
proc_EXPR$61_α:
#=======================================================================================================================
    .global proc_EXPR$61_α
    .global proc_EXPR$61_β
    .global proc_EXPR$61_γ
    .global proc_EXPR$61_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$61_α_body:
# IR_VAR
 xchain315_n0_α:
 mov rax, qword ptr [rbx + 272]
 mov rdx, qword ptr [rbx + 280]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain315_n1_α
 xchain315_n0_β:
 jmp proc_EXPR$61_ω
# IR_ASSIGN gva
 xchain315_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1328], rax
 mov qword ptr [rbx + 1336], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$61_γ
 xchain315_n1_β:
 jmp proc_EXPR$61_ω
proc_EXPR$61_β:
jmp proc_EXPR$61_ω
proc_EXPR$61_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$61_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$62_α
proc_EXPR$62_α:
#=======================================================================================================================
    .global proc_EXPR$62_α
    .global proc_EXPR$62_β
    .global proc_EXPR$62_γ
    .global proc_EXPR$62_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$62_α_body:
# IR_VAR
 xchain318_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain318_n1_α
 xchain318_n0_β:
 jmp proc_EXPR$62_ω
# IR_ASSIGN gva
 xchain318_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1344], rax
 mov qword ptr [rbx + 1352], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$62_γ
 xchain318_n1_β:
 jmp proc_EXPR$62_ω
proc_EXPR$62_β:
jmp proc_EXPR$62_ω
proc_EXPR$62_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$62_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$63_α
proc_EXPR$63_α:
#=======================================================================================================================
    .global proc_EXPR$63_α
    .global proc_EXPR$63_β
    .global proc_EXPR$63_γ
    .global proc_EXPR$63_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$63_α_body:
 xchain321_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx323_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx323_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx323_2
.Lx323_1:
 call rt_faildescr@PLT
.Lx323_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$63_ω
 jmp xchain321_n1_α
 xchain321_n0_β:
 jmp proc_EXPR$63_ω
.Lx323_0:
 .quad .Lx323_0_s
.Lx323_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain321_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1360], rax
 mov qword ptr [rbx + 1368], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$63_γ
 xchain321_n1_β:
 jmp proc_EXPR$63_ω
proc_EXPR$63_β:
jmp proc_EXPR$63_ω
proc_EXPR$63_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$63_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$64_α
proc_EXPR$64_α:
#=======================================================================================================================
    .global proc_EXPR$64_α
    .global proc_EXPR$64_β
    .global proc_EXPR$64_γ
    .global proc_EXPR$64_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$64_α_body:
# IR_VAR
 xchain325_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain325_n1_α
 xchain325_n0_β:
 jmp proc_EXPR$64_ω
# IR_ASSIGN gva
 xchain325_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1376], rax
 mov qword ptr [rbx + 1384], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$64_γ
 xchain325_n1_β:
 jmp proc_EXPR$64_ω
proc_EXPR$64_β:
jmp proc_EXPR$64_ω
proc_EXPR$64_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$64_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$65_α
proc_EXPR$65_α:
#=======================================================================================================================
    .global proc_EXPR$65_α
    .global proc_EXPR$65_β
    .global proc_EXPR$65_γ
    .global proc_EXPR$65_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$65_α_body:
# IR_VAR
 xchain328_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain328_n1_α
 xchain328_n0_β:
 jmp proc_EXPR$65_ω
# IR_ASSIGN gva
 xchain328_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1392], rax
 mov qword ptr [rbx + 1400], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$65_γ
 xchain328_n1_β:
 jmp proc_EXPR$65_ω
proc_EXPR$65_β:
jmp proc_EXPR$65_ω
proc_EXPR$65_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$65_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$66_α
proc_EXPR$66_α:
#=======================================================================================================================
    .global proc_EXPR$66_α
    .global proc_EXPR$66_β
    .global proc_EXPR$66_γ
    .global proc_EXPR$66_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$66_α_body:
# IR_VAR
 xchain331_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain331_n1_α
 xchain331_n0_β:
 jmp proc_EXPR$66_ω
# IR_ASSIGN gva
 xchain331_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1408], rax
 mov qword ptr [rbx + 1416], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$66_γ
 xchain331_n1_β:
 jmp proc_EXPR$66_ω
proc_EXPR$66_β:
jmp proc_EXPR$66_ω
proc_EXPR$66_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$66_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$67_α
proc_EXPR$67_α:
#=======================================================================================================================
    .global proc_EXPR$67_α
    .global proc_EXPR$67_β
    .global proc_EXPR$67_γ
    .global proc_EXPR$67_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$67_α_body:
 xchain334_n0_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx336_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx336_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx336_2
.Lx336_1:
 call rt_faildescr@PLT
.Lx336_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$67_ω
 jmp xchain334_n1_α
 xchain334_n0_β:
 jmp proc_EXPR$67_ω
.Lx336_0:
 .quad .Lx336_0_s
.Lx336_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain334_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1424], rax
 mov qword ptr [rbx + 1432], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$67_γ
 xchain334_n1_β:
 jmp proc_EXPR$67_ω
proc_EXPR$67_β:
jmp proc_EXPR$67_ω
proc_EXPR$67_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$67_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$68_α
proc_EXPR$68_α:
#=======================================================================================================================
    .global proc_EXPR$68_α
    .global proc_EXPR$68_β
    .global proc_EXPR$68_γ
    .global proc_EXPR$68_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$68_α_body:
# IR_VAR
 xchain338_n0_α:
 mov rax, qword ptr [rbx + 320]
 mov rdx, qword ptr [rbx + 328]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain338_n1_α
 xchain338_n0_β:
 jmp proc_EXPR$68_ω
# IR_ASSIGN gva
 xchain338_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1440], rax
 mov qword ptr [rbx + 1448], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$68_γ
 xchain338_n1_β:
 jmp proc_EXPR$68_ω
proc_EXPR$68_β:
jmp proc_EXPR$68_ω
proc_EXPR$68_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$68_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$69_α
proc_EXPR$69_α:
#=======================================================================================================================
    .global proc_EXPR$69_α
    .global proc_EXPR$69_β
    .global proc_EXPR$69_γ
    .global proc_EXPR$69_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$69_α_body:
# IR_VAR
 xchain341_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain341_n1_α
 xchain341_n0_β:
 jmp proc_EXPR$69_ω
# IR_ASSIGN gva
 xchain341_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1456], rax
 mov qword ptr [rbx + 1464], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$69_γ
 xchain341_n1_β:
 jmp proc_EXPR$69_ω
proc_EXPR$69_β:
jmp proc_EXPR$69_ω
proc_EXPR$69_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$69_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$0_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$0_α_body:
lea rax, [rip + xchain344_n0_β]
mov qword ptr [r12 + 48], rax
# IR_MATCH_SPAN
 xchain344_n0_α:
 mov dword ptr [r12 + 32], 0
.Lx346_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 32]
 cmp eax, r15d
 jge .Lx346_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx346_1
 add dword ptr [r12 + 32], 1
 jmp .Lx346_0
.Lx346_1:
 mov eax, dword ptr [r12 + 32]
 test eax, eax
 jle proc_PAT$0_ω
 mov edx, r14d
 mov dword ptr [r12 + 36], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$0_γ
 xchain344_n0_β:
 xchain344_n0_β:
 mov r14d, dword ptr [r12 + 36]
 jmp proc_PAT$0_ω
proc_PAT$0_β:
jmp qword ptr [r12 + 48]
proc_PAT$0_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$0_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$1_α
proc_PAT$1_α:
#=======================================================================================================================
    .global proc_PAT$1_α
    .global proc_PAT$1_β
    .global proc_PAT$1_γ
    .global proc_PAT$1_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$1_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 192], rax
 pop rsi
proc_PAT$1_α_body:
lea rax, [rip + xchain347_n0_β]
mov qword ptr [r12 + 176], rax
# IR_MATCH_SEQ_NARY
 xchain347_n0_α:
 mov dword ptr [r12 + 32], r14d
 mov dword ptr [r12 + 36], 0
 jmp xchain347_n1_α
xchain347_n0_as:
 mov eax, dword ptr [r12 + 36]
 add eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 1
 je xchain347_n2_α
 cmp eax, 2
 je xchain347_n3_α
 jmp proc_PAT$1_γ
 xchain347_n0_β:
 mov dword ptr [r12 + 36], 3
xchain347_n0_af:
 mov eax, dword ptr [r12 + 36]
 sub eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 0
 je xchain347_n1_β
 cmp eax, 1
 je xchain347_n2_β
 cmp eax, 2
 je xchain347_n3_β
 jmp proc_PAT$1_ω
# IR_MATCH_ANY
 xchain347_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain347_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain347_n0_af
 add r14d, 1
 jmp xchain347_n0_as
 xchain347_n1_β:
 sub r14d, 1
 jmp xchain347_n0_af
# IR_MATCH_ALT_NARY
 xchain347_n2_α:
 mov dword ptr [r12 + 80], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 84], eax
 mov dword ptr [r12 + 88], 0
 jmp xchain347_n4_α
xchain347_n2_as:
 jmp xchain347_n0_as
 xchain347_n2_β:
 mov eax, dword ptr [r12 + 88]
 cmp eax, 0
 je xchain347_n4_β
 jmp xchain347_n5_β
xchain347_n2_af:
 add dword ptr [r12 + 88], 1
 mov r14d, dword ptr [r12 + 80]
 mov edi, dword ptr [r12 + 84]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 88]
 cmp eax, 1
 je xchain347_n5_α
 jmp xchain347_n0_af
# IR_MATCH_DEFER
 xchain347_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 160], rcx
 test rax, rax
 jz .Lx353_0
 mov qword ptr [r12 + 160], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 168], rax
 mov rcx, qword ptr [r12 + 160]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx353_1
 mov rdi, qword ptr [r12 + 168]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 160], rax
 jmp xchain347_n0_af
.Lx353_1:
 jmp xchain347_n0_as
.Lx353_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx353_2:
 test rax, rax
 je .Lx353_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx353_2
.Lx353_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain347_n0_af
 mov r14d, eax
 jmp xchain347_n0_as
 xchain347_n3_β:
 mov rcx, qword ptr [r12 + 160]
 test rcx, rcx
 jz xchain347_n0_af
 mov rdi, qword ptr [r12 + 168]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx353_1
 mov rdi, qword ptr [r12 + 168]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 160], rax
 jmp xchain347_n0_af
# IR_MATCH_ANY
 xchain347_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain347_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain347_n2_af
 add r14d, 1
 jmp xchain347_n2_as
 xchain347_n4_β:
 sub r14d, 1
 jmp xchain347_n2_af
# IR_MATCH_DEFER
 xchain347_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 128], rcx
 test rax, rax
 jz .Lx355_0
 mov qword ptr [r12 + 128], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 136], rax
 mov rcx, qword ptr [r12 + 128]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx355_1
 mov rdi, qword ptr [r12 + 136]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 128], rax
 jmp xchain347_n2_af
.Lx355_1:
 jmp xchain347_n2_as
.Lx355_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx355_2:
 test rax, rax
 je .Lx355_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx355_2
.Lx355_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain347_n2_af
 mov r14d, eax
 jmp xchain347_n2_as
 xchain347_n5_β:
 mov rcx, qword ptr [r12 + 128]
 test rcx, rcx
 jz xchain347_n2_af
 mov rdi, qword ptr [r12 + 136]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx355_1
 mov rdi, qword ptr [r12 + 136]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 128], rax
 jmp xchain347_n2_af
proc_PAT$1_β:
jmp qword ptr [r12 + 176]
proc_PAT$1_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 192]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$1_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$2_α
proc_PAT$2_α:
#=======================================================================================================================
    .global proc_PAT$2_α
    .global proc_PAT$2_β
    .global proc_PAT$2_γ
    .global proc_PAT$2_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$2_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 432], rax
 pop rsi
proc_PAT$2_α_body:
lea rax, [rip + xchain356_n0_β]
mov qword ptr [r12 + 416], rax
# IR_MATCH_ALT_NARY
 xchain356_n0_α:
 mov dword ptr [r12 + 32], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 36], eax
 mov dword ptr [r12 + 40], 0
 jmp xchain356_n1_α
xchain356_n0_as:
 jmp proc_PAT$2_γ
 xchain356_n0_β:
 mov eax, dword ptr [r12 + 40]
 cmp eax, 0
 je xchain356_n1_β
 jmp xchain356_n2_β
xchain356_n0_af:
 add dword ptr [r12 + 40], 1
 mov r14d, dword ptr [r12 + 32]
 mov edi, dword ptr [r12 + 36]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 40]
 cmp eax, 1
 je xchain356_n2_α
 jmp proc_PAT$2_ω
# IR_MATCH_SEQ_NARY
 xchain356_n1_α:
 mov dword ptr [r12 + 64], r14d
 mov dword ptr [r12 + 68], 0
 jmp xchain356_n3_α
xchain356_n1_as:
 mov eax, dword ptr [r12 + 68]
 add eax, 1
 mov dword ptr [r12 + 68], eax
 cmp eax, 1
 je xchain356_n4_α
 cmp eax, 2
 je xchain356_n5_α
 cmp eax, 3
 je xchain356_n6_α
 jmp xchain356_n0_as
 xchain356_n1_β:
 mov dword ptr [r12 + 68], 4
xchain356_n1_af:
 mov eax, dword ptr [r12 + 68]
 sub eax, 1
 mov dword ptr [r12 + 68], eax
 cmp eax, 0
 je xchain356_n3_β
 cmp eax, 1
 je xchain356_n4_β
 cmp eax, 2
 je xchain356_n5_β
 cmp eax, 3
 je xchain356_n6_β
 jmp xchain356_n0_af
# IR_MATCH_SEQ_NARY
 xchain356_n2_α:
 mov dword ptr [r12 + 336], r14d
 mov dword ptr [r12 + 340], 0
 jmp xchain356_n7_α
xchain356_n2_as:
 mov eax, dword ptr [r12 + 340]
 add eax, 1
 mov dword ptr [r12 + 340], eax
 cmp eax, 1
 je xchain356_n8_α
 jmp xchain356_n0_as
 xchain356_n2_β:
 mov dword ptr [r12 + 340], 2
xchain356_n2_af:
 mov eax, dword ptr [r12 + 340]
 sub eax, 1
 mov dword ptr [r12 + 340], eax
 cmp eax, 0
 je xchain356_n7_β
 cmp eax, 1
 je xchain356_n8_β
 jmp xchain356_n0_af
# IR_MATCH_DEFER
 xchain356_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 96], rcx
 test rax, rax
 jz .Lx363_0
 mov qword ptr [r12 + 96], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 104], rax
 mov rcx, qword ptr [r12 + 96]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx363_1
 mov rdi, qword ptr [r12 + 104]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 96], rax
 jmp xchain356_n1_af
.Lx363_1:
 jmp xchain356_n1_as
.Lx363_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx363_2:
 test rax, rax
 je .Lx363_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx363_2
.Lx363_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain356_n1_af
 mov r14d, eax
 jmp xchain356_n1_as
 xchain356_n3_β:
 mov rcx, qword ptr [r12 + 96]
 test rcx, rcx
 jz xchain356_n1_af
 mov rdi, qword ptr [r12 + 104]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx363_1
 mov rdi, qword ptr [r12 + 104]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 96], rax
 jmp xchain356_n1_af
# IR_MATCH_LIT
 xchain356_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain356_n1_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain356_n1_af
 add r14d, 1
 jmp xchain356_n1_as
 xchain356_n4_β:
 sub r14d, 1
 jmp xchain356_n1_af
# IR_MATCH_ALT_NARY
 xchain356_n5_α:
 mov dword ptr [r12 + 144], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 148], eax
 mov dword ptr [r12 + 152], 0
 jmp xchain356_n9_α
xchain356_n5_as:
 jmp xchain356_n1_as
 xchain356_n5_β:
 mov eax, dword ptr [r12 + 152]
 cmp eax, 0
 je xchain356_n9_β
 jmp xchain356_n10_β
xchain356_n5_af:
 add dword ptr [r12 + 152], 1
 mov r14d, dword ptr [r12 + 144]
 mov edi, dword ptr [r12 + 148]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 152]
 cmp eax, 1
 je xchain356_n10_α
 jmp xchain356_n1_af
# IR_MATCH_ALT_NARY
 xchain356_n6_α:
 mov dword ptr [r12 + 240], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 244], eax
 mov dword ptr [r12 + 248], 0
 jmp xchain356_n11_α
xchain356_n6_as:
 jmp xchain356_n1_as
 xchain356_n6_β:
 mov eax, dword ptr [r12 + 248]
 cmp eax, 0
 je xchain356_n11_β
 jmp xchain356_n12_β
xchain356_n6_af:
 add dword ptr [r12 + 248], 1
 mov r14d, dword ptr [r12 + 240]
 mov edi, dword ptr [r12 + 244]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 248]
 cmp eax, 1
 je xchain356_n12_α
 jmp xchain356_n1_af
# IR_MATCH_DEFER
 xchain356_n7_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 368], rcx
 test rax, rax
 jz .Lx370_0
 mov qword ptr [r12 + 368], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 376], rax
 mov rcx, qword ptr [r12 + 368]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx370_1
 mov rdi, qword ptr [r12 + 376]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain356_n2_af
.Lx370_1:
 jmp xchain356_n2_as
.Lx370_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx370_2:
 test rax, rax
 je .Lx370_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx370_2
.Lx370_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain356_n2_af
 mov r14d, eax
 jmp xchain356_n2_as
 xchain356_n7_β:
 mov rcx, qword ptr [r12 + 368]
 test rcx, rcx
 jz xchain356_n2_af
 mov rdi, qword ptr [r12 + 376]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx370_1
 mov rdi, qword ptr [r12 + 376]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain356_n2_af
# IR_MATCH_DEFER
 xchain356_n8_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 400], rcx
 test rax, rax
 jz .Lx371_0
 mov qword ptr [r12 + 400], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 408], rax
 mov rcx, qword ptr [r12 + 400]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx371_1
 mov rdi, qword ptr [r12 + 408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 400], rax
 jmp xchain356_n2_af
.Lx371_1:
 jmp xchain356_n2_as
.Lx371_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx371_2:
 test rax, rax
 je .Lx371_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx371_2
.Lx371_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain356_n2_af
 mov r14d, eax
 jmp xchain356_n2_as
 xchain356_n8_β:
 mov rcx, qword ptr [r12 + 400]
 test rcx, rcx
 jz xchain356_n2_af
 mov rdi, qword ptr [r12 + 408]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx371_1
 mov rdi, qword ptr [r12 + 408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 400], rax
 jmp xchain356_n2_af
# IR_MATCH_DEFER
 xchain356_n9_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 176], rcx
 test rax, rax
 jz .Lx372_0
 mov qword ptr [r12 + 176], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 184], rax
 mov rcx, qword ptr [r12 + 176]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx372_1
 mov rdi, qword ptr [r12 + 184]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 176], rax
 jmp xchain356_n5_af
.Lx372_1:
 jmp xchain356_n5_as
.Lx372_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx372_2:
 test rax, rax
 je .Lx372_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx372_2
.Lx372_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain356_n5_af
 mov r14d, eax
 jmp xchain356_n5_as
 xchain356_n9_β:
 mov rcx, qword ptr [r12 + 176]
 test rcx, rcx
 jz xchain356_n5_af
 mov rdi, qword ptr [r12 + 184]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx372_1
 mov rdi, qword ptr [r12 + 184]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 176], rax
 jmp xchain356_n5_af
# IR_MATCH_DEFER
 xchain356_n10_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 208], rcx
 test rax, rax
 jz .Lx373_0
 mov qword ptr [r12 + 208], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 216], rax
 mov rcx, qword ptr [r12 + 208]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx373_1
 mov rdi, qword ptr [r12 + 216]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 208], rax
 jmp xchain356_n5_af
.Lx373_1:
 jmp xchain356_n5_as
.Lx373_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx373_2:
 test rax, rax
 je .Lx373_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx373_2
.Lx373_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain356_n5_af
 mov r14d, eax
 jmp xchain356_n5_as
 xchain356_n10_β:
 mov rcx, qword ptr [r12 + 208]
 test rcx, rcx
 jz xchain356_n5_af
 mov rdi, qword ptr [r12 + 216]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx373_1
 mov rdi, qword ptr [r12 + 216]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 208], rax
 jmp xchain356_n5_af
# IR_MATCH_DEFER
 xchain356_n11_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 272], rcx
 test rax, rax
 jz .Lx374_0
 mov qword ptr [r12 + 272], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 280], rax
 mov rcx, qword ptr [r12 + 272]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx374_1
 mov rdi, qword ptr [r12 + 280]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 272], rax
 jmp xchain356_n6_af
.Lx374_1:
 jmp xchain356_n6_as
.Lx374_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx374_2:
 test rax, rax
 je .Lx374_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx374_2
.Lx374_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain356_n6_af
 mov r14d, eax
 jmp xchain356_n6_as
 xchain356_n11_β:
 mov rcx, qword ptr [r12 + 272]
 test rcx, rcx
 jz xchain356_n6_af
 mov rdi, qword ptr [r12 + 280]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx374_1
 mov rdi, qword ptr [r12 + 280]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 272], rax
 jmp xchain356_n6_af
# IR_MATCH_DEFER
 xchain356_n12_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 304], rcx
 test rax, rax
 jz .Lx375_0
 mov qword ptr [r12 + 304], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 312], rax
 mov rcx, qword ptr [r12 + 304]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx375_1
 mov rdi, qword ptr [r12 + 312]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 304], rax
 jmp xchain356_n6_af
.Lx375_1:
 jmp xchain356_n6_as
.Lx375_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx375_2:
 test rax, rax
 je .Lx375_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx375_2
.Lx375_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain356_n6_af
 mov r14d, eax
 jmp xchain356_n6_as
 xchain356_n12_β:
 mov rcx, qword ptr [r12 + 304]
 test rcx, rcx
 jz xchain356_n6_af
 mov rdi, qword ptr [r12 + 312]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx375_1
 mov rdi, qword ptr [r12 + 312]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 304], rax
 jmp xchain356_n6_af
proc_PAT$2_β:
jmp qword ptr [r12 + 416]
proc_PAT$2_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 432]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$2_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$3_α
proc_PAT$3_α:
#=======================================================================================================================
    .global proc_PAT$3_α
    .global proc_PAT$3_β
    .global proc_PAT$3_γ
    .global proc_PAT$3_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$3_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 192], rax
 pop rsi
proc_PAT$3_α_body:
lea rax, [rip + xchain376_n0_β]
mov qword ptr [r12 + 176], rax
# IR_MATCH_ALT_NARY
 xchain376_n0_α:
 mov dword ptr [r12 + 32], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 36], eax
 mov dword ptr [r12 + 40], 0
 jmp xchain376_n1_α
xchain376_n0_as:
 jmp proc_PAT$3_γ
 xchain376_n0_β:
 mov eax, dword ptr [r12 + 40]
 cmp eax, 0
 je xchain376_n1_β
 jmp xchain376_n2_β
xchain376_n0_af:
 add dword ptr [r12 + 40], 1
 mov r14d, dword ptr [r12 + 32]
 mov edi, dword ptr [r12 + 36]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 40]
 cmp eax, 1
 je xchain376_n2_α
 jmp proc_PAT$3_ω
# IR_MATCH_DEFER
 xchain376_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 64], rcx
 test rax, rax
 jz .Lx379_0
 mov qword ptr [r12 + 64], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 72], rax
 mov rcx, qword ptr [r12 + 64]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx379_1
 mov rdi, qword ptr [r12 + 72]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 64], rax
 jmp xchain376_n0_af
.Lx379_1:
 jmp xchain376_n0_as
.Lx379_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx379_2:
 test rax, rax
 je .Lx379_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx379_2
.Lx379_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain376_n0_af
 mov r14d, eax
 jmp xchain376_n0_as
 xchain376_n1_β:
 mov rcx, qword ptr [r12 + 64]
 test rcx, rcx
 jz xchain376_n0_af
 mov rdi, qword ptr [r12 + 72]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx379_1
 mov rdi, qword ptr [r12 + 72]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 64], rax
 jmp xchain376_n0_af
# IR_MATCH_SEQ_NARY
 xchain376_n2_α:
 mov dword ptr [r12 + 96], r14d
 mov dword ptr [r12 + 100], 0
 jmp xchain376_n3_α
xchain376_n2_as:
 mov eax, dword ptr [r12 + 100]
 add eax, 1
 mov dword ptr [r12 + 100], eax
 cmp eax, 1
 je xchain376_n4_α
 cmp eax, 2
 je xchain376_n5_α
 jmp xchain376_n0_as
 xchain376_n2_β:
 mov dword ptr [r12 + 100], 3
xchain376_n2_af:
 mov eax, dword ptr [r12 + 100]
 sub eax, 1
 mov dword ptr [r12 + 100], eax
 cmp eax, 0
 je xchain376_n3_β
 cmp eax, 1
 je xchain376_n4_β
 cmp eax, 2
 je xchain376_n5_β
 jmp xchain376_n0_af
# IR_MATCH_LIT
 xchain376_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain376_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain376_n2_af
 add r14d, 1
 jmp xchain376_n2_as
 xchain376_n3_β:
 sub r14d, 1
 jmp xchain376_n2_af
# IR_MATCH_DEFER
 xchain376_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 144], rcx
 test rax, rax
 jz .Lx384_0
 mov qword ptr [r12 + 144], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 152], rax
 mov rcx, qword ptr [r12 + 144]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx384_1
 mov rdi, qword ptr [r12 + 152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 144], rax
 jmp xchain376_n2_af
.Lx384_1:
 jmp xchain376_n2_as
.Lx384_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx384_2:
 test rax, rax
 je .Lx384_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx384_2
.Lx384_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain376_n2_af
 mov r14d, eax
 jmp xchain376_n2_as
 xchain376_n4_β:
 mov rcx, qword ptr [r12 + 144]
 test rcx, rcx
 jz xchain376_n2_af
 mov rdi, qword ptr [r12 + 152]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx384_1
 mov rdi, qword ptr [r12 + 152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 144], rax
 jmp xchain376_n2_af
# IR_MATCH_LIT
 xchain376_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain376_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain376_n2_af
 add r14d, 1
 jmp xchain376_n2_as
 xchain376_n5_β:
 sub r14d, 1
 jmp xchain376_n2_af
proc_PAT$3_β:
jmp qword ptr [r12 + 176]
proc_PAT$3_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 192]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$3_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$4_α
proc_PAT$4_α:
#=======================================================================================================================
    .global proc_PAT$4_α
    .global proc_PAT$4_β
    .global proc_PAT$4_γ
    .global proc_PAT$4_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$4_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$4_α_body:
lea rax, [rip + xchain387_n0_β]
mov qword ptr [r12 + 48], rax
# IR_MATCH_SPAN
 xchain387_n0_α:
 mov dword ptr [r12 + 32], 0
.Lx389_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 32]
 cmp eax, r15d
 jge .Lx389_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx389_1
 add dword ptr [r12 + 32], 1
 jmp .Lx389_0
.Lx389_1:
 mov eax, dword ptr [r12 + 32]
 test eax, eax
 jle proc_PAT$4_ω
 mov edx, r14d
 mov dword ptr [r12 + 36], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$4_γ
 xchain387_n0_β:
 xchain387_n0_β:
 mov r14d, dword ptr [r12 + 36]
 jmp proc_PAT$4_ω
proc_PAT$4_β:
jmp qword ptr [r12 + 48]
proc_PAT$4_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$4_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$5_α
proc_PAT$5_α:
#=======================================================================================================================
    .global proc_PAT$5_α
    .global proc_PAT$5_β
    .global proc_PAT$5_γ
    .global proc_PAT$5_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$5_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 192], rax
 pop rsi
proc_PAT$5_α_body:
lea rax, [rip + xchain390_n0_β]
mov qword ptr [r12 + 176], rax
# IR_MATCH_SEQ_NARY
 xchain390_n0_α:
 mov dword ptr [r12 + 32], r14d
 mov dword ptr [r12 + 36], 0
 jmp xchain390_n1_α
xchain390_n0_as:
 mov eax, dword ptr [r12 + 36]
 add eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 1
 je xchain390_n2_α
 cmp eax, 2
 je xchain390_n3_α
 jmp proc_PAT$5_γ
 xchain390_n0_β:
 mov dword ptr [r12 + 36], 3
xchain390_n0_af:
 mov eax, dword ptr [r12 + 36]
 sub eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 0
 je xchain390_n1_β
 cmp eax, 1
 je xchain390_n2_β
 cmp eax, 2
 je xchain390_n3_β
 jmp proc_PAT$5_ω
# IR_MATCH_ANY
 xchain390_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain390_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain390_n0_af
 add r14d, 1
 jmp xchain390_n0_as
 xchain390_n1_β:
 sub r14d, 1
 jmp xchain390_n0_af
# IR_MATCH_ALT_NARY
 xchain390_n2_α:
 mov dword ptr [r12 + 80], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 84], eax
 mov dword ptr [r12 + 88], 0
 jmp xchain390_n4_α
xchain390_n2_as:
 jmp xchain390_n0_as
 xchain390_n2_β:
 mov eax, dword ptr [r12 + 88]
 cmp eax, 0
 je xchain390_n4_β
 jmp xchain390_n5_β
xchain390_n2_af:
 add dword ptr [r12 + 88], 1
 mov r14d, dword ptr [r12 + 80]
 mov edi, dword ptr [r12 + 84]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 88]
 cmp eax, 1
 je xchain390_n5_α
 jmp xchain390_n0_af
# IR_MATCH_DEFER
 xchain390_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 160], rcx
 test rax, rax
 jz .Lx396_0
 mov qword ptr [r12 + 160], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 168], rax
 mov rcx, qword ptr [r12 + 160]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx396_1
 mov rdi, qword ptr [r12 + 168]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 160], rax
 jmp xchain390_n0_af
.Lx396_1:
 jmp xchain390_n0_as
.Lx396_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx396_2:
 test rax, rax
 je .Lx396_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx396_2
.Lx396_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain390_n0_af
 mov r14d, eax
 jmp xchain390_n0_as
 xchain390_n3_β:
 mov rcx, qword ptr [r12 + 160]
 test rcx, rcx
 jz xchain390_n0_af
 mov rdi, qword ptr [r12 + 168]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx396_1
 mov rdi, qword ptr [r12 + 168]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 160], rax
 jmp xchain390_n0_af
# IR_MATCH_ANY
 xchain390_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain390_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain390_n2_af
 add r14d, 1
 jmp xchain390_n2_as
 xchain390_n4_β:
 sub r14d, 1
 jmp xchain390_n2_af
# IR_MATCH_DEFER
 xchain390_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 128], rcx
 test rax, rax
 jz .Lx398_0
 mov qword ptr [r12 + 128], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 136], rax
 mov rcx, qword ptr [r12 + 128]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx398_1
 mov rdi, qword ptr [r12 + 136]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 128], rax
 jmp xchain390_n2_af
.Lx398_1:
 jmp xchain390_n2_as
.Lx398_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx398_2:
 test rax, rax
 je .Lx398_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx398_2
.Lx398_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain390_n2_af
 mov r14d, eax
 jmp xchain390_n2_as
 xchain390_n5_β:
 mov rcx, qword ptr [r12 + 128]
 test rcx, rcx
 jz xchain390_n2_af
 mov rdi, qword ptr [r12 + 136]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx398_1
 mov rdi, qword ptr [r12 + 136]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 128], rax
 jmp xchain390_n2_af
proc_PAT$5_β:
jmp qword ptr [r12 + 176]
proc_PAT$5_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 192]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$5_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$6_α
proc_PAT$6_α:
#=======================================================================================================================
    .global proc_PAT$6_α
    .global proc_PAT$6_β
    .global proc_PAT$6_γ
    .global proc_PAT$6_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$6_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 432], rax
 pop rsi
proc_PAT$6_α_body:
lea rax, [rip + xchain399_n0_β]
mov qword ptr [r12 + 416], rax
# IR_MATCH_ALT_NARY
 xchain399_n0_α:
 mov dword ptr [r12 + 32], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 36], eax
 mov dword ptr [r12 + 40], 0
 jmp xchain399_n1_α
xchain399_n0_as:
 jmp proc_PAT$6_γ
 xchain399_n0_β:
 mov eax, dword ptr [r12 + 40]
 cmp eax, 0
 je xchain399_n1_β
 jmp xchain399_n2_β
xchain399_n0_af:
 add dword ptr [r12 + 40], 1
 mov r14d, dword ptr [r12 + 32]
 mov edi, dword ptr [r12 + 36]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 40]
 cmp eax, 1
 je xchain399_n2_α
 jmp proc_PAT$6_ω
# IR_MATCH_SEQ_NARY
 xchain399_n1_α:
 mov dword ptr [r12 + 64], r14d
 mov dword ptr [r12 + 68], 0
 jmp xchain399_n3_α
xchain399_n1_as:
 mov eax, dword ptr [r12 + 68]
 add eax, 1
 mov dword ptr [r12 + 68], eax
 cmp eax, 1
 je xchain399_n4_α
 cmp eax, 2
 je xchain399_n5_α
 cmp eax, 3
 je xchain399_n6_α
 jmp xchain399_n0_as
 xchain399_n1_β:
 mov dword ptr [r12 + 68], 4
xchain399_n1_af:
 mov eax, dword ptr [r12 + 68]
 sub eax, 1
 mov dword ptr [r12 + 68], eax
 cmp eax, 0
 je xchain399_n3_β
 cmp eax, 1
 je xchain399_n4_β
 cmp eax, 2
 je xchain399_n5_β
 cmp eax, 3
 je xchain399_n6_β
 jmp xchain399_n0_af
# IR_MATCH_SEQ_NARY
 xchain399_n2_α:
 mov dword ptr [r12 + 336], r14d
 mov dword ptr [r12 + 340], 0
 jmp xchain399_n7_α
xchain399_n2_as:
 mov eax, dword ptr [r12 + 340]
 add eax, 1
 mov dword ptr [r12 + 340], eax
 cmp eax, 1
 je xchain399_n8_α
 jmp xchain399_n0_as
 xchain399_n2_β:
 mov dword ptr [r12 + 340], 2
xchain399_n2_af:
 mov eax, dword ptr [r12 + 340]
 sub eax, 1
 mov dword ptr [r12 + 340], eax
 cmp eax, 0
 je xchain399_n7_β
 cmp eax, 1
 je xchain399_n8_β
 jmp xchain399_n0_af
# IR_MATCH_DEFER
 xchain399_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 96], rcx
 test rax, rax
 jz .Lx406_0
 mov qword ptr [r12 + 96], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 104], rax
 mov rcx, qword ptr [r12 + 96]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx406_1
 mov rdi, qword ptr [r12 + 104]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 96], rax
 jmp xchain399_n1_af
.Lx406_1:
 jmp xchain399_n1_as
.Lx406_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx406_2:
 test rax, rax
 je .Lx406_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx406_2
.Lx406_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain399_n1_af
 mov r14d, eax
 jmp xchain399_n1_as
 xchain399_n3_β:
 mov rcx, qword ptr [r12 + 96]
 test rcx, rcx
 jz xchain399_n1_af
 mov rdi, qword ptr [r12 + 104]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx406_1
 mov rdi, qword ptr [r12 + 104]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 96], rax
 jmp xchain399_n1_af
# IR_MATCH_LIT
 xchain399_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain399_n1_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain399_n1_af
 add r14d, 1
 jmp xchain399_n1_as
 xchain399_n4_β:
 sub r14d, 1
 jmp xchain399_n1_af
# IR_MATCH_ALT_NARY
 xchain399_n5_α:
 mov dword ptr [r12 + 144], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 148], eax
 mov dword ptr [r12 + 152], 0
 jmp xchain399_n9_α
xchain399_n5_as:
 jmp xchain399_n1_as
 xchain399_n5_β:
 mov eax, dword ptr [r12 + 152]
 cmp eax, 0
 je xchain399_n9_β
 jmp xchain399_n10_β
xchain399_n5_af:
 add dword ptr [r12 + 152], 1
 mov r14d, dword ptr [r12 + 144]
 mov edi, dword ptr [r12 + 148]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 152]
 cmp eax, 1
 je xchain399_n10_α
 jmp xchain399_n1_af
# IR_MATCH_ALT_NARY
 xchain399_n6_α:
 mov dword ptr [r12 + 240], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 244], eax
 mov dword ptr [r12 + 248], 0
 jmp xchain399_n11_α
xchain399_n6_as:
 jmp xchain399_n1_as
 xchain399_n6_β:
 mov eax, dword ptr [r12 + 248]
 cmp eax, 0
 je xchain399_n11_β
 jmp xchain399_n12_β
xchain399_n6_af:
 add dword ptr [r12 + 248], 1
 mov r14d, dword ptr [r12 + 240]
 mov edi, dword ptr [r12 + 244]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 248]
 cmp eax, 1
 je xchain399_n12_α
 jmp xchain399_n1_af
# IR_MATCH_DEFER
 xchain399_n7_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 368], rcx
 test rax, rax
 jz .Lx413_0
 mov qword ptr [r12 + 368], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 376], rax
 mov rcx, qword ptr [r12 + 368]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx413_1
 mov rdi, qword ptr [r12 + 376]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain399_n2_af
.Lx413_1:
 jmp xchain399_n2_as
.Lx413_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx413_2:
 test rax, rax
 je .Lx413_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx413_2
.Lx413_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain399_n2_af
 mov r14d, eax
 jmp xchain399_n2_as
 xchain399_n7_β:
 mov rcx, qword ptr [r12 + 368]
 test rcx, rcx
 jz xchain399_n2_af
 mov rdi, qword ptr [r12 + 376]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx413_1
 mov rdi, qword ptr [r12 + 376]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain399_n2_af
# IR_MATCH_DEFER
 xchain399_n8_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 400], rcx
 test rax, rax
 jz .Lx414_0
 mov qword ptr [r12 + 400], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 408], rax
 mov rcx, qword ptr [r12 + 400]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx414_1
 mov rdi, qword ptr [r12 + 408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 400], rax
 jmp xchain399_n2_af
.Lx414_1:
 jmp xchain399_n2_as
.Lx414_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx414_2:
 test rax, rax
 je .Lx414_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx414_2
.Lx414_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain399_n2_af
 mov r14d, eax
 jmp xchain399_n2_as
 xchain399_n8_β:
 mov rcx, qword ptr [r12 + 400]
 test rcx, rcx
 jz xchain399_n2_af
 mov rdi, qword ptr [r12 + 408]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx414_1
 mov rdi, qword ptr [r12 + 408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 400], rax
 jmp xchain399_n2_af
# IR_MATCH_DEFER
 xchain399_n9_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 176], rcx
 test rax, rax
 jz .Lx415_0
 mov qword ptr [r12 + 176], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 184], rax
 mov rcx, qword ptr [r12 + 176]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx415_1
 mov rdi, qword ptr [r12 + 184]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 176], rax
 jmp xchain399_n5_af
.Lx415_1:
 jmp xchain399_n5_as
.Lx415_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx415_2:
 test rax, rax
 je .Lx415_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx415_2
.Lx415_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain399_n5_af
 mov r14d, eax
 jmp xchain399_n5_as
 xchain399_n9_β:
 mov rcx, qword ptr [r12 + 176]
 test rcx, rcx
 jz xchain399_n5_af
 mov rdi, qword ptr [r12 + 184]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx415_1
 mov rdi, qword ptr [r12 + 184]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 176], rax
 jmp xchain399_n5_af
# IR_MATCH_DEFER
 xchain399_n10_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 208], rcx
 test rax, rax
 jz .Lx416_0
 mov qword ptr [r12 + 208], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 216], rax
 mov rcx, qword ptr [r12 + 208]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx416_1
 mov rdi, qword ptr [r12 + 216]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 208], rax
 jmp xchain399_n5_af
.Lx416_1:
 jmp xchain399_n5_as
.Lx416_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx416_2:
 test rax, rax
 je .Lx416_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx416_2
.Lx416_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain399_n5_af
 mov r14d, eax
 jmp xchain399_n5_as
 xchain399_n10_β:
 mov rcx, qword ptr [r12 + 208]
 test rcx, rcx
 jz xchain399_n5_af
 mov rdi, qword ptr [r12 + 216]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx416_1
 mov rdi, qword ptr [r12 + 216]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 208], rax
 jmp xchain399_n5_af
# IR_MATCH_DEFER
 xchain399_n11_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 272], rcx
 test rax, rax
 jz .Lx417_0
 mov qword ptr [r12 + 272], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 280], rax
 mov rcx, qword ptr [r12 + 272]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx417_1
 mov rdi, qword ptr [r12 + 280]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 272], rax
 jmp xchain399_n6_af
.Lx417_1:
 jmp xchain399_n6_as
.Lx417_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx417_2:
 test rax, rax
 je .Lx417_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx417_2
.Lx417_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain399_n6_af
 mov r14d, eax
 jmp xchain399_n6_as
 xchain399_n11_β:
 mov rcx, qword ptr [r12 + 272]
 test rcx, rcx
 jz xchain399_n6_af
 mov rdi, qword ptr [r12 + 280]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx417_1
 mov rdi, qword ptr [r12 + 280]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 272], rax
 jmp xchain399_n6_af
# IR_MATCH_DEFER
 xchain399_n12_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 304], rcx
 test rax, rax
 jz .Lx418_0
 mov qword ptr [r12 + 304], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 312], rax
 mov rcx, qword ptr [r12 + 304]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx418_1
 mov rdi, qword ptr [r12 + 312]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 304], rax
 jmp xchain399_n6_af
.Lx418_1:
 jmp xchain399_n6_as
.Lx418_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx418_2:
 test rax, rax
 je .Lx418_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx418_2
.Lx418_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain399_n6_af
 mov r14d, eax
 jmp xchain399_n6_as
 xchain399_n12_β:
 mov rcx, qword ptr [r12 + 304]
 test rcx, rcx
 jz xchain399_n6_af
 mov rdi, qword ptr [r12 + 312]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx418_1
 mov rdi, qword ptr [r12 + 312]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 304], rax
 jmp xchain399_n6_af
proc_PAT$6_β:
jmp qword ptr [r12 + 416]
proc_PAT$6_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 432]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$6_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$7_α
proc_PAT$7_α:
#=======================================================================================================================
    .global proc_PAT$7_α
    .global proc_PAT$7_β
    .global proc_PAT$7_γ
    .global proc_PAT$7_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$7_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 192], rax
 pop rsi
proc_PAT$7_α_body:
lea rax, [rip + xchain419_n0_β]
mov qword ptr [r12 + 176], rax
# IR_MATCH_ALT_NARY
 xchain419_n0_α:
 mov dword ptr [r12 + 32], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 36], eax
 mov dword ptr [r12 + 40], 0
 jmp xchain419_n1_α
xchain419_n0_as:
 jmp proc_PAT$7_γ
 xchain419_n0_β:
 mov eax, dword ptr [r12 + 40]
 cmp eax, 0
 je xchain419_n1_β
 jmp xchain419_n2_β
xchain419_n0_af:
 add dword ptr [r12 + 40], 1
 mov r14d, dword ptr [r12 + 32]
 mov edi, dword ptr [r12 + 36]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 40]
 cmp eax, 1
 je xchain419_n2_α
 jmp proc_PAT$7_ω
# IR_MATCH_DEFER
 xchain419_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 64], rcx
 test rax, rax
 jz .Lx422_0
 mov qword ptr [r12 + 64], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 72], rax
 mov rcx, qword ptr [r12 + 64]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx422_1
 mov rdi, qword ptr [r12 + 72]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 64], rax
 jmp xchain419_n0_af
.Lx422_1:
 jmp xchain419_n0_as
.Lx422_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx422_2:
 test rax, rax
 je .Lx422_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx422_2
.Lx422_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain419_n0_af
 mov r14d, eax
 jmp xchain419_n0_as
 xchain419_n1_β:
 mov rcx, qword ptr [r12 + 64]
 test rcx, rcx
 jz xchain419_n0_af
 mov rdi, qword ptr [r12 + 72]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx422_1
 mov rdi, qword ptr [r12 + 72]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 64], rax
 jmp xchain419_n0_af
# IR_MATCH_SEQ_NARY
 xchain419_n2_α:
 mov dword ptr [r12 + 96], r14d
 mov dword ptr [r12 + 100], 0
 jmp xchain419_n3_α
xchain419_n2_as:
 mov eax, dword ptr [r12 + 100]
 add eax, 1
 mov dword ptr [r12 + 100], eax
 cmp eax, 1
 je xchain419_n4_α
 cmp eax, 2
 je xchain419_n5_α
 jmp xchain419_n0_as
 xchain419_n2_β:
 mov dword ptr [r12 + 100], 3
xchain419_n2_af:
 mov eax, dword ptr [r12 + 100]
 sub eax, 1
 mov dword ptr [r12 + 100], eax
 cmp eax, 0
 je xchain419_n3_β
 cmp eax, 1
 je xchain419_n4_β
 cmp eax, 2
 je xchain419_n5_β
 jmp xchain419_n0_af
# IR_MATCH_LIT
 xchain419_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain419_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain419_n2_af
 add r14d, 1
 jmp xchain419_n2_as
 xchain419_n3_β:
 sub r14d, 1
 jmp xchain419_n2_af
# IR_MATCH_DEFER
 xchain419_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 144], rcx
 test rax, rax
 jz .Lx427_0
 mov qword ptr [r12 + 144], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 152], rax
 mov rcx, qword ptr [r12 + 144]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx427_1
 mov rdi, qword ptr [r12 + 152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 144], rax
 jmp xchain419_n2_af
.Lx427_1:
 jmp xchain419_n2_as
.Lx427_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx427_2:
 test rax, rax
 je .Lx427_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx427_2
.Lx427_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain419_n2_af
 mov r14d, eax
 jmp xchain419_n2_as
 xchain419_n4_β:
 mov rcx, qword ptr [r12 + 144]
 test rcx, rcx
 jz xchain419_n2_af
 mov rdi, qword ptr [r12 + 152]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx427_1
 mov rdi, qword ptr [r12 + 152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 144], rax
 jmp xchain419_n2_af
# IR_MATCH_LIT
 xchain419_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain419_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain419_n2_af
 add r14d, 1
 jmp xchain419_n2_as
 xchain419_n5_β:
 sub r14d, 1
 jmp xchain419_n2_af
proc_PAT$7_β:
jmp qword ptr [r12 + 176]
proc_PAT$7_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 192]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$7_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$8_α
proc_PAT$8_α:
#=======================================================================================================================
    .global proc_PAT$8_α
    .global proc_PAT$8_β
    .global proc_PAT$8_γ
    .global proc_PAT$8_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$8_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$8_α_body:
lea rax, [rip + xchain430_n0_β]
mov qword ptr [r12 + 48], rax
# IR_MATCH_SPAN
 xchain430_n0_α:
 mov dword ptr [r12 + 32], 0
.Lx432_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 32]
 cmp eax, r15d
 jge .Lx432_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx432_1
 add dword ptr [r12 + 32], 1
 jmp .Lx432_0
.Lx432_1:
 mov eax, dword ptr [r12 + 32]
 test eax, eax
 jle proc_PAT$8_ω
 mov edx, r14d
 mov dword ptr [r12 + 36], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$8_γ
 xchain430_n0_β:
 xchain430_n0_β:
 mov r14d, dword ptr [r12 + 36]
 jmp proc_PAT$8_ω
proc_PAT$8_β:
jmp qword ptr [r12 + 48]
proc_PAT$8_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$8_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$9_α
proc_PAT$9_α:
#=======================================================================================================================
    .global proc_PAT$9_α
    .global proc_PAT$9_β
    .global proc_PAT$9_γ
    .global proc_PAT$9_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$9_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 192], rax
 pop rsi
proc_PAT$9_α_body:
lea rax, [rip + xchain433_n0_β]
mov qword ptr [r12 + 176], rax
# IR_MATCH_SEQ_NARY
 xchain433_n0_α:
 mov dword ptr [r12 + 32], r14d
 mov dword ptr [r12 + 36], 0
 jmp xchain433_n1_α
xchain433_n0_as:
 mov eax, dword ptr [r12 + 36]
 add eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 1
 je xchain433_n2_α
 cmp eax, 2
 je xchain433_n3_α
 jmp proc_PAT$9_γ
 xchain433_n0_β:
 mov dword ptr [r12 + 36], 3
xchain433_n0_af:
 mov eax, dword ptr [r12 + 36]
 sub eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 0
 je xchain433_n1_β
 cmp eax, 1
 je xchain433_n2_β
 cmp eax, 2
 je xchain433_n3_β
 jmp proc_PAT$9_ω
# IR_MATCH_ANY
 xchain433_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain433_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain433_n0_af
 add r14d, 1
 jmp xchain433_n0_as
 xchain433_n1_β:
 sub r14d, 1
 jmp xchain433_n0_af
# IR_MATCH_ALT_NARY
 xchain433_n2_α:
 mov dword ptr [r12 + 80], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 84], eax
 mov dword ptr [r12 + 88], 0
 jmp xchain433_n4_α
xchain433_n2_as:
 jmp xchain433_n0_as
 xchain433_n2_β:
 mov eax, dword ptr [r12 + 88]
 cmp eax, 0
 je xchain433_n4_β
 jmp xchain433_n5_β
xchain433_n2_af:
 add dword ptr [r12 + 88], 1
 mov r14d, dword ptr [r12 + 80]
 mov edi, dword ptr [r12 + 84]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 88]
 cmp eax, 1
 je xchain433_n5_α
 jmp xchain433_n0_af
# IR_MATCH_DEFER
 xchain433_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 160], rcx
 test rax, rax
 jz .Lx439_0
 mov qword ptr [r12 + 160], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 168], rax
 mov rcx, qword ptr [r12 + 160]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx439_1
 mov rdi, qword ptr [r12 + 168]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 160], rax
 jmp xchain433_n0_af
.Lx439_1:
 jmp xchain433_n0_as
.Lx439_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx439_2:
 test rax, rax
 je .Lx439_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx439_2
.Lx439_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain433_n0_af
 mov r14d, eax
 jmp xchain433_n0_as
 xchain433_n3_β:
 mov rcx, qword ptr [r12 + 160]
 test rcx, rcx
 jz xchain433_n0_af
 mov rdi, qword ptr [r12 + 168]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx439_1
 mov rdi, qword ptr [r12 + 168]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 160], rax
 jmp xchain433_n0_af
# IR_MATCH_ANY
 xchain433_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain433_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain433_n2_af
 add r14d, 1
 jmp xchain433_n2_as
 xchain433_n4_β:
 sub r14d, 1
 jmp xchain433_n2_af
# IR_MATCH_DEFER
 xchain433_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 128], rcx
 test rax, rax
 jz .Lx441_0
 mov qword ptr [r12 + 128], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 136], rax
 mov rcx, qword ptr [r12 + 128]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx441_1
 mov rdi, qword ptr [r12 + 136]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 128], rax
 jmp xchain433_n2_af
.Lx441_1:
 jmp xchain433_n2_as
.Lx441_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx441_2:
 test rax, rax
 je .Lx441_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx441_2
.Lx441_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain433_n2_af
 mov r14d, eax
 jmp xchain433_n2_as
 xchain433_n5_β:
 mov rcx, qword ptr [r12 + 128]
 test rcx, rcx
 jz xchain433_n2_af
 mov rdi, qword ptr [r12 + 136]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx441_1
 mov rdi, qword ptr [r12 + 136]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 128], rax
 jmp xchain433_n2_af
proc_PAT$9_β:
jmp qword ptr [r12 + 176]
proc_PAT$9_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 192]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$9_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$10_α
proc_PAT$10_α:
#=======================================================================================================================
    .global proc_PAT$10_α
    .global proc_PAT$10_β
    .global proc_PAT$10_γ
    .global proc_PAT$10_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$10_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 432], rax
 pop rsi
proc_PAT$10_α_body:
lea rax, [rip + xchain442_n0_β]
mov qword ptr [r12 + 416], rax
# IR_MATCH_ALT_NARY
 xchain442_n0_α:
 mov dword ptr [r12 + 32], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 36], eax
 mov dword ptr [r12 + 40], 0
 jmp xchain442_n1_α
xchain442_n0_as:
 jmp proc_PAT$10_γ
 xchain442_n0_β:
 mov eax, dword ptr [r12 + 40]
 cmp eax, 0
 je xchain442_n1_β
 jmp xchain442_n2_β
xchain442_n0_af:
 add dword ptr [r12 + 40], 1
 mov r14d, dword ptr [r12 + 32]
 mov edi, dword ptr [r12 + 36]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 40]
 cmp eax, 1
 je xchain442_n2_α
 jmp proc_PAT$10_ω
# IR_MATCH_SEQ_NARY
 xchain442_n1_α:
 mov dword ptr [r12 + 64], r14d
 mov dword ptr [r12 + 68], 0
 jmp xchain442_n3_α
xchain442_n1_as:
 mov eax, dword ptr [r12 + 68]
 add eax, 1
 mov dword ptr [r12 + 68], eax
 cmp eax, 1
 je xchain442_n4_α
 cmp eax, 2
 je xchain442_n5_α
 cmp eax, 3
 je xchain442_n6_α
 jmp xchain442_n0_as
 xchain442_n1_β:
 mov dword ptr [r12 + 68], 4
xchain442_n1_af:
 mov eax, dword ptr [r12 + 68]
 sub eax, 1
 mov dword ptr [r12 + 68], eax
 cmp eax, 0
 je xchain442_n3_β
 cmp eax, 1
 je xchain442_n4_β
 cmp eax, 2
 je xchain442_n5_β
 cmp eax, 3
 je xchain442_n6_β
 jmp xchain442_n0_af
# IR_MATCH_SEQ_NARY
 xchain442_n2_α:
 mov dword ptr [r12 + 336], r14d
 mov dword ptr [r12 + 340], 0
 jmp xchain442_n7_α
xchain442_n2_as:
 mov eax, dword ptr [r12 + 340]
 add eax, 1
 mov dword ptr [r12 + 340], eax
 cmp eax, 1
 je xchain442_n8_α
 jmp xchain442_n0_as
 xchain442_n2_β:
 mov dword ptr [r12 + 340], 2
xchain442_n2_af:
 mov eax, dword ptr [r12 + 340]
 sub eax, 1
 mov dword ptr [r12 + 340], eax
 cmp eax, 0
 je xchain442_n7_β
 cmp eax, 1
 je xchain442_n8_β
 jmp xchain442_n0_af
# IR_MATCH_DEFER
 xchain442_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 96], rcx
 test rax, rax
 jz .Lx449_0
 mov qword ptr [r12 + 96], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 104], rax
 mov rcx, qword ptr [r12 + 96]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx449_1
 mov rdi, qword ptr [r12 + 104]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 96], rax
 jmp xchain442_n1_af
.Lx449_1:
 jmp xchain442_n1_as
.Lx449_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx449_2:
 test rax, rax
 je .Lx449_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx449_2
.Lx449_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain442_n1_af
 mov r14d, eax
 jmp xchain442_n1_as
 xchain442_n3_β:
 mov rcx, qword ptr [r12 + 96]
 test rcx, rcx
 jz xchain442_n1_af
 mov rdi, qword ptr [r12 + 104]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx449_1
 mov rdi, qword ptr [r12 + 104]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 96], rax
 jmp xchain442_n1_af
# IR_MATCH_LIT
 xchain442_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain442_n1_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain442_n1_af
 add r14d, 1
 jmp xchain442_n1_as
 xchain442_n4_β:
 sub r14d, 1
 jmp xchain442_n1_af
# IR_MATCH_ALT_NARY
 xchain442_n5_α:
 mov dword ptr [r12 + 144], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 148], eax
 mov dword ptr [r12 + 152], 0
 jmp xchain442_n9_α
xchain442_n5_as:
 jmp xchain442_n1_as
 xchain442_n5_β:
 mov eax, dword ptr [r12 + 152]
 cmp eax, 0
 je xchain442_n9_β
 jmp xchain442_n10_β
xchain442_n5_af:
 add dword ptr [r12 + 152], 1
 mov r14d, dword ptr [r12 + 144]
 mov edi, dword ptr [r12 + 148]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 152]
 cmp eax, 1
 je xchain442_n10_α
 jmp xchain442_n1_af
# IR_MATCH_ALT_NARY
 xchain442_n6_α:
 mov dword ptr [r12 + 240], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 244], eax
 mov dword ptr [r12 + 248], 0
 jmp xchain442_n11_α
xchain442_n6_as:
 jmp xchain442_n1_as
 xchain442_n6_β:
 mov eax, dword ptr [r12 + 248]
 cmp eax, 0
 je xchain442_n11_β
 jmp xchain442_n12_β
xchain442_n6_af:
 add dword ptr [r12 + 248], 1
 mov r14d, dword ptr [r12 + 240]
 mov edi, dword ptr [r12 + 244]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 248]
 cmp eax, 1
 je xchain442_n12_α
 jmp xchain442_n1_af
# IR_MATCH_DEFER
 xchain442_n7_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 368], rcx
 test rax, rax
 jz .Lx456_0
 mov qword ptr [r12 + 368], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 376], rax
 mov rcx, qword ptr [r12 + 368]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx456_1
 mov rdi, qword ptr [r12 + 376]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain442_n2_af
.Lx456_1:
 jmp xchain442_n2_as
.Lx456_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx456_2:
 test rax, rax
 je .Lx456_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx456_2
.Lx456_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain442_n2_af
 mov r14d, eax
 jmp xchain442_n2_as
 xchain442_n7_β:
 mov rcx, qword ptr [r12 + 368]
 test rcx, rcx
 jz xchain442_n2_af
 mov rdi, qword ptr [r12 + 376]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx456_1
 mov rdi, qword ptr [r12 + 376]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain442_n2_af
# IR_MATCH_DEFER
 xchain442_n8_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 400], rcx
 test rax, rax
 jz .Lx457_0
 mov qword ptr [r12 + 400], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 408], rax
 mov rcx, qword ptr [r12 + 400]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx457_1
 mov rdi, qword ptr [r12 + 408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 400], rax
 jmp xchain442_n2_af
.Lx457_1:
 jmp xchain442_n2_as
.Lx457_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx457_2:
 test rax, rax
 je .Lx457_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx457_2
.Lx457_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain442_n2_af
 mov r14d, eax
 jmp xchain442_n2_as
 xchain442_n8_β:
 mov rcx, qword ptr [r12 + 400]
 test rcx, rcx
 jz xchain442_n2_af
 mov rdi, qword ptr [r12 + 408]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx457_1
 mov rdi, qword ptr [r12 + 408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 400], rax
 jmp xchain442_n2_af
# IR_MATCH_DEFER
 xchain442_n9_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 176], rcx
 test rax, rax
 jz .Lx458_0
 mov qword ptr [r12 + 176], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 184], rax
 mov rcx, qword ptr [r12 + 176]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx458_1
 mov rdi, qword ptr [r12 + 184]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 176], rax
 jmp xchain442_n5_af
.Lx458_1:
 jmp xchain442_n5_as
.Lx458_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx458_2:
 test rax, rax
 je .Lx458_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx458_2
.Lx458_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain442_n5_af
 mov r14d, eax
 jmp xchain442_n5_as
 xchain442_n9_β:
 mov rcx, qword ptr [r12 + 176]
 test rcx, rcx
 jz xchain442_n5_af
 mov rdi, qword ptr [r12 + 184]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx458_1
 mov rdi, qword ptr [r12 + 184]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 176], rax
 jmp xchain442_n5_af
# IR_MATCH_DEFER
 xchain442_n10_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 208], rcx
 test rax, rax
 jz .Lx459_0
 mov qword ptr [r12 + 208], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 216], rax
 mov rcx, qword ptr [r12 + 208]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx459_1
 mov rdi, qword ptr [r12 + 216]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 208], rax
 jmp xchain442_n5_af
.Lx459_1:
 jmp xchain442_n5_as
.Lx459_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx459_2:
 test rax, rax
 je .Lx459_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx459_2
.Lx459_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain442_n5_af
 mov r14d, eax
 jmp xchain442_n5_as
 xchain442_n10_β:
 mov rcx, qword ptr [r12 + 208]
 test rcx, rcx
 jz xchain442_n5_af
 mov rdi, qword ptr [r12 + 216]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx459_1
 mov rdi, qword ptr [r12 + 216]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 208], rax
 jmp xchain442_n5_af
# IR_MATCH_DEFER
 xchain442_n11_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 272], rcx
 test rax, rax
 jz .Lx460_0
 mov qword ptr [r12 + 272], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 280], rax
 mov rcx, qword ptr [r12 + 272]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx460_1
 mov rdi, qword ptr [r12 + 280]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 272], rax
 jmp xchain442_n6_af
.Lx460_1:
 jmp xchain442_n6_as
.Lx460_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx460_2:
 test rax, rax
 je .Lx460_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx460_2
.Lx460_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain442_n6_af
 mov r14d, eax
 jmp xchain442_n6_as
 xchain442_n11_β:
 mov rcx, qword ptr [r12 + 272]
 test rcx, rcx
 jz xchain442_n6_af
 mov rdi, qword ptr [r12 + 280]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx460_1
 mov rdi, qword ptr [r12 + 280]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 272], rax
 jmp xchain442_n6_af
# IR_MATCH_DEFER
 xchain442_n12_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 304], rcx
 test rax, rax
 jz .Lx461_0
 mov qword ptr [r12 + 304], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 312], rax
 mov rcx, qword ptr [r12 + 304]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx461_1
 mov rdi, qword ptr [r12 + 312]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 304], rax
 jmp xchain442_n6_af
.Lx461_1:
 jmp xchain442_n6_as
.Lx461_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx461_2:
 test rax, rax
 je .Lx461_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx461_2
.Lx461_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain442_n6_af
 mov r14d, eax
 jmp xchain442_n6_as
 xchain442_n12_β:
 mov rcx, qword ptr [r12 + 304]
 test rcx, rcx
 jz xchain442_n6_af
 mov rdi, qword ptr [r12 + 312]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx461_1
 mov rdi, qword ptr [r12 + 312]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 304], rax
 jmp xchain442_n6_af
proc_PAT$10_β:
jmp qword ptr [r12 + 416]
proc_PAT$10_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 432]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$10_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$11_α
proc_PAT$11_α:
#=======================================================================================================================
    .global proc_PAT$11_α
    .global proc_PAT$11_β
    .global proc_PAT$11_γ
    .global proc_PAT$11_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$11_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 192], rax
 pop rsi
proc_PAT$11_α_body:
lea rax, [rip + xchain462_n0_β]
mov qword ptr [r12 + 176], rax
# IR_MATCH_ALT_NARY
 xchain462_n0_α:
 mov dword ptr [r12 + 32], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 36], eax
 mov dword ptr [r12 + 40], 0
 jmp xchain462_n1_α
xchain462_n0_as:
 jmp proc_PAT$11_γ
 xchain462_n0_β:
 mov eax, dword ptr [r12 + 40]
 cmp eax, 0
 je xchain462_n1_β
 jmp xchain462_n2_β
xchain462_n0_af:
 add dword ptr [r12 + 40], 1
 mov r14d, dword ptr [r12 + 32]
 mov edi, dword ptr [r12 + 36]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 40]
 cmp eax, 1
 je xchain462_n2_α
 jmp proc_PAT$11_ω
# IR_MATCH_DEFER
 xchain462_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 64], rcx
 test rax, rax
 jz .Lx465_0
 mov qword ptr [r12 + 64], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 72], rax
 mov rcx, qword ptr [r12 + 64]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx465_1
 mov rdi, qword ptr [r12 + 72]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 64], rax
 jmp xchain462_n0_af
.Lx465_1:
 jmp xchain462_n0_as
.Lx465_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx465_2:
 test rax, rax
 je .Lx465_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx465_2
.Lx465_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain462_n0_af
 mov r14d, eax
 jmp xchain462_n0_as
 xchain462_n1_β:
 mov rcx, qword ptr [r12 + 64]
 test rcx, rcx
 jz xchain462_n0_af
 mov rdi, qword ptr [r12 + 72]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx465_1
 mov rdi, qword ptr [r12 + 72]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 64], rax
 jmp xchain462_n0_af
# IR_MATCH_SEQ_NARY
 xchain462_n2_α:
 mov dword ptr [r12 + 96], r14d
 mov dword ptr [r12 + 100], 0
 jmp xchain462_n3_α
xchain462_n2_as:
 mov eax, dword ptr [r12 + 100]
 add eax, 1
 mov dword ptr [r12 + 100], eax
 cmp eax, 1
 je xchain462_n4_α
 cmp eax, 2
 je xchain462_n5_α
 jmp xchain462_n0_as
 xchain462_n2_β:
 mov dword ptr [r12 + 100], 3
xchain462_n2_af:
 mov eax, dword ptr [r12 + 100]
 sub eax, 1
 mov dword ptr [r12 + 100], eax
 cmp eax, 0
 je xchain462_n3_β
 cmp eax, 1
 je xchain462_n4_β
 cmp eax, 2
 je xchain462_n5_β
 jmp xchain462_n0_af
# IR_MATCH_LIT
 xchain462_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain462_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain462_n2_af
 add r14d, 1
 jmp xchain462_n2_as
 xchain462_n3_β:
 sub r14d, 1
 jmp xchain462_n2_af
# IR_MATCH_DEFER
 xchain462_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 144], rcx
 test rax, rax
 jz .Lx470_0
 mov qword ptr [r12 + 144], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 152], rax
 mov rcx, qword ptr [r12 + 144]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx470_1
 mov rdi, qword ptr [r12 + 152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 144], rax
 jmp xchain462_n2_af
.Lx470_1:
 jmp xchain462_n2_as
.Lx470_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx470_2:
 test rax, rax
 je .Lx470_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx470_2
.Lx470_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain462_n2_af
 mov r14d, eax
 jmp xchain462_n2_as
 xchain462_n4_β:
 mov rcx, qword ptr [r12 + 144]
 test rcx, rcx
 jz xchain462_n2_af
 mov rdi, qword ptr [r12 + 152]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx470_1
 mov rdi, qword ptr [r12 + 152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 144], rax
 jmp xchain462_n2_af
# IR_MATCH_LIT
 xchain462_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain462_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain462_n2_af
 add r14d, 1
 jmp xchain462_n2_as
 xchain462_n5_β:
 sub r14d, 1
 jmp xchain462_n2_af
proc_PAT$11_β:
jmp qword ptr [r12 + 176]
proc_PAT$11_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 192]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$11_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$12_α
proc_PAT$12_α:
#=======================================================================================================================
    .global proc_PAT$12_α
    .global proc_PAT$12_β
    .global proc_PAT$12_γ
    .global proc_PAT$12_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$12_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$12_α_body:
lea rax, [rip + xchain473_n0_β]
mov qword ptr [r12 + 48], rax
# IR_MATCH_SPAN
 xchain473_n0_α:
 mov dword ptr [r12 + 32], 0
.Lx475_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 32]
 cmp eax, r15d
 jge .Lx475_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx475_1
 add dword ptr [r12 + 32], 1
 jmp .Lx475_0
.Lx475_1:
 mov eax, dword ptr [r12 + 32]
 test eax, eax
 jle proc_PAT$12_ω
 mov edx, r14d
 mov dword ptr [r12 + 36], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$12_γ
 xchain473_n0_β:
 xchain473_n0_β:
 mov r14d, dword ptr [r12 + 36]
 jmp proc_PAT$12_ω
proc_PAT$12_β:
jmp qword ptr [r12 + 48]
proc_PAT$12_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$12_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$13_α
proc_PAT$13_α:
#=======================================================================================================================
    .global proc_PAT$13_α
    .global proc_PAT$13_β
    .global proc_PAT$13_γ
    .global proc_PAT$13_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$13_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 192], rax
 pop rsi
proc_PAT$13_α_body:
lea rax, [rip + xchain476_n0_β]
mov qword ptr [r12 + 176], rax
# IR_MATCH_SEQ_NARY
 xchain476_n0_α:
 mov dword ptr [r12 + 32], r14d
 mov dword ptr [r12 + 36], 0
 jmp xchain476_n1_α
xchain476_n0_as:
 mov eax, dword ptr [r12 + 36]
 add eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 1
 je xchain476_n2_α
 cmp eax, 2
 je xchain476_n3_α
 jmp proc_PAT$13_γ
 xchain476_n0_β:
 mov dword ptr [r12 + 36], 3
xchain476_n0_af:
 mov eax, dword ptr [r12 + 36]
 sub eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 0
 je xchain476_n1_β
 cmp eax, 1
 je xchain476_n2_β
 cmp eax, 2
 je xchain476_n3_β
 jmp proc_PAT$13_ω
# IR_MATCH_ANY
 xchain476_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain476_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain476_n0_af
 add r14d, 1
 jmp xchain476_n0_as
 xchain476_n1_β:
 sub r14d, 1
 jmp xchain476_n0_af
# IR_MATCH_ALT_NARY
 xchain476_n2_α:
 mov dword ptr [r12 + 80], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 84], eax
 mov dword ptr [r12 + 88], 0
 jmp xchain476_n4_α
xchain476_n2_as:
 jmp xchain476_n0_as
 xchain476_n2_β:
 mov eax, dword ptr [r12 + 88]
 cmp eax, 0
 je xchain476_n4_β
 jmp xchain476_n5_β
xchain476_n2_af:
 add dword ptr [r12 + 88], 1
 mov r14d, dword ptr [r12 + 80]
 mov edi, dword ptr [r12 + 84]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 88]
 cmp eax, 1
 je xchain476_n5_α
 jmp xchain476_n0_af
# IR_MATCH_DEFER
 xchain476_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 160], rcx
 test rax, rax
 jz .Lx482_0
 mov qword ptr [r12 + 160], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 168], rax
 mov rcx, qword ptr [r12 + 160]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx482_1
 mov rdi, qword ptr [r12 + 168]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 160], rax
 jmp xchain476_n0_af
.Lx482_1:
 jmp xchain476_n0_as
.Lx482_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx482_2:
 test rax, rax
 je .Lx482_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx482_2
.Lx482_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain476_n0_af
 mov r14d, eax
 jmp xchain476_n0_as
 xchain476_n3_β:
 mov rcx, qword ptr [r12 + 160]
 test rcx, rcx
 jz xchain476_n0_af
 mov rdi, qword ptr [r12 + 168]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx482_1
 mov rdi, qword ptr [r12 + 168]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 160], rax
 jmp xchain476_n0_af
# IR_MATCH_ANY
 xchain476_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain476_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain476_n2_af
 add r14d, 1
 jmp xchain476_n2_as
 xchain476_n4_β:
 sub r14d, 1
 jmp xchain476_n2_af
# IR_MATCH_DEFER
 xchain476_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 128], rcx
 test rax, rax
 jz .Lx484_0
 mov qword ptr [r12 + 128], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 136], rax
 mov rcx, qword ptr [r12 + 128]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx484_1
 mov rdi, qword ptr [r12 + 136]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 128], rax
 jmp xchain476_n2_af
.Lx484_1:
 jmp xchain476_n2_as
.Lx484_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx484_2:
 test rax, rax
 je .Lx484_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx484_2
.Lx484_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain476_n2_af
 mov r14d, eax
 jmp xchain476_n2_as
 xchain476_n5_β:
 mov rcx, qword ptr [r12 + 128]
 test rcx, rcx
 jz xchain476_n2_af
 mov rdi, qword ptr [r12 + 136]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx484_1
 mov rdi, qword ptr [r12 + 136]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 128], rax
 jmp xchain476_n2_af
proc_PAT$13_β:
jmp qword ptr [r12 + 176]
proc_PAT$13_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 192]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$13_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$14_α
proc_PAT$14_α:
#=======================================================================================================================
    .global proc_PAT$14_α
    .global proc_PAT$14_β
    .global proc_PAT$14_γ
    .global proc_PAT$14_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$14_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 432], rax
 pop rsi
proc_PAT$14_α_body:
lea rax, [rip + xchain485_n0_β]
mov qword ptr [r12 + 416], rax
# IR_MATCH_ALT_NARY
 xchain485_n0_α:
 mov dword ptr [r12 + 32], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 36], eax
 mov dword ptr [r12 + 40], 0
 jmp xchain485_n1_α
xchain485_n0_as:
 jmp proc_PAT$14_γ
 xchain485_n0_β:
 mov eax, dword ptr [r12 + 40]
 cmp eax, 0
 je xchain485_n1_β
 jmp xchain485_n2_β
xchain485_n0_af:
 add dword ptr [r12 + 40], 1
 mov r14d, dword ptr [r12 + 32]
 mov edi, dword ptr [r12 + 36]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 40]
 cmp eax, 1
 je xchain485_n2_α
 jmp proc_PAT$14_ω
# IR_MATCH_SEQ_NARY
 xchain485_n1_α:
 mov dword ptr [r12 + 64], r14d
 mov dword ptr [r12 + 68], 0
 jmp xchain485_n3_α
xchain485_n1_as:
 mov eax, dword ptr [r12 + 68]
 add eax, 1
 mov dword ptr [r12 + 68], eax
 cmp eax, 1
 je xchain485_n4_α
 cmp eax, 2
 je xchain485_n5_α
 cmp eax, 3
 je xchain485_n6_α
 jmp xchain485_n0_as
 xchain485_n1_β:
 mov dword ptr [r12 + 68], 4
xchain485_n1_af:
 mov eax, dword ptr [r12 + 68]
 sub eax, 1
 mov dword ptr [r12 + 68], eax
 cmp eax, 0
 je xchain485_n3_β
 cmp eax, 1
 je xchain485_n4_β
 cmp eax, 2
 je xchain485_n5_β
 cmp eax, 3
 je xchain485_n6_β
 jmp xchain485_n0_af
# IR_MATCH_SEQ_NARY
 xchain485_n2_α:
 mov dword ptr [r12 + 336], r14d
 mov dword ptr [r12 + 340], 0
 jmp xchain485_n7_α
xchain485_n2_as:
 mov eax, dword ptr [r12 + 340]
 add eax, 1
 mov dword ptr [r12 + 340], eax
 cmp eax, 1
 je xchain485_n8_α
 jmp xchain485_n0_as
 xchain485_n2_β:
 mov dword ptr [r12 + 340], 2
xchain485_n2_af:
 mov eax, dword ptr [r12 + 340]
 sub eax, 1
 mov dword ptr [r12 + 340], eax
 cmp eax, 0
 je xchain485_n7_β
 cmp eax, 1
 je xchain485_n8_β
 jmp xchain485_n0_af
# IR_MATCH_DEFER
 xchain485_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 96], rcx
 test rax, rax
 jz .Lx492_0
 mov qword ptr [r12 + 96], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 104], rax
 mov rcx, qword ptr [r12 + 96]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx492_1
 mov rdi, qword ptr [r12 + 104]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 96], rax
 jmp xchain485_n1_af
.Lx492_1:
 jmp xchain485_n1_as
.Lx492_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx492_2:
 test rax, rax
 je .Lx492_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx492_2
.Lx492_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain485_n1_af
 mov r14d, eax
 jmp xchain485_n1_as
 xchain485_n3_β:
 mov rcx, qword ptr [r12 + 96]
 test rcx, rcx
 jz xchain485_n1_af
 mov rdi, qword ptr [r12 + 104]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx492_1
 mov rdi, qword ptr [r12 + 104]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 96], rax
 jmp xchain485_n1_af
# IR_MATCH_LIT
 xchain485_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain485_n1_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain485_n1_af
 add r14d, 1
 jmp xchain485_n1_as
 xchain485_n4_β:
 sub r14d, 1
 jmp xchain485_n1_af
# IR_MATCH_ALT_NARY
 xchain485_n5_α:
 mov dword ptr [r12 + 144], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 148], eax
 mov dword ptr [r12 + 152], 0
 jmp xchain485_n9_α
xchain485_n5_as:
 jmp xchain485_n1_as
 xchain485_n5_β:
 mov eax, dword ptr [r12 + 152]
 cmp eax, 0
 je xchain485_n9_β
 jmp xchain485_n10_β
xchain485_n5_af:
 add dword ptr [r12 + 152], 1
 mov r14d, dword ptr [r12 + 144]
 mov edi, dword ptr [r12 + 148]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 152]
 cmp eax, 1
 je xchain485_n10_α
 jmp xchain485_n1_af
# IR_MATCH_ALT_NARY
 xchain485_n6_α:
 mov dword ptr [r12 + 240], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 244], eax
 mov dword ptr [r12 + 248], 0
 jmp xchain485_n11_α
xchain485_n6_as:
 jmp xchain485_n1_as
 xchain485_n6_β:
 mov eax, dword ptr [r12 + 248]
 cmp eax, 0
 je xchain485_n11_β
 jmp xchain485_n12_β
xchain485_n6_af:
 add dword ptr [r12 + 248], 1
 mov r14d, dword ptr [r12 + 240]
 mov edi, dword ptr [r12 + 244]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 248]
 cmp eax, 1
 je xchain485_n12_α
 jmp xchain485_n1_af
# IR_MATCH_DEFER
 xchain485_n7_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 368], rcx
 test rax, rax
 jz .Lx499_0
 mov qword ptr [r12 + 368], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 376], rax
 mov rcx, qword ptr [r12 + 368]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx499_1
 mov rdi, qword ptr [r12 + 376]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain485_n2_af
.Lx499_1:
 jmp xchain485_n2_as
.Lx499_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx499_2:
 test rax, rax
 je .Lx499_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx499_2
.Lx499_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain485_n2_af
 mov r14d, eax
 jmp xchain485_n2_as
 xchain485_n7_β:
 mov rcx, qword ptr [r12 + 368]
 test rcx, rcx
 jz xchain485_n2_af
 mov rdi, qword ptr [r12 + 376]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx499_1
 mov rdi, qword ptr [r12 + 376]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain485_n2_af
# IR_MATCH_DEFER
 xchain485_n8_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 400], rcx
 test rax, rax
 jz .Lx500_0
 mov qword ptr [r12 + 400], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 408], rax
 mov rcx, qword ptr [r12 + 400]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx500_1
 mov rdi, qword ptr [r12 + 408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 400], rax
 jmp xchain485_n2_af
.Lx500_1:
 jmp xchain485_n2_as
.Lx500_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx500_2:
 test rax, rax
 je .Lx500_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx500_2
.Lx500_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain485_n2_af
 mov r14d, eax
 jmp xchain485_n2_as
 xchain485_n8_β:
 mov rcx, qword ptr [r12 + 400]
 test rcx, rcx
 jz xchain485_n2_af
 mov rdi, qword ptr [r12 + 408]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx500_1
 mov rdi, qword ptr [r12 + 408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 400], rax
 jmp xchain485_n2_af
# IR_MATCH_DEFER
 xchain485_n9_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 176], rcx
 test rax, rax
 jz .Lx501_0
 mov qword ptr [r12 + 176], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 184], rax
 mov rcx, qword ptr [r12 + 176]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx501_1
 mov rdi, qword ptr [r12 + 184]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 176], rax
 jmp xchain485_n5_af
.Lx501_1:
 jmp xchain485_n5_as
.Lx501_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx501_2:
 test rax, rax
 je .Lx501_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx501_2
.Lx501_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain485_n5_af
 mov r14d, eax
 jmp xchain485_n5_as
 xchain485_n9_β:
 mov rcx, qword ptr [r12 + 176]
 test rcx, rcx
 jz xchain485_n5_af
 mov rdi, qword ptr [r12 + 184]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx501_1
 mov rdi, qword ptr [r12 + 184]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 176], rax
 jmp xchain485_n5_af
# IR_MATCH_DEFER
 xchain485_n10_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 208], rcx
 test rax, rax
 jz .Lx502_0
 mov qword ptr [r12 + 208], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 216], rax
 mov rcx, qword ptr [r12 + 208]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx502_1
 mov rdi, qword ptr [r12 + 216]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 208], rax
 jmp xchain485_n5_af
.Lx502_1:
 jmp xchain485_n5_as
.Lx502_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx502_2:
 test rax, rax
 je .Lx502_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx502_2
.Lx502_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain485_n5_af
 mov r14d, eax
 jmp xchain485_n5_as
 xchain485_n10_β:
 mov rcx, qword ptr [r12 + 208]
 test rcx, rcx
 jz xchain485_n5_af
 mov rdi, qword ptr [r12 + 216]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx502_1
 mov rdi, qword ptr [r12 + 216]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 208], rax
 jmp xchain485_n5_af
# IR_MATCH_DEFER
 xchain485_n11_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 272], rcx
 test rax, rax
 jz .Lx503_0
 mov qword ptr [r12 + 272], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 280], rax
 mov rcx, qword ptr [r12 + 272]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx503_1
 mov rdi, qword ptr [r12 + 280]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 272], rax
 jmp xchain485_n6_af
.Lx503_1:
 jmp xchain485_n6_as
.Lx503_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx503_2:
 test rax, rax
 je .Lx503_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx503_2
.Lx503_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain485_n6_af
 mov r14d, eax
 jmp xchain485_n6_as
 xchain485_n11_β:
 mov rcx, qword ptr [r12 + 272]
 test rcx, rcx
 jz xchain485_n6_af
 mov rdi, qword ptr [r12 + 280]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx503_1
 mov rdi, qword ptr [r12 + 280]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 272], rax
 jmp xchain485_n6_af
# IR_MATCH_DEFER
 xchain485_n12_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 304], rcx
 test rax, rax
 jz .Lx504_0
 mov qword ptr [r12 + 304], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 312], rax
 mov rcx, qword ptr [r12 + 304]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx504_1
 mov rdi, qword ptr [r12 + 312]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 304], rax
 jmp xchain485_n6_af
.Lx504_1:
 jmp xchain485_n6_as
.Lx504_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx504_2:
 test rax, rax
 je .Lx504_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx504_2
.Lx504_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain485_n6_af
 mov r14d, eax
 jmp xchain485_n6_as
 xchain485_n12_β:
 mov rcx, qword ptr [r12 + 304]
 test rcx, rcx
 jz xchain485_n6_af
 mov rdi, qword ptr [r12 + 312]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx504_1
 mov rdi, qword ptr [r12 + 312]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 304], rax
 jmp xchain485_n6_af
proc_PAT$14_β:
jmp qword ptr [r12 + 416]
proc_PAT$14_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 432]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$14_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$15_α
proc_PAT$15_α:
#=======================================================================================================================
    .global proc_PAT$15_α
    .global proc_PAT$15_β
    .global proc_PAT$15_γ
    .global proc_PAT$15_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$15_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 192], rax
 pop rsi
proc_PAT$15_α_body:
lea rax, [rip + xchain505_n0_β]
mov qword ptr [r12 + 176], rax
# IR_MATCH_ALT_NARY
 xchain505_n0_α:
 mov dword ptr [r12 + 32], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 36], eax
 mov dword ptr [r12 + 40], 0
 jmp xchain505_n1_α
xchain505_n0_as:
 jmp proc_PAT$15_γ
 xchain505_n0_β:
 mov eax, dword ptr [r12 + 40]
 cmp eax, 0
 je xchain505_n1_β
 jmp xchain505_n2_β
xchain505_n0_af:
 add dword ptr [r12 + 40], 1
 mov r14d, dword ptr [r12 + 32]
 mov edi, dword ptr [r12 + 36]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 40]
 cmp eax, 1
 je xchain505_n2_α
 jmp proc_PAT$15_ω
# IR_MATCH_DEFER
 xchain505_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 64], rcx
 test rax, rax
 jz .Lx508_0
 mov qword ptr [r12 + 64], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 72], rax
 mov rcx, qword ptr [r12 + 64]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx508_1
 mov rdi, qword ptr [r12 + 72]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 64], rax
 jmp xchain505_n0_af
.Lx508_1:
 jmp xchain505_n0_as
.Lx508_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx508_2:
 test rax, rax
 je .Lx508_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx508_2
.Lx508_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain505_n0_af
 mov r14d, eax
 jmp xchain505_n0_as
 xchain505_n1_β:
 mov rcx, qword ptr [r12 + 64]
 test rcx, rcx
 jz xchain505_n0_af
 mov rdi, qword ptr [r12 + 72]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx508_1
 mov rdi, qword ptr [r12 + 72]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 64], rax
 jmp xchain505_n0_af
# IR_MATCH_SEQ_NARY
 xchain505_n2_α:
 mov dword ptr [r12 + 96], r14d
 mov dword ptr [r12 + 100], 0
 jmp xchain505_n3_α
xchain505_n2_as:
 mov eax, dword ptr [r12 + 100]
 add eax, 1
 mov dword ptr [r12 + 100], eax
 cmp eax, 1
 je xchain505_n4_α
 cmp eax, 2
 je xchain505_n5_α
 jmp xchain505_n0_as
 xchain505_n2_β:
 mov dword ptr [r12 + 100], 3
xchain505_n2_af:
 mov eax, dword ptr [r12 + 100]
 sub eax, 1
 mov dword ptr [r12 + 100], eax
 cmp eax, 0
 je xchain505_n3_β
 cmp eax, 1
 je xchain505_n4_β
 cmp eax, 2
 je xchain505_n5_β
 jmp xchain505_n0_af
# IR_MATCH_LIT
 xchain505_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain505_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain505_n2_af
 add r14d, 1
 jmp xchain505_n2_as
 xchain505_n3_β:
 sub r14d, 1
 jmp xchain505_n2_af
# IR_MATCH_DEFER
 xchain505_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 144], rcx
 test rax, rax
 jz .Lx513_0
 mov qword ptr [r12 + 144], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 152], rax
 mov rcx, qword ptr [r12 + 144]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx513_1
 mov rdi, qword ptr [r12 + 152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 144], rax
 jmp xchain505_n2_af
.Lx513_1:
 jmp xchain505_n2_as
.Lx513_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx513_2:
 test rax, rax
 je .Lx513_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx513_2
.Lx513_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain505_n2_af
 mov r14d, eax
 jmp xchain505_n2_as
 xchain505_n4_β:
 mov rcx, qword ptr [r12 + 144]
 test rcx, rcx
 jz xchain505_n2_af
 mov rdi, qword ptr [r12 + 152]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx513_1
 mov rdi, qword ptr [r12 + 152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 144], rax
 jmp xchain505_n2_af
# IR_MATCH_LIT
 xchain505_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain505_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain505_n2_af
 add r14d, 1
 jmp xchain505_n2_as
 xchain505_n5_β:
 sub r14d, 1
 jmp xchain505_n2_af
proc_PAT$15_β:
jmp qword ptr [r12 + 176]
proc_PAT$15_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 192]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$15_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$16_α
proc_PAT$16_α:
#=======================================================================================================================
    .global proc_PAT$16_α
    .global proc_PAT$16_β
    .global proc_PAT$16_γ
    .global proc_PAT$16_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$16_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$16_α_body:
lea rax, [rip + xchain516_n0_β]
mov qword ptr [r12 + 48], rax
# IR_MATCH_SPAN
 xchain516_n0_α:
 mov dword ptr [r12 + 32], 0
.Lx518_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 32]
 cmp eax, r15d
 jge .Lx518_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx518_1
 add dword ptr [r12 + 32], 1
 jmp .Lx518_0
.Lx518_1:
 mov eax, dword ptr [r12 + 32]
 test eax, eax
 jle proc_PAT$16_ω
 mov edx, r14d
 mov dword ptr [r12 + 36], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$16_γ
 xchain516_n0_β:
 xchain516_n0_β:
 mov r14d, dword ptr [r12 + 36]
 jmp proc_PAT$16_ω
proc_PAT$16_β:
jmp qword ptr [r12 + 48]
proc_PAT$16_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$16_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$17_α
proc_PAT$17_α:
#=======================================================================================================================
    .global proc_PAT$17_α
    .global proc_PAT$17_β
    .global proc_PAT$17_γ
    .global proc_PAT$17_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$17_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 192], rax
 pop rsi
proc_PAT$17_α_body:
lea rax, [rip + xchain519_n0_β]
mov qword ptr [r12 + 176], rax
# IR_MATCH_SEQ_NARY
 xchain519_n0_α:
 mov dword ptr [r12 + 32], r14d
 mov dword ptr [r12 + 36], 0
 jmp xchain519_n1_α
xchain519_n0_as:
 mov eax, dword ptr [r12 + 36]
 add eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 1
 je xchain519_n2_α
 cmp eax, 2
 je xchain519_n3_α
 jmp proc_PAT$17_γ
 xchain519_n0_β:
 mov dword ptr [r12 + 36], 3
xchain519_n0_af:
 mov eax, dword ptr [r12 + 36]
 sub eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 0
 je xchain519_n1_β
 cmp eax, 1
 je xchain519_n2_β
 cmp eax, 2
 je xchain519_n3_β
 jmp proc_PAT$17_ω
# IR_MATCH_ANY
 xchain519_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain519_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain519_n0_af
 add r14d, 1
 jmp xchain519_n0_as
 xchain519_n1_β:
 sub r14d, 1
 jmp xchain519_n0_af
# IR_MATCH_ALT_NARY
 xchain519_n2_α:
 mov dword ptr [r12 + 80], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 84], eax
 mov dword ptr [r12 + 88], 0
 jmp xchain519_n4_α
xchain519_n2_as:
 jmp xchain519_n0_as
 xchain519_n2_β:
 mov eax, dword ptr [r12 + 88]
 cmp eax, 0
 je xchain519_n4_β
 jmp xchain519_n5_β
xchain519_n2_af:
 add dword ptr [r12 + 88], 1
 mov r14d, dword ptr [r12 + 80]
 mov edi, dword ptr [r12 + 84]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 88]
 cmp eax, 1
 je xchain519_n5_α
 jmp xchain519_n0_af
# IR_MATCH_DEFER
 xchain519_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 160], rcx
 test rax, rax
 jz .Lx525_0
 mov qword ptr [r12 + 160], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 168], rax
 mov rcx, qword ptr [r12 + 160]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx525_1
 mov rdi, qword ptr [r12 + 168]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 160], rax
 jmp xchain519_n0_af
.Lx525_1:
 jmp xchain519_n0_as
.Lx525_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx525_2:
 test rax, rax
 je .Lx525_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx525_2
.Lx525_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain519_n0_af
 mov r14d, eax
 jmp xchain519_n0_as
 xchain519_n3_β:
 mov rcx, qword ptr [r12 + 160]
 test rcx, rcx
 jz xchain519_n0_af
 mov rdi, qword ptr [r12 + 168]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx525_1
 mov rdi, qword ptr [r12 + 168]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 160], rax
 jmp xchain519_n0_af
# IR_MATCH_ANY
 xchain519_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain519_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain519_n2_af
 add r14d, 1
 jmp xchain519_n2_as
 xchain519_n4_β:
 sub r14d, 1
 jmp xchain519_n2_af
# IR_MATCH_DEFER
 xchain519_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 128], rcx
 test rax, rax
 jz .Lx527_0
 mov qword ptr [r12 + 128], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 136], rax
 mov rcx, qword ptr [r12 + 128]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx527_1
 mov rdi, qword ptr [r12 + 136]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 128], rax
 jmp xchain519_n2_af
.Lx527_1:
 jmp xchain519_n2_as
.Lx527_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx527_2:
 test rax, rax
 je .Lx527_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx527_2
.Lx527_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain519_n2_af
 mov r14d, eax
 jmp xchain519_n2_as
 xchain519_n5_β:
 mov rcx, qword ptr [r12 + 128]
 test rcx, rcx
 jz xchain519_n2_af
 mov rdi, qword ptr [r12 + 136]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx527_1
 mov rdi, qword ptr [r12 + 136]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 128], rax
 jmp xchain519_n2_af
proc_PAT$17_β:
jmp qword ptr [r12 + 176]
proc_PAT$17_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 192]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$17_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$18_α
proc_PAT$18_α:
#=======================================================================================================================
    .global proc_PAT$18_α
    .global proc_PAT$18_β
    .global proc_PAT$18_γ
    .global proc_PAT$18_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$18_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 432], rax
 pop rsi
proc_PAT$18_α_body:
lea rax, [rip + xchain528_n0_β]
mov qword ptr [r12 + 416], rax
# IR_MATCH_ALT_NARY
 xchain528_n0_α:
 mov dword ptr [r12 + 32], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 36], eax
 mov dword ptr [r12 + 40], 0
 jmp xchain528_n1_α
xchain528_n0_as:
 jmp proc_PAT$18_γ
 xchain528_n0_β:
 mov eax, dword ptr [r12 + 40]
 cmp eax, 0
 je xchain528_n1_β
 jmp xchain528_n2_β
xchain528_n0_af:
 add dword ptr [r12 + 40], 1
 mov r14d, dword ptr [r12 + 32]
 mov edi, dword ptr [r12 + 36]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 40]
 cmp eax, 1
 je xchain528_n2_α
 jmp proc_PAT$18_ω
# IR_MATCH_SEQ_NARY
 xchain528_n1_α:
 mov dword ptr [r12 + 64], r14d
 mov dword ptr [r12 + 68], 0
 jmp xchain528_n3_α
xchain528_n1_as:
 mov eax, dword ptr [r12 + 68]
 add eax, 1
 mov dword ptr [r12 + 68], eax
 cmp eax, 1
 je xchain528_n4_α
 cmp eax, 2
 je xchain528_n5_α
 cmp eax, 3
 je xchain528_n6_α
 jmp xchain528_n0_as
 xchain528_n1_β:
 mov dword ptr [r12 + 68], 4
xchain528_n1_af:
 mov eax, dword ptr [r12 + 68]
 sub eax, 1
 mov dword ptr [r12 + 68], eax
 cmp eax, 0
 je xchain528_n3_β
 cmp eax, 1
 je xchain528_n4_β
 cmp eax, 2
 je xchain528_n5_β
 cmp eax, 3
 je xchain528_n6_β
 jmp xchain528_n0_af
# IR_MATCH_SEQ_NARY
 xchain528_n2_α:
 mov dword ptr [r12 + 336], r14d
 mov dword ptr [r12 + 340], 0
 jmp xchain528_n7_α
xchain528_n2_as:
 mov eax, dword ptr [r12 + 340]
 add eax, 1
 mov dword ptr [r12 + 340], eax
 cmp eax, 1
 je xchain528_n8_α
 jmp xchain528_n0_as
 xchain528_n2_β:
 mov dword ptr [r12 + 340], 2
xchain528_n2_af:
 mov eax, dword ptr [r12 + 340]
 sub eax, 1
 mov dword ptr [r12 + 340], eax
 cmp eax, 0
 je xchain528_n7_β
 cmp eax, 1
 je xchain528_n8_β
 jmp xchain528_n0_af
# IR_MATCH_DEFER
 xchain528_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 96], rcx
 test rax, rax
 jz .Lx535_0
 mov qword ptr [r12 + 96], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 104], rax
 mov rcx, qword ptr [r12 + 96]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx535_1
 mov rdi, qword ptr [r12 + 104]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 96], rax
 jmp xchain528_n1_af
.Lx535_1:
 jmp xchain528_n1_as
.Lx535_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx535_2:
 test rax, rax
 je .Lx535_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx535_2
.Lx535_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain528_n1_af
 mov r14d, eax
 jmp xchain528_n1_as
 xchain528_n3_β:
 mov rcx, qword ptr [r12 + 96]
 test rcx, rcx
 jz xchain528_n1_af
 mov rdi, qword ptr [r12 + 104]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx535_1
 mov rdi, qword ptr [r12 + 104]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 96], rax
 jmp xchain528_n1_af
# IR_MATCH_LIT
 xchain528_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain528_n1_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain528_n1_af
 add r14d, 1
 jmp xchain528_n1_as
 xchain528_n4_β:
 sub r14d, 1
 jmp xchain528_n1_af
# IR_MATCH_ALT_NARY
 xchain528_n5_α:
 mov dword ptr [r12 + 144], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 148], eax
 mov dword ptr [r12 + 152], 0
 jmp xchain528_n9_α
xchain528_n5_as:
 jmp xchain528_n1_as
 xchain528_n5_β:
 mov eax, dword ptr [r12 + 152]
 cmp eax, 0
 je xchain528_n9_β
 jmp xchain528_n10_β
xchain528_n5_af:
 add dword ptr [r12 + 152], 1
 mov r14d, dword ptr [r12 + 144]
 mov edi, dword ptr [r12 + 148]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 152]
 cmp eax, 1
 je xchain528_n10_α
 jmp xchain528_n1_af
# IR_MATCH_ALT_NARY
 xchain528_n6_α:
 mov dword ptr [r12 + 240], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 244], eax
 mov dword ptr [r12 + 248], 0
 jmp xchain528_n11_α
xchain528_n6_as:
 jmp xchain528_n1_as
 xchain528_n6_β:
 mov eax, dword ptr [r12 + 248]
 cmp eax, 0
 je xchain528_n11_β
 jmp xchain528_n12_β
xchain528_n6_af:
 add dword ptr [r12 + 248], 1
 mov r14d, dword ptr [r12 + 240]
 mov edi, dword ptr [r12 + 244]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 248]
 cmp eax, 1
 je xchain528_n12_α
 jmp xchain528_n1_af
# IR_MATCH_DEFER
 xchain528_n7_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 368], rcx
 test rax, rax
 jz .Lx542_0
 mov qword ptr [r12 + 368], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 376], rax
 mov rcx, qword ptr [r12 + 368]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx542_1
 mov rdi, qword ptr [r12 + 376]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain528_n2_af
.Lx542_1:
 jmp xchain528_n2_as
.Lx542_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx542_2:
 test rax, rax
 je .Lx542_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx542_2
.Lx542_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain528_n2_af
 mov r14d, eax
 jmp xchain528_n2_as
 xchain528_n7_β:
 mov rcx, qword ptr [r12 + 368]
 test rcx, rcx
 jz xchain528_n2_af
 mov rdi, qword ptr [r12 + 376]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx542_1
 mov rdi, qword ptr [r12 + 376]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain528_n2_af
# IR_MATCH_DEFER
 xchain528_n8_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 400], rcx
 test rax, rax
 jz .Lx543_0
 mov qword ptr [r12 + 400], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 408], rax
 mov rcx, qword ptr [r12 + 400]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx543_1
 mov rdi, qword ptr [r12 + 408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 400], rax
 jmp xchain528_n2_af
.Lx543_1:
 jmp xchain528_n2_as
.Lx543_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx543_2:
 test rax, rax
 je .Lx543_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx543_2
.Lx543_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain528_n2_af
 mov r14d, eax
 jmp xchain528_n2_as
 xchain528_n8_β:
 mov rcx, qword ptr [r12 + 400]
 test rcx, rcx
 jz xchain528_n2_af
 mov rdi, qword ptr [r12 + 408]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx543_1
 mov rdi, qword ptr [r12 + 408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 400], rax
 jmp xchain528_n2_af
# IR_MATCH_DEFER
 xchain528_n9_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 176], rcx
 test rax, rax
 jz .Lx544_0
 mov qword ptr [r12 + 176], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 184], rax
 mov rcx, qword ptr [r12 + 176]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx544_1
 mov rdi, qword ptr [r12 + 184]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 176], rax
 jmp xchain528_n5_af
.Lx544_1:
 jmp xchain528_n5_as
.Lx544_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx544_2:
 test rax, rax
 je .Lx544_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx544_2
.Lx544_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain528_n5_af
 mov r14d, eax
 jmp xchain528_n5_as
 xchain528_n9_β:
 mov rcx, qword ptr [r12 + 176]
 test rcx, rcx
 jz xchain528_n5_af
 mov rdi, qword ptr [r12 + 184]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx544_1
 mov rdi, qword ptr [r12 + 184]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 176], rax
 jmp xchain528_n5_af
# IR_MATCH_DEFER
 xchain528_n10_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 208], rcx
 test rax, rax
 jz .Lx545_0
 mov qword ptr [r12 + 208], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 216], rax
 mov rcx, qword ptr [r12 + 208]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx545_1
 mov rdi, qword ptr [r12 + 216]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 208], rax
 jmp xchain528_n5_af
.Lx545_1:
 jmp xchain528_n5_as
.Lx545_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx545_2:
 test rax, rax
 je .Lx545_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx545_2
.Lx545_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain528_n5_af
 mov r14d, eax
 jmp xchain528_n5_as
 xchain528_n10_β:
 mov rcx, qword ptr [r12 + 208]
 test rcx, rcx
 jz xchain528_n5_af
 mov rdi, qword ptr [r12 + 216]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx545_1
 mov rdi, qword ptr [r12 + 216]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 208], rax
 jmp xchain528_n5_af
# IR_MATCH_DEFER
 xchain528_n11_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 272], rcx
 test rax, rax
 jz .Lx546_0
 mov qword ptr [r12 + 272], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 280], rax
 mov rcx, qword ptr [r12 + 272]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx546_1
 mov rdi, qword ptr [r12 + 280]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 272], rax
 jmp xchain528_n6_af
.Lx546_1:
 jmp xchain528_n6_as
.Lx546_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx546_2:
 test rax, rax
 je .Lx546_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx546_2
.Lx546_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain528_n6_af
 mov r14d, eax
 jmp xchain528_n6_as
 xchain528_n11_β:
 mov rcx, qword ptr [r12 + 272]
 test rcx, rcx
 jz xchain528_n6_af
 mov rdi, qword ptr [r12 + 280]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx546_1
 mov rdi, qword ptr [r12 + 280]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 272], rax
 jmp xchain528_n6_af
# IR_MATCH_DEFER
 xchain528_n12_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 304], rcx
 test rax, rax
 jz .Lx547_0
 mov qword ptr [r12 + 304], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 312], rax
 mov rcx, qword ptr [r12 + 304]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx547_1
 mov rdi, qword ptr [r12 + 312]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 304], rax
 jmp xchain528_n6_af
.Lx547_1:
 jmp xchain528_n6_as
.Lx547_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx547_2:
 test rax, rax
 je .Lx547_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx547_2
.Lx547_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain528_n6_af
 mov r14d, eax
 jmp xchain528_n6_as
 xchain528_n12_β:
 mov rcx, qword ptr [r12 + 304]
 test rcx, rcx
 jz xchain528_n6_af
 mov rdi, qword ptr [r12 + 312]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx547_1
 mov rdi, qword ptr [r12 + 312]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 304], rax
 jmp xchain528_n6_af
proc_PAT$18_β:
jmp qword ptr [r12 + 416]
proc_PAT$18_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 432]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$18_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_PAT$19_α
proc_PAT$19_α:
#=======================================================================================================================
    .global proc_PAT$19_α
    .global proc_PAT$19_β
    .global proc_PAT$19_γ
    .global proc_PAT$19_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$19_β
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 192], rax
 pop rsi
proc_PAT$19_α_body:
lea rax, [rip + xchain548_n0_β]
mov qword ptr [r12 + 176], rax
# IR_MATCH_ALT_NARY
 xchain548_n0_α:
 mov dword ptr [r12 + 32], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 36], eax
 mov dword ptr [r12 + 40], 0
 jmp xchain548_n1_α
xchain548_n0_as:
 jmp proc_PAT$19_γ
 xchain548_n0_β:
 mov eax, dword ptr [r12 + 40]
 cmp eax, 0
 je xchain548_n1_β
 jmp xchain548_n2_β
xchain548_n0_af:
 add dword ptr [r12 + 40], 1
 mov r14d, dword ptr [r12 + 32]
 mov edi, dword ptr [r12 + 36]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 40]
 cmp eax, 1
 je xchain548_n2_α
 jmp proc_PAT$19_ω
# IR_MATCH_DEFER
 xchain548_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 64], rcx
 test rax, rax
 jz .Lx551_0
 mov qword ptr [r12 + 64], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 72], rax
 mov rcx, qword ptr [r12 + 64]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx551_1
 mov rdi, qword ptr [r12 + 72]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 64], rax
 jmp xchain548_n0_af
.Lx551_1:
 jmp xchain548_n0_as
.Lx551_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx551_2:
 test rax, rax
 je .Lx551_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx551_2
.Lx551_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain548_n0_af
 mov r14d, eax
 jmp xchain548_n0_as
 xchain548_n1_β:
 mov rcx, qword ptr [r12 + 64]
 test rcx, rcx
 jz xchain548_n0_af
 mov rdi, qword ptr [r12 + 72]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx551_1
 mov rdi, qword ptr [r12 + 72]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 64], rax
 jmp xchain548_n0_af
# IR_MATCH_SEQ_NARY
 xchain548_n2_α:
 mov dword ptr [r12 + 96], r14d
 mov dword ptr [r12 + 100], 0
 jmp xchain548_n3_α
xchain548_n2_as:
 mov eax, dword ptr [r12 + 100]
 add eax, 1
 mov dword ptr [r12 + 100], eax
 cmp eax, 1
 je xchain548_n4_α
 cmp eax, 2
 je xchain548_n5_α
 jmp xchain548_n0_as
 xchain548_n2_β:
 mov dword ptr [r12 + 100], 3
xchain548_n2_af:
 mov eax, dword ptr [r12 + 100]
 sub eax, 1
 mov dword ptr [r12 + 100], eax
 cmp eax, 0
 je xchain548_n3_β
 cmp eax, 1
 je xchain548_n4_β
 cmp eax, 2
 je xchain548_n5_β
 jmp xchain548_n0_af
# IR_MATCH_LIT
 xchain548_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain548_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain548_n2_af
 add r14d, 1
 jmp xchain548_n2_as
 xchain548_n3_β:
 sub r14d, 1
 jmp xchain548_n2_af
# IR_MATCH_DEFER
 xchain548_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 144], rcx
 test rax, rax
 jz .Lx556_0
 mov qword ptr [r12 + 144], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 152], rax
 mov rcx, qword ptr [r12 + 144]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx556_1
 mov rdi, qword ptr [r12 + 152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 144], rax
 jmp xchain548_n2_af
.Lx556_1:
 jmp xchain548_n2_as
.Lx556_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx556_2:
 test rax, rax
 je .Lx556_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx556_2
.Lx556_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain548_n2_af
 mov r14d, eax
 jmp xchain548_n2_as
 xchain548_n4_β:
 mov rcx, qword ptr [r12 + 144]
 test rcx, rcx
 jz xchain548_n2_af
 mov rdi, qword ptr [r12 + 152]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx556_1
 mov rdi, qword ptr [r12 + 152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 144], rax
 jmp xchain548_n2_af
# IR_MATCH_LIT
 xchain548_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain548_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain548_n2_af
 add r14d, 1
 jmp xchain548_n2_as
 xchain548_n5_β:
 sub r14d, 1
 jmp xchain548_n2_af
proc_PAT$19_β:
jmp qword ptr [r12 + 176]
proc_PAT$19_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 192]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_PAT$19_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
proc_startup:
  push rbp
  mov rbp, rsp
  .section .rodata
  .Lstartup_pname0: .string "Push"
  .Lstartup_pp0_0: .string "x"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 1
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_Push_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 5760
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname1: .string "Pop"
  .align 8
  .Lstartup_pnames1:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname1]
  lea rsi, [rip + .Lstartup_pnames1]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname1]
  lea rsi, [rip + proc_Pop_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 5760
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname2: .string "Unary"
  .Lstartup_pp2_0: .string "arg"
  .Lstartup_pp2_1: .string "op"
  .align 8
  .Lstartup_pnames2:
  .quad .Lstartup_pp2_0
  .quad .Lstartup_pp2_1
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname2]
  lea rsi, [rip + .Lstartup_pnames2]
  mov edx, 2
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname2]
  lea rsi, [rip + proc_Unary_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 2
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 5760
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname3: .string "Binary"
  .Lstartup_pp3_0: .string "op"
  .Lstartup_pp3_1: .string "left"
  .Lstartup_pp3_2: .string "right"
  .align 8
  .Lstartup_pnames3:
  .quad .Lstartup_pp3_0
  .quad .Lstartup_pp3_1
  .quad .Lstartup_pp3_2
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname3]
  lea rsi, [rip + .Lstartup_pnames3]
  mov edx, 3
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname3]
  lea rsi, [rip + proc_Binary_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 3
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 5760
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname4: .string "EXPR$0"
  .align 8
  .Lstartup_pnames4:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname4]
  lea rsi, [rip + .Lstartup_pnames4]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname4]
  lea rsi, [rip + proc_EXPR$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname5: .string "EXPR$1"
  .align 8
  .Lstartup_pnames5:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname5]
  lea rsi, [rip + .Lstartup_pnames5]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname5]
  lea rsi, [rip + proc_EXPR$1_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname6: .string "EXPR$2"
  .align 8
  .Lstartup_pnames6:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname6]
  lea rsi, [rip + .Lstartup_pnames6]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname6]
  lea rsi, [rip + proc_EXPR$2_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname7: .string "EXPR$3"
  .align 8
  .Lstartup_pnames7:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname7]
  lea rsi, [rip + .Lstartup_pnames7]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname7]
  lea rsi, [rip + proc_EXPR$3_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname8: .string "EXPR$4"
  .align 8
  .Lstartup_pnames8:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname8]
  lea rsi, [rip + .Lstartup_pnames8]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname8]
  lea rsi, [rip + proc_EXPR$4_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname9: .string "EXPR$5"
  .align 8
  .Lstartup_pnames9:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname9]
  lea rsi, [rip + .Lstartup_pnames9]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname9]
  lea rsi, [rip + proc_EXPR$5_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname10: .string "EXPR$6"
  .align 8
  .Lstartup_pnames10:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname10]
  lea rsi, [rip + .Lstartup_pnames10]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname10]
  lea rsi, [rip + proc_EXPR$6_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname11: .string "EXPR$7"
  .align 8
  .Lstartup_pnames11:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname11]
  lea rsi, [rip + .Lstartup_pnames11]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname11]
  lea rsi, [rip + proc_EXPR$7_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname12: .string "EXPR$8"
  .align 8
  .Lstartup_pnames12:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname12]
  lea rsi, [rip + .Lstartup_pnames12]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname12]
  lea rsi, [rip + proc_EXPR$8_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname13: .string "EXPR$9"
  .align 8
  .Lstartup_pnames13:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname13]
  lea rsi, [rip + .Lstartup_pnames13]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname13]
  lea rsi, [rip + proc_EXPR$9_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname14: .string "EXPR$10"
  .align 8
  .Lstartup_pnames14:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname14]
  lea rsi, [rip + .Lstartup_pnames14]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname14]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname14]
  lea rsi, [rip + proc_EXPR$10_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname14]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname14]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname15: .string "EXPR$11"
  .align 8
  .Lstartup_pnames15:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname15]
  lea rsi, [rip + .Lstartup_pnames15]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname15]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname15]
  lea rsi, [rip + proc_EXPR$11_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname15]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname15]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname16: .string "EXPR$12"
  .align 8
  .Lstartup_pnames16:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname16]
  lea rsi, [rip + .Lstartup_pnames16]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname16]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname16]
  lea rsi, [rip + proc_EXPR$12_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname16]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname16]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname17: .string "EXPR$13"
  .align 8
  .Lstartup_pnames17:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname17]
  lea rsi, [rip + .Lstartup_pnames17]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname17]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname17]
  lea rsi, [rip + proc_EXPR$13_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname17]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname17]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname18: .string "EXPR$14"
  .align 8
  .Lstartup_pnames18:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname18]
  lea rsi, [rip + .Lstartup_pnames18]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname18]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname18]
  lea rsi, [rip + proc_EXPR$14_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname18]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname18]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname19: .string "EXPR$15"
  .align 8
  .Lstartup_pnames19:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname19]
  lea rsi, [rip + .Lstartup_pnames19]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname19]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname19]
  lea rsi, [rip + proc_EXPR$15_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname19]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname19]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname20: .string "EXPR$16"
  .align 8
  .Lstartup_pnames20:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname20]
  lea rsi, [rip + .Lstartup_pnames20]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname20]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname20]
  lea rsi, [rip + proc_EXPR$16_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname20]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname20]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname21: .string "EXPR$17"
  .align 8
  .Lstartup_pnames21:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname21]
  lea rsi, [rip + .Lstartup_pnames21]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname21]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname21]
  lea rsi, [rip + proc_EXPR$17_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname21]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname21]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname22: .string "EXPR$18"
  .align 8
  .Lstartup_pnames22:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname22]
  lea rsi, [rip + .Lstartup_pnames22]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname22]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname22]
  lea rsi, [rip + proc_EXPR$18_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname22]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname22]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname23: .string "EXPR$19"
  .align 8
  .Lstartup_pnames23:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname23]
  lea rsi, [rip + .Lstartup_pnames23]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname23]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname23]
  lea rsi, [rip + proc_EXPR$19_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname23]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname23]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname24: .string "EXPR$20"
  .align 8
  .Lstartup_pnames24:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname24]
  lea rsi, [rip + .Lstartup_pnames24]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname24]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname24]
  lea rsi, [rip + proc_EXPR$20_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname24]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname24]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname25: .string "EXPR$21"
  .align 8
  .Lstartup_pnames25:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname25]
  lea rsi, [rip + .Lstartup_pnames25]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname25]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname25]
  lea rsi, [rip + proc_EXPR$21_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname25]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname25]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname26: .string "EXPR$22"
  .align 8
  .Lstartup_pnames26:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname26]
  lea rsi, [rip + .Lstartup_pnames26]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname26]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname26]
  lea rsi, [rip + proc_EXPR$22_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname26]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname26]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname27: .string "EXPR$23"
  .align 8
  .Lstartup_pnames27:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname27]
  lea rsi, [rip + .Lstartup_pnames27]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname27]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname27]
  lea rsi, [rip + proc_EXPR$23_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname27]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname27]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname28: .string "EXPR$24"
  .align 8
  .Lstartup_pnames28:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname28]
  lea rsi, [rip + .Lstartup_pnames28]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname28]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname28]
  lea rsi, [rip + proc_EXPR$24_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname28]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname28]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname29: .string "EXPR$25"
  .align 8
  .Lstartup_pnames29:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname29]
  lea rsi, [rip + .Lstartup_pnames29]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname29]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname29]
  lea rsi, [rip + proc_EXPR$25_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname29]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname29]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname30: .string "EXPR$26"
  .align 8
  .Lstartup_pnames30:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname30]
  lea rsi, [rip + .Lstartup_pnames30]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname30]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname30]
  lea rsi, [rip + proc_EXPR$26_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname30]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname30]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname31: .string "EXPR$27"
  .align 8
  .Lstartup_pnames31:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname31]
  lea rsi, [rip + .Lstartup_pnames31]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname31]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname31]
  lea rsi, [rip + proc_EXPR$27_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname31]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname31]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname32: .string "EXPR$28"
  .align 8
  .Lstartup_pnames32:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname32]
  lea rsi, [rip + .Lstartup_pnames32]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname32]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname32]
  lea rsi, [rip + proc_EXPR$28_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname32]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname32]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname33: .string "EXPR$29"
  .align 8
  .Lstartup_pnames33:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname33]
  lea rsi, [rip + .Lstartup_pnames33]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname33]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname33]
  lea rsi, [rip + proc_EXPR$29_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname33]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname33]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname34: .string "EXPR$30"
  .align 8
  .Lstartup_pnames34:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname34]
  lea rsi, [rip + .Lstartup_pnames34]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname34]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname34]
  lea rsi, [rip + proc_EXPR$30_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname34]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname34]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname35: .string "EXPR$31"
  .align 8
  .Lstartup_pnames35:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname35]
  lea rsi, [rip + .Lstartup_pnames35]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname35]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname35]
  lea rsi, [rip + proc_EXPR$31_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname35]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname35]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname36: .string "EXPR$32"
  .align 8
  .Lstartup_pnames36:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname36]
  lea rsi, [rip + .Lstartup_pnames36]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname36]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname36]
  lea rsi, [rip + proc_EXPR$32_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname36]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname36]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname37: .string "EXPR$33"
  .align 8
  .Lstartup_pnames37:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname37]
  lea rsi, [rip + .Lstartup_pnames37]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname37]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname37]
  lea rsi, [rip + proc_EXPR$33_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname37]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname37]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname38: .string "EXPR$34"
  .align 8
  .Lstartup_pnames38:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname38]
  lea rsi, [rip + .Lstartup_pnames38]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname38]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname38]
  lea rsi, [rip + proc_EXPR$34_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname38]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname38]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname39: .string "EXPR$35"
  .align 8
  .Lstartup_pnames39:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname39]
  lea rsi, [rip + .Lstartup_pnames39]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname39]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname39]
  lea rsi, [rip + proc_EXPR$35_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname39]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname39]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname40: .string "EXPR$36"
  .align 8
  .Lstartup_pnames40:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname40]
  lea rsi, [rip + .Lstartup_pnames40]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname40]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname40]
  lea rsi, [rip + proc_EXPR$36_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname40]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname40]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname41: .string "EXPR$37"
  .align 8
  .Lstartup_pnames41:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname41]
  lea rsi, [rip + .Lstartup_pnames41]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname41]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname41]
  lea rsi, [rip + proc_EXPR$37_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname41]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname41]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname42: .string "EXPR$38"
  .align 8
  .Lstartup_pnames42:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname42]
  lea rsi, [rip + .Lstartup_pnames42]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname42]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname42]
  lea rsi, [rip + proc_EXPR$38_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname42]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname42]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname43: .string "EXPR$39"
  .align 8
  .Lstartup_pnames43:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname43]
  lea rsi, [rip + .Lstartup_pnames43]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname43]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname43]
  lea rsi, [rip + proc_EXPR$39_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname43]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname43]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname44: .string "EXPR$40"
  .align 8
  .Lstartup_pnames44:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname44]
  lea rsi, [rip + .Lstartup_pnames44]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname44]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname44]
  lea rsi, [rip + proc_EXPR$40_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname44]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname44]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname45: .string "EXPR$41"
  .align 8
  .Lstartup_pnames45:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname45]
  lea rsi, [rip + .Lstartup_pnames45]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname45]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname45]
  lea rsi, [rip + proc_EXPR$41_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname45]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname45]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname46: .string "EXPR$42"
  .align 8
  .Lstartup_pnames46:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname46]
  lea rsi, [rip + .Lstartup_pnames46]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname46]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname46]
  lea rsi, [rip + proc_EXPR$42_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname46]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname46]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname47: .string "EXPR$43"
  .align 8
  .Lstartup_pnames47:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname47]
  lea rsi, [rip + .Lstartup_pnames47]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname47]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname47]
  lea rsi, [rip + proc_EXPR$43_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname47]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname47]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname48: .string "EXPR$44"
  .align 8
  .Lstartup_pnames48:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname48]
  lea rsi, [rip + .Lstartup_pnames48]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname48]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname48]
  lea rsi, [rip + proc_EXPR$44_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname48]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname48]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname49: .string "EXPR$45"
  .align 8
  .Lstartup_pnames49:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname49]
  lea rsi, [rip + .Lstartup_pnames49]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname49]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname49]
  lea rsi, [rip + proc_EXPR$45_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname49]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname49]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname50: .string "EXPR$46"
  .align 8
  .Lstartup_pnames50:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname50]
  lea rsi, [rip + .Lstartup_pnames50]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname50]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname50]
  lea rsi, [rip + proc_EXPR$46_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname50]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname50]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname51: .string "EXPR$47"
  .align 8
  .Lstartup_pnames51:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname51]
  lea rsi, [rip + .Lstartup_pnames51]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname51]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname51]
  lea rsi, [rip + proc_EXPR$47_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname51]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname51]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname52: .string "EXPR$48"
  .align 8
  .Lstartup_pnames52:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname52]
  lea rsi, [rip + .Lstartup_pnames52]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname52]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname52]
  lea rsi, [rip + proc_EXPR$48_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname52]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname52]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname53: .string "EXPR$49"
  .align 8
  .Lstartup_pnames53:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname53]
  lea rsi, [rip + .Lstartup_pnames53]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname53]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname53]
  lea rsi, [rip + proc_EXPR$49_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname53]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname53]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname54: .string "EXPR$50"
  .align 8
  .Lstartup_pnames54:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname54]
  lea rsi, [rip + .Lstartup_pnames54]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname54]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname54]
  lea rsi, [rip + proc_EXPR$50_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname54]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname54]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname55: .string "EXPR$51"
  .align 8
  .Lstartup_pnames55:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname55]
  lea rsi, [rip + .Lstartup_pnames55]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname55]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname55]
  lea rsi, [rip + proc_EXPR$51_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname55]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname55]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname56: .string "EXPR$52"
  .align 8
  .Lstartup_pnames56:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname56]
  lea rsi, [rip + .Lstartup_pnames56]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname56]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname56]
  lea rsi, [rip + proc_EXPR$52_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname56]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname56]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname57: .string "EXPR$53"
  .align 8
  .Lstartup_pnames57:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname57]
  lea rsi, [rip + .Lstartup_pnames57]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname57]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname57]
  lea rsi, [rip + proc_EXPR$53_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname57]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname57]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname58: .string "EXPR$54"
  .align 8
  .Lstartup_pnames58:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname58]
  lea rsi, [rip + .Lstartup_pnames58]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname58]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname58]
  lea rsi, [rip + proc_EXPR$54_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname58]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname58]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname59: .string "EXPR$55"
  .align 8
  .Lstartup_pnames59:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname59]
  lea rsi, [rip + .Lstartup_pnames59]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname59]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname59]
  lea rsi, [rip + proc_EXPR$55_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname59]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname59]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname60: .string "EXPR$56"
  .align 8
  .Lstartup_pnames60:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname60]
  lea rsi, [rip + .Lstartup_pnames60]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname60]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname60]
  lea rsi, [rip + proc_EXPR$56_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname60]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname60]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname61: .string "EXPR$57"
  .align 8
  .Lstartup_pnames61:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname61]
  lea rsi, [rip + .Lstartup_pnames61]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname61]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname61]
  lea rsi, [rip + proc_EXPR$57_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname61]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname61]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname62: .string "EXPR$58"
  .align 8
  .Lstartup_pnames62:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname62]
  lea rsi, [rip + .Lstartup_pnames62]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname62]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname62]
  lea rsi, [rip + proc_EXPR$58_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname62]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname62]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname63: .string "EXPR$59"
  .align 8
  .Lstartup_pnames63:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname63]
  lea rsi, [rip + .Lstartup_pnames63]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname63]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname63]
  lea rsi, [rip + proc_EXPR$59_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname63]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname63]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname64: .string "EXPR$60"
  .align 8
  .Lstartup_pnames64:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname64]
  lea rsi, [rip + .Lstartup_pnames64]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname64]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname64]
  lea rsi, [rip + proc_EXPR$60_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname64]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname64]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname65: .string "EXPR$61"
  .align 8
  .Lstartup_pnames65:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname65]
  lea rsi, [rip + .Lstartup_pnames65]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname65]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname65]
  lea rsi, [rip + proc_EXPR$61_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname65]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname65]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname66: .string "EXPR$62"
  .align 8
  .Lstartup_pnames66:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname66]
  lea rsi, [rip + .Lstartup_pnames66]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname66]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname66]
  lea rsi, [rip + proc_EXPR$62_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname66]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname66]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname67: .string "EXPR$63"
  .align 8
  .Lstartup_pnames67:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname67]
  lea rsi, [rip + .Lstartup_pnames67]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname67]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname67]
  lea rsi, [rip + proc_EXPR$63_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname67]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname67]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname68: .string "EXPR$64"
  .align 8
  .Lstartup_pnames68:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname68]
  lea rsi, [rip + .Lstartup_pnames68]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname68]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname68]
  lea rsi, [rip + proc_EXPR$64_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname68]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname68]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname69: .string "EXPR$65"
  .align 8
  .Lstartup_pnames69:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname69]
  lea rsi, [rip + .Lstartup_pnames69]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname69]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname69]
  lea rsi, [rip + proc_EXPR$65_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname69]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname69]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname70: .string "EXPR$66"
  .align 8
  .Lstartup_pnames70:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname70]
  lea rsi, [rip + .Lstartup_pnames70]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname70]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname70]
  lea rsi, [rip + proc_EXPR$66_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname70]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname70]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname71: .string "EXPR$67"
  .align 8
  .Lstartup_pnames71:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname71]
  lea rsi, [rip + .Lstartup_pnames71]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname71]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname71]
  lea rsi, [rip + proc_EXPR$67_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname71]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname71]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname72: .string "EXPR$68"
  .align 8
  .Lstartup_pnames72:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname72]
  lea rsi, [rip + .Lstartup_pnames72]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname72]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname72]
  lea rsi, [rip + proc_EXPR$68_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname72]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname72]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname73: .string "EXPR$69"
  .align 8
  .Lstartup_pnames73:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname73]
  lea rsi, [rip + .Lstartup_pnames73]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname73]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname73]
  lea rsi, [rip + proc_EXPR$69_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname73]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname73]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname74: .string "PAT$0"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname74]
  lea rsi, [rip + proc_PAT$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname74]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname74]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname75: .string "PAT$1"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname75]
  lea rsi, [rip + proc_PAT$1_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname75]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname75]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname76: .string "PAT$2"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname76]
  lea rsi, [rip + proc_PAT$2_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname76]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname76]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname77: .string "PAT$3"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname77]
  lea rsi, [rip + proc_PAT$3_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname77]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname77]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname78: .string "PAT$4"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname78]
  lea rsi, [rip + proc_PAT$4_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname78]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname78]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname79: .string "PAT$5"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname79]
  lea rsi, [rip + proc_PAT$5_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname79]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname79]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname80: .string "PAT$6"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname80]
  lea rsi, [rip + proc_PAT$6_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname80]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname80]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname81: .string "PAT$7"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname81]
  lea rsi, [rip + proc_PAT$7_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname81]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname81]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname82: .string "PAT$8"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname82]
  lea rsi, [rip + proc_PAT$8_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname82]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname82]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname83: .string "PAT$9"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname83]
  lea rsi, [rip + proc_PAT$9_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname83]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname83]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname84: .string "PAT$10"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname84]
  lea rsi, [rip + proc_PAT$10_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname84]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname84]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname85: .string "PAT$11"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname85]
  lea rsi, [rip + proc_PAT$11_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname85]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname85]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname86: .string "PAT$12"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname86]
  lea rsi, [rip + proc_PAT$12_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname86]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname86]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname87: .string "PAT$13"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname87]
  lea rsi, [rip + proc_PAT$13_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname87]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname87]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname88: .string "PAT$14"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname88]
  lea rsi, [rip + proc_PAT$14_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname88]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname88]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname89: .string "PAT$15"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname89]
  lea rsi, [rip + proc_PAT$15_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname89]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname89]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname90: .string "PAT$16"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname90]
  lea rsi, [rip + proc_PAT$16_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname90]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname90]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname91: .string "PAT$17"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname91]
  lea rsi, [rip + proc_PAT$17_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname91]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname91]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname92: .string "PAT$18"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname92]
  lea rsi, [rip + proc_PAT$18_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname92]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname92]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname93: .string "PAT$19"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname93]
  lea rsi, [rip + proc_PAT$19_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname93]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname93]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "Push"
  .Lgvan1: .string "x"
  .Lgvan2: .string "Pop"
  .Lgvan3: .string "Unary"
  .Lgvan4: .string "arg"
  .Lgvan5: .string "op"
  .Lgvan6: .string "Binary"
  .Lgvan7: .string "left"
  .Lgvan8: .string "right"
  .Lgvan9: .string "stk"
  .Lgvan10: .string "dummy"
  .Lgvan11: .string "integer"
  .Lgvan12: .string "exponent"
  .Lgvan13: .string "real"
  .Lgvan14: .string "addop"
  .Lgvan15: .string "mulop"
  .Lgvan16: .string "constant"
  .Lgvan17: .string "primary"
  .Lgvan18: .string "factor"
  .Lgvan19: .string "term"
  .Lgvan20: .string "expr"
  .Lgvan21: .string "line"
  .Lgvan22: .string "EXPR$0"
  .Lgvan23: .string "EXPR$1"
  .Lgvan24: .string "EXPR$2"
  .Lgvan25: .string "EXPR$3"
  .Lgvan26: .string "EXPR$4"
  .Lgvan27: .string "EXPR$5"
  .Lgvan28: .string "EXPR$6"
  .Lgvan29: .string "EXPR$7"
  .Lgvan30: .string "EXPR$8"
  .Lgvan31: .string "EXPR$9"
  .Lgvan32: .string "EXPR$10"
  .Lgvan33: .string "EXPR$11"
  .Lgvan34: .string "EXPR$12"
  .Lgvan35: .string "EXPR$13"
  .Lgvan36: .string "EXPR$14"
  .Lgvan37: .string "EXPR$15"
  .Lgvan38: .string "EXPR$16"
  .Lgvan39: .string "EXPR$17"
  .Lgvan40: .string "EXPR$18"
  .Lgvan41: .string "EXPR$19"
  .Lgvan42: .string "EXPR$20"
  .Lgvan43: .string "EXPR$21"
  .Lgvan44: .string "EXPR$22"
  .Lgvan45: .string "EXPR$23"
  .Lgvan46: .string "EXPR$24"
  .Lgvan47: .string "EXPR$25"
  .Lgvan48: .string "EXPR$26"
  .Lgvan49: .string "EXPR$27"
  .Lgvan50: .string "EXPR$28"
  .Lgvan51: .string "EXPR$29"
  .Lgvan52: .string "EXPR$30"
  .Lgvan53: .string "EXPR$31"
  .Lgvan54: .string "EXPR$32"
  .Lgvan55: .string "EXPR$33"
  .Lgvan56: .string "EXPR$34"
  .Lgvan57: .string "EXPR$35"
  .Lgvan58: .string "EXPR$36"
  .Lgvan59: .string "EXPR$37"
  .Lgvan60: .string "EXPR$38"
  .Lgvan61: .string "EXPR$39"
  .Lgvan62: .string "EXPR$40"
  .Lgvan63: .string "EXPR$41"
  .Lgvan64: .string "EXPR$42"
  .Lgvan65: .string "EXPR$43"
  .Lgvan66: .string "EXPR$44"
  .Lgvan67: .string "EXPR$45"
  .Lgvan68: .string "EXPR$46"
  .Lgvan69: .string "EXPR$47"
  .Lgvan70: .string "EXPR$48"
  .Lgvan71: .string "EXPR$49"
  .Lgvan72: .string "EXPR$50"
  .Lgvan73: .string "EXPR$51"
  .Lgvan74: .string "EXPR$52"
  .Lgvan75: .string "EXPR$53"
  .Lgvan76: .string "EXPR$54"
  .Lgvan77: .string "EXPR$55"
  .Lgvan78: .string "EXPR$56"
  .Lgvan79: .string "EXPR$57"
  .Lgvan80: .string "EXPR$58"
  .Lgvan81: .string "EXPR$59"
  .Lgvan82: .string "EXPR$60"
  .Lgvan83: .string "EXPR$61"
  .Lgvan84: .string "EXPR$62"
  .Lgvan85: .string "EXPR$63"
  .Lgvan86: .string "EXPR$64"
  .Lgvan87: .string "EXPR$65"
  .Lgvan88: .string "EXPR$66"
  .Lgvan89: .string "EXPR$67"
  .Lgvan90: .string "EXPR$68"
  .Lgvan91: .string "EXPR$69"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .quad .Lgvan6
  .quad .Lgvan7
  .quad .Lgvan8
  .quad .Lgvan9
  .quad .Lgvan10
  .quad .Lgvan11
  .quad .Lgvan12
  .quad .Lgvan13
  .quad .Lgvan14
  .quad .Lgvan15
  .quad .Lgvan16
  .quad .Lgvan17
  .quad .Lgvan18
  .quad .Lgvan19
  .quad .Lgvan20
  .quad .Lgvan21
  .quad .Lgvan22
  .quad .Lgvan23
  .quad .Lgvan24
  .quad .Lgvan25
  .quad .Lgvan26
  .quad .Lgvan27
  .quad .Lgvan28
  .quad .Lgvan29
  .quad .Lgvan30
  .quad .Lgvan31
  .quad .Lgvan32
  .quad .Lgvan33
  .quad .Lgvan34
  .quad .Lgvan35
  .quad .Lgvan36
  .quad .Lgvan37
  .quad .Lgvan38
  .quad .Lgvan39
  .quad .Lgvan40
  .quad .Lgvan41
  .quad .Lgvan42
  .quad .Lgvan43
  .quad .Lgvan44
  .quad .Lgvan45
  .quad .Lgvan46
  .quad .Lgvan47
  .quad .Lgvan48
  .quad .Lgvan49
  .quad .Lgvan50
  .quad .Lgvan51
  .quad .Lgvan52
  .quad .Lgvan53
  .quad .Lgvan54
  .quad .Lgvan55
  .quad .Lgvan56
  .quad .Lgvan57
  .quad .Lgvan58
  .quad .Lgvan59
  .quad .Lgvan60
  .quad .Lgvan61
  .quad .Lgvan62
  .quad .Lgvan63
  .quad .Lgvan64
  .quad .Lgvan65
  .quad .Lgvan66
  .quad .Lgvan67
  .quad .Lgvan68
  .quad .Lgvan69
  .quad .Lgvan70
  .quad .Lgvan71
  .quad .Lgvan72
  .quad .Lgvan73
  .quad .Lgvan74
  .quad .Lgvan75
  .quad .Lgvan76
  .quad .Lgvan77
  .quad .Lgvan78
  .quad .Lgvan79
  .quad .Lgvan80
  .quad .Lgvan81
  .quad .Lgvan82
  .quad .Lgvan83
  .quad .Lgvan84
  .quad .Lgvan85
  .quad .Lgvan86
  .quad .Lgvan87
  .quad .Lgvan88
  .quad .Lgvan89
  .quad .Lgvan90
  .quad .Lgvan91
  .section .bss
  .align 16
__gva: .space 1472, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 92
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call main_α
  xor eax, eax
  mov rsp, rbp
  pop rbp
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 5744], rax
 pop rsi
main_α_body:
 xchain559_n0_α:
# BOX IR_CALL TABLE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
  .section .rodata
  .Lrkfn561: .string "TABLE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn561]
 lea rsi, [r12 + 128]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain559_n2_α
 jmp xchain559_n1_α
 xchain559_n0_β:
 jmp xchain559_n2_α
# IR_ASSIGN gva
 xchain559_n1_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 144], rax
 mov qword ptr [rbx + 152], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain559_n2_α
 xchain559_n1_β:
 jmp xchain559_n2_α
# IR_LIT_STRING
 xchain559_n2_α:
 mov qword ptr [r12 + 2480], 1
 mov rax, qword ptr [rip + .Lx563_0]
 mov qword ptr [r12 + 2488], rax
 jmp xchain559_n3_α
 xchain559_n2_β:
 jmp xchain559_n5_α
.Lx563_0:
 .quad .Lx563_0_s
.Lx563_0_s:
 .string "PAT$0"
 xchain559_n3_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2480] -> [zr+2448]
 mov rax, qword ptr [r12 + 2480]
 mov qword ptr [r12 + 2448], rax
 mov rax, qword ptr [r12 + 2488]
 mov qword ptr [r12 + 2456], rax
  .section .rodata
  .Lrkfn565: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn565]
 lea rsi, [r12 + 2448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2432], rax
 mov qword ptr [r12 + 2440], rdx
 cmp eax, 99
 je xchain559_n5_α
 jmp xchain559_n4_α
 xchain559_n3_β:
 jmp xchain559_n5_α
# IR_ASSIGN gva
 xchain559_n4_α:
 mov rax, qword ptr [r12 + 2432]
 mov rdx, qword ptr [r12 + 2440]
 mov qword ptr [rbx + 176], rax
 mov qword ptr [rbx + 184], rdx
 mov qword ptr [r12 + 2416], rax
 mov qword ptr [r12 + 2424], rdx
 jmp xchain559_n5_α
 xchain559_n4_β:
 jmp xchain559_n5_α
# IR_LIT_STRING
 xchain559_n5_α:
 mov qword ptr [r12 + 2576], 1
 mov rax, qword ptr [rip + .Lx567_0]
 mov qword ptr [r12 + 2584], rax
 jmp xchain559_n6_α
 xchain559_n5_β:
 jmp xchain559_n8_α
.Lx567_0:
 .quad .Lx567_0_s
.Lx567_0_s:
 .string "PAT$1"
 xchain559_n6_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2576] -> [zr+2544]
 mov rax, qword ptr [r12 + 2576]
 mov qword ptr [r12 + 2544], rax
 mov rax, qword ptr [r12 + 2584]
 mov qword ptr [r12 + 2552], rax
  .section .rodata
  .Lrkfn569: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn569]
 lea rsi, [r12 + 2544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2528], rax
 mov qword ptr [r12 + 2536], rdx
 cmp eax, 99
 je xchain559_n8_α
 jmp xchain559_n7_α
 xchain559_n6_β:
 jmp xchain559_n8_α
# IR_ASSIGN gva
 xchain559_n7_α:
 mov rax, qword ptr [r12 + 2528]
 mov rdx, qword ptr [r12 + 2536]
 mov qword ptr [rbx + 192], rax
 mov qword ptr [rbx + 200], rdx
 mov qword ptr [r12 + 2512], rax
 mov qword ptr [r12 + 2520], rdx
 jmp xchain559_n8_α
 xchain559_n7_β:
 jmp xchain559_n8_α
# IR_LIT_STRING
 xchain559_n8_α:
 mov qword ptr [r12 + 2672], 1
 mov rax, qword ptr [rip + .Lx571_0]
 mov qword ptr [r12 + 2680], rax
 jmp xchain559_n9_α
 xchain559_n8_β:
 jmp xchain559_n11_α
.Lx571_0:
 .quad .Lx571_0_s
.Lx571_0_s:
 .string "PAT$2"
 xchain559_n9_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2672] -> [zr+2640]
 mov rax, qword ptr [r12 + 2672]
 mov qword ptr [r12 + 2640], rax
 mov rax, qword ptr [r12 + 2680]
 mov qword ptr [r12 + 2648], rax
  .section .rodata
  .Lrkfn573: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn573]
 lea rsi, [r12 + 2640]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2624], rax
 mov qword ptr [r12 + 2632], rdx
 cmp eax, 99
 je xchain559_n11_α
 jmp xchain559_n10_α
 xchain559_n9_β:
 jmp xchain559_n11_α
# IR_ASSIGN gva
 xchain559_n10_α:
 mov rax, qword ptr [r12 + 2624]
 mov rdx, qword ptr [r12 + 2632]
 mov qword ptr [rbx + 208], rax
 mov qword ptr [rbx + 216], rdx
 mov qword ptr [r12 + 2608], rax
 mov qword ptr [r12 + 2616], rdx
 jmp xchain559_n11_α
 xchain559_n10_β:
 jmp xchain559_n11_α
# IR_LIT_INTEGER
 xchain559_n11_α:
 mov qword ptr [r12 + 2800], 6
 mov rax, qword ptr [rip + .Lx575_0]
 mov qword ptr [r12 + 2808], rax
 jmp xchain559_n12_α
 xchain559_n11_β:
 jmp xchain559_n17_α
.Lx575_0:
 .quad 42
# IR_LIT_STRING
 xchain559_n12_α:
 mov qword ptr [r12 + 2832], 1
 mov rax, qword ptr [rip + .Lx576_0]
 mov qword ptr [r12 + 2840], rax
 jmp xchain559_n13_α
 xchain559_n12_β:
 jmp xchain559_n17_α
.Lx576_0:
 .quad .Lx576_0_s
.Lx576_0_s:
 .string "*EXPR$0"
# IR_LIT_INTEGER
 xchain559_n13_α:
 mov qword ptr [r12 + 2928], 6
 mov rax, qword ptr [rip + .Lx577_0]
 mov qword ptr [r12 + 2936], rax
 jmp xchain559_n14_α
 xchain559_n13_β:
 jmp xchain559_n17_α
.Lx577_0:
 .quad 28
# IR_LIT_STRING
 xchain559_n14_α:
 mov qword ptr [r12 + 2960], 1
 mov rax, qword ptr [rip + .Lx578_0]
 mov qword ptr [r12 + 2968], rax
 jmp xchain559_n15_α
 xchain559_n14_β:
 jmp xchain559_n17_α
.Lx578_0:
 .quad .Lx578_0_s
.Lx578_0_s:
 .string "+-"
 xchain559_n15_α:
# BOX CALL SNO$PBK(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2928] -> [zr+2880]
 mov rax, qword ptr [r12 + 2928]
 mov qword ptr [r12 + 2880], rax
 mov rax, qword ptr [r12 + 2936]
 mov qword ptr [r12 + 2888], rax
# marshal arg1 = producer-box slot [zr+2960] -> [zr+2896]
 mov rax, qword ptr [r12 + 2960]
 mov qword ptr [r12 + 2896], rax
 mov rax, qword ptr [r12 + 2968]
 mov qword ptr [r12 + 2904], rax
  .section .rodata
  .Lbynamefn369: .string "SNO$PBK"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn369]
 lea rsi, [r12 + 2880]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2864], rax
 mov qword ptr [r12 + 2872], rdx
 cmp eax, 99
 je xchain559_n17_α
 jmp xchain559_n16_α
 xchain559_n15_β:
 jmp xchain559_n17_α
 xchain559_n16_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2800] -> [zr+2736]
 mov rax, qword ptr [r12 + 2800]
 mov qword ptr [r12 + 2736], rax
 mov rax, qword ptr [r12 + 2808]
 mov qword ptr [r12 + 2744], rax
# marshal arg1 = producer-box slot [zr+2832] -> [zr+2752]
 mov rax, qword ptr [r12 + 2832]
 mov qword ptr [r12 + 2752], rax
 mov rax, qword ptr [r12 + 2840]
 mov qword ptr [r12 + 2760], rax
# marshal arg2 = producer-box slot [zr+2864] -> [zr+2768]
 mov rax, qword ptr [r12 + 2864]
 mov qword ptr [r12 + 2768], rax
 mov rax, qword ptr [r12 + 2872]
 mov qword ptr [r12 + 2776], rax
  .section .rodata
  .Lbynamefn370: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn370]
 lea rsi, [r12 + 2736]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2720], rax
 mov qword ptr [r12 + 2728], rdx
 cmp eax, 99
 je xchain559_n17_α
 jmp xchain559_n18_α
 xchain559_n16_β:
 jmp xchain559_n17_α
# IR_LIT_INTEGER
 xchain559_n17_α:
 mov qword ptr [r12 + 3088], 6
 mov rax, qword ptr [rip + .Lx581_0]
 mov qword ptr [r12 + 3096], rax
 jmp xchain559_n19_α
 xchain559_n17_β:
 jmp xchain559_n24_α
.Lx581_0:
 .quad 42
# IR_ASSIGN gva
 xchain559_n18_α:
 mov rax, qword ptr [r12 + 2720]
 mov rdx, qword ptr [r12 + 2728]
 mov qword ptr [rbx + 224], rax
 mov qword ptr [rbx + 232], rdx
 mov qword ptr [r12 + 2704], rax
 mov qword ptr [r12 + 2712], rdx
 jmp xchain559_n17_α
 xchain559_n18_β:
 jmp xchain559_n17_α
# IR_LIT_STRING
 xchain559_n19_α:
 mov qword ptr [r12 + 3120], 1
 mov rax, qword ptr [rip + .Lx583_0]
 mov qword ptr [r12 + 3128], rax
 jmp xchain559_n20_α
 xchain559_n19_β:
 jmp xchain559_n24_α
.Lx583_0:
 .quad .Lx583_0_s
.Lx583_0_s:
 .string "*EXPR$1"
# IR_LIT_INTEGER
 xchain559_n20_α:
 mov qword ptr [r12 + 3216], 6
 mov rax, qword ptr [rip + .Lx584_0]
 mov qword ptr [r12 + 3224], rax
 jmp xchain559_n21_α
 xchain559_n20_β:
 jmp xchain559_n24_α
.Lx584_0:
 .quad 28
# IR_LIT_STRING
 xchain559_n21_α:
 mov qword ptr [r12 + 3248], 1
 mov rax, qword ptr [rip + .Lx585_0]
 mov qword ptr [r12 + 3256], rax
 jmp xchain559_n22_α
 xchain559_n21_β:
 jmp xchain559_n24_α
.Lx585_0:
 .quad .Lx585_0_s
.Lx585_0_s:
 .string "*/"
 xchain559_n22_α:
# BOX CALL SNO$PBK(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3216] -> [zr+3168]
 mov rax, qword ptr [r12 + 3216]
 mov qword ptr [r12 + 3168], rax
 mov rax, qword ptr [r12 + 3224]
 mov qword ptr [r12 + 3176], rax
# marshal arg1 = producer-box slot [zr+3248] -> [zr+3184]
 mov rax, qword ptr [r12 + 3248]
 mov qword ptr [r12 + 3184], rax
 mov rax, qword ptr [r12 + 3256]
 mov qword ptr [r12 + 3192], rax
  .section .rodata
  .Lbynamefn376: .string "SNO$PBK"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn376]
 lea rsi, [r12 + 3168]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3152], rax
 mov qword ptr [r12 + 3160], rdx
 cmp eax, 99
 je xchain559_n24_α
 jmp xchain559_n23_α
 xchain559_n22_β:
 jmp xchain559_n24_α
 xchain559_n23_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3088] -> [zr+3024]
 mov rax, qword ptr [r12 + 3088]
 mov qword ptr [r12 + 3024], rax
 mov rax, qword ptr [r12 + 3096]
 mov qword ptr [r12 + 3032], rax
# marshal arg1 = producer-box slot [zr+3120] -> [zr+3040]
 mov rax, qword ptr [r12 + 3120]
 mov qword ptr [r12 + 3040], rax
 mov rax, qword ptr [r12 + 3128]
 mov qword ptr [r12 + 3048], rax
# marshal arg2 = producer-box slot [zr+3152] -> [zr+3056]
 mov rax, qword ptr [r12 + 3152]
 mov qword ptr [r12 + 3056], rax
 mov rax, qword ptr [r12 + 3160]
 mov qword ptr [r12 + 3064], rax
  .section .rodata
  .Lbynamefn377: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn377]
 lea rsi, [r12 + 3024]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3008], rax
 mov qword ptr [r12 + 3016], rdx
 cmp eax, 99
 je xchain559_n24_α
 jmp xchain559_n25_α
 xchain559_n23_β:
 jmp xchain559_n24_α
# IR_LIT_INTEGER
 xchain559_n24_α:
 mov qword ptr [r12 + 3376], 6
 mov rax, qword ptr [rip + .Lx588_0]
 mov qword ptr [r12 + 3384], rax
 jmp xchain559_n26_α
 xchain559_n24_β:
 jmp xchain559_n29_α
.Lx588_0:
 .quad 42
# IR_ASSIGN gva
 xchain559_n25_α:
 mov rax, qword ptr [r12 + 3008]
 mov rdx, qword ptr [r12 + 3016]
 mov qword ptr [rbx + 240], rax
 mov qword ptr [rbx + 248], rdx
 mov qword ptr [r12 + 2992], rax
 mov qword ptr [r12 + 3000], rdx
 jmp xchain559_n24_α
 xchain559_n25_β:
 jmp xchain559_n24_α
# IR_LIT_STRING
 xchain559_n26_α:
 mov qword ptr [r12 + 3408], 1
 mov rax, qword ptr [rip + .Lx590_0]
 mov qword ptr [r12 + 3416], rax
 jmp xchain559_n27_α
 xchain559_n26_β:
 jmp xchain559_n29_α
.Lx590_0:
 .quad .Lx590_0_s
.Lx590_0_s:
 .string "*EXPR$2"
# IR_VAR
 xchain559_n27_α:
 mov rax, qword ptr [rbx + 208]
 mov rdx, qword ptr [rbx + 216]
 mov qword ptr [r12 + 3504], rax
 mov qword ptr [r12 + 3512], rdx
 jmp xchain559_n28_α
 xchain559_n27_β:
 jmp xchain559_n29_α
# IR_VAR
 xchain559_n28_α:
 mov rax, qword ptr [rbx + 176]
 mov rdx, qword ptr [rbx + 184]
 mov qword ptr [r12 + 3536], rax
 mov qword ptr [r12 + 3544], rdx
 jmp xchain559_n30_α
 xchain559_n28_β:
 jmp xchain559_n29_α
# IR_LIT_STRING
 xchain559_n29_α:
 mov qword ptr [r12 + 3632], 1
 mov rax, qword ptr [rip + .Lx593_0]
 mov qword ptr [r12 + 3640], rax
 jmp xchain559_n31_α
 xchain559_n29_β:
 jmp xchain559_n34_α
.Lx593_0:
 .quad .Lx593_0_s
.Lx593_0_s:
 .string "PAT$3"
 xchain559_n30_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3504] -> [zr+3456]
 mov rax, qword ptr [r12 + 3504]
 mov qword ptr [r12 + 3456], rax
 mov rax, qword ptr [r12 + 3512]
 mov qword ptr [r12 + 3464], rax
# marshal arg1 = producer-box slot [zr+3536] -> [zr+3472]
 mov rax, qword ptr [r12 + 3536]
 mov qword ptr [r12 + 3472], rax
 mov rax, qword ptr [r12 + 3544]
 mov qword ptr [r12 + 3480], rax
  .section .rodata
  .Lbynamefn384: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn384]
 lea rsi, [r12 + 3456]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3440], rax
 mov qword ptr [r12 + 3448], rdx
 cmp eax, 99
 je xchain559_n29_α
 jmp xchain559_n32_α
 xchain559_n30_β:
 jmp xchain559_n29_α
 xchain559_n31_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3632] -> [zr+3600]
 mov rax, qword ptr [r12 + 3632]
 mov qword ptr [r12 + 3600], rax
 mov rax, qword ptr [r12 + 3640]
 mov qword ptr [r12 + 3608], rax
  .section .rodata
  .Lrkfn596: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn596]
 lea rsi, [r12 + 3600]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3584], rax
 mov qword ptr [r12 + 3592], rdx
 cmp eax, 99
 je xchain559_n34_α
 jmp xchain559_n33_α
 xchain559_n31_β:
 jmp xchain559_n34_α
 xchain559_n32_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3376] -> [zr+3312]
 mov rax, qword ptr [r12 + 3376]
 mov qword ptr [r12 + 3312], rax
 mov rax, qword ptr [r12 + 3384]
 mov qword ptr [r12 + 3320], rax
# marshal arg1 = producer-box slot [zr+3408] -> [zr+3328]
 mov rax, qword ptr [r12 + 3408]
 mov qword ptr [r12 + 3328], rax
 mov rax, qword ptr [r12 + 3416]
 mov qword ptr [r12 + 3336], rax
# marshal arg2 = producer-box slot [zr+3440] -> [zr+3344]
 mov rax, qword ptr [r12 + 3440]
 mov qword ptr [r12 + 3344], rax
 mov rax, qword ptr [r12 + 3448]
 mov qword ptr [r12 + 3352], rax
  .section .rodata
  .Lbynamefn386: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn386]
 lea rsi, [r12 + 3312]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3296], rax
 mov qword ptr [r12 + 3304], rdx
 cmp eax, 99
 je xchain559_n29_α
 jmp xchain559_n35_α
 xchain559_n32_β:
 jmp xchain559_n29_α
# IR_ASSIGN gva
 xchain559_n33_α:
 mov rax, qword ptr [r12 + 3584]
 mov rdx, qword ptr [r12 + 3592]
 mov qword ptr [rbx + 272], rax
 mov qword ptr [rbx + 280], rdx
 mov qword ptr [r12 + 3568], rax
 mov qword ptr [r12 + 3576], rdx
 jmp xchain559_n34_α
 xchain559_n33_β:
 jmp xchain559_n34_α
# IR_VAR
 xchain559_n34_α:
 mov rax, qword ptr [rbx + 224]
 mov rdx, qword ptr [rbx + 232]
 mov qword ptr [r12 + 3776], rax
 mov qword ptr [r12 + 3784], rdx
 jmp xchain559_n36_α
 xchain559_n34_β:
 jmp xchain559_n37_α
# IR_ASSIGN gva
 xchain559_n35_α:
 mov rax, qword ptr [r12 + 3296]
 mov rdx, qword ptr [r12 + 3304]
 mov qword ptr [rbx + 256], rax
 mov qword ptr [rbx + 264], rdx
 mov qword ptr [r12 + 3280], rax
 mov qword ptr [r12 + 3288], rdx
 jmp xchain559_n29_α
 xchain559_n35_β:
 jmp xchain559_n29_α
# IR_LIT_INTEGER
 xchain559_n36_α:
 mov qword ptr [r12 + 3888], 6
 mov rax, qword ptr [rip + .Lx601_0]
 mov qword ptr [r12 + 3896], rax
 jmp xchain559_n38_α
 xchain559_n36_β:
 jmp xchain559_n37_α
.Lx601_0:
 .quad 42
# IR_LIT_STRING
 xchain559_n37_α:
 mov qword ptr [r12 + 4304], 1
 mov rax, qword ptr [rip + .Lx602_0]
 mov qword ptr [r12 + 4312], rax
 jmp xchain559_n39_α
 xchain559_n37_β:
 jmp xchain559_n42_α
.Lx602_0:
 .quad .Lx602_0_s
.Lx602_0_s:
 .string "EXPR$6"
# IR_LIT_STRING
 xchain559_n38_α:
 mov qword ptr [r12 + 3920], 1
 mov rax, qword ptr [rip + .Lx603_0]
 mov qword ptr [r12 + 3928], rax
 jmp xchain559_n40_α
 xchain559_n38_β:
 jmp xchain559_n37_α
.Lx603_0:
 .quad .Lx603_0_s
.Lx603_0_s:
 .string "*EXPR$3"
 xchain559_n39_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4304] -> [zr+4272]
 mov rax, qword ptr [r12 + 4304]
 mov qword ptr [r12 + 4272], rax
 mov rax, qword ptr [r12 + 4312]
 mov qword ptr [r12 + 4280], rax
  .section .rodata
  .Lrkfn605: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn605]
 lea rsi, [r12 + 4272]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 4256], rax
 mov qword ptr [r12 + 4264], rdx
 cmp eax, 99
 je xchain559_n42_α
 jmp xchain559_n41_α
 xchain559_n39_β:
 jmp xchain559_n42_α
# IR_LIT_STRING
 xchain559_n40_α:
 mov qword ptr [r12 + 4000], 1
 mov rax, qword ptr [rip + .Lx606_0]
 mov qword ptr [r12 + 4008], rax
 jmp xchain559_n43_α
 xchain559_n40_β:
 jmp xchain559_n37_α
.Lx606_0:
 .quad .Lx606_0_s
.Lx606_0_s:
 .string "EXPR$4"
# IR_VAR
 xchain559_n41_α:
 mov rax, qword ptr [rbx + 240]
 mov rdx, qword ptr [rbx + 248]
 mov qword ptr [r12 + 4336], rax
 mov qword ptr [r12 + 4344], rdx
 jmp xchain559_n44_α
 xchain559_n41_β:
 jmp xchain559_n42_α
# IR_LIT_STRING
 xchain559_n42_α:
 mov qword ptr [r12 + 4864], 1
 mov rax, qword ptr [rip + .Lx608_0]
 mov qword ptr [r12 + 4872], rax
 jmp xchain559_n45_α
 xchain559_n42_β:
 jmp xchain559_n49_α
.Lx608_0:
 .quad .Lx608_0_s
.Lx608_0_s:
 .string "EXPR$10"
 xchain559_n43_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4000] -> [zr+3968]
 mov rax, qword ptr [r12 + 4000]
 mov qword ptr [r12 + 3968], rax
 mov rax, qword ptr [r12 + 4008]
 mov qword ptr [r12 + 3976], rax
  .section .rodata
  .Lrkfn610: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn610]
 lea rsi, [r12 + 3968]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3952], rax
 mov qword ptr [r12 + 3960], rdx
 cmp eax, 99
 je xchain559_n37_α
 jmp xchain559_n46_α
 xchain559_n43_β:
 jmp xchain559_n37_α
 xchain559_n44_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 4256]
 mov rsi, qword ptr [r12 + 4264]
 mov rdx, qword ptr [r12 + 4336]
 mov rcx, qword ptr [r12 + 4344]
 call str_concat_d@PLT
 mov qword ptr [r12 + 4224], rax
 mov qword ptr [r12 + 4232], rdx
 jmp xchain559_n47_α
 xchain559_n44_β:
 jmp xchain559_n42_α
 xchain559_n45_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4864] -> [zr+4832]
 mov rax, qword ptr [r12 + 4864]
 mov qword ptr [r12 + 4832], rax
 mov rax, qword ptr [r12 + 4872]
 mov qword ptr [r12 + 4840], rax
  .section .rodata
  .Lrkfn613: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn613]
 lea rsi, [r12 + 4832]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 4816], rax
 mov qword ptr [r12 + 4824], rdx
 cmp eax, 99
 je xchain559_n49_α
 jmp xchain559_n48_α
 xchain559_n45_β:
 jmp xchain559_n49_α
 xchain559_n46_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3888] -> [zr+3824]
 mov rax, qword ptr [r12 + 3888]
 mov qword ptr [r12 + 3824], rax
 mov rax, qword ptr [r12 + 3896]
 mov qword ptr [r12 + 3832], rax
# marshal arg1 = producer-box slot [zr+3920] -> [zr+3840]
 mov rax, qword ptr [r12 + 3920]
 mov qword ptr [r12 + 3840], rax
 mov rax, qword ptr [r12 + 3928]
 mov qword ptr [r12 + 3848], rax
# marshal arg2 = producer-box slot [zr+3952] -> [zr+3856]
 mov rax, qword ptr [r12 + 3952]
 mov qword ptr [r12 + 3856], rax
 mov rax, qword ptr [r12 + 3960]
 mov qword ptr [r12 + 3864], rax
  .section .rodata
  .Lbynamefn400: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn400]
 lea rsi, [r12 + 3824]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3808], rax
 mov qword ptr [r12 + 3816], rdx
 cmp eax, 99
 je xchain559_n37_α
 jmp xchain559_n50_α
 xchain559_n46_β:
 jmp xchain559_n37_α
# IR_LIT_INTEGER
 xchain559_n47_α:
 mov qword ptr [r12 + 4448], 6
 mov rax, qword ptr [rip + .Lx615_0]
 mov qword ptr [r12 + 4456], rax
 jmp xchain559_n51_α
 xchain559_n47_β:
 jmp xchain559_n42_α
.Lx615_0:
 .quad 42
# IR_VAR
 xchain559_n48_α:
 mov rax, qword ptr [rbx + 224]
 mov rdx, qword ptr [rbx + 232]
 mov qword ptr [r12 + 4896], rax
 mov qword ptr [r12 + 4904], rdx
 jmp xchain559_n52_α
 xchain559_n48_β:
 jmp xchain559_n49_α
# IR_LIT_STRING
 xchain559_n49_α:
 mov qword ptr [r12 + 5296], 1
 mov rax, qword ptr [rip + .Lx617_0]
 mov qword ptr [r12 + 5304], rax
 jmp xchain559_n53_α
 xchain559_n49_β:
 jmp xchain559_n61_α
.Lx617_0:
 .quad .Lx617_0_s
.Lx617_0_s:
 .string "TRIM"
 xchain559_n50_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 3776]
 mov rsi, qword ptr [r12 + 3784]
 mov rdx, qword ptr [r12 + 3808]
 mov rcx, qword ptr [r12 + 3816]
 call str_concat_d@PLT
 mov qword ptr [r12 + 3744], rax
 mov qword ptr [r12 + 3752], rdx
 jmp xchain559_n54_α
 xchain559_n50_β:
 jmp xchain559_n37_α
# IR_LIT_STRING
 xchain559_n51_α:
 mov qword ptr [r12 + 4480], 1
 mov rax, qword ptr [rip + .Lx619_0]
 mov qword ptr [r12 + 4488], rax
 jmp xchain559_n55_α
 xchain559_n51_β:
 jmp xchain559_n42_α
.Lx619_0:
 .quad .Lx619_0_s
.Lx619_0_s:
 .string "*EXPR$7"
 xchain559_n52_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 4816]
 mov rsi, qword ptr [r12 + 4824]
 mov rdx, qword ptr [r12 + 4896]
 mov rcx, qword ptr [r12 + 4904]
 call str_concat_d@PLT
 mov qword ptr [r12 + 4784], rax
 mov qword ptr [r12 + 4792], rdx
 jmp xchain559_n56_α
 xchain559_n52_β:
 jmp xchain559_n49_α
# IR_LIT_INTEGER
 xchain559_n53_α:
 mov qword ptr [r12 + 5328], 6
 mov rax, qword ptr [rip + .Lx621_0]
 mov qword ptr [r12 + 5336], rax
 jmp xchain559_n57_α
 xchain559_n53_β:
 jmp xchain559_n61_α
.Lx621_0:
 .quad 1
# IR_LIT_STRING
 xchain559_n54_α:
 mov qword ptr [r12 + 4080], 1
 mov rax, qword ptr [rip + .Lx622_0]
 mov qword ptr [r12 + 4088], rax
 jmp xchain559_n58_α
 xchain559_n54_β:
 jmp xchain559_n37_α
.Lx622_0:
 .quad .Lx622_0_s
.Lx622_0_s:
 .string "EXPR$5"
# IR_LIT_STRING
 xchain559_n55_α:
 mov qword ptr [r12 + 4560], 1
 mov rax, qword ptr [rip + .Lx623_0]
 mov qword ptr [r12 + 4568], rax
 jmp xchain559_n59_α
 xchain559_n55_β:
 jmp xchain559_n42_α
.Lx623_0:
 .quad .Lx623_0_s
.Lx623_0_s:
 .string "EXPR$8"
# IR_LIT_INTEGER
 xchain559_n56_α:
 mov qword ptr [r12 + 5008], 6
 mov rax, qword ptr [rip + .Lx624_0]
 mov qword ptr [r12 + 5016], rax
 jmp xchain559_n60_α
 xchain559_n56_β:
 jmp xchain559_n49_α
.Lx624_0:
 .quad 42
 xchain559_n57_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+5296] -> [zr+5248]
 mov rax, qword ptr [r12 + 5296]
 mov qword ptr [r12 + 5248], rax
 mov rax, qword ptr [r12 + 5304]
 mov qword ptr [r12 + 5256], rax
# marshal arg1 = producer-box slot [zr+5328] -> [zr+5264]
 mov rax, qword ptr [r12 + 5328]
 mov qword ptr [r12 + 5264], rax
 mov rax, qword ptr [r12 + 5336]
 mov qword ptr [r12 + 5272], rax
  .section .rodata
  .Lrkfn626: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn626]
 lea rsi, [r12 + 5248]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 5232], rax
 mov qword ptr [r12 + 5240], rdx
 cmp eax, 99
 je xchain559_n61_α
 jmp xchain559_n61_α
 xchain559_n57_β:
 jmp xchain559_n61_α
 xchain559_n58_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4080] -> [zr+4048]
 mov rax, qword ptr [r12 + 4080]
 mov qword ptr [r12 + 4048], rax
 mov rax, qword ptr [r12 + 4088]
 mov qword ptr [r12 + 4056], rax
  .section .rodata
  .Lrkfn628: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn628]
 lea rsi, [r12 + 4048]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 4032], rax
 mov qword ptr [r12 + 4040], rdx
 cmp eax, 99
 je xchain559_n37_α
 jmp xchain559_n62_α
 xchain559_n58_β:
 jmp xchain559_n37_α
 xchain559_n59_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4560] -> [zr+4528]
 mov rax, qword ptr [r12 + 4560]
 mov qword ptr [r12 + 4528], rax
 mov rax, qword ptr [r12 + 4568]
 mov qword ptr [r12 + 4536], rax
  .section .rodata
  .Lrkfn630: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn630]
 lea rsi, [r12 + 4528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 4512], rax
 mov qword ptr [r12 + 4520], rdx
 cmp eax, 99
 je xchain559_n42_α
 jmp xchain559_n63_α
 xchain559_n59_β:
 jmp xchain559_n42_α
# IR_LIT_STRING
 xchain559_n60_α:
 mov qword ptr [r12 + 5040], 1
 mov rax, qword ptr [rip + .Lx631_0]
 mov qword ptr [r12 + 5048], rax
 jmp xchain559_n64_α
 xchain559_n60_β:
 jmp xchain559_n49_α
.Lx631_0:
 .quad .Lx631_0_s
.Lx631_0_s:
 .string "*EXPR$11"
# IR_VAR
 xchain559_n61_α:
 mov rdi, qword ptr [rip + .Lx632_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 5376], rax
 mov qword ptr [r12 + 5384], rdx
 jmp xchain559_n65_α
 xchain559_n61_β:
 jmp main_γ
.Lx632_0:
 .quad .Lx632_0_s
.Lx632_0_s:
 .string "INPUT"
 xchain559_n62_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3744] -> [zr+3696]
 mov rax, qword ptr [r12 + 3744]
 mov qword ptr [r12 + 3696], rax
 mov rax, qword ptr [r12 + 3752]
 mov qword ptr [r12 + 3704], rax
# marshal arg1 = producer-box slot [zr+4032] -> [zr+3712]
 mov rax, qword ptr [r12 + 4032]
 mov qword ptr [r12 + 3712], rax
 mov rax, qword ptr [r12 + 4040]
 mov qword ptr [r12 + 3720], rax
  .section .rodata
  .Lbynamefn416: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn416]
 lea rsi, [r12 + 3696]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3680], rax
 mov qword ptr [r12 + 3688], rdx
 cmp eax, 99
 je xchain559_n37_α
 jmp xchain559_n66_α
 xchain559_n62_β:
 jmp xchain559_n37_α
 xchain559_n63_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+4448] -> [zr+4384]
 mov rax, qword ptr [r12 + 4448]
 mov qword ptr [r12 + 4384], rax
 mov rax, qword ptr [r12 + 4456]
 mov qword ptr [r12 + 4392], rax
# marshal arg1 = producer-box slot [zr+4480] -> [zr+4400]
 mov rax, qword ptr [r12 + 4480]
 mov qword ptr [r12 + 4400], rax
 mov rax, qword ptr [r12 + 4488]
 mov qword ptr [r12 + 4408], rax
# marshal arg2 = producer-box slot [zr+4512] -> [zr+4416]
 mov rax, qword ptr [r12 + 4512]
 mov qword ptr [r12 + 4416], rax
 mov rax, qword ptr [r12 + 4520]
 mov qword ptr [r12 + 4424], rax
  .section .rodata
  .Lbynamefn417: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn417]
 lea rsi, [r12 + 4384]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 4368], rax
 mov qword ptr [r12 + 4376], rdx
 cmp eax, 99
 je xchain559_n42_α
 jmp xchain559_n67_α
 xchain559_n63_β:
 jmp xchain559_n42_α
# IR_LIT_STRING
 xchain559_n64_α:
 mov qword ptr [r12 + 5120], 1
 mov rax, qword ptr [rip + .Lx635_0]
 mov qword ptr [r12 + 5128], rax
 jmp xchain559_n68_α
 xchain559_n64_β:
 jmp xchain559_n49_α
.Lx635_0:
 .quad .Lx635_0_s
.Lx635_0_s:
 .string "EXPR$12"
# IR_ASSIGN gva
 xchain559_n65_α:
 mov rax, qword ptr [r12 + 5376]
 mov rdx, qword ptr [r12 + 5384]
 mov qword ptr [rbx + 336], rax
 mov qword ptr [rbx + 344], rdx
 mov qword ptr [r12 + 5360], rax
 mov qword ptr [r12 + 5368], rdx
 jmp xchain559_n69_α
 xchain559_n65_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain559_n66_α:
 mov rax, qword ptr [r12 + 3680]
 mov rdx, qword ptr [r12 + 3688]
 mov qword ptr [rbx + 288], rax
 mov qword ptr [rbx + 296], rdx
 mov qword ptr [r12 + 3664], rax
 mov qword ptr [r12 + 3672], rdx
 jmp xchain559_n37_α
 xchain559_n66_β:
 jmp xchain559_n37_α
 xchain559_n67_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 4224]
 mov rsi, qword ptr [r12 + 4232]
 mov rdx, qword ptr [r12 + 4368]
 mov rcx, qword ptr [r12 + 4376]
 call str_concat_d@PLT
 mov qword ptr [r12 + 4192], rax
 mov qword ptr [r12 + 4200], rdx
 jmp xchain559_n70_α
 xchain559_n67_β:
 jmp xchain559_n42_α
 xchain559_n68_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+5120] -> [zr+5088]
 mov rax, qword ptr [r12 + 5120]
 mov qword ptr [r12 + 5088], rax
 mov rax, qword ptr [r12 + 5128]
 mov qword ptr [r12 + 5096], rax
  .section .rodata
  .Lrkfn640: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn640]
 lea rsi, [r12 + 5088]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 5072], rax
 mov qword ptr [r12 + 5080], rdx
 cmp eax, 99
 je xchain559_n49_α
 jmp xchain559_n71_α
 xchain559_n68_β:
 jmp xchain559_n49_α
# IR_VAR
 xchain559_n69_α:
 mov rax, qword ptr [rbx + 336]
 mov rdx, qword ptr [rbx + 344]
 mov qword ptr [r12 + 5616], rax
 mov qword ptr [r12 + 5624], rdx
 jmp xchain559_n72_α
 xchain559_n69_β:
 jmp xchain559_n73_α
# IR_LIT_STRING
 xchain559_n70_α:
 mov qword ptr [r12 + 4640], 1
 mov rax, qword ptr [rip + .Lx642_0]
 mov qword ptr [r12 + 4648], rax
 jmp xchain559_n74_α
 xchain559_n70_β:
 jmp xchain559_n42_α
.Lx642_0:
 .quad .Lx642_0_s
.Lx642_0_s:
 .string "EXPR$9"
 xchain559_n71_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+5008] -> [zr+4944]
 mov rax, qword ptr [r12 + 5008]
 mov qword ptr [r12 + 4944], rax
 mov rax, qword ptr [r12 + 5016]
 mov qword ptr [r12 + 4952], rax
# marshal arg1 = producer-box slot [zr+5040] -> [zr+4960]
 mov rax, qword ptr [r12 + 5040]
 mov qword ptr [r12 + 4960], rax
 mov rax, qword ptr [r12 + 5048]
 mov qword ptr [r12 + 4968], rax
# marshal arg2 = producer-box slot [zr+5072] -> [zr+4976]
 mov rax, qword ptr [r12 + 5072]
 mov qword ptr [r12 + 4976], rax
 mov rax, qword ptr [r12 + 5080]
 mov qword ptr [r12 + 4984], rax
  .section .rodata
  .Lbynamefn425: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn425]
 lea rsi, [r12 + 4944]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 4928], rax
 mov qword ptr [r12 + 4936], rdx
 cmp eax, 99
 je xchain559_n49_α
 jmp xchain559_n75_α
 xchain559_n71_β:
 jmp xchain559_n49_α
# IR_MATCH_HEAD
 xchain559_n72_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 5432], rax
 mov qword ptr [r12 + 5440], rsp
 mov rdi, qword ptr [r12 + 5616]
 mov rsi, qword ptr [r12 + 5624]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 5424], 0
.Lx645_0:
 mov r14d, dword ptr [r12 + 5424]
 jmp xchain559_n76_α
 xchain559_n72_β:
 add dword ptr [r12 + 5424], 1
 mov eax, dword ptr [r12 + 5424]
 cmp eax, r15d
 jg .Lx645_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx645_1
 jmp .Lx645_0
.Lx645_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 5432]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 5440]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain559_n73_α
# IR_LIT_STRING
 xchain559_n73_α:
 mov qword ptr [r12 + 5712], 1
 mov rax, qword ptr [rip + .Lx646_0]
 mov qword ptr [r12 + 5720], rax
 jmp xchain559_n77_α
 xchain559_n73_β:
 jmp xchain559_n61_α
.Lx646_0:
 .quad .Lx646_0_s
.Lx646_0_s:
 .string "Bad input, try again"
 xchain559_n74_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4640] -> [zr+4608]
 mov rax, qword ptr [r12 + 4640]
 mov qword ptr [r12 + 4608], rax
 mov rax, qword ptr [r12 + 4648]
 mov qword ptr [r12 + 4616], rax
  .section .rodata
  .Lrkfn648: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn648]
 lea rsi, [r12 + 4608]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 4592], rax
 mov qword ptr [r12 + 4600], rdx
 cmp eax, 99
 je xchain559_n42_α
 jmp xchain559_n78_α
 xchain559_n74_β:
 jmp xchain559_n42_α
 xchain559_n75_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 4784]
 mov rsi, qword ptr [r12 + 4792]
 mov rdx, qword ptr [r12 + 4928]
 mov rcx, qword ptr [r12 + 4936]
 call str_concat_d@PLT
 mov qword ptr [r12 + 4752], rax
 mov qword ptr [r12 + 4760], rdx
 jmp xchain559_n79_α
 xchain559_n75_β:
 jmp xchain559_n49_α
# IR_MATCH_SEQ_NARY
 xchain559_n76_α:
 mov dword ptr [r12 + 5472], r14d
 mov dword ptr [r12 + 5476], 0
 jmp xchain559_n81_α
xchain559_n76_as:
 mov eax, dword ptr [r12 + 5476]
 add eax, 1
 mov dword ptr [r12 + 5476], eax
 cmp eax, 1
 je xchain559_n83_α
 cmp eax, 2
 je xchain559_n84_α
 jmp xchain559_n80_α
 xchain559_n76_β:
 mov dword ptr [r12 + 5476], 3
xchain559_n76_af:
 mov eax, dword ptr [r12 + 5476]
 sub eax, 1
 mov dword ptr [r12 + 5476], eax
 cmp eax, 0
 je xchain559_n82_β
 cmp eax, 1
 je xchain559_n83_β
 cmp eax, 2
 je xchain559_n85_β
 jmp xchain559_n72_β
# IR_ASSIGN global
 xchain559_n77_α:
 mov rsi, qword ptr [r12 + 5712]
 mov rdx, qword ptr [r12 + 5720]
 mov rdi, qword ptr [rip + .Lx652_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 5696], rax
 mov qword ptr [r12 + 5704], rdx
 jmp xchain559_n61_α
 xchain559_n77_β:
 jmp xchain559_n61_α
.Lx652_0:
 .quad .Lx652_0_s
.Lx652_0_s:
 .string "OUTPUT"
 xchain559_n78_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+4192] -> [zr+4144]
 mov rax, qword ptr [r12 + 4192]
 mov qword ptr [r12 + 4144], rax
 mov rax, qword ptr [r12 + 4200]
 mov qword ptr [r12 + 4152], rax
# marshal arg1 = producer-box slot [zr+4592] -> [zr+4160]
 mov rax, qword ptr [r12 + 4592]
 mov qword ptr [r12 + 4160], rax
 mov rax, qword ptr [r12 + 4600]
 mov qword ptr [r12 + 4168], rax
  .section .rodata
  .Lbynamefn432: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn432]
 lea rsi, [r12 + 4144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 4128], rax
 mov qword ptr [r12 + 4136], rdx
 cmp eax, 99
 je xchain559_n42_α
 jmp xchain559_n86_α
 xchain559_n78_β:
 jmp xchain559_n42_α
# IR_LIT_STRING
 xchain559_n79_α:
 mov qword ptr [r12 + 5200], 1
 mov rax, qword ptr [rip + .Lx654_0]
 mov qword ptr [r12 + 5208], rax
 jmp xchain559_n87_α
 xchain559_n79_β:
 jmp xchain559_n49_α
.Lx654_0:
 .quad .Lx654_0_s
.Lx654_0_s:
 .string "EXPR$13"
# IR_MATCH_RELEASE
 xchain559_n80_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 5432]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 5440]
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rsp, rbp
 pop rbp
 push r14
 push r15
 push r13
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok_open@PLT
.Lx656_1:
 test rax, rax
 je .Lx656_2
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx656_1
.Lx656_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain559_n88_α
# IR_LIT_INTEGER
 xchain559_n81_α:
 mov qword ptr [r12 + 5504], 6
 mov rax, qword ptr [rip + .Lx657_0]
 mov qword ptr [r12 + 5512], rax
 jmp xchain559_n82_α
 xchain559_n81_β:
 jmp xchain559_n76_af
.Lx657_0:
 .quad 0
# IR_MATCH_POS
 xchain559_n82_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain559_n76_af
 jmp xchain559_n76_as
 xchain559_n82_β:
 jmp xchain559_n76_af
# IR_MATCH_DEFER
 xchain559_n83_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 5552], rcx
 test rax, rax
 jz .Lx659_0
 mov qword ptr [r12 + 5552], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 5560], rax
 mov rcx, qword ptr [r12 + 5552]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx659_1
 mov rdi, qword ptr [r12 + 5560]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 5552], rax
 jmp xchain559_n76_af
.Lx659_1:
 jmp xchain559_n76_as
.Lx659_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx659_2:
 test rax, rax
 je .Lx659_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx659_2
.Lx659_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain559_n76_af
 mov r14d, eax
 jmp xchain559_n76_as
 xchain559_n83_β:
 mov rcx, qword ptr [r12 + 5552]
 test rcx, rcx
 jz xchain559_n76_af
 mov rdi, qword ptr [r12 + 5560]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx659_1
 mov rdi, qword ptr [r12 + 5560]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 5552], rax
 jmp xchain559_n76_af
# IR_LIT_INTEGER
 xchain559_n84_α:
 mov qword ptr [r12 + 5584], 6
 mov rax, qword ptr [rip + .Lx660_0]
 mov qword ptr [r12 + 5592], rax
 jmp xchain559_n85_α
 xchain559_n84_β:
 jmp xchain559_n76_af
.Lx660_0:
 .quad 0
# IR_MATCH_RPOS
 xchain559_n85_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain559_n76_af
 jmp xchain559_n76_as
 xchain559_n85_β:
 jmp xchain559_n76_af
# IR_ASSIGN gva
 xchain559_n86_α:
 mov rax, qword ptr [r12 + 4128]
 mov rdx, qword ptr [r12 + 4136]
 mov qword ptr [rbx + 304], rax
 mov qword ptr [rbx + 312], rdx
 mov qword ptr [r12 + 4112], rax
 mov qword ptr [r12 + 4120], rdx
 jmp xchain559_n42_α
 xchain559_n86_β:
 jmp xchain559_n42_α
 xchain559_n87_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+5200] -> [zr+5168]
 mov rax, qword ptr [r12 + 5200]
 mov qword ptr [r12 + 5168], rax
 mov rax, qword ptr [r12 + 5208]
 mov qword ptr [r12 + 5176], rax
  .section .rodata
  .Lrkfn664: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn664]
 lea rsi, [r12 + 5168]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 5152], rax
 mov qword ptr [r12 + 5160], rdx
 cmp eax, 99
 je xchain559_n49_α
 jmp xchain559_n89_α
 xchain559_n87_β:
 jmp xchain559_n49_α
 xchain559_n88_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx666_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx666_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx666_2
.Lx666_1:
 call rt_faildescr@PLT
.Lx666_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 5664], rax
 mov qword ptr [r12 + 5672], rdx
 cmp eax, 99
 je xchain559_n61_α
 jmp xchain559_n90_α
 xchain559_n88_β:
 jmp xchain559_n61_α
.Lx666_0:
 .quad .Lx666_0_s
.Lx666_0_s:
 .string "Pop"
 xchain559_n89_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+4752] -> [zr+4704]
 mov rax, qword ptr [r12 + 4752]
 mov qword ptr [r12 + 4704], rax
 mov rax, qword ptr [r12 + 4760]
 mov qword ptr [r12 + 4712], rax
# marshal arg1 = producer-box slot [zr+5152] -> [zr+4720]
 mov rax, qword ptr [r12 + 5152]
 mov qword ptr [r12 + 4720], rax
 mov rax, qword ptr [r12 + 5160]
 mov qword ptr [r12 + 4728], rax
  .section .rodata
  .Lbynamefn443: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn443]
 lea rsi, [r12 + 4704]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 4688], rax
 mov qword ptr [r12 + 4696], rdx
 cmp eax, 99
 je xchain559_n49_α
 jmp xchain559_n91_α
 xchain559_n89_β:
 jmp xchain559_n49_α
# IR_ASSIGN global
 xchain559_n90_α:
 mov rsi, qword ptr [r12 + 5664]
 mov rdx, qword ptr [r12 + 5672]
 mov rdi, qword ptr [rip + .Lx668_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 5648], rax
 mov qword ptr [r12 + 5656], rdx
 jmp xchain559_n61_α
 xchain559_n90_β:
 jmp xchain559_n61_α
.Lx668_0:
 .quad .Lx668_0_s
.Lx668_0_s:
 .string "OUTPUT"
# IR_ASSIGN gva
 xchain559_n91_α:
 mov rax, qword ptr [r12 + 4688]
 mov rdx, qword ptr [r12 + 4696]
 mov qword ptr [rbx + 320], rax
 mov qword ptr [rbx + 328], rdx
 mov qword ptr [r12 + 4672], rax
 mov qword ptr [r12 + 4680], rdx
 jmp xchain559_n49_α
 xchain559_n91_β:
 jmp xchain559_n49_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 5744]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
.section .rodata
.S0: .string "0123456789"
.S1: .string "eEdD"
.S2: .string "integer"
.S3: .string "+-"
.S4: .string "epsilon"
.S5: .string "."
.S6: .string "exponent"
.S7: .string "constant"
.S8: .string "("
.S9: .string "expr"
.S10: .string ")"
.text
