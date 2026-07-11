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
 mov qword ptr [r12 + 3520], rax
 pop rsi
proc_Push_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n3_α
 xchain0_n1_β:
 jmp xchain0_n2_α
.Lx2_0:
 .quad 0
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n5_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n3_α:
 mov rdi, qword ptr [r12 + 96]
 mov rsi, qword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 112]
 mov rcx, qword ptr [r12 + 120]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain0_n6_α
 xchain0_n3_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xchain0_n7_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain0_n8_α
 xchain0_n5_β:
 jmp xchain0_n9_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n10_α
 xchain0_n6_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [r12 + 304], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n11_α
 xchain0_n7_β:
 jmp xchain0_n5_α
.Lx8_0:
 .quad 0
 xchain0_n8_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+400] -> [zr+384]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lrkfn10: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn10]
 lea rsi, [r12 + 384]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
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
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n2_α
.Lx12_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain0_n11_α:
 mov rdi, qword ptr [r12 + 288]
 mov rsi, qword ptr [r12 + 296]
 mov rdx, qword ptr [r12 + 304]
 mov rcx, qword ptr [r12 + 312]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n9_α
 xchain0_n13_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn16: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn16]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_Push_ω
 jmp proc_Push_γ
 xchain0_n13_β:
 jmp proc_Push_ω
# IR_SUBSCRIPT x[i] variable
 xchain0_n14_α:
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp xchain0_n2_α
# IR_DEREF variable -> value
 xchain0_n15_α:
 mov rdi, qword ptr [r12 + 320]
 mov rsi, qword ptr [r12 + 328]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp xchain0_n5_α
# IR_ASSIGN_VAR
 xchain0_n16_α:
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 mov rdx, qword ptr [r12 + 416]
 mov rcx, qword ptr [r12 + 424]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n9_α
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain0_n9_α
 xchain0_n16_β:
 jmp xchain0_n9_α
# IR_DEREF variable -> value
 xchain0_n17_α:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n2_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n18_α:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 336]
 mov rcx, qword ptr [r12 + 344]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [r12 + 224], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n2_α
.Lx22_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n5_α
 xchain0_n20_β:
 jmp xchain0_n5_α
 xchain0_n21_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 208]
 cmp eax, 100
 je .Lx24_0
 mov eax, dword ptr [r12 + 208]
 cmp eax, 6
 jne .Lx24_2
.Lx24_1:
 mov rax, qword ptr [r12 + 216]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 144], 6
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n22_α
.Lx24_0:
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 mov r8d, 0
 lea r9, [r12 + 144]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx24_3
.Lx24_2:
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
.Lx24_3:
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n2_α
# IR_ASSIGN_VAR
 xchain0_n22_α:
 mov rdi, qword ptr [r12 + 128]
 mov rsi, qword ptr [r12 + 136]
 mov rdx, qword ptr [r12 + 144]
 mov rcx, qword ptr [r12 + 152]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
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
 mov rdi, qword ptr [r12 + 3520]
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
 mov qword ptr [r12 + 3520], rax
 pop rsi
proc_Pop_α_body:
# IR_VAR
 xchain26_n0_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain26_n1_α
 xchain26_n0_β:
 jmp xchain26_n2_α
# IR_VAR
 xchain26_n1_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain26_n3_α
 xchain26_n1_β:
 jmp xchain26_n2_α
# IR_VAR
 xchain26_n2_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp xchain26_n4_α
 xchain26_n2_β:
 jmp proc_Pop_γ
# IR_LIT_INTEGER
 xchain26_n3_α:
 mov qword ptr [r12 + 496], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain26_n5_α
 xchain26_n3_β:
 jmp xchain26_n2_α
.Lx30_0:
 .quad 0
# IR_LIT_INTEGER
 xchain26_n4_α:
 mov qword ptr [r12 + 592], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 600], rax
 jmp xchain26_n6_α
 xchain26_n4_β:
 jmp proc_Pop_γ
.Lx31_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain26_n5_α:
 mov rdi, qword ptr [r12 + 480]
 mov rsi, qword ptr [r12 + 488]
 mov rdx, qword ptr [r12 + 496]
 mov rcx, qword ptr [r12 + 504]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain26_n7_α
 xchain26_n5_β:
 jmp xchain26_n2_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n6_α:
 mov rdi, qword ptr [r12 + 576]
 mov rsi, qword ptr [r12 + 584]
 mov rdx, qword ptr [r12 + 592]
 mov rcx, qword ptr [r12 + 600]
 call rt_subscript_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp xchain26_n8_α
 xchain26_n6_β:
 jmp proc_Pop_γ
# IR_DEREF variable -> value
 xchain26_n7_α:
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 call rt_deref@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain26_n9_α
 xchain26_n7_β:
 jmp xchain26_n2_α
# IR_VAR
 xchain26_n8_α:
 mov rax, qword ptr [rbx + 144]
 mov rdx, qword ptr [rbx + 152]
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xchain26_n10_α
 xchain26_n8_β:
 jmp proc_Pop_γ
# IR_SUBSCRIPT x[i] variable
 xchain26_n9_α:
 mov rdi, qword ptr [r12 + 464]
 mov rsi, qword ptr [r12 + 472]
 mov rdx, qword ptr [r12 + 528]
 mov rcx, qword ptr [r12 + 536]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain26_n11_α
 xchain26_n9_β:
 jmp xchain26_n2_α
# IR_LIT_INTEGER
 xchain26_n10_α:
 mov qword ptr [r12 + 656], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain26_n12_α
 xchain26_n10_β:
 jmp proc_Pop_γ
.Lx37_0:
 .quad 0
# IR_DEREF variable -> value
 xchain26_n11_α:
 mov rdi, qword ptr [r12 + 544]
 mov rsi, qword ptr [r12 + 552]
 call rt_deref@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain26_n13_α
 xchain26_n11_β:
 jmp xchain26_n2_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n12_α:
 mov rdi, qword ptr [r12 + 640]
 mov rsi, qword ptr [r12 + 648]
 mov rdx, qword ptr [r12 + 656]
 mov rcx, qword ptr [r12 + 664]
 call rt_subscript_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xchain26_n14_α
 xchain26_n12_β:
 jmp proc_Pop_γ
# IR_ASSIGN gva
 xchain26_n13_α:
 mov rax, qword ptr [r12 + 560]
 mov rdx, qword ptr [r12 + 568]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain26_n2_α
 xchain26_n13_β:
 jmp xchain26_n2_α
# IR_DEREF variable -> value
 xchain26_n14_α:
 mov rdi, qword ptr [r12 + 672]
 mov rsi, qword ptr [r12 + 680]
 call rt_deref@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp xchain26_n15_α
 xchain26_n14_β:
 jmp proc_Pop_γ
# IR_LIT_INTEGER
 xchain26_n15_α:
 mov qword ptr [r12 + 704], 6
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain26_n16_α
 xchain26_n15_β:
 jmp proc_Pop_γ
.Lx42_0:
 .quad 1
 xchain26_n16_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 688]
 cmp eax, 100
 je .Lx43_0
 mov eax, dword ptr [r12 + 688]
 cmp eax, 6
 jne .Lx43_2
.Lx43_1:
 mov rax, qword ptr [r12 + 696]
 mov rcx, 1
 sub rax, rcx
 mov qword ptr [r12 + 624], 6
 mov qword ptr [r12 + 632], rax
 jmp xchain26_n17_α
.Lx43_0:
 mov rdi, qword ptr [r12 + 688]
 mov rsi, qword ptr [r12 + 696]
 mov rdx, qword ptr [r12 + 704]
 mov rcx, qword ptr [r12 + 712]
 mov r8d, 1
 lea r9, [r12 + 624]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx43_3
.Lx43_2:
 mov rdi, qword ptr [r12 + 688]
 mov rsi, qword ptr [r12 + 696]
 mov rdx, qword ptr [r12 + 704]
 mov rcx, qword ptr [r12 + 712]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
.Lx43_3:
 jmp xchain26_n17_α
 xchain26_n16_β:
 jmp proc_Pop_γ
# IR_ASSIGN_VAR
 xchain26_n17_α:
 mov rdi, qword ptr [r12 + 608]
 mov rsi, qword ptr [r12 + 616]
 mov rdx, qword ptr [r12 + 624]
 mov rcx, qword ptr [r12 + 632]
 call rt_assign_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
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
 mov rdi, qword ptr [r12 + 3520]
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
 mov qword ptr [r12 + 3520], rax
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
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
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
 mov rax, qword ptr [r12 + 752]
 mov rdx, qword ptr [r12 + 760]
 mov qword ptr [rbx + 64], rax
 mov qword ptr [rbx + 72], rdx
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
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
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
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
 mov rax, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov qword ptr [rbx + 80], rax
 mov qword ptr [rbx + 88], rdx
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xchain45_n4_α
 xchain45_n3_β:
 jmp xchain45_n4_α
# IR_LIT_STRING
 xchain45_n4_α:
 mov qword ptr [r12 + 800], 1
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 808], rax
 jmp xchain45_n5_α
 xchain45_n4_β:
 jmp xchain45_n7_α
.Lx52_0:
 .quad .Lx52_0_s
.Lx52_0_s:
 .string ""
 xchain45_n5_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+800] -> [zr+832]
 mov rax, qword ptr [r12 + 800]
 mov qword ptr [r12 + 832], rax
 mov rax, qword ptr [r12 + 808]
 mov qword ptr [r12 + 840], rax
  .section .rodata
  .Lrkfn54: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn54]
 lea rsi, [r12 + 832]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
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
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
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
 mov qword ptr [r12 + 1008], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 1016], rax
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
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp xchain45_n10_α
 xchain45_n8_β:
 jmp xchain45_n7_α
 xchain45_n9_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1008] -> [zr+992]
 mov rax, qword ptr [r12 + 1008]
 mov qword ptr [r12 + 992], rax
 mov rax, qword ptr [r12 + 1016]
 mov qword ptr [r12 + 1000], rax
  .section .rodata
  .Lrkfn60: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn60]
 lea rsi, [r12 + 992]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 cmp eax, 99
 je xchain45_n12_α
 jmp xchain45_n11_α
 xchain45_n9_β:
 jmp xchain45_n12_α
# IR_VAR
 xchain45_n10_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xchain45_n13_α
 xchain45_n10_β:
 jmp xchain45_n7_α
# IR_ASSIGN gva
 xchain45_n11_α:
 mov rax, qword ptr [r12 + 976]
 mov rdx, qword ptr [r12 + 984]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
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
 mov rdi, qword ptr [r12 + 912]
 mov rsi, qword ptr [r12 + 920]
 mov rdx, qword ptr [r12 + 928]
 mov rcx, qword ptr [r12 + 936]
 call str_concat_d@PLT
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp xchain45_n15_α
 xchain45_n13_β:
 jmp xchain45_n7_α
 xchain45_n14_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn66: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn66]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_Unary_ω
 jmp proc_Unary_γ
 xchain45_n14_β:
 jmp proc_Unary_ω
 xchain45_n15_α:
# BOX IR_CALL EVAL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+896] -> [zr+880]
 mov rax, qword ptr [r12 + 896]
 mov qword ptr [r12 + 880], rax
 mov rax, qword ptr [r12 + 904]
 mov qword ptr [r12 + 888], rax
  .section .rodata
  .Lrkfn68: .string "EVAL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn68]
 lea rsi, [r12 + 880]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n16_α
 xchain45_n15_β:
 jmp xchain45_n7_α
# IR_ASSIGN_VAR
 xchain45_n16_α:
 mov rdi, qword ptr [r12 + 848]
 mov rsi, qword ptr [r12 + 856]
 mov rdx, qword ptr [r12 + 864]
 mov rcx, qword ptr [r12 + 872]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain45_n7_α
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
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
 mov rdi, qword ptr [r12 + 3520]
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
 mov qword ptr [r12 + 3520], rax
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
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
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
 mov rax, qword ptr [r12 + 1040]
 mov rdx, qword ptr [r12 + 1048]
 mov qword ptr [rbx + 128], rax
 mov qword ptr [rbx + 136], rdx
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
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
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
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
 mov rax, qword ptr [r12 + 1072]
 mov rdx, qword ptr [r12 + 1080]
 mov qword ptr [rbx + 80], rax
 mov qword ptr [rbx + 88], rdx
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
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
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
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
 mov rax, qword ptr [r12 + 1104]
 mov rdx, qword ptr [r12 + 1112]
 mov qword ptr [rbx + 112], rax
 mov qword ptr [rbx + 120], rdx
 mov qword ptr [r12 + 1088], rax
 mov qword ptr [r12 + 1096], rdx
 jmp xchain70_n6_α
 xchain70_n5_β:
 jmp xchain70_n6_α
# IR_LIT_STRING
 xchain70_n6_α:
 mov qword ptr [r12 + 1120], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain70_n7_α
 xchain70_n6_β:
 jmp xchain70_n9_α
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string ""
 xchain70_n7_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1120] -> [zr+1152]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1152], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1160], rax
  .section .rodata
  .Lrkfn82: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn82]
 lea rsi, [r12 + 1152]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
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
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
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
 mov qword ptr [r12 + 1424], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [r12 + 1432], rax
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
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 jmp xchain70_n12_α
 xchain70_n10_β:
 jmp xchain70_n9_α
 xchain70_n11_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1424] -> [zr+1408]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1408], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1416], rax
  .section .rodata
  .Lrkfn88: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn88]
 lea rsi, [r12 + 1408]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1392], rax
 mov qword ptr [r12 + 1400], rdx
 cmp eax, 99
 je xchain70_n14_α
 jmp xchain70_n13_α
 xchain70_n11_β:
 jmp xchain70_n14_α
# IR_LIT_STRING
 xchain70_n12_α:
 mov qword ptr [r12 + 1296], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain70_n15_α
 xchain70_n12_β:
 jmp xchain70_n9_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string " "
# IR_ASSIGN gva
 xchain70_n13_α:
 mov rax, qword ptr [r12 + 1392]
 mov rdx, qword ptr [r12 + 1400]
 mov qword ptr [rbx + 96], rax
 mov qword ptr [rbx + 104], rdx
 mov qword ptr [r12 + 1376], rax
 mov qword ptr [r12 + 1384], rdx
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
 mov rdi, qword ptr [r12 + 1280]
 mov rsi, qword ptr [r12 + 1288]
 mov rdx, qword ptr [r12 + 1296]
 mov rcx, qword ptr [r12 + 1304]
 call str_concat_d@PLT
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 jmp xchain70_n17_α
 xchain70_n15_β:
 jmp xchain70_n9_α
 xchain70_n16_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn94: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn94]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_Binary_ω
 jmp proc_Binary_γ
 xchain70_n16_β:
 jmp proc_Binary_ω
# IR_VAR
 xchain70_n17_α:
 mov rax, qword ptr [rbx + 80]
 mov rdx, qword ptr [rbx + 88]
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 jmp xchain70_n18_α
 xchain70_n17_β:
 jmp xchain70_n9_α
 xchain70_n18_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 1264]
 mov rsi, qword ptr [r12 + 1272]
 mov rdx, qword ptr [r12 + 1312]
 mov rcx, qword ptr [r12 + 1320]
 call str_concat_d@PLT
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 jmp xchain70_n19_α
 xchain70_n18_β:
 jmp xchain70_n9_α
# IR_LIT_STRING
 xchain70_n19_α:
 mov qword ptr [r12 + 1328], 1
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [r12 + 1336], rax
 jmp xchain70_n20_α
 xchain70_n19_β:
 jmp xchain70_n9_α
.Lx97_0:
 .quad .Lx97_0_s
.Lx97_0_s:
 .string " "
 xchain70_n20_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 1248]
 mov rsi, qword ptr [r12 + 1256]
 mov rdx, qword ptr [r12 + 1328]
 mov rcx, qword ptr [r12 + 1336]
 call str_concat_d@PLT
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 jmp xchain70_n21_α
 xchain70_n20_β:
 jmp xchain70_n9_α
# IR_VAR
 xchain70_n21_α:
 mov rax, qword ptr [rbx + 128]
 mov rdx, qword ptr [rbx + 136]
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 jmp xchain70_n22_α
 xchain70_n21_β:
 jmp xchain70_n9_α
 xchain70_n22_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 1232]
 mov rsi, qword ptr [r12 + 1240]
 mov rdx, qword ptr [r12 + 1344]
 mov rcx, qword ptr [r12 + 1352]
 call str_concat_d@PLT
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 jmp xchain70_n23_α
 xchain70_n22_β:
 jmp xchain70_n9_α
 xchain70_n23_α:
# BOX IR_CALL EVAL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1216] -> [zr+1200]
 mov rax, qword ptr [r12 + 1216]
 mov qword ptr [r12 + 1200], rax
 mov rax, qword ptr [r12 + 1224]
 mov qword ptr [r12 + 1208], rax
  .section .rodata
  .Lrkfn102: .string "EVAL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn102]
 lea rsi, [r12 + 1200]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 cmp eax, 99
 je xchain70_n9_α
 jmp xchain70_n24_α
 xchain70_n23_β:
 jmp xchain70_n9_α
# IR_ASSIGN_VAR
 xchain70_n24_α:
 mov rdi, qword ptr [r12 + 1168]
 mov rsi, qword ptr [r12 + 1176]
 mov rdx, qword ptr [r12 + 1184]
 mov rcx, qword ptr [r12 + 1192]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain70_n9_α
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
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
 mov rdi, qword ptr [r12 + 3520]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 mov qword ptr [r12 + 48], rax
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
 mov rdi, qword ptr [r12 + 48]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 pop rsi
proc_PAT$0_α_body:
# IR_MATCH_SPAN
 xchain344_n0_α:
 mov dword ptr [r12 + 16], 0
.Lx346_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
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
 add dword ptr [r12 + 16], 1
 jmp .Lx346_0
.Lx346_1:
 mov eax, dword ptr [r12 + 16]
 test eax, eax
 jle proc_PAT$0_ω
 mov edx, r14d
 mov dword ptr [r12 + 20], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$0_γ
 xchain344_n0_β:
 xchain344_n0_β:
 mov r14d, dword ptr [r12 + 20]
 jmp proc_PAT$0_ω
proc_PAT$0_β:
jmp proc_PAT$0_ω
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
 mov rdi, qword ptr [r12 + 32]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$1_α_body:
# IR_MATCH_ANY
 xchain347_n0_α:
 mov eax, r14d
 cmp eax, r15d
 jge proc_PAT$1_ω
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je proc_PAT$1_ω
 add r14d, 1
 jmp xchain347_n1_α
 xchain347_n0_β:
 sub r14d, 1
 jmp proc_PAT$1_ω
# IR_MATCH_ALT_SAVE
 xchain347_n1_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain347_n2_α
 xchain347_n1_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_ANY
 xchain347_n2_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain347_n3_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S2]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain347_n3_β
 add r14d, 1
 jmp xchain347_n3_α
 xchain347_n2_β:
 sub r14d, 1
 jmp xchain347_n3_β
# IR_MATCH_ALT_JOIN
 xchain347_n3_α:
 lea rax, [rip + .Lx353_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain347_n5_α
 xchain347_n3_β:
.Lx353_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain347_n4_α
# IR_MATCH_DEFER
 xchain347_n4_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx354_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx354_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain347_n6_β
.Lx354_1:
 jmp xchain347_n6_α
.Lx354_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx354_2:
 test rax, rax
 je .Lx354_3
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
 jmp .Lx354_2
.Lx354_3:
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
 js xchain347_n6_β
 mov r14d, eax
 jmp xchain347_n6_α
 xchain347_n4_β:
 jmp xchain347_n6_β
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
 test rax, rax
 jz .Lx355_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx355_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain347_n1_β
.Lx355_1:
 jmp proc_PAT$1_γ
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
 js xchain347_n1_β
 mov r14d, eax
 jmp proc_PAT$1_γ
 xchain347_n5_β:
 jmp xchain347_n1_β
# IR_MATCH_ALT_JOIN
 xchain347_n6_α:
 lea rax, [rip + .Lx357_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain347_n5_α
 xchain347_n6_β:
.Lx357_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain347_n0_β
proc_PAT$1_β:
jmp proc_PAT$1_ω
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
 mov rdi, qword ptr [r12 + 64]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 160], rax
 pop rsi
proc_PAT$2_α_body:
# IR_MATCH_ALT_SAVE
 xchain358_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain358_n1_α
 xchain358_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain358_n1_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx361_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx361_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n3_β
.Lx361_1:
 jmp xchain358_n2_α
.Lx361_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx361_2:
 test rax, rax
 je .Lx361_3
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
 jmp .Lx361_2
.Lx361_3:
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
 js xchain358_n3_β
 mov r14d, eax
 jmp xchain358_n2_α
 xchain358_n1_β:
 jmp xchain358_n3_β
# IR_MATCH_LIT
 xchain358_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain358_n1_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain358_n1_β
 add r14d, 1
 jmp xchain358_n4_α
 xchain358_n2_β:
 sub r14d, 1
 jmp xchain358_n1_β
# IR_MATCH_ALT_JOIN
 xchain358_n3_α:
 lea rax, [rip + .Lx365_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$2_γ
 xchain358_n3_β:
.Lx365_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n5_α
# IR_MATCH_ALT_SAVE
 xchain358_n4_α:
 mov dword ptr [r12 + 112], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 116], eax
 jmp xchain358_n6_α
 xchain358_n4_β:
 jmp qword ptr [r12 + 120]
# IR_MATCH_DEFER
 xchain358_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx368_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx368_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n8_β
.Lx368_1:
 jmp xchain358_n7_α
.Lx368_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx368_2:
 test rax, rax
 je .Lx368_3
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
 jmp .Lx368_2
.Lx368_3:
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
 js xchain358_n8_β
 mov r14d, eax
 jmp xchain358_n7_α
 xchain358_n5_β:
 jmp xchain358_n8_β
# IR_MATCH_DEFER
 xchain358_n6_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx369_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx369_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n9_β
.Lx369_1:
 jmp xchain358_n9_α
.Lx369_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx369_2:
 test rax, rax
 je .Lx369_3
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
 jmp .Lx369_2
.Lx369_3:
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
 js xchain358_n9_β
 mov r14d, eax
 jmp xchain358_n9_α
 xchain358_n6_β:
 jmp xchain358_n9_β
# IR_MATCH_DEFER
 xchain358_n7_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx370_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx370_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n5_β
.Lx370_1:
 jmp xchain358_n8_α
.Lx370_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
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
 js xchain358_n5_β
 mov r14d, eax
 jmp xchain358_n8_α
 xchain358_n7_β:
 jmp xchain358_n5_β
# IR_MATCH_ALT_JOIN
 xchain358_n8_α:
 lea rax, [rip + .Lx372_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$2_γ
 xchain358_n8_β:
.Lx372_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$2_ω
# IR_MATCH_ALT_JOIN
 xchain358_n9_α:
 lea rax, [rip + .Lx374_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain358_n11_α
 xchain358_n9_β:
.Lx374_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n10_α
# IR_MATCH_DEFER
 xchain358_n10_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx375_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx375_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n12_β
.Lx375_1:
 jmp xchain358_n12_α
.Lx375_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
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
 js xchain358_n12_β
 mov r14d, eax
 jmp xchain358_n12_α
 xchain358_n10_β:
 jmp xchain358_n12_β
# IR_MATCH_ALT_SAVE
 xchain358_n11_α:
 mov dword ptr [r12 + 64], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 68], eax
 jmp xchain358_n13_α
 xchain358_n11_β:
 jmp qword ptr [r12 + 72]
# IR_MATCH_ALT_JOIN
 xchain358_n12_α:
 lea rax, [rip + .Lx379_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain358_n11_α
 xchain358_n12_β:
.Lx379_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n2_β
# IR_MATCH_DEFER
 xchain358_n13_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx380_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx380_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n14_β
.Lx380_1:
 jmp xchain358_n14_α
.Lx380_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx380_2:
 test rax, rax
 je .Lx380_3
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
 jmp .Lx380_2
.Lx380_3:
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
 js xchain358_n14_β
 mov r14d, eax
 jmp xchain358_n14_α
 xchain358_n13_β:
 jmp xchain358_n14_β
# IR_MATCH_ALT_JOIN
 xchain358_n14_α:
 lea rax, [rip + .Lx382_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain358_n3_α
 xchain358_n14_β:
.Lx382_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n15_α
# IR_MATCH_DEFER
 xchain358_n15_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx383_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx383_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n16_β
.Lx383_1:
 jmp xchain358_n16_α
.Lx383_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx383_2:
 test rax, rax
 je .Lx383_3
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
 jmp .Lx383_2
.Lx383_3:
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
 js xchain358_n16_β
 mov r14d, eax
 jmp xchain358_n16_α
 xchain358_n15_β:
 jmp xchain358_n16_β
# IR_MATCH_ALT_JOIN
 xchain358_n16_α:
 lea rax, [rip + .Lx385_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain358_n3_α
 xchain358_n16_β:
.Lx385_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain358_n4_β
proc_PAT$2_β:
jmp proc_PAT$2_ω
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
 mov rdi, qword ptr [r12 + 160]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$3_α_body:
# IR_MATCH_ALT_SAVE
 xchain386_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain386_n1_α
 xchain386_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain386_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx389_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx389_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain386_n2_β
.Lx389_1:
 jmp xchain386_n2_α
.Lx389_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx389_2:
 test rax, rax
 je .Lx389_3
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
 jmp .Lx389_2
.Lx389_3:
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
 js xchain386_n2_β
 mov r14d, eax
 jmp xchain386_n2_α
 xchain386_n1_β:
 jmp xchain386_n2_β
# IR_MATCH_ALT_JOIN
 xchain386_n2_α:
 lea rax, [rip + .Lx391_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$3_γ
 xchain386_n2_β:
.Lx391_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain386_n3_α
# IR_MATCH_LIT
 xchain386_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain386_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain386_n5_β
 add r14d, 1
 jmp xchain386_n4_α
 xchain386_n3_β:
 sub r14d, 1
 jmp xchain386_n5_β
# IR_MATCH_DEFER
 xchain386_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx394_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx394_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain386_n3_β
.Lx394_1:
 jmp xchain386_n6_α
.Lx394_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx394_2:
 test rax, rax
 je .Lx394_3
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
 jmp .Lx394_2
.Lx394_3:
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
 js xchain386_n3_β
 mov r14d, eax
 jmp xchain386_n6_α
 xchain386_n4_β:
 jmp xchain386_n3_β
# IR_MATCH_ALT_JOIN
 xchain386_n5_α:
 lea rax, [rip + .Lx396_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$3_γ
 xchain386_n5_β:
.Lx396_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$3_ω
# IR_MATCH_LIT
 xchain386_n6_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain386_n4_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain386_n4_β
 add r14d, 1
 jmp xchain386_n5_α
 xchain386_n6_β:
 sub r14d, 1
 jmp xchain386_n4_β
proc_PAT$3_β:
jmp proc_PAT$3_ω
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
 mov rdi, qword ptr [r12 + 64]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 pop rsi
proc_PAT$4_α_body:
# IR_MATCH_SPAN
 xchain399_n0_α:
 mov dword ptr [r12 + 16], 0
.Lx401_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jge .Lx401_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx401_1
 add dword ptr [r12 + 16], 1
 jmp .Lx401_0
.Lx401_1:
 mov eax, dword ptr [r12 + 16]
 test eax, eax
 jle proc_PAT$4_ω
 mov edx, r14d
 mov dword ptr [r12 + 20], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$4_γ
 xchain399_n0_β:
 xchain399_n0_β:
 mov r14d, dword ptr [r12 + 20]
 jmp proc_PAT$4_ω
proc_PAT$4_β:
jmp proc_PAT$4_ω
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
 mov rdi, qword ptr [r12 + 32]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$5_α_body:
# IR_MATCH_ANY
 xchain402_n0_α:
 mov eax, r14d
 cmp eax, r15d
 jge proc_PAT$5_ω
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je proc_PAT$5_ω
 add r14d, 1
 jmp xchain402_n1_α
 xchain402_n0_β:
 sub r14d, 1
 jmp proc_PAT$5_ω
# IR_MATCH_ALT_SAVE
 xchain402_n1_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain402_n2_α
 xchain402_n1_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_ANY
 xchain402_n2_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain402_n3_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S2]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain402_n3_β
 add r14d, 1
 jmp xchain402_n3_α
 xchain402_n2_β:
 sub r14d, 1
 jmp xchain402_n3_β
# IR_MATCH_ALT_JOIN
 xchain402_n3_α:
 lea rax, [rip + .Lx408_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain402_n5_α
 xchain402_n3_β:
.Lx408_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain402_n4_α
# IR_MATCH_DEFER
 xchain402_n4_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx409_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx409_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain402_n6_β
.Lx409_1:
 jmp xchain402_n6_α
.Lx409_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx409_2:
 test rax, rax
 je .Lx409_3
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
 jmp .Lx409_2
.Lx409_3:
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
 js xchain402_n6_β
 mov r14d, eax
 jmp xchain402_n6_α
 xchain402_n4_β:
 jmp xchain402_n6_β
# IR_MATCH_DEFER
 xchain402_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx410_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx410_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain402_n1_β
.Lx410_1:
 jmp proc_PAT$5_γ
.Lx410_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx410_2:
 test rax, rax
 je .Lx410_3
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
 jmp .Lx410_2
.Lx410_3:
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
 js xchain402_n1_β
 mov r14d, eax
 jmp proc_PAT$5_γ
 xchain402_n5_β:
 jmp xchain402_n1_β
# IR_MATCH_ALT_JOIN
 xchain402_n6_α:
 lea rax, [rip + .Lx412_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain402_n5_α
 xchain402_n6_β:
.Lx412_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain402_n0_β
proc_PAT$5_β:
jmp proc_PAT$5_ω
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
 mov rdi, qword ptr [r12 + 64]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 160], rax
 pop rsi
proc_PAT$6_α_body:
# IR_MATCH_ALT_SAVE
 xchain413_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain413_n1_α
 xchain413_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain413_n1_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx416_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx416_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n3_β
.Lx416_1:
 jmp xchain413_n2_α
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
 js xchain413_n3_β
 mov r14d, eax
 jmp xchain413_n2_α
 xchain413_n1_β:
 jmp xchain413_n3_β
# IR_MATCH_LIT
 xchain413_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain413_n1_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain413_n1_β
 add r14d, 1
 jmp xchain413_n4_α
 xchain413_n2_β:
 sub r14d, 1
 jmp xchain413_n1_β
# IR_MATCH_ALT_JOIN
 xchain413_n3_α:
 lea rax, [rip + .Lx420_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$6_γ
 xchain413_n3_β:
.Lx420_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n5_α
# IR_MATCH_ALT_SAVE
 xchain413_n4_α:
 mov dword ptr [r12 + 112], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 116], eax
 jmp xchain413_n6_α
 xchain413_n4_β:
 jmp qword ptr [r12 + 120]
# IR_MATCH_DEFER
 xchain413_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx423_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx423_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n8_β
.Lx423_1:
 jmp xchain413_n7_α
.Lx423_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx423_2:
 test rax, rax
 je .Lx423_3
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
 jmp .Lx423_2
.Lx423_3:
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
 js xchain413_n8_β
 mov r14d, eax
 jmp xchain413_n7_α
 xchain413_n5_β:
 jmp xchain413_n8_β
# IR_MATCH_DEFER
 xchain413_n6_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx424_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx424_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n9_β
.Lx424_1:
 jmp xchain413_n9_α
.Lx424_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx424_2:
 test rax, rax
 je .Lx424_3
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
 jmp .Lx424_2
.Lx424_3:
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
 js xchain413_n9_β
 mov r14d, eax
 jmp xchain413_n9_α
 xchain413_n6_β:
 jmp xchain413_n9_β
# IR_MATCH_DEFER
 xchain413_n7_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx425_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx425_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n5_β
.Lx425_1:
 jmp xchain413_n8_α
.Lx425_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx425_2:
 test rax, rax
 je .Lx425_3
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
 jmp .Lx425_2
.Lx425_3:
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
 js xchain413_n5_β
 mov r14d, eax
 jmp xchain413_n8_α
 xchain413_n7_β:
 jmp xchain413_n5_β
# IR_MATCH_ALT_JOIN
 xchain413_n8_α:
 lea rax, [rip + .Lx427_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$6_γ
 xchain413_n8_β:
.Lx427_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$6_ω
# IR_MATCH_ALT_JOIN
 xchain413_n9_α:
 lea rax, [rip + .Lx429_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain413_n11_α
 xchain413_n9_β:
.Lx429_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n10_α
# IR_MATCH_DEFER
 xchain413_n10_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx430_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx430_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n12_β
.Lx430_1:
 jmp xchain413_n12_α
.Lx430_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx430_2:
 test rax, rax
 je .Lx430_3
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
 jmp .Lx430_2
.Lx430_3:
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
 js xchain413_n12_β
 mov r14d, eax
 jmp xchain413_n12_α
 xchain413_n10_β:
 jmp xchain413_n12_β
# IR_MATCH_ALT_SAVE
 xchain413_n11_α:
 mov dword ptr [r12 + 64], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 68], eax
 jmp xchain413_n13_α
 xchain413_n11_β:
 jmp qword ptr [r12 + 72]
# IR_MATCH_ALT_JOIN
 xchain413_n12_α:
 lea rax, [rip + .Lx434_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain413_n11_α
 xchain413_n12_β:
.Lx434_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n2_β
# IR_MATCH_DEFER
 xchain413_n13_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx435_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx435_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n14_β
.Lx435_1:
 jmp xchain413_n14_α
.Lx435_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx435_2:
 test rax, rax
 je .Lx435_3
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
 jmp .Lx435_2
.Lx435_3:
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
 js xchain413_n14_β
 mov r14d, eax
 jmp xchain413_n14_α
 xchain413_n13_β:
 jmp xchain413_n14_β
# IR_MATCH_ALT_JOIN
 xchain413_n14_α:
 lea rax, [rip + .Lx437_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain413_n3_α
 xchain413_n14_β:
.Lx437_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n15_α
# IR_MATCH_DEFER
 xchain413_n15_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx438_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx438_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n16_β
.Lx438_1:
 jmp xchain413_n16_α
.Lx438_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx438_2:
 test rax, rax
 je .Lx438_3
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
 jmp .Lx438_2
.Lx438_3:
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
 js xchain413_n16_β
 mov r14d, eax
 jmp xchain413_n16_α
 xchain413_n15_β:
 jmp xchain413_n16_β
# IR_MATCH_ALT_JOIN
 xchain413_n16_α:
 lea rax, [rip + .Lx440_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain413_n3_α
 xchain413_n16_β:
.Lx440_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain413_n4_β
proc_PAT$6_β:
jmp proc_PAT$6_ω
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
 mov rdi, qword ptr [r12 + 160]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$7_α_body:
# IR_MATCH_ALT_SAVE
 xchain441_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain441_n1_α
 xchain441_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain441_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx444_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx444_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain441_n2_β
.Lx444_1:
 jmp xchain441_n2_α
.Lx444_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx444_2:
 test rax, rax
 je .Lx444_3
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
 jmp .Lx444_2
.Lx444_3:
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
 js xchain441_n2_β
 mov r14d, eax
 jmp xchain441_n2_α
 xchain441_n1_β:
 jmp xchain441_n2_β
# IR_MATCH_ALT_JOIN
 xchain441_n2_α:
 lea rax, [rip + .Lx446_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$7_γ
 xchain441_n2_β:
.Lx446_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain441_n3_α
# IR_MATCH_LIT
 xchain441_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain441_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain441_n5_β
 add r14d, 1
 jmp xchain441_n4_α
 xchain441_n3_β:
 sub r14d, 1
 jmp xchain441_n5_β
# IR_MATCH_DEFER
 xchain441_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx449_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx449_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain441_n3_β
.Lx449_1:
 jmp xchain441_n6_α
.Lx449_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
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
 js xchain441_n3_β
 mov r14d, eax
 jmp xchain441_n6_α
 xchain441_n4_β:
 jmp xchain441_n3_β
# IR_MATCH_ALT_JOIN
 xchain441_n5_α:
 lea rax, [rip + .Lx451_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$7_γ
 xchain441_n5_β:
.Lx451_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$7_ω
# IR_MATCH_LIT
 xchain441_n6_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain441_n4_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain441_n4_β
 add r14d, 1
 jmp xchain441_n5_α
 xchain441_n6_β:
 sub r14d, 1
 jmp xchain441_n4_β
proc_PAT$7_β:
jmp proc_PAT$7_ω
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
 mov rdi, qword ptr [r12 + 64]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 pop rsi
proc_PAT$8_α_body:
# IR_MATCH_SPAN
 xchain454_n0_α:
 mov dword ptr [r12 + 16], 0
.Lx456_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jge .Lx456_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx456_1
 add dword ptr [r12 + 16], 1
 jmp .Lx456_0
.Lx456_1:
 mov eax, dword ptr [r12 + 16]
 test eax, eax
 jle proc_PAT$8_ω
 mov edx, r14d
 mov dword ptr [r12 + 20], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$8_γ
 xchain454_n0_β:
 xchain454_n0_β:
 mov r14d, dword ptr [r12 + 20]
 jmp proc_PAT$8_ω
proc_PAT$8_β:
jmp proc_PAT$8_ω
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
 mov rdi, qword ptr [r12 + 32]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$9_α_body:
# IR_MATCH_ANY
 xchain457_n0_α:
 mov eax, r14d
 cmp eax, r15d
 jge proc_PAT$9_ω
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je proc_PAT$9_ω
 add r14d, 1
 jmp xchain457_n1_α
 xchain457_n0_β:
 sub r14d, 1
 jmp proc_PAT$9_ω
# IR_MATCH_ALT_SAVE
 xchain457_n1_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain457_n2_α
 xchain457_n1_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_ANY
 xchain457_n2_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain457_n3_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S2]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain457_n3_β
 add r14d, 1
 jmp xchain457_n3_α
 xchain457_n2_β:
 sub r14d, 1
 jmp xchain457_n3_β
# IR_MATCH_ALT_JOIN
 xchain457_n3_α:
 lea rax, [rip + .Lx463_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain457_n5_α
 xchain457_n3_β:
.Lx463_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain457_n4_α
# IR_MATCH_DEFER
 xchain457_n4_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx464_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx464_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain457_n6_β
.Lx464_1:
 jmp xchain457_n6_α
.Lx464_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx464_2:
 test rax, rax
 je .Lx464_3
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
 jmp .Lx464_2
.Lx464_3:
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
 js xchain457_n6_β
 mov r14d, eax
 jmp xchain457_n6_α
 xchain457_n4_β:
 jmp xchain457_n6_β
# IR_MATCH_DEFER
 xchain457_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx465_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx465_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain457_n1_β
.Lx465_1:
 jmp proc_PAT$9_γ
.Lx465_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
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
 js xchain457_n1_β
 mov r14d, eax
 jmp proc_PAT$9_γ
 xchain457_n5_β:
 jmp xchain457_n1_β
# IR_MATCH_ALT_JOIN
 xchain457_n6_α:
 lea rax, [rip + .Lx467_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain457_n5_α
 xchain457_n6_β:
.Lx467_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain457_n0_β
proc_PAT$9_β:
jmp proc_PAT$9_ω
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
 mov rdi, qword ptr [r12 + 64]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 160], rax
 pop rsi
proc_PAT$10_α_body:
# IR_MATCH_ALT_SAVE
 xchain468_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain468_n1_α
 xchain468_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain468_n1_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx471_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx471_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n3_β
.Lx471_1:
 jmp xchain468_n2_α
.Lx471_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx471_2:
 test rax, rax
 je .Lx471_3
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
 jmp .Lx471_2
.Lx471_3:
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
 js xchain468_n3_β
 mov r14d, eax
 jmp xchain468_n2_α
 xchain468_n1_β:
 jmp xchain468_n3_β
# IR_MATCH_LIT
 xchain468_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain468_n1_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain468_n1_β
 add r14d, 1
 jmp xchain468_n4_α
 xchain468_n2_β:
 sub r14d, 1
 jmp xchain468_n1_β
# IR_MATCH_ALT_JOIN
 xchain468_n3_α:
 lea rax, [rip + .Lx475_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$10_γ
 xchain468_n3_β:
.Lx475_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n5_α
# IR_MATCH_ALT_SAVE
 xchain468_n4_α:
 mov dword ptr [r12 + 112], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 116], eax
 jmp xchain468_n6_α
 xchain468_n4_β:
 jmp qword ptr [r12 + 120]
# IR_MATCH_DEFER
 xchain468_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx478_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx478_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n8_β
.Lx478_1:
 jmp xchain468_n7_α
.Lx478_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx478_2:
 test rax, rax
 je .Lx478_3
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
 jmp .Lx478_2
.Lx478_3:
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
 js xchain468_n8_β
 mov r14d, eax
 jmp xchain468_n7_α
 xchain468_n5_β:
 jmp xchain468_n8_β
# IR_MATCH_DEFER
 xchain468_n6_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx479_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx479_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n9_β
.Lx479_1:
 jmp xchain468_n9_α
.Lx479_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx479_2:
 test rax, rax
 je .Lx479_3
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
 jmp .Lx479_2
.Lx479_3:
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
 js xchain468_n9_β
 mov r14d, eax
 jmp xchain468_n9_α
 xchain468_n6_β:
 jmp xchain468_n9_β
# IR_MATCH_DEFER
 xchain468_n7_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx480_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx480_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n5_β
.Lx480_1:
 jmp xchain468_n8_α
.Lx480_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx480_2:
 test rax, rax
 je .Lx480_3
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
 jmp .Lx480_2
.Lx480_3:
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
 js xchain468_n5_β
 mov r14d, eax
 jmp xchain468_n8_α
 xchain468_n7_β:
 jmp xchain468_n5_β
# IR_MATCH_ALT_JOIN
 xchain468_n8_α:
 lea rax, [rip + .Lx482_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$10_γ
 xchain468_n8_β:
.Lx482_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$10_ω
# IR_MATCH_ALT_JOIN
 xchain468_n9_α:
 lea rax, [rip + .Lx484_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain468_n11_α
 xchain468_n9_β:
.Lx484_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n10_α
# IR_MATCH_DEFER
 xchain468_n10_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx485_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx485_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n12_β
.Lx485_1:
 jmp xchain468_n12_α
.Lx485_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx485_2:
 test rax, rax
 je .Lx485_3
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
 jmp .Lx485_2
.Lx485_3:
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
 js xchain468_n12_β
 mov r14d, eax
 jmp xchain468_n12_α
 xchain468_n10_β:
 jmp xchain468_n12_β
# IR_MATCH_ALT_SAVE
 xchain468_n11_α:
 mov dword ptr [r12 + 64], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 68], eax
 jmp xchain468_n13_α
 xchain468_n11_β:
 jmp qword ptr [r12 + 72]
# IR_MATCH_ALT_JOIN
 xchain468_n12_α:
 lea rax, [rip + .Lx489_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain468_n11_α
 xchain468_n12_β:
.Lx489_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n2_β
# IR_MATCH_DEFER
 xchain468_n13_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx490_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx490_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n14_β
.Lx490_1:
 jmp xchain468_n14_α
.Lx490_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx490_2:
 test rax, rax
 je .Lx490_3
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
 jmp .Lx490_2
.Lx490_3:
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
 js xchain468_n14_β
 mov r14d, eax
 jmp xchain468_n14_α
 xchain468_n13_β:
 jmp xchain468_n14_β
# IR_MATCH_ALT_JOIN
 xchain468_n14_α:
 lea rax, [rip + .Lx492_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain468_n3_α
 xchain468_n14_β:
.Lx492_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n15_α
# IR_MATCH_DEFER
 xchain468_n15_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx493_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx493_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n16_β
.Lx493_1:
 jmp xchain468_n16_α
.Lx493_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx493_2:
 test rax, rax
 je .Lx493_3
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
 jmp .Lx493_2
.Lx493_3:
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
 js xchain468_n16_β
 mov r14d, eax
 jmp xchain468_n16_α
 xchain468_n15_β:
 jmp xchain468_n16_β
# IR_MATCH_ALT_JOIN
 xchain468_n16_α:
 lea rax, [rip + .Lx495_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain468_n3_α
 xchain468_n16_β:
.Lx495_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain468_n4_β
proc_PAT$10_β:
jmp proc_PAT$10_ω
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
 mov rdi, qword ptr [r12 + 160]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$11_α_body:
# IR_MATCH_ALT_SAVE
 xchain496_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain496_n1_α
 xchain496_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain496_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx499_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx499_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain496_n2_β
.Lx499_1:
 jmp xchain496_n2_α
.Lx499_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S7]
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
 js xchain496_n2_β
 mov r14d, eax
 jmp xchain496_n2_α
 xchain496_n1_β:
 jmp xchain496_n2_β
# IR_MATCH_ALT_JOIN
 xchain496_n2_α:
 lea rax, [rip + .Lx501_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$11_γ
 xchain496_n2_β:
.Lx501_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain496_n3_α
# IR_MATCH_LIT
 xchain496_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain496_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain496_n5_β
 add r14d, 1
 jmp xchain496_n4_α
 xchain496_n3_β:
 sub r14d, 1
 jmp xchain496_n5_β
# IR_MATCH_DEFER
 xchain496_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx504_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx504_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain496_n3_β
.Lx504_1:
 jmp xchain496_n6_α
.Lx504_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
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
 js xchain496_n3_β
 mov r14d, eax
 jmp xchain496_n6_α
 xchain496_n4_β:
 jmp xchain496_n3_β
# IR_MATCH_ALT_JOIN
 xchain496_n5_α:
 lea rax, [rip + .Lx506_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$11_γ
 xchain496_n5_β:
.Lx506_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$11_ω
# IR_MATCH_LIT
 xchain496_n6_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain496_n4_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain496_n4_β
 add r14d, 1
 jmp xchain496_n5_α
 xchain496_n6_β:
 sub r14d, 1
 jmp xchain496_n4_β
proc_PAT$11_β:
jmp proc_PAT$11_ω
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
 mov rdi, qword ptr [r12 + 64]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 pop rsi
proc_PAT$12_α_body:
# IR_MATCH_SPAN
 xchain509_n0_α:
 mov dword ptr [r12 + 16], 0
.Lx511_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jge .Lx511_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx511_1
 add dword ptr [r12 + 16], 1
 jmp .Lx511_0
.Lx511_1:
 mov eax, dword ptr [r12 + 16]
 test eax, eax
 jle proc_PAT$12_ω
 mov edx, r14d
 mov dword ptr [r12 + 20], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$12_γ
 xchain509_n0_β:
 xchain509_n0_β:
 mov r14d, dword ptr [r12 + 20]
 jmp proc_PAT$12_ω
proc_PAT$12_β:
jmp proc_PAT$12_ω
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
 mov rdi, qword ptr [r12 + 32]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$13_α_body:
# IR_MATCH_ANY
 xchain512_n0_α:
 mov eax, r14d
 cmp eax, r15d
 jge proc_PAT$13_ω
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je proc_PAT$13_ω
 add r14d, 1
 jmp xchain512_n1_α
 xchain512_n0_β:
 sub r14d, 1
 jmp proc_PAT$13_ω
# IR_MATCH_ALT_SAVE
 xchain512_n1_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain512_n2_α
 xchain512_n1_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_ANY
 xchain512_n2_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain512_n3_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S2]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain512_n3_β
 add r14d, 1
 jmp xchain512_n3_α
 xchain512_n2_β:
 sub r14d, 1
 jmp xchain512_n3_β
# IR_MATCH_ALT_JOIN
 xchain512_n3_α:
 lea rax, [rip + .Lx518_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain512_n5_α
 xchain512_n3_β:
.Lx518_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain512_n4_α
# IR_MATCH_DEFER
 xchain512_n4_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx519_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx519_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain512_n6_β
.Lx519_1:
 jmp xchain512_n6_α
.Lx519_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx519_2:
 test rax, rax
 je .Lx519_3
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
 jmp .Lx519_2
.Lx519_3:
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
 js xchain512_n6_β
 mov r14d, eax
 jmp xchain512_n6_α
 xchain512_n4_β:
 jmp xchain512_n6_β
# IR_MATCH_DEFER
 xchain512_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx520_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx520_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain512_n1_β
.Lx520_1:
 jmp proc_PAT$13_γ
.Lx520_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx520_2:
 test rax, rax
 je .Lx520_3
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
 jmp .Lx520_2
.Lx520_3:
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
 js xchain512_n1_β
 mov r14d, eax
 jmp proc_PAT$13_γ
 xchain512_n5_β:
 jmp xchain512_n1_β
# IR_MATCH_ALT_JOIN
 xchain512_n6_α:
 lea rax, [rip + .Lx522_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain512_n5_α
 xchain512_n6_β:
.Lx522_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain512_n0_β
proc_PAT$13_β:
jmp proc_PAT$13_ω
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
 mov rdi, qword ptr [r12 + 64]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 160], rax
 pop rsi
proc_PAT$14_α_body:
# IR_MATCH_ALT_SAVE
 xchain523_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain523_n1_α
 xchain523_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain523_n1_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx526_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx526_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n3_β
.Lx526_1:
 jmp xchain523_n2_α
.Lx526_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx526_2:
 test rax, rax
 je .Lx526_3
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
 jmp .Lx526_2
.Lx526_3:
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
 js xchain523_n3_β
 mov r14d, eax
 jmp xchain523_n2_α
 xchain523_n1_β:
 jmp xchain523_n3_β
# IR_MATCH_LIT
 xchain523_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain523_n1_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain523_n1_β
 add r14d, 1
 jmp xchain523_n4_α
 xchain523_n2_β:
 sub r14d, 1
 jmp xchain523_n1_β
# IR_MATCH_ALT_JOIN
 xchain523_n3_α:
 lea rax, [rip + .Lx530_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$14_γ
 xchain523_n3_β:
.Lx530_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n5_α
# IR_MATCH_ALT_SAVE
 xchain523_n4_α:
 mov dword ptr [r12 + 112], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 116], eax
 jmp xchain523_n6_α
 xchain523_n4_β:
 jmp qword ptr [r12 + 120]
# IR_MATCH_DEFER
 xchain523_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx533_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx533_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n8_β
.Lx533_1:
 jmp xchain523_n7_α
.Lx533_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx533_2:
 test rax, rax
 je .Lx533_3
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
 jmp .Lx533_2
.Lx533_3:
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
 js xchain523_n8_β
 mov r14d, eax
 jmp xchain523_n7_α
 xchain523_n5_β:
 jmp xchain523_n8_β
# IR_MATCH_DEFER
 xchain523_n6_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx534_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx534_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n9_β
.Lx534_1:
 jmp xchain523_n9_α
.Lx534_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx534_2:
 test rax, rax
 je .Lx534_3
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
 jmp .Lx534_2
.Lx534_3:
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
 js xchain523_n9_β
 mov r14d, eax
 jmp xchain523_n9_α
 xchain523_n6_β:
 jmp xchain523_n9_β
# IR_MATCH_DEFER
 xchain523_n7_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx535_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx535_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n5_β
.Lx535_1:
 jmp xchain523_n8_α
.Lx535_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
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
 js xchain523_n5_β
 mov r14d, eax
 jmp xchain523_n8_α
 xchain523_n7_β:
 jmp xchain523_n5_β
# IR_MATCH_ALT_JOIN
 xchain523_n8_α:
 lea rax, [rip + .Lx537_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$14_γ
 xchain523_n8_β:
.Lx537_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$14_ω
# IR_MATCH_ALT_JOIN
 xchain523_n9_α:
 lea rax, [rip + .Lx539_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain523_n11_α
 xchain523_n9_β:
.Lx539_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n10_α
# IR_MATCH_DEFER
 xchain523_n10_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx540_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx540_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n12_β
.Lx540_1:
 jmp xchain523_n12_α
.Lx540_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx540_2:
 test rax, rax
 je .Lx540_3
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
 jmp .Lx540_2
.Lx540_3:
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
 js xchain523_n12_β
 mov r14d, eax
 jmp xchain523_n12_α
 xchain523_n10_β:
 jmp xchain523_n12_β
# IR_MATCH_ALT_SAVE
 xchain523_n11_α:
 mov dword ptr [r12 + 64], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 68], eax
 jmp xchain523_n13_α
 xchain523_n11_β:
 jmp qword ptr [r12 + 72]
# IR_MATCH_ALT_JOIN
 xchain523_n12_α:
 lea rax, [rip + .Lx544_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain523_n11_α
 xchain523_n12_β:
.Lx544_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n2_β
# IR_MATCH_DEFER
 xchain523_n13_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx545_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx545_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n14_β
.Lx545_1:
 jmp xchain523_n14_α
.Lx545_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
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
 js xchain523_n14_β
 mov r14d, eax
 jmp xchain523_n14_α
 xchain523_n13_β:
 jmp xchain523_n14_β
# IR_MATCH_ALT_JOIN
 xchain523_n14_α:
 lea rax, [rip + .Lx547_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain523_n3_α
 xchain523_n14_β:
.Lx547_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n15_α
# IR_MATCH_DEFER
 xchain523_n15_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx548_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx548_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n16_β
.Lx548_1:
 jmp xchain523_n16_α
.Lx548_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx548_2:
 test rax, rax
 je .Lx548_3
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
 jmp .Lx548_2
.Lx548_3:
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
 js xchain523_n16_β
 mov r14d, eax
 jmp xchain523_n16_α
 xchain523_n15_β:
 jmp xchain523_n16_β
# IR_MATCH_ALT_JOIN
 xchain523_n16_α:
 lea rax, [rip + .Lx550_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain523_n3_α
 xchain523_n16_β:
.Lx550_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain523_n4_β
proc_PAT$14_β:
jmp proc_PAT$14_ω
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
 mov rdi, qword ptr [r12 + 160]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$15_α_body:
# IR_MATCH_ALT_SAVE
 xchain551_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain551_n1_α
 xchain551_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain551_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx554_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx554_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain551_n2_β
.Lx554_1:
 jmp xchain551_n2_α
.Lx554_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx554_2:
 test rax, rax
 je .Lx554_3
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
 jmp .Lx554_2
.Lx554_3:
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
 js xchain551_n2_β
 mov r14d, eax
 jmp xchain551_n2_α
 xchain551_n1_β:
 jmp xchain551_n2_β
# IR_MATCH_ALT_JOIN
 xchain551_n2_α:
 lea rax, [rip + .Lx556_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$15_γ
 xchain551_n2_β:
.Lx556_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain551_n3_α
# IR_MATCH_LIT
 xchain551_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain551_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain551_n5_β
 add r14d, 1
 jmp xchain551_n4_α
 xchain551_n3_β:
 sub r14d, 1
 jmp xchain551_n5_β
# IR_MATCH_DEFER
 xchain551_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx559_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx559_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain551_n3_β
.Lx559_1:
 jmp xchain551_n6_α
.Lx559_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx559_2:
 test rax, rax
 je .Lx559_3
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
 jmp .Lx559_2
.Lx559_3:
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
 js xchain551_n3_β
 mov r14d, eax
 jmp xchain551_n6_α
 xchain551_n4_β:
 jmp xchain551_n3_β
# IR_MATCH_ALT_JOIN
 xchain551_n5_α:
 lea rax, [rip + .Lx561_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$15_γ
 xchain551_n5_β:
.Lx561_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$15_ω
# IR_MATCH_LIT
 xchain551_n6_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain551_n4_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain551_n4_β
 add r14d, 1
 jmp xchain551_n5_α
 xchain551_n6_β:
 sub r14d, 1
 jmp xchain551_n4_β
proc_PAT$15_β:
jmp proc_PAT$15_ω
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
 mov rdi, qword ptr [r12 + 64]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 32], rax
 pop rsi
proc_PAT$16_α_body:
# IR_MATCH_SPAN
 xchain564_n0_α:
 mov dword ptr [r12 + 16], 0
.Lx566_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jge .Lx566_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx566_1
 add dword ptr [r12 + 16], 1
 jmp .Lx566_0
.Lx566_1:
 mov eax, dword ptr [r12 + 16]
 test eax, eax
 jle proc_PAT$16_ω
 mov edx, r14d
 mov dword ptr [r12 + 20], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$16_γ
 xchain564_n0_β:
 xchain564_n0_β:
 mov r14d, dword ptr [r12 + 20]
 jmp proc_PAT$16_ω
proc_PAT$16_β:
jmp proc_PAT$16_ω
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
 mov rdi, qword ptr [r12 + 32]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$17_α_body:
# IR_MATCH_ANY
 xchain567_n0_α:
 mov eax, r14d
 cmp eax, r15d
 jge proc_PAT$17_ω
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je proc_PAT$17_ω
 add r14d, 1
 jmp xchain567_n1_α
 xchain567_n0_β:
 sub r14d, 1
 jmp proc_PAT$17_ω
# IR_MATCH_ALT_SAVE
 xchain567_n1_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain567_n2_α
 xchain567_n1_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_ANY
 xchain567_n2_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain567_n3_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S2]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain567_n3_β
 add r14d, 1
 jmp xchain567_n3_α
 xchain567_n2_β:
 sub r14d, 1
 jmp xchain567_n3_β
# IR_MATCH_ALT_JOIN
 xchain567_n3_α:
 lea rax, [rip + .Lx573_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain567_n5_α
 xchain567_n3_β:
.Lx573_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain567_n4_α
# IR_MATCH_DEFER
 xchain567_n4_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx574_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx574_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain567_n6_β
.Lx574_1:
 jmp xchain567_n6_α
.Lx574_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx574_2:
 test rax, rax
 je .Lx574_3
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
 jmp .Lx574_2
.Lx574_3:
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
 js xchain567_n6_β
 mov r14d, eax
 jmp xchain567_n6_α
 xchain567_n4_β:
 jmp xchain567_n6_β
# IR_MATCH_DEFER
 xchain567_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx575_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx575_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain567_n1_β
.Lx575_1:
 jmp proc_PAT$17_γ
.Lx575_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx575_2:
 test rax, rax
 je .Lx575_3
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
 jmp .Lx575_2
.Lx575_3:
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
 js xchain567_n1_β
 mov r14d, eax
 jmp proc_PAT$17_γ
 xchain567_n5_β:
 jmp xchain567_n1_β
# IR_MATCH_ALT_JOIN
 xchain567_n6_α:
 lea rax, [rip + .Lx577_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain567_n5_α
 xchain567_n6_β:
.Lx577_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain567_n0_β
proc_PAT$17_β:
jmp proc_PAT$17_ω
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
 mov rdi, qword ptr [r12 + 64]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 160], rax
 pop rsi
proc_PAT$18_α_body:
# IR_MATCH_ALT_SAVE
 xchain578_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain578_n1_α
 xchain578_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain578_n1_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx581_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx581_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n3_β
.Lx581_1:
 jmp xchain578_n2_α
.Lx581_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx581_2:
 test rax, rax
 je .Lx581_3
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
 jmp .Lx581_2
.Lx581_3:
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
 js xchain578_n3_β
 mov r14d, eax
 jmp xchain578_n2_α
 xchain578_n1_β:
 jmp xchain578_n3_β
# IR_MATCH_LIT
 xchain578_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain578_n1_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain578_n1_β
 add r14d, 1
 jmp xchain578_n4_α
 xchain578_n2_β:
 sub r14d, 1
 jmp xchain578_n1_β
# IR_MATCH_ALT_JOIN
 xchain578_n3_α:
 lea rax, [rip + .Lx585_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$18_γ
 xchain578_n3_β:
.Lx585_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n5_α
# IR_MATCH_ALT_SAVE
 xchain578_n4_α:
 mov dword ptr [r12 + 112], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 116], eax
 jmp xchain578_n6_α
 xchain578_n4_β:
 jmp qword ptr [r12 + 120]
# IR_MATCH_DEFER
 xchain578_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx588_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx588_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n8_β
.Lx588_1:
 jmp xchain578_n7_α
.Lx588_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx588_2:
 test rax, rax
 je .Lx588_3
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
 jmp .Lx588_2
.Lx588_3:
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
 js xchain578_n8_β
 mov r14d, eax
 jmp xchain578_n7_α
 xchain578_n5_β:
 jmp xchain578_n8_β
# IR_MATCH_DEFER
 xchain578_n6_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx589_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx589_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n9_β
.Lx589_1:
 jmp xchain578_n9_α
.Lx589_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx589_2:
 test rax, rax
 je .Lx589_3
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
 jmp .Lx589_2
.Lx589_3:
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
 js xchain578_n9_β
 mov r14d, eax
 jmp xchain578_n9_α
 xchain578_n6_β:
 jmp xchain578_n9_β
# IR_MATCH_DEFER
 xchain578_n7_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx590_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx590_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n5_β
.Lx590_1:
 jmp xchain578_n8_α
.Lx590_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx590_2:
 test rax, rax
 je .Lx590_3
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
 jmp .Lx590_2
.Lx590_3:
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
 js xchain578_n5_β
 mov r14d, eax
 jmp xchain578_n8_α
 xchain578_n7_β:
 jmp xchain578_n5_β
# IR_MATCH_ALT_JOIN
 xchain578_n8_α:
 lea rax, [rip + .Lx592_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$18_γ
 xchain578_n8_β:
.Lx592_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$18_ω
# IR_MATCH_ALT_JOIN
 xchain578_n9_α:
 lea rax, [rip + .Lx594_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain578_n11_α
 xchain578_n9_β:
.Lx594_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n10_α
# IR_MATCH_DEFER
 xchain578_n10_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx595_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx595_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n12_β
.Lx595_1:
 jmp xchain578_n12_α
.Lx595_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx595_2:
 test rax, rax
 je .Lx595_3
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
 jmp .Lx595_2
.Lx595_3:
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
 js xchain578_n12_β
 mov r14d, eax
 jmp xchain578_n12_α
 xchain578_n10_β:
 jmp xchain578_n12_β
# IR_MATCH_ALT_SAVE
 xchain578_n11_α:
 mov dword ptr [r12 + 64], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 68], eax
 jmp xchain578_n13_α
 xchain578_n11_β:
 jmp qword ptr [r12 + 72]
# IR_MATCH_ALT_JOIN
 xchain578_n12_α:
 lea rax, [rip + .Lx599_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain578_n11_α
 xchain578_n12_β:
.Lx599_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n2_β
# IR_MATCH_DEFER
 xchain578_n13_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx600_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx600_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n14_β
.Lx600_1:
 jmp xchain578_n14_α
.Lx600_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx600_2:
 test rax, rax
 je .Lx600_3
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
 jmp .Lx600_2
.Lx600_3:
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
 js xchain578_n14_β
 mov r14d, eax
 jmp xchain578_n14_α
 xchain578_n13_β:
 jmp xchain578_n14_β
# IR_MATCH_ALT_JOIN
 xchain578_n14_α:
 lea rax, [rip + .Lx602_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain578_n3_α
 xchain578_n14_β:
.Lx602_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n15_α
# IR_MATCH_DEFER
 xchain578_n15_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx603_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx603_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n16_β
.Lx603_1:
 jmp xchain578_n16_α
.Lx603_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx603_2:
 test rax, rax
 je .Lx603_3
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
 jmp .Lx603_2
.Lx603_3:
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
 js xchain578_n16_β
 mov r14d, eax
 jmp xchain578_n16_α
 xchain578_n15_β:
 jmp xchain578_n16_β
# IR_MATCH_ALT_JOIN
 xchain578_n16_α:
 lea rax, [rip + .Lx605_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain578_n3_α
 xchain578_n16_β:
.Lx605_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain578_n4_β
proc_PAT$18_β:
jmp proc_PAT$18_ω
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
 mov rdi, qword ptr [r12 + 160]
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_PAT$19_α_body:
# IR_MATCH_ALT_SAVE
 xchain606_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain606_n1_α
 xchain606_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain606_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx609_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx609_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain606_n2_β
.Lx609_1:
 jmp xchain606_n2_α
.Lx609_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx609_2:
 test rax, rax
 je .Lx609_3
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
 jmp .Lx609_2
.Lx609_3:
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
 js xchain606_n2_β
 mov r14d, eax
 jmp xchain606_n2_α
 xchain606_n1_β:
 jmp xchain606_n2_β
# IR_MATCH_ALT_JOIN
 xchain606_n2_α:
 lea rax, [rip + .Lx611_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$19_γ
 xchain606_n2_β:
.Lx611_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain606_n3_α
# IR_MATCH_LIT
 xchain606_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain606_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain606_n5_β
 add r14d, 1
 jmp xchain606_n4_α
 xchain606_n3_β:
 sub r14d, 1
 jmp xchain606_n5_β
# IR_MATCH_DEFER
 xchain606_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx614_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx614_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain606_n3_β
.Lx614_1:
 jmp xchain606_n6_α
.Lx614_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx614_2:
 test rax, rax
 je .Lx614_3
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
 jmp .Lx614_2
.Lx614_3:
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
 js xchain606_n3_β
 mov r14d, eax
 jmp xchain606_n6_α
 xchain606_n4_β:
 jmp xchain606_n3_β
# IR_MATCH_ALT_JOIN
 xchain606_n5_α:
 lea rax, [rip + .Lx616_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$19_γ
 xchain606_n5_β:
.Lx616_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$19_ω
# IR_MATCH_LIT
 xchain606_n6_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain606_n4_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain606_n4_β
 add r14d, 1
 jmp xchain606_n5_α
 xchain606_n6_β:
 sub r14d, 1
 jmp xchain606_n4_β
proc_PAT$19_β:
jmp proc_PAT$19_ω
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
 mov rdi, qword ptr [r12 + 64]
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
  mov esi, 3536
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
  mov esi, 3536
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
  mov esi, 3536
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
  mov esi, 3536
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 64
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
  mov esi, 48
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
  mov esi, 80
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
  mov esi, 176
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
  mov esi, 80
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
  mov esi, 48
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
  mov esi, 80
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
  mov esi, 176
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
  mov esi, 80
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
  mov esi, 48
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
  mov esi, 80
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
  mov esi, 176
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
  mov esi, 80
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
  mov esi, 48
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
  mov esi, 80
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
  mov esi, 176
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
  mov esi, 80
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
  mov esi, 48
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
  mov esi, 80
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
  mov esi, 176
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
  mov esi, 80
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
 mov qword ptr [r12 + 3520], rax
 pop rsi
main_α_body:
 xchain619_n0_α:
# BOX IR_CALL TABLE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
  .section .rodata
  .Lrkfn621: .string "TABLE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn621]
 lea rsi, [r12 + 96]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je xchain619_n2_α
 jmp xchain619_n1_α
 xchain619_n0_β:
 jmp xchain619_n2_α
# IR_ASSIGN gva
 xchain619_n1_α:
 mov rax, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 mov qword ptr [rbx + 144], rax
 mov qword ptr [rbx + 152], rdx
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain619_n2_α
 xchain619_n1_β:
 jmp xchain619_n2_α
# IR_LIT_STRING
 xchain619_n2_α:
 mov qword ptr [r12 + 1488], 1
 mov rax, qword ptr [rip + .Lx623_0]
 mov qword ptr [r12 + 1496], rax
 jmp xchain619_n3_α
 xchain619_n2_β:
 jmp xchain619_n5_α
.Lx623_0:
 .quad .Lx623_0_s
.Lx623_0_s:
 .string "PAT$0"
 xchain619_n3_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1488] -> [zr+1472]
 mov rax, qword ptr [r12 + 1488]
 mov qword ptr [r12 + 1472], rax
 mov rax, qword ptr [r12 + 1496]
 mov qword ptr [r12 + 1480], rax
  .section .rodata
  .Lrkfn625: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn625]
 lea rsi, [r12 + 1472]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1456], rax
 mov qword ptr [r12 + 1464], rdx
 cmp eax, 99
 je xchain619_n5_α
 jmp xchain619_n4_α
 xchain619_n3_β:
 jmp xchain619_n5_α
# IR_ASSIGN gva
 xchain619_n4_α:
 mov rax, qword ptr [r12 + 1456]
 mov rdx, qword ptr [r12 + 1464]
 mov qword ptr [rbx + 176], rax
 mov qword ptr [rbx + 184], rdx
 mov qword ptr [r12 + 1440], rax
 mov qword ptr [r12 + 1448], rdx
 jmp xchain619_n5_α
 xchain619_n4_β:
 jmp xchain619_n5_α
# IR_LIT_STRING
 xchain619_n5_α:
 mov qword ptr [r12 + 1552], 1
 mov rax, qword ptr [rip + .Lx627_0]
 mov qword ptr [r12 + 1560], rax
 jmp xchain619_n6_α
 xchain619_n5_β:
 jmp xchain619_n8_α
.Lx627_0:
 .quad .Lx627_0_s
.Lx627_0_s:
 .string "PAT$1"
 xchain619_n6_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1552] -> [zr+1536]
 mov rax, qword ptr [r12 + 1552]
 mov qword ptr [r12 + 1536], rax
 mov rax, qword ptr [r12 + 1560]
 mov qword ptr [r12 + 1544], rax
  .section .rodata
  .Lrkfn629: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn629]
 lea rsi, [r12 + 1536]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
 cmp eax, 99
 je xchain619_n8_α
 jmp xchain619_n7_α
 xchain619_n6_β:
 jmp xchain619_n8_α
# IR_ASSIGN gva
 xchain619_n7_α:
 mov rax, qword ptr [r12 + 1520]
 mov rdx, qword ptr [r12 + 1528]
 mov qword ptr [rbx + 192], rax
 mov qword ptr [rbx + 200], rdx
 mov qword ptr [r12 + 1504], rax
 mov qword ptr [r12 + 1512], rdx
 jmp xchain619_n8_α
 xchain619_n7_β:
 jmp xchain619_n8_α
# IR_LIT_STRING
 xchain619_n8_α:
 mov qword ptr [r12 + 1616], 1
 mov rax, qword ptr [rip + .Lx631_0]
 mov qword ptr [r12 + 1624], rax
 jmp xchain619_n9_α
 xchain619_n8_β:
 jmp xchain619_n11_α
.Lx631_0:
 .quad .Lx631_0_s
.Lx631_0_s:
 .string "PAT$2"
 xchain619_n9_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1616] -> [zr+1600]
 mov rax, qword ptr [r12 + 1616]
 mov qword ptr [r12 + 1600], rax
 mov rax, qword ptr [r12 + 1624]
 mov qword ptr [r12 + 1608], rax
  .section .rodata
  .Lrkfn633: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn633]
 lea rsi, [r12 + 1600]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 cmp eax, 99
 je xchain619_n11_α
 jmp xchain619_n10_α
 xchain619_n9_β:
 jmp xchain619_n11_α
# IR_ASSIGN gva
 xchain619_n10_α:
 mov rax, qword ptr [r12 + 1584]
 mov rdx, qword ptr [r12 + 1592]
 mov qword ptr [rbx + 208], rax
 mov qword ptr [rbx + 216], rdx
 mov qword ptr [r12 + 1568], rax
 mov qword ptr [r12 + 1576], rdx
 jmp xchain619_n11_α
 xchain619_n10_β:
 jmp xchain619_n11_α
# IR_LIT_INTEGER
 xchain619_n11_α:
 mov qword ptr [r12 + 1712], 6
 mov rax, qword ptr [rip + .Lx635_0]
 mov qword ptr [r12 + 1720], rax
 jmp xchain619_n12_α
 xchain619_n11_β:
 jmp xchain619_n17_α
.Lx635_0:
 .quad 42
# IR_LIT_STRING
 xchain619_n12_α:
 mov qword ptr [r12 + 1728], 1
 mov rax, qword ptr [rip + .Lx636_0]
 mov qword ptr [r12 + 1736], rax
 jmp xchain619_n13_α
 xchain619_n12_β:
 jmp xchain619_n17_α
.Lx636_0:
 .quad .Lx636_0_s
.Lx636_0_s:
 .string "*EXPR$0"
# IR_LIT_INTEGER
 xchain619_n13_α:
 mov qword ptr [r12 + 1792], 6
 mov rax, qword ptr [rip + .Lx637_0]
 mov qword ptr [r12 + 1800], rax
 jmp xchain619_n14_α
 xchain619_n13_β:
 jmp xchain619_n17_α
.Lx637_0:
 .quad 28
# IR_LIT_STRING
 xchain619_n14_α:
 mov qword ptr [r12 + 1808], 1
 mov rax, qword ptr [rip + .Lx638_0]
 mov qword ptr [r12 + 1816], rax
 jmp xchain619_n15_α
 xchain619_n14_β:
 jmp xchain619_n17_α
.Lx638_0:
 .quad .Lx638_0_s
.Lx638_0_s:
 .string "+-"
 xchain619_n15_α:
# BOX CALL SNO$PBK(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1792] -> [zr+1760]
 mov rax, qword ptr [r12 + 1792]
 mov qword ptr [r12 + 1760], rax
 mov rax, qword ptr [r12 + 1800]
 mov qword ptr [r12 + 1768], rax
# marshal arg1 = producer-box slot [zr+1808] -> [zr+1776]
 mov rax, qword ptr [r12 + 1808]
 mov qword ptr [r12 + 1776], rax
 mov rax, qword ptr [r12 + 1816]
 mov qword ptr [r12 + 1784], rax
  .section .rodata
  .Lbynamefn399: .string "SNO$PBK"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn399]
 lea rsi, [r12 + 1760]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1744], rax
 mov qword ptr [r12 + 1752], rdx
 cmp eax, 99
 je xchain619_n17_α
 jmp xchain619_n16_α
 xchain619_n15_β:
 jmp xchain619_n17_α
 xchain619_n16_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1712] -> [zr+1664]
 mov rax, qword ptr [r12 + 1712]
 mov qword ptr [r12 + 1664], rax
 mov rax, qword ptr [r12 + 1720]
 mov qword ptr [r12 + 1672], rax
# marshal arg1 = producer-box slot [zr+1728] -> [zr+1680]
 mov rax, qword ptr [r12 + 1728]
 mov qword ptr [r12 + 1680], rax
 mov rax, qword ptr [r12 + 1736]
 mov qword ptr [r12 + 1688], rax
# marshal arg2 = producer-box slot [zr+1744] -> [zr+1696]
 mov rax, qword ptr [r12 + 1744]
 mov qword ptr [r12 + 1696], rax
 mov rax, qword ptr [r12 + 1752]
 mov qword ptr [r12 + 1704], rax
  .section .rodata
  .Lbynamefn400: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn400]
 lea rsi, [r12 + 1664]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1648], rax
 mov qword ptr [r12 + 1656], rdx
 cmp eax, 99
 je xchain619_n17_α
 jmp xchain619_n18_α
 xchain619_n16_β:
 jmp xchain619_n17_α
# IR_LIT_INTEGER
 xchain619_n17_α:
 mov qword ptr [r12 + 1904], 6
 mov rax, qword ptr [rip + .Lx641_0]
 mov qword ptr [r12 + 1912], rax
 jmp xchain619_n19_α
 xchain619_n17_β:
 jmp xchain619_n24_α
.Lx641_0:
 .quad 42
# IR_ASSIGN gva
 xchain619_n18_α:
 mov rax, qword ptr [r12 + 1648]
 mov rdx, qword ptr [r12 + 1656]
 mov qword ptr [rbx + 224], rax
 mov qword ptr [rbx + 232], rdx
 mov qword ptr [r12 + 1632], rax
 mov qword ptr [r12 + 1640], rdx
 jmp xchain619_n17_α
 xchain619_n18_β:
 jmp xchain619_n17_α
# IR_LIT_STRING
 xchain619_n19_α:
 mov qword ptr [r12 + 1920], 1
 mov rax, qword ptr [rip + .Lx643_0]
 mov qword ptr [r12 + 1928], rax
 jmp xchain619_n20_α
 xchain619_n19_β:
 jmp xchain619_n24_α
.Lx643_0:
 .quad .Lx643_0_s
.Lx643_0_s:
 .string "*EXPR$1"
# IR_LIT_INTEGER
 xchain619_n20_α:
 mov qword ptr [r12 + 1984], 6
 mov rax, qword ptr [rip + .Lx644_0]
 mov qword ptr [r12 + 1992], rax
 jmp xchain619_n21_α
 xchain619_n20_β:
 jmp xchain619_n24_α
.Lx644_0:
 .quad 28
# IR_LIT_STRING
 xchain619_n21_α:
 mov qword ptr [r12 + 2000], 1
 mov rax, qword ptr [rip + .Lx645_0]
 mov qword ptr [r12 + 2008], rax
 jmp xchain619_n22_α
 xchain619_n21_β:
 jmp xchain619_n24_α
.Lx645_0:
 .quad .Lx645_0_s
.Lx645_0_s:
 .string "*/"
 xchain619_n22_α:
# BOX CALL SNO$PBK(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1984] -> [zr+1952]
 mov rax, qword ptr [r12 + 1984]
 mov qword ptr [r12 + 1952], rax
 mov rax, qword ptr [r12 + 1992]
 mov qword ptr [r12 + 1960], rax
# marshal arg1 = producer-box slot [zr+2000] -> [zr+1968]
 mov rax, qword ptr [r12 + 2000]
 mov qword ptr [r12 + 1968], rax
 mov rax, qword ptr [r12 + 2008]
 mov qword ptr [r12 + 1976], rax
  .section .rodata
  .Lbynamefn406: .string "SNO$PBK"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn406]
 lea rsi, [r12 + 1952]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1936], rax
 mov qword ptr [r12 + 1944], rdx
 cmp eax, 99
 je xchain619_n24_α
 jmp xchain619_n23_α
 xchain619_n22_β:
 jmp xchain619_n24_α
 xchain619_n23_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1904] -> [zr+1856]
 mov rax, qword ptr [r12 + 1904]
 mov qword ptr [r12 + 1856], rax
 mov rax, qword ptr [r12 + 1912]
 mov qword ptr [r12 + 1864], rax
# marshal arg1 = producer-box slot [zr+1920] -> [zr+1872]
 mov rax, qword ptr [r12 + 1920]
 mov qword ptr [r12 + 1872], rax
 mov rax, qword ptr [r12 + 1928]
 mov qword ptr [r12 + 1880], rax
# marshal arg2 = producer-box slot [zr+1936] -> [zr+1888]
 mov rax, qword ptr [r12 + 1936]
 mov qword ptr [r12 + 1888], rax
 mov rax, qword ptr [r12 + 1944]
 mov qword ptr [r12 + 1896], rax
  .section .rodata
  .Lbynamefn407: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn407]
 lea rsi, [r12 + 1856]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1840], rax
 mov qword ptr [r12 + 1848], rdx
 cmp eax, 99
 je xchain619_n24_α
 jmp xchain619_n25_α
 xchain619_n23_β:
 jmp xchain619_n24_α
# IR_LIT_INTEGER
 xchain619_n24_α:
 mov qword ptr [r12 + 2096], 6
 mov rax, qword ptr [rip + .Lx648_0]
 mov qword ptr [r12 + 2104], rax
 jmp xchain619_n26_α
 xchain619_n24_β:
 jmp xchain619_n29_α
.Lx648_0:
 .quad 42
# IR_ASSIGN gva
 xchain619_n25_α:
 mov rax, qword ptr [r12 + 1840]
 mov rdx, qword ptr [r12 + 1848]
 mov qword ptr [rbx + 240], rax
 mov qword ptr [rbx + 248], rdx
 mov qword ptr [r12 + 1824], rax
 mov qword ptr [r12 + 1832], rdx
 jmp xchain619_n24_α
 xchain619_n25_β:
 jmp xchain619_n24_α
# IR_LIT_STRING
 xchain619_n26_α:
 mov qword ptr [r12 + 2112], 1
 mov rax, qword ptr [rip + .Lx650_0]
 mov qword ptr [r12 + 2120], rax
 jmp xchain619_n27_α
 xchain619_n26_β:
 jmp xchain619_n29_α
.Lx650_0:
 .quad .Lx650_0_s
.Lx650_0_s:
 .string "*EXPR$2"
# IR_VAR
 xchain619_n27_α:
 mov rax, qword ptr [rbx + 208]
 mov rdx, qword ptr [rbx + 216]
 mov qword ptr [r12 + 2176], rax
 mov qword ptr [r12 + 2184], rdx
 jmp xchain619_n28_α
 xchain619_n27_β:
 jmp xchain619_n29_α
# IR_VAR
 xchain619_n28_α:
 mov rax, qword ptr [rbx + 176]
 mov rdx, qword ptr [rbx + 184]
 mov qword ptr [r12 + 2192], rax
 mov qword ptr [r12 + 2200], rdx
 jmp xchain619_n30_α
 xchain619_n28_β:
 jmp xchain619_n29_α
# IR_LIT_STRING
 xchain619_n29_α:
 mov qword ptr [r12 + 2256], 1
 mov rax, qword ptr [rip + .Lx653_0]
 mov qword ptr [r12 + 2264], rax
 jmp xchain619_n31_α
 xchain619_n29_β:
 jmp xchain619_n34_α
.Lx653_0:
 .quad .Lx653_0_s
.Lx653_0_s:
 .string "PAT$3"
 xchain619_n30_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2176] -> [zr+2144]
 mov rax, qword ptr [r12 + 2176]
 mov qword ptr [r12 + 2144], rax
 mov rax, qword ptr [r12 + 2184]
 mov qword ptr [r12 + 2152], rax
# marshal arg1 = producer-box slot [zr+2192] -> [zr+2160]
 mov rax, qword ptr [r12 + 2192]
 mov qword ptr [r12 + 2160], rax
 mov rax, qword ptr [r12 + 2200]
 mov qword ptr [r12 + 2168], rax
  .section .rodata
  .Lbynamefn414: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn414]
 lea rsi, [r12 + 2144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2128], rax
 mov qword ptr [r12 + 2136], rdx
 cmp eax, 99
 je xchain619_n29_α
 jmp xchain619_n32_α
 xchain619_n30_β:
 jmp xchain619_n29_α
 xchain619_n31_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2256] -> [zr+2240]
 mov rax, qword ptr [r12 + 2256]
 mov qword ptr [r12 + 2240], rax
 mov rax, qword ptr [r12 + 2264]
 mov qword ptr [r12 + 2248], rax
  .section .rodata
  .Lrkfn656: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn656]
 lea rsi, [r12 + 2240]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2224], rax
 mov qword ptr [r12 + 2232], rdx
 cmp eax, 99
 je xchain619_n34_α
 jmp xchain619_n33_α
 xchain619_n31_β:
 jmp xchain619_n34_α
 xchain619_n32_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2096] -> [zr+2048]
 mov rax, qword ptr [r12 + 2096]
 mov qword ptr [r12 + 2048], rax
 mov rax, qword ptr [r12 + 2104]
 mov qword ptr [r12 + 2056], rax
# marshal arg1 = producer-box slot [zr+2112] -> [zr+2064]
 mov rax, qword ptr [r12 + 2112]
 mov qword ptr [r12 + 2064], rax
 mov rax, qword ptr [r12 + 2120]
 mov qword ptr [r12 + 2072], rax
# marshal arg2 = producer-box slot [zr+2128] -> [zr+2080]
 mov rax, qword ptr [r12 + 2128]
 mov qword ptr [r12 + 2080], rax
 mov rax, qword ptr [r12 + 2136]
 mov qword ptr [r12 + 2088], rax
  .section .rodata
  .Lbynamefn416: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn416]
 lea rsi, [r12 + 2048]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2032], rax
 mov qword ptr [r12 + 2040], rdx
 cmp eax, 99
 je xchain619_n29_α
 jmp xchain619_n35_α
 xchain619_n32_β:
 jmp xchain619_n29_α
# IR_ASSIGN gva
 xchain619_n33_α:
 mov rax, qword ptr [r12 + 2224]
 mov rdx, qword ptr [r12 + 2232]
 mov qword ptr [rbx + 272], rax
 mov qword ptr [rbx + 280], rdx
 mov qword ptr [r12 + 2208], rax
 mov qword ptr [r12 + 2216], rdx
 jmp xchain619_n34_α
 xchain619_n33_β:
 jmp xchain619_n34_α
# IR_VAR
 xchain619_n34_α:
 mov rax, qword ptr [rbx + 224]
 mov rdx, qword ptr [rbx + 232]
 mov qword ptr [r12 + 2352], rax
 mov qword ptr [r12 + 2360], rdx
 jmp xchain619_n36_α
 xchain619_n34_β:
 jmp xchain619_n37_α
# IR_ASSIGN gva
 xchain619_n35_α:
 mov rax, qword ptr [r12 + 2032]
 mov rdx, qword ptr [r12 + 2040]
 mov qword ptr [rbx + 256], rax
 mov qword ptr [rbx + 264], rdx
 mov qword ptr [r12 + 2016], rax
 mov qword ptr [r12 + 2024], rdx
 jmp xchain619_n29_α
 xchain619_n35_β:
 jmp xchain619_n29_α
# IR_LIT_INTEGER
 xchain619_n36_α:
 mov qword ptr [r12 + 2432], 6
 mov rax, qword ptr [rip + .Lx661_0]
 mov qword ptr [r12 + 2440], rax
 jmp xchain619_n38_α
 xchain619_n36_β:
 jmp xchain619_n37_α
.Lx661_0:
 .quad 42
# IR_LIT_STRING
 xchain619_n37_α:
 mov qword ptr [r12 + 2688], 1
 mov rax, qword ptr [rip + .Lx662_0]
 mov qword ptr [r12 + 2696], rax
 jmp xchain619_n39_α
 xchain619_n37_β:
 jmp xchain619_n42_α
.Lx662_0:
 .quad .Lx662_0_s
.Lx662_0_s:
 .string "EXPR$6"
# IR_LIT_STRING
 xchain619_n38_α:
 mov qword ptr [r12 + 2448], 1
 mov rax, qword ptr [rip + .Lx663_0]
 mov qword ptr [r12 + 2456], rax
 jmp xchain619_n40_α
 xchain619_n38_β:
 jmp xchain619_n37_α
.Lx663_0:
 .quad .Lx663_0_s
.Lx663_0_s:
 .string "*EXPR$3"
 xchain619_n39_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2688] -> [zr+2672]
 mov rax, qword ptr [r12 + 2688]
 mov qword ptr [r12 + 2672], rax
 mov rax, qword ptr [r12 + 2696]
 mov qword ptr [r12 + 2680], rax
  .section .rodata
  .Lrkfn665: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn665]
 lea rsi, [r12 + 2672]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2656], rax
 mov qword ptr [r12 + 2664], rdx
 cmp eax, 99
 je xchain619_n42_α
 jmp xchain619_n41_α
 xchain619_n39_β:
 jmp xchain619_n42_α
# IR_LIT_STRING
 xchain619_n40_α:
 mov qword ptr [r12 + 2496], 1
 mov rax, qword ptr [rip + .Lx666_0]
 mov qword ptr [r12 + 2504], rax
 jmp xchain619_n43_α
 xchain619_n40_β:
 jmp xchain619_n37_α
.Lx666_0:
 .quad .Lx666_0_s
.Lx666_0_s:
 .string "EXPR$4"
# IR_VAR
 xchain619_n41_α:
 mov rax, qword ptr [rbx + 240]
 mov rdx, qword ptr [rbx + 248]
 mov qword ptr [r12 + 2704], rax
 mov qword ptr [r12 + 2712], rdx
 jmp xchain619_n44_α
 xchain619_n41_β:
 jmp xchain619_n42_α
# IR_LIT_STRING
 xchain619_n42_α:
 mov qword ptr [r12 + 3040], 1
 mov rax, qword ptr [rip + .Lx668_0]
 mov qword ptr [r12 + 3048], rax
 jmp xchain619_n45_α
 xchain619_n42_β:
 jmp xchain619_n49_α
.Lx668_0:
 .quad .Lx668_0_s
.Lx668_0_s:
 .string "EXPR$10"
 xchain619_n43_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2496] -> [zr+2480]
 mov rax, qword ptr [r12 + 2496]
 mov qword ptr [r12 + 2480], rax
 mov rax, qword ptr [r12 + 2504]
 mov qword ptr [r12 + 2488], rax
  .section .rodata
  .Lrkfn670: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn670]
 lea rsi, [r12 + 2480]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2464], rax
 mov qword ptr [r12 + 2472], rdx
 cmp eax, 99
 je xchain619_n37_α
 jmp xchain619_n46_α
 xchain619_n43_β:
 jmp xchain619_n37_α
 xchain619_n44_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2656]
 mov rsi, qword ptr [r12 + 2664]
 mov rdx, qword ptr [r12 + 2704]
 mov rcx, qword ptr [r12 + 2712]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2640], rax
 mov qword ptr [r12 + 2648], rdx
 jmp xchain619_n47_α
 xchain619_n44_β:
 jmp xchain619_n42_α
 xchain619_n45_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3040] -> [zr+3024]
 mov rax, qword ptr [r12 + 3040]
 mov qword ptr [r12 + 3024], rax
 mov rax, qword ptr [r12 + 3048]
 mov qword ptr [r12 + 3032], rax
  .section .rodata
  .Lrkfn673: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn673]
 lea rsi, [r12 + 3024]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3008], rax
 mov qword ptr [r12 + 3016], rdx
 cmp eax, 99
 je xchain619_n49_α
 jmp xchain619_n48_α
 xchain619_n45_β:
 jmp xchain619_n49_α
 xchain619_n46_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2432] -> [zr+2384]
 mov rax, qword ptr [r12 + 2432]
 mov qword ptr [r12 + 2384], rax
 mov rax, qword ptr [r12 + 2440]
 mov qword ptr [r12 + 2392], rax
# marshal arg1 = producer-box slot [zr+2448] -> [zr+2400]
 mov rax, qword ptr [r12 + 2448]
 mov qword ptr [r12 + 2400], rax
 mov rax, qword ptr [r12 + 2456]
 mov qword ptr [r12 + 2408], rax
# marshal arg2 = producer-box slot [zr+2464] -> [zr+2416]
 mov rax, qword ptr [r12 + 2464]
 mov qword ptr [r12 + 2416], rax
 mov rax, qword ptr [r12 + 2472]
 mov qword ptr [r12 + 2424], rax
  .section .rodata
  .Lbynamefn430: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn430]
 lea rsi, [r12 + 2384]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2368], rax
 mov qword ptr [r12 + 2376], rdx
 cmp eax, 99
 je xchain619_n37_α
 jmp xchain619_n50_α
 xchain619_n46_β:
 jmp xchain619_n37_α
# IR_LIT_INTEGER
 xchain619_n47_α:
 mov qword ptr [r12 + 2784], 6
 mov rax, qword ptr [rip + .Lx675_0]
 mov qword ptr [r12 + 2792], rax
 jmp xchain619_n51_α
 xchain619_n47_β:
 jmp xchain619_n42_α
.Lx675_0:
 .quad 42
# IR_VAR
 xchain619_n48_α:
 mov rax, qword ptr [rbx + 224]
 mov rdx, qword ptr [rbx + 232]
 mov qword ptr [r12 + 3056], rax
 mov qword ptr [r12 + 3064], rdx
 jmp xchain619_n52_α
 xchain619_n48_β:
 jmp xchain619_n49_α
# IR_LIT_STRING
 xchain619_n49_α:
 mov qword ptr [r12 + 3312], 1
 mov rax, qword ptr [rip + .Lx677_0]
 mov qword ptr [r12 + 3320], rax
 jmp xchain619_n53_α
 xchain619_n49_β:
 jmp xchain619_n61_α
.Lx677_0:
 .quad .Lx677_0_s
.Lx677_0_s:
 .string "TRIM"
 xchain619_n50_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2352]
 mov rsi, qword ptr [r12 + 2360]
 mov rdx, qword ptr [r12 + 2368]
 mov rcx, qword ptr [r12 + 2376]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2336], rax
 mov qword ptr [r12 + 2344], rdx
 jmp xchain619_n54_α
 xchain619_n50_β:
 jmp xchain619_n37_α
# IR_LIT_STRING
 xchain619_n51_α:
 mov qword ptr [r12 + 2800], 1
 mov rax, qword ptr [rip + .Lx679_0]
 mov qword ptr [r12 + 2808], rax
 jmp xchain619_n55_α
 xchain619_n51_β:
 jmp xchain619_n42_α
.Lx679_0:
 .quad .Lx679_0_s
.Lx679_0_s:
 .string "*EXPR$7"
 xchain619_n52_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 3008]
 mov rsi, qword ptr [r12 + 3016]
 mov rdx, qword ptr [r12 + 3056]
 mov rcx, qword ptr [r12 + 3064]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2992], rax
 mov qword ptr [r12 + 3000], rdx
 jmp xchain619_n56_α
 xchain619_n52_β:
 jmp xchain619_n49_α
# IR_LIT_INTEGER
 xchain619_n53_α:
 mov qword ptr [r12 + 3328], 6
 mov rax, qword ptr [rip + .Lx681_0]
 mov qword ptr [r12 + 3336], rax
 jmp xchain619_n57_α
 xchain619_n53_β:
 jmp xchain619_n61_α
.Lx681_0:
 .quad 1
# IR_LIT_STRING
 xchain619_n54_α:
 mov qword ptr [r12 + 2544], 1
 mov rax, qword ptr [rip + .Lx682_0]
 mov qword ptr [r12 + 2552], rax
 jmp xchain619_n58_α
 xchain619_n54_β:
 jmp xchain619_n37_α
.Lx682_0:
 .quad .Lx682_0_s
.Lx682_0_s:
 .string "EXPR$5"
# IR_LIT_STRING
 xchain619_n55_α:
 mov qword ptr [r12 + 2848], 1
 mov rax, qword ptr [rip + .Lx683_0]
 mov qword ptr [r12 + 2856], rax
 jmp xchain619_n59_α
 xchain619_n55_β:
 jmp xchain619_n42_α
.Lx683_0:
 .quad .Lx683_0_s
.Lx683_0_s:
 .string "EXPR$8"
# IR_LIT_INTEGER
 xchain619_n56_α:
 mov qword ptr [r12 + 3136], 6
 mov rax, qword ptr [rip + .Lx684_0]
 mov qword ptr [r12 + 3144], rax
 jmp xchain619_n60_α
 xchain619_n56_β:
 jmp xchain619_n49_α
.Lx684_0:
 .quad 42
 xchain619_n57_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3312] -> [zr+3280]
 mov rax, qword ptr [r12 + 3312]
 mov qword ptr [r12 + 3280], rax
 mov rax, qword ptr [r12 + 3320]
 mov qword ptr [r12 + 3288], rax
# marshal arg1 = producer-box slot [zr+3328] -> [zr+3296]
 mov rax, qword ptr [r12 + 3328]
 mov qword ptr [r12 + 3296], rax
 mov rax, qword ptr [r12 + 3336]
 mov qword ptr [r12 + 3304], rax
  .section .rodata
  .Lrkfn686: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn686]
 lea rsi, [r12 + 3280]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3264], rax
 mov qword ptr [r12 + 3272], rdx
 cmp eax, 99
 je xchain619_n61_α
 jmp xchain619_n61_α
 xchain619_n57_β:
 jmp xchain619_n61_α
 xchain619_n58_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2544] -> [zr+2528]
 mov rax, qword ptr [r12 + 2544]
 mov qword ptr [r12 + 2528], rax
 mov rax, qword ptr [r12 + 2552]
 mov qword ptr [r12 + 2536], rax
  .section .rodata
  .Lrkfn688: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn688]
 lea rsi, [r12 + 2528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2512], rax
 mov qword ptr [r12 + 2520], rdx
 cmp eax, 99
 je xchain619_n37_α
 jmp xchain619_n62_α
 xchain619_n58_β:
 jmp xchain619_n37_α
 xchain619_n59_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2848] -> [zr+2832]
 mov rax, qword ptr [r12 + 2848]
 mov qword ptr [r12 + 2832], rax
 mov rax, qword ptr [r12 + 2856]
 mov qword ptr [r12 + 2840], rax
  .section .rodata
  .Lrkfn690: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn690]
 lea rsi, [r12 + 2832]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2816], rax
 mov qword ptr [r12 + 2824], rdx
 cmp eax, 99
 je xchain619_n42_α
 jmp xchain619_n63_α
 xchain619_n59_β:
 jmp xchain619_n42_α
# IR_LIT_STRING
 xchain619_n60_α:
 mov qword ptr [r12 + 3152], 1
 mov rax, qword ptr [rip + .Lx691_0]
 mov qword ptr [r12 + 3160], rax
 jmp xchain619_n64_α
 xchain619_n60_β:
 jmp xchain619_n49_α
.Lx691_0:
 .quad .Lx691_0_s
.Lx691_0_s:
 .string "*EXPR$11"
# IR_VAR
 xchain619_n61_α:
 mov rdi, qword ptr [rip + .Lx692_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 3360], rax
 mov qword ptr [r12 + 3368], rdx
 jmp xchain619_n65_α
 xchain619_n61_β:
 jmp main_γ
.Lx692_0:
 .quad .Lx692_0_s
.Lx692_0_s:
 .string "INPUT"
 xchain619_n62_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2336] -> [zr+2304]
 mov rax, qword ptr [r12 + 2336]
 mov qword ptr [r12 + 2304], rax
 mov rax, qword ptr [r12 + 2344]
 mov qword ptr [r12 + 2312], rax
# marshal arg1 = producer-box slot [zr+2512] -> [zr+2320]
 mov rax, qword ptr [r12 + 2512]
 mov qword ptr [r12 + 2320], rax
 mov rax, qword ptr [r12 + 2520]
 mov qword ptr [r12 + 2328], rax
  .section .rodata
  .Lbynamefn446: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn446]
 lea rsi, [r12 + 2304]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2288], rax
 mov qword ptr [r12 + 2296], rdx
 cmp eax, 99
 je xchain619_n37_α
 jmp xchain619_n66_α
 xchain619_n62_β:
 jmp xchain619_n37_α
 xchain619_n63_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2784] -> [zr+2736]
 mov rax, qword ptr [r12 + 2784]
 mov qword ptr [r12 + 2736], rax
 mov rax, qword ptr [r12 + 2792]
 mov qword ptr [r12 + 2744], rax
# marshal arg1 = producer-box slot [zr+2800] -> [zr+2752]
 mov rax, qword ptr [r12 + 2800]
 mov qword ptr [r12 + 2752], rax
 mov rax, qword ptr [r12 + 2808]
 mov qword ptr [r12 + 2760], rax
# marshal arg2 = producer-box slot [zr+2816] -> [zr+2768]
 mov rax, qword ptr [r12 + 2816]
 mov qword ptr [r12 + 2768], rax
 mov rax, qword ptr [r12 + 2824]
 mov qword ptr [r12 + 2776], rax
  .section .rodata
  .Lbynamefn447: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn447]
 lea rsi, [r12 + 2736]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2720], rax
 mov qword ptr [r12 + 2728], rdx
 cmp eax, 99
 je xchain619_n42_α
 jmp xchain619_n67_α
 xchain619_n63_β:
 jmp xchain619_n42_α
# IR_LIT_STRING
 xchain619_n64_α:
 mov qword ptr [r12 + 3200], 1
 mov rax, qword ptr [rip + .Lx695_0]
 mov qword ptr [r12 + 3208], rax
 jmp xchain619_n68_α
 xchain619_n64_β:
 jmp xchain619_n49_α
.Lx695_0:
 .quad .Lx695_0_s
.Lx695_0_s:
 .string "EXPR$12"
# IR_ASSIGN gva
 xchain619_n65_α:
 mov rax, qword ptr [r12 + 3360]
 mov rdx, qword ptr [r12 + 3368]
 mov qword ptr [rbx + 336], rax
 mov qword ptr [rbx + 344], rdx
 mov qword ptr [r12 + 3344], rax
 mov qword ptr [r12 + 3352], rdx
 jmp xchain619_n69_α
 xchain619_n65_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain619_n66_α:
 mov rax, qword ptr [r12 + 2288]
 mov rdx, qword ptr [r12 + 2296]
 mov qword ptr [rbx + 288], rax
 mov qword ptr [rbx + 296], rdx
 mov qword ptr [r12 + 2272], rax
 mov qword ptr [r12 + 2280], rdx
 jmp xchain619_n37_α
 xchain619_n66_β:
 jmp xchain619_n37_α
 xchain619_n67_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2640]
 mov rsi, qword ptr [r12 + 2648]
 mov rdx, qword ptr [r12 + 2720]
 mov rcx, qword ptr [r12 + 2728]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2624], rax
 mov qword ptr [r12 + 2632], rdx
 jmp xchain619_n70_α
 xchain619_n67_β:
 jmp xchain619_n42_α
 xchain619_n68_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3200] -> [zr+3184]
 mov rax, qword ptr [r12 + 3200]
 mov qword ptr [r12 + 3184], rax
 mov rax, qword ptr [r12 + 3208]
 mov qword ptr [r12 + 3192], rax
  .section .rodata
  .Lrkfn700: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn700]
 lea rsi, [r12 + 3184]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3168], rax
 mov qword ptr [r12 + 3176], rdx
 cmp eax, 99
 je xchain619_n49_α
 jmp xchain619_n71_α
 xchain619_n68_β:
 jmp xchain619_n49_α
# IR_VAR
 xchain619_n69_α:
 mov rax, qword ptr [rbx + 336]
 mov rdx, qword ptr [rbx + 344]
 mov qword ptr [r12 + 3440], rax
 mov qword ptr [r12 + 3448], rdx
 jmp xchain619_n72_α
 xchain619_n69_β:
 jmp xchain619_n73_α
# IR_LIT_STRING
 xchain619_n70_α:
 mov qword ptr [r12 + 2896], 1
 mov rax, qword ptr [rip + .Lx702_0]
 mov qword ptr [r12 + 2904], rax
 jmp xchain619_n74_α
 xchain619_n70_β:
 jmp xchain619_n42_α
.Lx702_0:
 .quad .Lx702_0_s
.Lx702_0_s:
 .string "EXPR$9"
 xchain619_n71_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3136] -> [zr+3088]
 mov rax, qword ptr [r12 + 3136]
 mov qword ptr [r12 + 3088], rax
 mov rax, qword ptr [r12 + 3144]
 mov qword ptr [r12 + 3096], rax
# marshal arg1 = producer-box slot [zr+3152] -> [zr+3104]
 mov rax, qword ptr [r12 + 3152]
 mov qword ptr [r12 + 3104], rax
 mov rax, qword ptr [r12 + 3160]
 mov qword ptr [r12 + 3112], rax
# marshal arg2 = producer-box slot [zr+3168] -> [zr+3120]
 mov rax, qword ptr [r12 + 3168]
 mov qword ptr [r12 + 3120], rax
 mov rax, qword ptr [r12 + 3176]
 mov qword ptr [r12 + 3128], rax
  .section .rodata
  .Lbynamefn455: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn455]
 lea rsi, [r12 + 3088]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3072], rax
 mov qword ptr [r12 + 3080], rdx
 cmp eax, 99
 je xchain619_n49_α
 jmp xchain619_n75_α
 xchain619_n71_β:
 jmp xchain619_n49_α
# IR_MATCH_HEAD
 xchain619_n72_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 3384], rax
 mov qword ptr [r12 + 3392], rsp
 mov rdi, qword ptr [r12 + 3440]
 mov rsi, qword ptr [r12 + 3448]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 3376], 0
.Lx705_0:
 mov r14d, dword ptr [r12 + 3376]
 jmp xchain619_n76_α
 xchain619_n72_β:
 add dword ptr [r12 + 3376], 1
 mov eax, dword ptr [r12 + 3376]
 cmp eax, r15d
 jg .Lx705_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx705_1
 jmp .Lx705_0
.Lx705_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 3384]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 3392]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain619_n73_α
# IR_LIT_STRING
 xchain619_n73_α:
 mov qword ptr [r12 + 3504], 1
 mov rax, qword ptr [rip + .Lx706_0]
 mov qword ptr [r12 + 3512], rax
 jmp xchain619_n77_α
 xchain619_n73_β:
 jmp xchain619_n61_α
.Lx706_0:
 .quad .Lx706_0_s
.Lx706_0_s:
 .string "Bad input, try again"
 xchain619_n74_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2896] -> [zr+2880]
 mov rax, qword ptr [r12 + 2896]
 mov qword ptr [r12 + 2880], rax
 mov rax, qword ptr [r12 + 2904]
 mov qword ptr [r12 + 2888], rax
  .section .rodata
  .Lrkfn708: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn708]
 lea rsi, [r12 + 2880]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2864], rax
 mov qword ptr [r12 + 2872], rdx
 cmp eax, 99
 je xchain619_n42_α
 jmp xchain619_n78_α
 xchain619_n74_β:
 jmp xchain619_n42_α
 xchain619_n75_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2992]
 mov rsi, qword ptr [r12 + 3000]
 mov rdx, qword ptr [r12 + 3072]
 mov rcx, qword ptr [r12 + 3080]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2976], rax
 mov qword ptr [r12 + 2984], rdx
 jmp xchain619_n79_α
 xchain619_n75_β:
 jmp xchain619_n49_α
# IR_LIT_INTEGER
 xchain619_n76_α:
 mov qword ptr [r12 + 3424], 6
 mov rax, qword ptr [rip + .Lx710_0]
 mov qword ptr [r12 + 3432], rax
 jmp xchain619_n80_α
 xchain619_n76_β:
 jmp xchain619_n72_α
.Lx710_0:
 .quad 0
# IR_ASSIGN global
 xchain619_n77_α:
 mov rsi, qword ptr [r12 + 3504]
 mov rdx, qword ptr [r12 + 3512]
 mov rdi, qword ptr [rip + .Lx711_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 3488], rax
 mov qword ptr [r12 + 3496], rdx
 jmp xchain619_n61_α
 xchain619_n77_β:
 jmp xchain619_n61_α
.Lx711_0:
 .quad .Lx711_0_s
.Lx711_0_s:
 .string "OUTPUT"
 xchain619_n78_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2624] -> [zr+2592]
 mov rax, qword ptr [r12 + 2624]
 mov qword ptr [r12 + 2592], rax
 mov rax, qword ptr [r12 + 2632]
 mov qword ptr [r12 + 2600], rax
# marshal arg1 = producer-box slot [zr+2864] -> [zr+2608]
 mov rax, qword ptr [r12 + 2864]
 mov qword ptr [r12 + 2608], rax
 mov rax, qword ptr [r12 + 2872]
 mov qword ptr [r12 + 2616], rax
  .section .rodata
  .Lbynamefn462: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn462]
 lea rsi, [r12 + 2592]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2576], rax
 mov qword ptr [r12 + 2584], rdx
 cmp eax, 99
 je xchain619_n42_α
 jmp xchain619_n81_α
 xchain619_n78_β:
 jmp xchain619_n42_α
# IR_LIT_STRING
 xchain619_n79_α:
 mov qword ptr [r12 + 3248], 1
 mov rax, qword ptr [rip + .Lx713_0]
 mov qword ptr [r12 + 3256], rax
 jmp xchain619_n82_α
 xchain619_n79_β:
 jmp xchain619_n49_α
.Lx713_0:
 .quad .Lx713_0_s
.Lx713_0_s:
 .string "EXPR$13"
# IR_MATCH_POS
 xchain619_n80_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain619_n72_β
 jmp xchain619_n83_α
 xchain619_n80_β:
 jmp xchain619_n72_β
# IR_ASSIGN gva
 xchain619_n81_α:
 mov rax, qword ptr [r12 + 2576]
 mov rdx, qword ptr [r12 + 2584]
 mov qword ptr [rbx + 304], rax
 mov qword ptr [rbx + 312], rdx
 mov qword ptr [r12 + 2560], rax
 mov qword ptr [r12 + 2568], rdx
 jmp xchain619_n42_α
 xchain619_n81_β:
 jmp xchain619_n42_α
 xchain619_n82_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3248] -> [zr+3232]
 mov rax, qword ptr [r12 + 3248]
 mov qword ptr [r12 + 3232], rax
 mov rax, qword ptr [r12 + 3256]
 mov qword ptr [r12 + 3240], rax
  .section .rodata
  .Lrkfn717: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn717]
 lea rsi, [r12 + 3232]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3216], rax
 mov qword ptr [r12 + 3224], rdx
 cmp eax, 99
 je xchain619_n49_α
 jmp xchain619_n84_α
 xchain619_n82_β:
 jmp xchain619_n49_α
# IR_MATCH_DEFER
 xchain619_n83_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx718_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx718_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain619_n80_β
.Lx718_1:
 jmp xchain619_n85_α
.Lx718_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx718_2:
 test rax, rax
 je .Lx718_3
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
 jmp .Lx718_2
.Lx718_3:
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
 js xchain619_n80_β
 mov r14d, eax
 jmp xchain619_n85_α
 xchain619_n83_β:
 jmp xchain619_n80_β
 xchain619_n84_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2976] -> [zr+2944]
 mov rax, qword ptr [r12 + 2976]
 mov qword ptr [r12 + 2944], rax
 mov rax, qword ptr [r12 + 2984]
 mov qword ptr [r12 + 2952], rax
# marshal arg1 = producer-box slot [zr+3216] -> [zr+2960]
 mov rax, qword ptr [r12 + 3216]
 mov qword ptr [r12 + 2960], rax
 mov rax, qword ptr [r12 + 3224]
 mov qword ptr [r12 + 2968], rax
  .section .rodata
  .Lbynamefn468: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn468]
 lea rsi, [r12 + 2944]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2928], rax
 mov qword ptr [r12 + 2936], rdx
 cmp eax, 99
 je xchain619_n49_α
 jmp xchain619_n86_α
 xchain619_n84_β:
 jmp xchain619_n49_α
# IR_LIT_INTEGER
 xchain619_n85_α:
 mov qword ptr [r12 + 3408], 6
 mov rax, qword ptr [rip + .Lx720_0]
 mov qword ptr [r12 + 3416], rax
 jmp xchain619_n87_α
 xchain619_n85_β:
 jmp xchain619_n72_α
.Lx720_0:
 .quad 0
# IR_ASSIGN gva
 xchain619_n86_α:
 mov rax, qword ptr [r12 + 2928]
 mov rdx, qword ptr [r12 + 2936]
 mov qword ptr [rbx + 320], rax
 mov qword ptr [rbx + 328], rdx
 mov qword ptr [r12 + 2912], rax
 mov qword ptr [r12 + 2920], rdx
 jmp xchain619_n49_α
 xchain619_n86_β:
 jmp xchain619_n49_α
# IR_MATCH_RPOS
 xchain619_n87_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain619_n83_β
 jmp xchain619_n88_α
 xchain619_n87_β:
 jmp xchain619_n83_β
# IR_MATCH_RELEASE
 xchain619_n88_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 3384]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 3392]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain619_n89_α
 xchain619_n89_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx726_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx726_1
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
 jmp .Lx726_2
.Lx726_1:
 call rt_faildescr@PLT
.Lx726_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 3472], rax
 mov qword ptr [r12 + 3480], rdx
 cmp eax, 99
 je xchain619_n61_α
 jmp xchain619_n90_α
 xchain619_n89_β:
 jmp xchain619_n61_α
.Lx726_0:
 .quad .Lx726_0_s
.Lx726_0_s:
 .string "Pop"
# IR_ASSIGN global
 xchain619_n90_α:
 mov rsi, qword ptr [r12 + 3472]
 mov rdx, qword ptr [r12 + 3480]
 mov rdi, qword ptr [rip + .Lx727_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 3456], rax
 mov qword ptr [r12 + 3464], rdx
 jmp xchain619_n61_α
 xchain619_n90_β:
 jmp xchain619_n61_α
.Lx727_0:
 .quad .Lx727_0_s
.Lx727_0_s:
 .string "OUTPUT"
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
 mov rdi, qword ptr [r12 + 3520]
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
.S2: .string "+-"
.S3: .string "epsilon"
.S4: .string "integer"
.S5: .string "."
.S6: .string "exponent"
.S7: .string "constant"
.S8: .string "("
.S9: .string "expr"
.S10: .string ")"
.text
