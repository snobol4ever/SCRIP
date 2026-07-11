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
  .section .rodata
  .Lcall42_pname: .string "Pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall42_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 cmp eax, 99
 je xchain45_n2_α
 jmp xchain45_n1_α
xchain45_n0_β:
 jmp xchain45_n2_α
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
  .section .rodata
  .Lcall44_pname: .string "Pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall44_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 cmp eax, 99
 je xchain45_n4_α
 jmp xchain45_n3_α
xchain45_n2_β:
 jmp xchain45_n4_α
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
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 808], rax
 jmp xchain45_n5_α
 xchain45_n4_β:
 jmp xchain45_n7_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string ""
 xchain45_n5_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+800] -> [zr+832]
 mov rax, qword ptr [r12 + 800]
 mov qword ptr [r12 + 832], rax
 mov rax, qword ptr [r12 + 808]
 mov qword ptr [r12 + 840], rax
  .section .rodata
  .Lrkfn52: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn52]
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
  .section .rodata
  .Lcall48_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall48_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n8_α
xchain45_n6_β:
 jmp xchain45_n7_α
# IR_LIT_STRING
 xchain45_n7_α:
 mov qword ptr [r12 + 1008], 1
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 1016], rax
 jmp xchain45_n9_α
 xchain45_n7_β:
 jmp xchain45_n12_α
.Lx54_0:
 .quad .Lx54_0_s
.Lx54_0_s:
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
  .Lrkfn57: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn57]
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
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain45_n14_α
 xchain45_n12_β:
 jmp proc_Unary_ω
.Lx60_0:
 .quad .Lx60_0_s
.Lx60_0_s:
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
  .Lrkfn63: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn63]
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
  .Lrkfn65: .string "EVAL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn65]
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
 xchain67_n0_α:
  .section .rodata
  .Lcall59_pname: .string "Pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall59_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 cmp eax, 99
 je xchain67_n2_α
 jmp xchain67_n1_α
xchain67_n0_β:
 jmp xchain67_n2_α
# IR_ASSIGN gva
 xchain67_n1_α:
 mov rax, qword ptr [r12 + 1040]
 mov rdx, qword ptr [r12 + 1048]
 mov qword ptr [rbx + 128], rax
 mov qword ptr [rbx + 136], rdx
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 jmp xchain67_n2_α
 xchain67_n1_β:
 jmp xchain67_n2_α
 xchain67_n2_α:
  .section .rodata
  .Lcall61_pname: .string "Pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall61_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 cmp eax, 99
 je xchain67_n4_α
 jmp xchain67_n3_α
xchain67_n2_β:
 jmp xchain67_n4_α
# IR_ASSIGN gva
 xchain67_n3_α:
 mov rax, qword ptr [r12 + 1072]
 mov rdx, qword ptr [r12 + 1080]
 mov qword ptr [rbx + 80], rax
 mov qword ptr [rbx + 88], rdx
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
 jmp xchain67_n4_α
 xchain67_n3_β:
 jmp xchain67_n4_α
 xchain67_n4_α:
  .section .rodata
  .Lcall63_pname: .string "Pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall63_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 cmp eax, 99
 je xchain67_n6_α
 jmp xchain67_n5_α
xchain67_n4_β:
 jmp xchain67_n6_α
# IR_ASSIGN gva
 xchain67_n5_α:
 mov rax, qword ptr [r12 + 1104]
 mov rdx, qword ptr [r12 + 1112]
 mov qword ptr [rbx + 112], rax
 mov qword ptr [rbx + 120], rdx
 mov qword ptr [r12 + 1088], rax
 mov qword ptr [r12 + 1096], rdx
 jmp xchain67_n6_α
 xchain67_n5_β:
 jmp xchain67_n6_α
# IR_LIT_STRING
 xchain67_n6_α:
 mov qword ptr [r12 + 1120], 1
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain67_n7_α
 xchain67_n6_β:
 jmp xchain67_n9_α
.Lx74_0:
 .quad .Lx74_0_s
.Lx74_0_s:
 .string ""
 xchain67_n7_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1120] -> [zr+1152]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1152], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1160], rax
  .section .rodata
  .Lrkfn76: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn76]
 lea rsi, [r12 + 1152]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
 cmp eax, 99
 je xchain67_n9_α
 jmp xchain67_n8_α
 xchain67_n7_β:
 jmp xchain67_n9_α
 xchain67_n8_α:
  .section .rodata
  .Lcall67_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall67_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
 cmp eax, 99
 je xchain67_n9_α
 jmp xchain67_n10_α
xchain67_n8_β:
 jmp xchain67_n9_α
# IR_LIT_STRING
 xchain67_n9_α:
 mov qword ptr [r12 + 1424], 1
 mov rax, qword ptr [rip + .Lx78_0]
 mov qword ptr [r12 + 1432], rax
 jmp xchain67_n11_α
 xchain67_n9_β:
 jmp xchain67_n14_α
.Lx78_0:
 .quad .Lx78_0_s
.Lx78_0_s:
 .string "dummy"
# IR_VAR
 xchain67_n10_α:
 mov rax, qword ptr [rbx + 112]
 mov rdx, qword ptr [rbx + 120]
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 jmp xchain67_n12_α
 xchain67_n10_β:
 jmp xchain67_n9_α
 xchain67_n11_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1424] -> [zr+1408]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1408], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1416], rax
  .section .rodata
  .Lrkfn81: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn81]
 lea rsi, [r12 + 1408]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1392], rax
 mov qword ptr [r12 + 1400], rdx
 cmp eax, 99
 je xchain67_n14_α
 jmp xchain67_n13_α
 xchain67_n11_β:
 jmp xchain67_n14_α
# IR_LIT_STRING
 xchain67_n12_α:
 mov qword ptr [r12 + 1296], 1
 mov rax, qword ptr [rip + .Lx82_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain67_n15_α
 xchain67_n12_β:
 jmp xchain67_n9_α
.Lx82_0:
 .quad .Lx82_0_s
.Lx82_0_s:
 .string " "
# IR_ASSIGN gva
 xchain67_n13_α:
 mov rax, qword ptr [r12 + 1392]
 mov rdx, qword ptr [r12 + 1400]
 mov qword ptr [rbx + 96], rax
 mov qword ptr [rbx + 104], rdx
 mov qword ptr [r12 + 1376], rax
 mov qword ptr [r12 + 1384], rdx
 jmp xchain67_n14_α
 xchain67_n13_β:
 jmp xchain67_n14_α
# IR_LIT_STRING
 xchain67_n14_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx84_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain67_n16_α
 xchain67_n14_β:
 jmp proc_Binary_ω
.Lx84_0:
 .quad .Lx84_0_s
.Lx84_0_s:
 .string ""
 xchain67_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 1280]
 mov rsi, qword ptr [r12 + 1288]
 mov rdx, qword ptr [r12 + 1296]
 mov rcx, qword ptr [r12 + 1304]
 call str_concat_d@PLT
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 jmp xchain67_n17_α
 xchain67_n15_β:
 jmp xchain67_n9_α
 xchain67_n16_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn87: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn87]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_Binary_ω
 jmp proc_Binary_γ
 xchain67_n16_β:
 jmp proc_Binary_ω
# IR_VAR
 xchain67_n17_α:
 mov rax, qword ptr [rbx + 80]
 mov rdx, qword ptr [rbx + 88]
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 jmp xchain67_n18_α
 xchain67_n17_β:
 jmp xchain67_n9_α
 xchain67_n18_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 1264]
 mov rsi, qword ptr [r12 + 1272]
 mov rdx, qword ptr [r12 + 1312]
 mov rcx, qword ptr [r12 + 1320]
 call str_concat_d@PLT
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 jmp xchain67_n19_α
 xchain67_n18_β:
 jmp xchain67_n9_α
# IR_LIT_STRING
 xchain67_n19_α:
 mov qword ptr [r12 + 1328], 1
 mov rax, qword ptr [rip + .Lx90_0]
 mov qword ptr [r12 + 1336], rax
 jmp xchain67_n20_α
 xchain67_n19_β:
 jmp xchain67_n9_α
.Lx90_0:
 .quad .Lx90_0_s
.Lx90_0_s:
 .string " "
 xchain67_n20_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 1248]
 mov rsi, qword ptr [r12 + 1256]
 mov rdx, qword ptr [r12 + 1328]
 mov rcx, qword ptr [r12 + 1336]
 call str_concat_d@PLT
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 jmp xchain67_n21_α
 xchain67_n20_β:
 jmp xchain67_n9_α
# IR_VAR
 xchain67_n21_α:
 mov rax, qword ptr [rbx + 128]
 mov rdx, qword ptr [rbx + 136]
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 jmp xchain67_n22_α
 xchain67_n21_β:
 jmp xchain67_n9_α
 xchain67_n22_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 1232]
 mov rsi, qword ptr [r12 + 1240]
 mov rdx, qword ptr [r12 + 1344]
 mov rcx, qword ptr [r12 + 1352]
 call str_concat_d@PLT
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 jmp xchain67_n23_α
 xchain67_n22_β:
 jmp xchain67_n9_α
 xchain67_n23_α:
# BOX IR_CALL EVAL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1216] -> [zr+1200]
 mov rax, qword ptr [r12 + 1216]
 mov qword ptr [r12 + 1200], rax
 mov rax, qword ptr [r12 + 1224]
 mov qword ptr [r12 + 1208], rax
  .section .rodata
  .Lrkfn95: .string "EVAL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn95]
 lea rsi, [r12 + 1200]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 cmp eax, 99
 je xchain67_n9_α
 jmp xchain67_n24_α
 xchain67_n23_β:
 jmp xchain67_n9_α
# IR_ASSIGN_VAR
 xchain67_n24_α:
 mov rdi, qword ptr [r12 + 1168]
 mov rsi, qword ptr [r12 + 1176]
 mov rdx, qword ptr [r12 + 1184]
 mov rcx, qword ptr [r12 + 1192]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain67_n9_α
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 jmp xchain67_n9_α
 xchain67_n24_β:
 jmp xchain67_n9_α
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
 xchain97_n0_α:
  .section .rodata
  .Lcall84_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall84_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$0_ω
 jmp xchain97_n1_α
xchain97_n0_β:
 jmp proc_EXPR$0_ω
# IR_ASSIGN gva
 xchain97_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 352], rax
 mov qword ptr [rbx + 360], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$0_γ
 xchain97_n1_β:
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
 xchain100_n0_α:
  .section .rodata
  .Lcall86_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall86_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$1_ω
 jmp xchain100_n1_α
xchain100_n0_β:
 jmp proc_EXPR$1_ω
# IR_ASSIGN gva
 xchain100_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 368], rax
 mov qword ptr [rbx + 376], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$1_γ
 xchain100_n1_β:
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
 xchain103_n0_α:
  .section .rodata
  .Lcall88_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall88_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$2_ω
 jmp xchain103_n1_α
xchain103_n0_β:
 jmp proc_EXPR$2_ω
# IR_ASSIGN gva
 xchain103_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 384], rax
 mov qword ptr [rbx + 392], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$2_γ
 xchain103_n1_β:
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
 xchain106_n0_α:
  .section .rodata
  .Lcall90_pname: .string "Unary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall90_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$3_ω
 jmp xchain106_n1_α
xchain106_n0_β:
 jmp proc_EXPR$3_ω
# IR_ASSIGN gva
 xchain106_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 400], rax
 mov qword ptr [rbx + 408], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$3_γ
 xchain106_n1_β:
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
 xchain109_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain109_n1_α
 xchain109_n0_β:
 jmp proc_EXPR$4_ω
# IR_ASSIGN gva
 xchain109_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 416], rax
 mov qword ptr [rbx + 424], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$4_γ
 xchain109_n1_β:
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
 xchain112_n0_α:
 mov rax, qword ptr [rbx + 272]
 mov rdx, qword ptr [rbx + 280]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain112_n1_α
 xchain112_n0_β:
 jmp proc_EXPR$5_ω
# IR_ASSIGN gva
 xchain112_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 432], rax
 mov qword ptr [rbx + 440], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$5_γ
 xchain112_n1_β:
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
 xchain115_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain115_n1_α
 xchain115_n0_β:
 jmp proc_EXPR$6_ω
# IR_ASSIGN gva
 xchain115_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 448], rax
 mov qword ptr [rbx + 456], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$6_γ
 xchain115_n1_β:
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
 xchain118_n0_α:
  .section .rodata
  .Lcall98_pname: .string "Binary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall98_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$7_ω
 jmp xchain118_n1_α
xchain118_n0_β:
 jmp proc_EXPR$7_ω
# IR_ASSIGN gva
 xchain118_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 464], rax
 mov qword ptr [rbx + 472], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$7_γ
 xchain118_n1_β:
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
 xchain121_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain121_n1_α
 xchain121_n0_β:
 jmp proc_EXPR$8_ω
# IR_ASSIGN gva
 xchain121_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 480], rax
 mov qword ptr [rbx + 488], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$8_γ
 xchain121_n1_β:
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
 xchain124_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain124_n1_α
 xchain124_n0_β:
 jmp proc_EXPR$9_ω
# IR_ASSIGN gva
 xchain124_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 496], rax
 mov qword ptr [rbx + 504], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$9_γ
 xchain124_n1_β:
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
 xchain127_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain127_n1_α
 xchain127_n0_β:
 jmp proc_EXPR$10_ω
# IR_ASSIGN gva
 xchain127_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 512], rax
 mov qword ptr [rbx + 520], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$10_γ
 xchain127_n1_β:
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
 xchain130_n0_α:
  .section .rodata
  .Lcall106_pname: .string "Binary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall106_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$11_ω
 jmp xchain130_n1_α
xchain130_n0_β:
 jmp proc_EXPR$11_ω
# IR_ASSIGN gva
 xchain130_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 528], rax
 mov qword ptr [rbx + 536], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$11_γ
 xchain130_n1_β:
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
 xchain133_n0_α:
 mov rax, qword ptr [rbx + 320]
 mov rdx, qword ptr [rbx + 328]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain133_n1_α
 xchain133_n0_β:
 jmp proc_EXPR$12_ω
# IR_ASSIGN gva
 xchain133_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 544], rax
 mov qword ptr [rbx + 552], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$12_γ
 xchain133_n1_β:
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
 xchain136_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain136_n1_α
 xchain136_n0_β:
 jmp proc_EXPR$13_ω
# IR_ASSIGN gva
 xchain136_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 560], rax
 mov qword ptr [rbx + 568], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$13_γ
 xchain136_n1_β:
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
 xchain139_n0_α:
  .section .rodata
  .Lcall112_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall112_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$14_ω
 jmp xchain139_n1_α
xchain139_n0_β:
 jmp proc_EXPR$14_ω
# IR_ASSIGN gva
 xchain139_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 576], rax
 mov qword ptr [rbx + 584], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$14_γ
 xchain139_n1_β:
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
 xchain142_n0_α:
  .section .rodata
  .Lcall114_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall114_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$15_ω
 jmp xchain142_n1_α
xchain142_n0_β:
 jmp proc_EXPR$15_ω
# IR_ASSIGN gva
 xchain142_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 592], rax
 mov qword ptr [rbx + 600], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$15_γ
 xchain142_n1_β:
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
 xchain145_n0_α:
  .section .rodata
  .Lcall116_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall116_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$16_ω
 jmp xchain145_n1_α
xchain145_n0_β:
 jmp proc_EXPR$16_ω
# IR_ASSIGN gva
 xchain145_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 608], rax
 mov qword ptr [rbx + 616], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$16_γ
 xchain145_n1_β:
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
 xchain148_n0_α:
  .section .rodata
  .Lcall118_pname: .string "Unary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall118_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$17_ω
 jmp xchain148_n1_α
xchain148_n0_β:
 jmp proc_EXPR$17_ω
# IR_ASSIGN gva
 xchain148_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 624], rax
 mov qword ptr [rbx + 632], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$17_γ
 xchain148_n1_β:
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
 xchain151_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain151_n1_α
 xchain151_n0_β:
 jmp proc_EXPR$18_ω
# IR_ASSIGN gva
 xchain151_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 640], rax
 mov qword ptr [rbx + 648], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$18_γ
 xchain151_n1_β:
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
 xchain154_n0_α:
 mov rax, qword ptr [rbx + 272]
 mov rdx, qword ptr [rbx + 280]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain154_n1_α
 xchain154_n0_β:
 jmp proc_EXPR$19_ω
# IR_ASSIGN gva
 xchain154_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 656], rax
 mov qword ptr [rbx + 664], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$19_γ
 xchain154_n1_β:
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
 xchain157_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain157_n1_α
 xchain157_n0_β:
 jmp proc_EXPR$20_ω
# IR_ASSIGN gva
 xchain157_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 672], rax
 mov qword ptr [rbx + 680], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$20_γ
 xchain157_n1_β:
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
 xchain160_n0_α:
  .section .rodata
  .Lcall126_pname: .string "Binary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall126_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$21_ω
 jmp xchain160_n1_α
xchain160_n0_β:
 jmp proc_EXPR$21_ω
# IR_ASSIGN gva
 xchain160_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 688], rax
 mov qword ptr [rbx + 696], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$21_γ
 xchain160_n1_β:
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
 xchain163_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain163_n1_α
 xchain163_n0_β:
 jmp proc_EXPR$22_ω
# IR_ASSIGN gva
 xchain163_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 704], rax
 mov qword ptr [rbx + 712], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$22_γ
 xchain163_n1_β:
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
 xchain166_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain166_n1_α
 xchain166_n0_β:
 jmp proc_EXPR$23_ω
# IR_ASSIGN gva
 xchain166_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 720], rax
 mov qword ptr [rbx + 728], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$23_γ
 xchain166_n1_β:
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
 xchain169_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain169_n1_α
 xchain169_n0_β:
 jmp proc_EXPR$24_ω
# IR_ASSIGN gva
 xchain169_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 736], rax
 mov qword ptr [rbx + 744], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$24_γ
 xchain169_n1_β:
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
 xchain172_n0_α:
  .section .rodata
  .Lcall134_pname: .string "Binary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall134_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$25_ω
 jmp xchain172_n1_α
xchain172_n0_β:
 jmp proc_EXPR$25_ω
# IR_ASSIGN gva
 xchain172_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 752], rax
 mov qword ptr [rbx + 760], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$25_γ
 xchain172_n1_β:
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
 xchain175_n0_α:
 mov rax, qword ptr [rbx + 320]
 mov rdx, qword ptr [rbx + 328]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain175_n1_α
 xchain175_n0_β:
 jmp proc_EXPR$26_ω
# IR_ASSIGN gva
 xchain175_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 768], rax
 mov qword ptr [rbx + 776], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$26_γ
 xchain175_n1_β:
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
 xchain178_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain178_n1_α
 xchain178_n0_β:
 jmp proc_EXPR$27_ω
# IR_ASSIGN gva
 xchain178_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 784], rax
 mov qword ptr [rbx + 792], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$27_γ
 xchain178_n1_β:
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
 xchain181_n0_α:
  .section .rodata
  .Lcall140_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall140_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$28_ω
 jmp xchain181_n1_α
xchain181_n0_β:
 jmp proc_EXPR$28_ω
# IR_ASSIGN gva
 xchain181_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 800], rax
 mov qword ptr [rbx + 808], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$28_γ
 xchain181_n1_β:
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
 xchain184_n0_α:
  .section .rodata
  .Lcall142_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall142_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$29_ω
 jmp xchain184_n1_α
xchain184_n0_β:
 jmp proc_EXPR$29_ω
# IR_ASSIGN gva
 xchain184_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 816], rax
 mov qword ptr [rbx + 824], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$29_γ
 xchain184_n1_β:
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
 xchain187_n0_α:
  .section .rodata
  .Lcall144_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall144_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$30_ω
 jmp xchain187_n1_α
xchain187_n0_β:
 jmp proc_EXPR$30_ω
# IR_ASSIGN gva
 xchain187_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 832], rax
 mov qword ptr [rbx + 840], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$30_γ
 xchain187_n1_β:
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
 xchain190_n0_α:
  .section .rodata
  .Lcall146_pname: .string "Unary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall146_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$31_ω
 jmp xchain190_n1_α
xchain190_n0_β:
 jmp proc_EXPR$31_ω
# IR_ASSIGN gva
 xchain190_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 848], rax
 mov qword ptr [rbx + 856], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$31_γ
 xchain190_n1_β:
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
 xchain193_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain193_n1_α
 xchain193_n0_β:
 jmp proc_EXPR$32_ω
# IR_ASSIGN gva
 xchain193_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 864], rax
 mov qword ptr [rbx + 872], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$32_γ
 xchain193_n1_β:
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
 xchain196_n0_α:
 mov rax, qword ptr [rbx + 272]
 mov rdx, qword ptr [rbx + 280]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain196_n1_α
 xchain196_n0_β:
 jmp proc_EXPR$33_ω
# IR_ASSIGN gva
 xchain196_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 880], rax
 mov qword ptr [rbx + 888], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$33_γ
 xchain196_n1_β:
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
 xchain199_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain199_n1_α
 xchain199_n0_β:
 jmp proc_EXPR$34_ω
# IR_ASSIGN gva
 xchain199_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 896], rax
 mov qword ptr [rbx + 904], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$34_γ
 xchain199_n1_β:
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
 xchain202_n0_α:
  .section .rodata
  .Lcall154_pname: .string "Binary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall154_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$35_ω
 jmp xchain202_n1_α
xchain202_n0_β:
 jmp proc_EXPR$35_ω
# IR_ASSIGN gva
 xchain202_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 912], rax
 mov qword ptr [rbx + 920], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$35_γ
 xchain202_n1_β:
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
 xchain205_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain205_n1_α
 xchain205_n0_β:
 jmp proc_EXPR$36_ω
# IR_ASSIGN gva
 xchain205_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 928], rax
 mov qword ptr [rbx + 936], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$36_γ
 xchain205_n1_β:
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
 xchain208_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain208_n1_α
 xchain208_n0_β:
 jmp proc_EXPR$37_ω
# IR_ASSIGN gva
 xchain208_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 944], rax
 mov qword ptr [rbx + 952], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$37_γ
 xchain208_n1_β:
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
 xchain211_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain211_n1_α
 xchain211_n0_β:
 jmp proc_EXPR$38_ω
# IR_ASSIGN gva
 xchain211_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 960], rax
 mov qword ptr [rbx + 968], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$38_γ
 xchain211_n1_β:
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
 xchain214_n0_α:
  .section .rodata
  .Lcall162_pname: .string "Binary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall162_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$39_ω
 jmp xchain214_n1_α
xchain214_n0_β:
 jmp proc_EXPR$39_ω
# IR_ASSIGN gva
 xchain214_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 976], rax
 mov qword ptr [rbx + 984], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$39_γ
 xchain214_n1_β:
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
 xchain217_n0_α:
 mov rax, qword ptr [rbx + 320]
 mov rdx, qword ptr [rbx + 328]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain217_n1_α
 xchain217_n0_β:
 jmp proc_EXPR$40_ω
# IR_ASSIGN gva
 xchain217_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 992], rax
 mov qword ptr [rbx + 1000], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$40_γ
 xchain217_n1_β:
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
 xchain220_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain220_n1_α
 xchain220_n0_β:
 jmp proc_EXPR$41_ω
# IR_ASSIGN gva
 xchain220_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1008], rax
 mov qword ptr [rbx + 1016], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$41_γ
 xchain220_n1_β:
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
 xchain223_n0_α:
  .section .rodata
  .Lcall168_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall168_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$42_ω
 jmp xchain223_n1_α
xchain223_n0_β:
 jmp proc_EXPR$42_ω
# IR_ASSIGN gva
 xchain223_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1024], rax
 mov qword ptr [rbx + 1032], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$42_γ
 xchain223_n1_β:
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
 xchain226_n0_α:
  .section .rodata
  .Lcall170_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall170_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$43_ω
 jmp xchain226_n1_α
xchain226_n0_β:
 jmp proc_EXPR$43_ω
# IR_ASSIGN gva
 xchain226_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1040], rax
 mov qword ptr [rbx + 1048], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$43_γ
 xchain226_n1_β:
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
 xchain229_n0_α:
  .section .rodata
  .Lcall172_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall172_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$44_ω
 jmp xchain229_n1_α
xchain229_n0_β:
 jmp proc_EXPR$44_ω
# IR_ASSIGN gva
 xchain229_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1056], rax
 mov qword ptr [rbx + 1064], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$44_γ
 xchain229_n1_β:
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
 xchain232_n0_α:
  .section .rodata
  .Lcall174_pname: .string "Unary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall174_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$45_ω
 jmp xchain232_n1_α
xchain232_n0_β:
 jmp proc_EXPR$45_ω
# IR_ASSIGN gva
 xchain232_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1072], rax
 mov qword ptr [rbx + 1080], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$45_γ
 xchain232_n1_β:
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
 xchain235_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain235_n1_α
 xchain235_n0_β:
 jmp proc_EXPR$46_ω
# IR_ASSIGN gva
 xchain235_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1088], rax
 mov qword ptr [rbx + 1096], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$46_γ
 xchain235_n1_β:
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
 xchain238_n0_α:
 mov rax, qword ptr [rbx + 272]
 mov rdx, qword ptr [rbx + 280]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain238_n1_α
 xchain238_n0_β:
 jmp proc_EXPR$47_ω
# IR_ASSIGN gva
 xchain238_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1104], rax
 mov qword ptr [rbx + 1112], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$47_γ
 xchain238_n1_β:
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
 xchain241_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain241_n1_α
 xchain241_n0_β:
 jmp proc_EXPR$48_ω
# IR_ASSIGN gva
 xchain241_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1120], rax
 mov qword ptr [rbx + 1128], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$48_γ
 xchain241_n1_β:
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
 xchain244_n0_α:
  .section .rodata
  .Lcall182_pname: .string "Binary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall182_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$49_ω
 jmp xchain244_n1_α
xchain244_n0_β:
 jmp proc_EXPR$49_ω
# IR_ASSIGN gva
 xchain244_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1136], rax
 mov qword ptr [rbx + 1144], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$49_γ
 xchain244_n1_β:
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
 xchain247_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain247_n1_α
 xchain247_n0_β:
 jmp proc_EXPR$50_ω
# IR_ASSIGN gva
 xchain247_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1152], rax
 mov qword ptr [rbx + 1160], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$50_γ
 xchain247_n1_β:
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
 xchain250_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain250_n1_α
 xchain250_n0_β:
 jmp proc_EXPR$51_ω
# IR_ASSIGN gva
 xchain250_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1168], rax
 mov qword ptr [rbx + 1176], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$51_γ
 xchain250_n1_β:
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
 xchain253_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain253_n1_α
 xchain253_n0_β:
 jmp proc_EXPR$52_ω
# IR_ASSIGN gva
 xchain253_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1184], rax
 mov qword ptr [rbx + 1192], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$52_γ
 xchain253_n1_β:
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
 xchain256_n0_α:
  .section .rodata
  .Lcall190_pname: .string "Binary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall190_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$53_ω
 jmp xchain256_n1_α
xchain256_n0_β:
 jmp proc_EXPR$53_ω
# IR_ASSIGN gva
 xchain256_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1200], rax
 mov qword ptr [rbx + 1208], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$53_γ
 xchain256_n1_β:
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
 xchain259_n0_α:
 mov rax, qword ptr [rbx + 320]
 mov rdx, qword ptr [rbx + 328]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain259_n1_α
 xchain259_n0_β:
 jmp proc_EXPR$54_ω
# IR_ASSIGN gva
 xchain259_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1216], rax
 mov qword ptr [rbx + 1224], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$54_γ
 xchain259_n1_β:
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
 xchain262_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain262_n1_α
 xchain262_n0_β:
 jmp proc_EXPR$55_ω
# IR_ASSIGN gva
 xchain262_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1232], rax
 mov qword ptr [rbx + 1240], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$55_γ
 xchain262_n1_β:
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
 xchain265_n0_α:
  .section .rodata
  .Lcall196_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall196_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$56_ω
 jmp xchain265_n1_α
xchain265_n0_β:
 jmp proc_EXPR$56_ω
# IR_ASSIGN gva
 xchain265_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1248], rax
 mov qword ptr [rbx + 1256], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$56_γ
 xchain265_n1_β:
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
 xchain268_n0_α:
  .section .rodata
  .Lcall198_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall198_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$57_ω
 jmp xchain268_n1_α
xchain268_n0_β:
 jmp proc_EXPR$57_ω
# IR_ASSIGN gva
 xchain268_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1264], rax
 mov qword ptr [rbx + 1272], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$57_γ
 xchain268_n1_β:
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
 xchain271_n0_α:
  .section .rodata
  .Lcall200_pname: .string "Push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall200_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$58_ω
 jmp xchain271_n1_α
xchain271_n0_β:
 jmp proc_EXPR$58_ω
# IR_ASSIGN gva
 xchain271_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1280], rax
 mov qword ptr [rbx + 1288], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$58_γ
 xchain271_n1_β:
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
 xchain274_n0_α:
  .section .rodata
  .Lcall202_pname: .string "Unary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall202_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$59_ω
 jmp xchain274_n1_α
xchain274_n0_β:
 jmp proc_EXPR$59_ω
# IR_ASSIGN gva
 xchain274_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1296], rax
 mov qword ptr [rbx + 1304], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$59_γ
 xchain274_n1_β:
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
 xchain277_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain277_n1_α
 xchain277_n0_β:
 jmp proc_EXPR$60_ω
# IR_ASSIGN gva
 xchain277_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1312], rax
 mov qword ptr [rbx + 1320], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$60_γ
 xchain277_n1_β:
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
 xchain280_n0_α:
 mov rax, qword ptr [rbx + 272]
 mov rdx, qword ptr [rbx + 280]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain280_n1_α
 xchain280_n0_β:
 jmp proc_EXPR$61_ω
# IR_ASSIGN gva
 xchain280_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1328], rax
 mov qword ptr [rbx + 1336], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$61_γ
 xchain280_n1_β:
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
 xchain283_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain283_n1_α
 xchain283_n0_β:
 jmp proc_EXPR$62_ω
# IR_ASSIGN gva
 xchain283_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1344], rax
 mov qword ptr [rbx + 1352], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$62_γ
 xchain283_n1_β:
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
 xchain286_n0_α:
  .section .rodata
  .Lcall210_pname: .string "Binary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall210_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$63_ω
 jmp xchain286_n1_α
xchain286_n0_β:
 jmp proc_EXPR$63_ω
# IR_ASSIGN gva
 xchain286_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1360], rax
 mov qword ptr [rbx + 1368], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$63_γ
 xchain286_n1_β:
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
 xchain289_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain289_n1_α
 xchain289_n0_β:
 jmp proc_EXPR$64_ω
# IR_ASSIGN gva
 xchain289_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1376], rax
 mov qword ptr [rbx + 1384], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$64_γ
 xchain289_n1_β:
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
 xchain292_n0_α:
 mov rax, qword ptr [rbx + 288]
 mov rdx, qword ptr [rbx + 296]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain292_n1_α
 xchain292_n0_β:
 jmp proc_EXPR$65_ω
# IR_ASSIGN gva
 xchain292_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1392], rax
 mov qword ptr [rbx + 1400], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$65_γ
 xchain292_n1_β:
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
 xchain295_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain295_n1_α
 xchain295_n0_β:
 jmp proc_EXPR$66_ω
# IR_ASSIGN gva
 xchain295_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1408], rax
 mov qword ptr [rbx + 1416], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$66_γ
 xchain295_n1_β:
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
 xchain298_n0_α:
  .section .rodata
  .Lcall218_pname: .string "Binary"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall218_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$67_ω
 jmp xchain298_n1_α
xchain298_n0_β:
 jmp proc_EXPR$67_ω
# IR_ASSIGN gva
 xchain298_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1424], rax
 mov qword ptr [rbx + 1432], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$67_γ
 xchain298_n1_β:
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
 xchain301_n0_α:
 mov rax, qword ptr [rbx + 320]
 mov rdx, qword ptr [rbx + 328]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain301_n1_α
 xchain301_n0_β:
 jmp proc_EXPR$68_ω
# IR_ASSIGN gva
 xchain301_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1440], rax
 mov qword ptr [rbx + 1448], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$68_γ
 xchain301_n1_β:
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
 xchain304_n0_α:
 mov rax, qword ptr [rbx + 304]
 mov rdx, qword ptr [rbx + 312]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain304_n1_α
 xchain304_n0_β:
 jmp proc_EXPR$69_ω
# IR_ASSIGN gva
 xchain304_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 1456], rax
 mov qword ptr [rbx + 1464], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$69_γ
 xchain304_n1_β:
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
 xchain307_n0_α:
 mov dword ptr [r12 + 16], 0
.Lx309_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jge .Lx309_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx309_1
 add dword ptr [r12 + 16], 1
 jmp .Lx309_0
.Lx309_1:
 mov eax, dword ptr [r12 + 16]
 test eax, eax
 jle proc_PAT$0_ω
 mov edx, r14d
 mov dword ptr [r12 + 20], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$0_γ
 xchain307_n0_β:
 xchain307_n0_β:
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
 xchain310_n0_α:
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
 jmp xchain310_n1_α
 xchain310_n0_β:
 sub r14d, 1
 jmp proc_PAT$1_ω
# IR_MATCH_ALT_SAVE
 xchain310_n1_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain310_n2_α
 xchain310_n1_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_ANY
 xchain310_n2_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain310_n3_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S2]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain310_n3_β
 add r14d, 1
 jmp xchain310_n3_α
 xchain310_n2_β:
 sub r14d, 1
 jmp xchain310_n3_β
# IR_MATCH_ALT_JOIN
 xchain310_n3_α:
 lea rax, [rip + .Lx316_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain310_n5_α
 xchain310_n3_β:
.Lx316_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain310_n4_α
# IR_MATCH_DEFER
 xchain310_n4_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx317_0
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
 je .Lx317_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain310_n6_β
.Lx317_1:
 jmp xchain310_n6_α
.Lx317_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain310_n6_β
 mov r14d, eax
 jmp xchain310_n6_α
 xchain310_n4_β:
 jmp xchain310_n6_β
# IR_MATCH_DEFER
 xchain310_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx318_0
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
 je .Lx318_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain310_n1_β
.Lx318_1:
 jmp proc_PAT$1_γ
.Lx318_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain310_n1_β
 mov r14d, eax
 jmp proc_PAT$1_γ
 xchain310_n5_β:
 jmp xchain310_n1_β
# IR_MATCH_ALT_JOIN
 xchain310_n6_α:
 lea rax, [rip + .Lx320_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain310_n5_α
 xchain310_n6_β:
.Lx320_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$1_ω
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
 xchain321_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain321_n1_α
 xchain321_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain321_n1_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx324_0
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
 je .Lx324_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n3_β
.Lx324_1:
 jmp xchain321_n2_α
.Lx324_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain321_n3_β
 mov r14d, eax
 jmp xchain321_n2_α
 xchain321_n1_β:
 jmp xchain321_n3_β
# IR_MATCH_LIT
 xchain321_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain321_n3_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain321_n3_β
 add r14d, 1
 jmp xchain321_n4_α
 xchain321_n2_β:
 sub r14d, 1
 jmp xchain321_n3_β
# IR_MATCH_ALT_JOIN
 xchain321_n3_α:
 lea rax, [rip + .Lx328_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$2_γ
 xchain321_n3_β:
.Lx328_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n5_α
# IR_MATCH_ALT_SAVE
 xchain321_n4_α:
 mov dword ptr [r12 + 112], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 116], eax
 jmp xchain321_n6_α
 xchain321_n4_β:
 jmp qword ptr [r12 + 120]
# IR_MATCH_DEFER
 xchain321_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx331_0
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
 je .Lx331_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n8_β
.Lx331_1:
 jmp xchain321_n7_α
.Lx331_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain321_n8_β
 mov r14d, eax
 jmp xchain321_n7_α
 xchain321_n5_β:
 jmp xchain321_n8_β
# IR_MATCH_DEFER
 xchain321_n6_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx332_0
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
 je .Lx332_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n9_β
.Lx332_1:
 jmp xchain321_n9_α
.Lx332_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain321_n9_β
 mov r14d, eax
 jmp xchain321_n9_α
 xchain321_n6_β:
 jmp xchain321_n9_β
# IR_MATCH_DEFER
 xchain321_n7_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx333_0
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
 je .Lx333_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n8_β
.Lx333_1:
 jmp xchain321_n8_α
.Lx333_0:
 lea rdi, [rip + .S6]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain321_n8_β
 mov r14d, eax
 jmp xchain321_n8_α
 xchain321_n7_β:
 jmp xchain321_n8_β
# IR_MATCH_ALT_JOIN
 xchain321_n8_α:
 lea rax, [rip + .Lx335_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$2_γ
 xchain321_n8_β:
.Lx335_0:
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
 xchain321_n9_α:
 lea rax, [rip + .Lx337_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain321_n11_α
 xchain321_n9_β:
.Lx337_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n10_α
# IR_MATCH_DEFER
 xchain321_n10_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx338_0
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
 je .Lx338_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n12_β
.Lx338_1:
 jmp xchain321_n12_α
.Lx338_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain321_n12_β
 mov r14d, eax
 jmp xchain321_n12_α
 xchain321_n10_β:
 jmp xchain321_n12_β
# IR_MATCH_ALT_SAVE
 xchain321_n11_α:
 mov dword ptr [r12 + 64], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 68], eax
 jmp xchain321_n13_α
 xchain321_n11_β:
 jmp qword ptr [r12 + 72]
# IR_MATCH_ALT_JOIN
 xchain321_n12_α:
 lea rax, [rip + .Lx342_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain321_n11_α
 xchain321_n12_β:
.Lx342_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n3_β
# IR_MATCH_DEFER
 xchain321_n13_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx343_0
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
 je .Lx343_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n14_β
.Lx343_1:
 jmp xchain321_n14_α
.Lx343_0:
 lea rdi, [rip + .S6]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain321_n14_β
 mov r14d, eax
 jmp xchain321_n14_α
 xchain321_n13_β:
 jmp xchain321_n14_β
# IR_MATCH_ALT_JOIN
 xchain321_n14_α:
 lea rax, [rip + .Lx345_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain321_n3_α
 xchain321_n14_β:
.Lx345_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n15_α
# IR_MATCH_DEFER
 xchain321_n15_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx346_0
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
 je .Lx346_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n16_β
.Lx346_1:
 jmp xchain321_n16_α
.Lx346_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain321_n16_β
 mov r14d, eax
 jmp xchain321_n16_α
 xchain321_n15_β:
 jmp xchain321_n16_β
# IR_MATCH_ALT_JOIN
 xchain321_n16_α:
 lea rax, [rip + .Lx348_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain321_n3_α
 xchain321_n16_β:
.Lx348_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain321_n4_β
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
 xchain349_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain349_n1_α
 xchain349_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain349_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx352_0
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
 je .Lx352_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain349_n2_β
.Lx352_1:
 jmp xchain349_n2_α
.Lx352_0:
 lea rdi, [rip + .S7]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain349_n2_β
 mov r14d, eax
 jmp xchain349_n2_α
 xchain349_n1_β:
 jmp xchain349_n2_β
# IR_MATCH_ALT_JOIN
 xchain349_n2_α:
 lea rax, [rip + .Lx354_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$3_γ
 xchain349_n2_β:
.Lx354_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain349_n3_α
# IR_MATCH_LIT
 xchain349_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain349_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain349_n5_β
 add r14d, 1
 jmp xchain349_n4_α
 xchain349_n3_β:
 sub r14d, 1
 jmp xchain349_n5_β
# IR_MATCH_DEFER
 xchain349_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx357_0
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
 je .Lx357_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain349_n5_β
.Lx357_1:
 jmp xchain349_n6_α
.Lx357_0:
 lea rdi, [rip + .S9]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain349_n5_β
 mov r14d, eax
 jmp xchain349_n6_α
 xchain349_n4_β:
 jmp xchain349_n5_β
# IR_MATCH_ALT_JOIN
 xchain349_n5_α:
 lea rax, [rip + .Lx359_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$3_γ
 xchain349_n5_β:
.Lx359_0:
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
 xchain349_n6_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain349_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain349_n5_β
 add r14d, 1
 jmp xchain349_n5_α
 xchain349_n6_β:
 sub r14d, 1
 jmp xchain349_n5_β
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
 xchain362_n0_α:
 mov dword ptr [r12 + 16], 0
.Lx364_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jge .Lx364_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx364_1
 add dword ptr [r12 + 16], 1
 jmp .Lx364_0
.Lx364_1:
 mov eax, dword ptr [r12 + 16]
 test eax, eax
 jle proc_PAT$4_ω
 mov edx, r14d
 mov dword ptr [r12 + 20], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$4_γ
 xchain362_n0_β:
 xchain362_n0_β:
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
 xchain365_n0_α:
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
 jmp xchain365_n1_α
 xchain365_n0_β:
 sub r14d, 1
 jmp proc_PAT$5_ω
# IR_MATCH_ALT_SAVE
 xchain365_n1_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain365_n2_α
 xchain365_n1_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_ANY
 xchain365_n2_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain365_n3_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S2]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain365_n3_β
 add r14d, 1
 jmp xchain365_n3_α
 xchain365_n2_β:
 sub r14d, 1
 jmp xchain365_n3_β
# IR_MATCH_ALT_JOIN
 xchain365_n3_α:
 lea rax, [rip + .Lx371_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain365_n5_α
 xchain365_n3_β:
.Lx371_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain365_n4_α
# IR_MATCH_DEFER
 xchain365_n4_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx372_0
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
 je .Lx372_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain365_n6_β
.Lx372_1:
 jmp xchain365_n6_α
.Lx372_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain365_n6_β
 mov r14d, eax
 jmp xchain365_n6_α
 xchain365_n4_β:
 jmp xchain365_n6_β
# IR_MATCH_DEFER
 xchain365_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx373_0
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
 je .Lx373_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain365_n1_β
.Lx373_1:
 jmp proc_PAT$5_γ
.Lx373_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain365_n1_β
 mov r14d, eax
 jmp proc_PAT$5_γ
 xchain365_n5_β:
 jmp xchain365_n1_β
# IR_MATCH_ALT_JOIN
 xchain365_n6_α:
 lea rax, [rip + .Lx375_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain365_n5_α
 xchain365_n6_β:
.Lx375_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$5_ω
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
 xchain376_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain376_n1_α
 xchain376_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain376_n1_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx379_0
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
 je .Lx379_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n3_β
.Lx379_1:
 jmp xchain376_n2_α
.Lx379_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain376_n3_β
 mov r14d, eax
 jmp xchain376_n2_α
 xchain376_n1_β:
 jmp xchain376_n3_β
# IR_MATCH_LIT
 xchain376_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain376_n3_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain376_n3_β
 add r14d, 1
 jmp xchain376_n4_α
 xchain376_n2_β:
 sub r14d, 1
 jmp xchain376_n3_β
# IR_MATCH_ALT_JOIN
 xchain376_n3_α:
 lea rax, [rip + .Lx383_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$6_γ
 xchain376_n3_β:
.Lx383_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n5_α
# IR_MATCH_ALT_SAVE
 xchain376_n4_α:
 mov dword ptr [r12 + 112], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 116], eax
 jmp xchain376_n6_α
 xchain376_n4_β:
 jmp qword ptr [r12 + 120]
# IR_MATCH_DEFER
 xchain376_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx386_0
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
 je .Lx386_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n8_β
.Lx386_1:
 jmp xchain376_n7_α
.Lx386_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain376_n8_β
 mov r14d, eax
 jmp xchain376_n7_α
 xchain376_n5_β:
 jmp xchain376_n8_β
# IR_MATCH_DEFER
 xchain376_n6_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx387_0
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
 je .Lx387_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n9_β
.Lx387_1:
 jmp xchain376_n9_α
.Lx387_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain376_n9_β
 mov r14d, eax
 jmp xchain376_n9_α
 xchain376_n6_β:
 jmp xchain376_n9_β
# IR_MATCH_DEFER
 xchain376_n7_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx388_0
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
 je .Lx388_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n8_β
.Lx388_1:
 jmp xchain376_n8_α
.Lx388_0:
 lea rdi, [rip + .S6]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain376_n8_β
 mov r14d, eax
 jmp xchain376_n8_α
 xchain376_n7_β:
 jmp xchain376_n8_β
# IR_MATCH_ALT_JOIN
 xchain376_n8_α:
 lea rax, [rip + .Lx390_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$6_γ
 xchain376_n8_β:
.Lx390_0:
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
 xchain376_n9_α:
 lea rax, [rip + .Lx392_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain376_n11_α
 xchain376_n9_β:
.Lx392_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n10_α
# IR_MATCH_DEFER
 xchain376_n10_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx393_0
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
 je .Lx393_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n12_β
.Lx393_1:
 jmp xchain376_n12_α
.Lx393_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain376_n12_β
 mov r14d, eax
 jmp xchain376_n12_α
 xchain376_n10_β:
 jmp xchain376_n12_β
# IR_MATCH_ALT_SAVE
 xchain376_n11_α:
 mov dword ptr [r12 + 64], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 68], eax
 jmp xchain376_n13_α
 xchain376_n11_β:
 jmp qword ptr [r12 + 72]
# IR_MATCH_ALT_JOIN
 xchain376_n12_α:
 lea rax, [rip + .Lx397_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain376_n11_α
 xchain376_n12_β:
.Lx397_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n3_β
# IR_MATCH_DEFER
 xchain376_n13_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx398_0
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
 je .Lx398_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n14_β
.Lx398_1:
 jmp xchain376_n14_α
.Lx398_0:
 lea rdi, [rip + .S6]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain376_n14_β
 mov r14d, eax
 jmp xchain376_n14_α
 xchain376_n13_β:
 jmp xchain376_n14_β
# IR_MATCH_ALT_JOIN
 xchain376_n14_α:
 lea rax, [rip + .Lx400_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain376_n3_α
 xchain376_n14_β:
.Lx400_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n15_α
# IR_MATCH_DEFER
 xchain376_n15_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx401_0
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
 je .Lx401_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n16_β
.Lx401_1:
 jmp xchain376_n16_α
.Lx401_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain376_n16_β
 mov r14d, eax
 jmp xchain376_n16_α
 xchain376_n15_β:
 jmp xchain376_n16_β
# IR_MATCH_ALT_JOIN
 xchain376_n16_α:
 lea rax, [rip + .Lx403_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain376_n3_α
 xchain376_n16_β:
.Lx403_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain376_n4_β
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
 xchain404_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain404_n1_α
 xchain404_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain404_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx407_0
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
 je .Lx407_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain404_n2_β
.Lx407_1:
 jmp xchain404_n2_α
.Lx407_0:
 lea rdi, [rip + .S7]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain404_n2_β
 mov r14d, eax
 jmp xchain404_n2_α
 xchain404_n1_β:
 jmp xchain404_n2_β
# IR_MATCH_ALT_JOIN
 xchain404_n2_α:
 lea rax, [rip + .Lx409_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$7_γ
 xchain404_n2_β:
.Lx409_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain404_n3_α
# IR_MATCH_LIT
 xchain404_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain404_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain404_n5_β
 add r14d, 1
 jmp xchain404_n4_α
 xchain404_n3_β:
 sub r14d, 1
 jmp xchain404_n5_β
# IR_MATCH_DEFER
 xchain404_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx412_0
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
 je .Lx412_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain404_n5_β
.Lx412_1:
 jmp xchain404_n6_α
.Lx412_0:
 lea rdi, [rip + .S9]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain404_n5_β
 mov r14d, eax
 jmp xchain404_n6_α
 xchain404_n4_β:
 jmp xchain404_n5_β
# IR_MATCH_ALT_JOIN
 xchain404_n5_α:
 lea rax, [rip + .Lx414_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$7_γ
 xchain404_n5_β:
.Lx414_0:
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
 xchain404_n6_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain404_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain404_n5_β
 add r14d, 1
 jmp xchain404_n5_α
 xchain404_n6_β:
 sub r14d, 1
 jmp xchain404_n5_β
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
 xchain417_n0_α:
 mov dword ptr [r12 + 16], 0
.Lx419_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jge .Lx419_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx419_1
 add dword ptr [r12 + 16], 1
 jmp .Lx419_0
.Lx419_1:
 mov eax, dword ptr [r12 + 16]
 test eax, eax
 jle proc_PAT$8_ω
 mov edx, r14d
 mov dword ptr [r12 + 20], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$8_γ
 xchain417_n0_β:
 xchain417_n0_β:
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
 xchain420_n0_α:
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
 jmp xchain420_n1_α
 xchain420_n0_β:
 sub r14d, 1
 jmp proc_PAT$9_ω
# IR_MATCH_ALT_SAVE
 xchain420_n1_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain420_n2_α
 xchain420_n1_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_ANY
 xchain420_n2_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain420_n3_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S2]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain420_n3_β
 add r14d, 1
 jmp xchain420_n3_α
 xchain420_n2_β:
 sub r14d, 1
 jmp xchain420_n3_β
# IR_MATCH_ALT_JOIN
 xchain420_n3_α:
 lea rax, [rip + .Lx426_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain420_n5_α
 xchain420_n3_β:
.Lx426_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain420_n4_α
# IR_MATCH_DEFER
 xchain420_n4_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx427_0
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
 je .Lx427_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain420_n6_β
.Lx427_1:
 jmp xchain420_n6_α
.Lx427_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain420_n6_β
 mov r14d, eax
 jmp xchain420_n6_α
 xchain420_n4_β:
 jmp xchain420_n6_β
# IR_MATCH_DEFER
 xchain420_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx428_0
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
 je .Lx428_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain420_n1_β
.Lx428_1:
 jmp proc_PAT$9_γ
.Lx428_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain420_n1_β
 mov r14d, eax
 jmp proc_PAT$9_γ
 xchain420_n5_β:
 jmp xchain420_n1_β
# IR_MATCH_ALT_JOIN
 xchain420_n6_α:
 lea rax, [rip + .Lx430_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain420_n5_α
 xchain420_n6_β:
.Lx430_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$9_ω
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
 xchain431_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain431_n1_α
 xchain431_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain431_n1_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx434_0
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
 je .Lx434_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n3_β
.Lx434_1:
 jmp xchain431_n2_α
.Lx434_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain431_n3_β
 mov r14d, eax
 jmp xchain431_n2_α
 xchain431_n1_β:
 jmp xchain431_n3_β
# IR_MATCH_LIT
 xchain431_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain431_n3_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain431_n3_β
 add r14d, 1
 jmp xchain431_n4_α
 xchain431_n2_β:
 sub r14d, 1
 jmp xchain431_n3_β
# IR_MATCH_ALT_JOIN
 xchain431_n3_α:
 lea rax, [rip + .Lx438_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$10_γ
 xchain431_n3_β:
.Lx438_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n5_α
# IR_MATCH_ALT_SAVE
 xchain431_n4_α:
 mov dword ptr [r12 + 112], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 116], eax
 jmp xchain431_n6_α
 xchain431_n4_β:
 jmp qword ptr [r12 + 120]
# IR_MATCH_DEFER
 xchain431_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx441_0
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
 je .Lx441_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n8_β
.Lx441_1:
 jmp xchain431_n7_α
.Lx441_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain431_n8_β
 mov r14d, eax
 jmp xchain431_n7_α
 xchain431_n5_β:
 jmp xchain431_n8_β
# IR_MATCH_DEFER
 xchain431_n6_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx442_0
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
 je .Lx442_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n9_β
.Lx442_1:
 jmp xchain431_n9_α
.Lx442_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain431_n9_β
 mov r14d, eax
 jmp xchain431_n9_α
 xchain431_n6_β:
 jmp xchain431_n9_β
# IR_MATCH_DEFER
 xchain431_n7_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx443_0
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
 je .Lx443_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n8_β
.Lx443_1:
 jmp xchain431_n8_α
.Lx443_0:
 lea rdi, [rip + .S6]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain431_n8_β
 mov r14d, eax
 jmp xchain431_n8_α
 xchain431_n7_β:
 jmp xchain431_n8_β
# IR_MATCH_ALT_JOIN
 xchain431_n8_α:
 lea rax, [rip + .Lx445_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$10_γ
 xchain431_n8_β:
.Lx445_0:
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
 xchain431_n9_α:
 lea rax, [rip + .Lx447_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain431_n11_α
 xchain431_n9_β:
.Lx447_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n10_α
# IR_MATCH_DEFER
 xchain431_n10_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx448_0
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
 je .Lx448_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n12_β
.Lx448_1:
 jmp xchain431_n12_α
.Lx448_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain431_n12_β
 mov r14d, eax
 jmp xchain431_n12_α
 xchain431_n10_β:
 jmp xchain431_n12_β
# IR_MATCH_ALT_SAVE
 xchain431_n11_α:
 mov dword ptr [r12 + 64], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 68], eax
 jmp xchain431_n13_α
 xchain431_n11_β:
 jmp qword ptr [r12 + 72]
# IR_MATCH_ALT_JOIN
 xchain431_n12_α:
 lea rax, [rip + .Lx452_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain431_n11_α
 xchain431_n12_β:
.Lx452_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n3_β
# IR_MATCH_DEFER
 xchain431_n13_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx453_0
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
 je .Lx453_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n14_β
.Lx453_1:
 jmp xchain431_n14_α
.Lx453_0:
 lea rdi, [rip + .S6]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain431_n14_β
 mov r14d, eax
 jmp xchain431_n14_α
 xchain431_n13_β:
 jmp xchain431_n14_β
# IR_MATCH_ALT_JOIN
 xchain431_n14_α:
 lea rax, [rip + .Lx455_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain431_n3_α
 xchain431_n14_β:
.Lx455_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n15_α
# IR_MATCH_DEFER
 xchain431_n15_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx456_0
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
 je .Lx456_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n16_β
.Lx456_1:
 jmp xchain431_n16_α
.Lx456_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain431_n16_β
 mov r14d, eax
 jmp xchain431_n16_α
 xchain431_n15_β:
 jmp xchain431_n16_β
# IR_MATCH_ALT_JOIN
 xchain431_n16_α:
 lea rax, [rip + .Lx458_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain431_n3_α
 xchain431_n16_β:
.Lx458_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain431_n4_β
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
 xchain459_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain459_n1_α
 xchain459_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain459_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx462_0
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
 je .Lx462_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain459_n2_β
.Lx462_1:
 jmp xchain459_n2_α
.Lx462_0:
 lea rdi, [rip + .S7]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain459_n2_β
 mov r14d, eax
 jmp xchain459_n2_α
 xchain459_n1_β:
 jmp xchain459_n2_β
# IR_MATCH_ALT_JOIN
 xchain459_n2_α:
 lea rax, [rip + .Lx464_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$11_γ
 xchain459_n2_β:
.Lx464_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain459_n3_α
# IR_MATCH_LIT
 xchain459_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain459_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain459_n5_β
 add r14d, 1
 jmp xchain459_n4_α
 xchain459_n3_β:
 sub r14d, 1
 jmp xchain459_n5_β
# IR_MATCH_DEFER
 xchain459_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx467_0
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
 je .Lx467_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain459_n5_β
.Lx467_1:
 jmp xchain459_n6_α
.Lx467_0:
 lea rdi, [rip + .S9]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain459_n5_β
 mov r14d, eax
 jmp xchain459_n6_α
 xchain459_n4_β:
 jmp xchain459_n5_β
# IR_MATCH_ALT_JOIN
 xchain459_n5_α:
 lea rax, [rip + .Lx469_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$11_γ
 xchain459_n5_β:
.Lx469_0:
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
 xchain459_n6_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain459_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain459_n5_β
 add r14d, 1
 jmp xchain459_n5_α
 xchain459_n6_β:
 sub r14d, 1
 jmp xchain459_n5_β
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
 xchain472_n0_α:
 mov dword ptr [r12 + 16], 0
.Lx474_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jge .Lx474_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx474_1
 add dword ptr [r12 + 16], 1
 jmp .Lx474_0
.Lx474_1:
 mov eax, dword ptr [r12 + 16]
 test eax, eax
 jle proc_PAT$12_ω
 mov edx, r14d
 mov dword ptr [r12 + 20], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$12_γ
 xchain472_n0_β:
 xchain472_n0_β:
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
 xchain475_n0_α:
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
 jmp xchain475_n1_α
 xchain475_n0_β:
 sub r14d, 1
 jmp proc_PAT$13_ω
# IR_MATCH_ALT_SAVE
 xchain475_n1_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain475_n2_α
 xchain475_n1_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_ANY
 xchain475_n2_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain475_n3_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S2]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain475_n3_β
 add r14d, 1
 jmp xchain475_n3_α
 xchain475_n2_β:
 sub r14d, 1
 jmp xchain475_n3_β
# IR_MATCH_ALT_JOIN
 xchain475_n3_α:
 lea rax, [rip + .Lx481_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain475_n5_α
 xchain475_n3_β:
.Lx481_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain475_n4_α
# IR_MATCH_DEFER
 xchain475_n4_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx482_0
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
 je .Lx482_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain475_n6_β
.Lx482_1:
 jmp xchain475_n6_α
.Lx482_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain475_n6_β
 mov r14d, eax
 jmp xchain475_n6_α
 xchain475_n4_β:
 jmp xchain475_n6_β
# IR_MATCH_DEFER
 xchain475_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx483_0
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
 je .Lx483_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain475_n1_β
.Lx483_1:
 jmp proc_PAT$13_γ
.Lx483_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain475_n1_β
 mov r14d, eax
 jmp proc_PAT$13_γ
 xchain475_n5_β:
 jmp xchain475_n1_β
# IR_MATCH_ALT_JOIN
 xchain475_n6_α:
 lea rax, [rip + .Lx485_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain475_n5_α
 xchain475_n6_β:
.Lx485_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$13_ω
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
 xchain486_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain486_n1_α
 xchain486_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain486_n1_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx489_0
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
 je .Lx489_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n3_β
.Lx489_1:
 jmp xchain486_n2_α
.Lx489_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain486_n3_β
 mov r14d, eax
 jmp xchain486_n2_α
 xchain486_n1_β:
 jmp xchain486_n3_β
# IR_MATCH_LIT
 xchain486_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain486_n3_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain486_n3_β
 add r14d, 1
 jmp xchain486_n4_α
 xchain486_n2_β:
 sub r14d, 1
 jmp xchain486_n3_β
# IR_MATCH_ALT_JOIN
 xchain486_n3_α:
 lea rax, [rip + .Lx493_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$14_γ
 xchain486_n3_β:
.Lx493_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n5_α
# IR_MATCH_ALT_SAVE
 xchain486_n4_α:
 mov dword ptr [r12 + 112], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 116], eax
 jmp xchain486_n6_α
 xchain486_n4_β:
 jmp qword ptr [r12 + 120]
# IR_MATCH_DEFER
 xchain486_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx496_0
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
 je .Lx496_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n8_β
.Lx496_1:
 jmp xchain486_n7_α
.Lx496_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain486_n8_β
 mov r14d, eax
 jmp xchain486_n7_α
 xchain486_n5_β:
 jmp xchain486_n8_β
# IR_MATCH_DEFER
 xchain486_n6_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx497_0
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
 je .Lx497_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n9_β
.Lx497_1:
 jmp xchain486_n9_α
.Lx497_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain486_n9_β
 mov r14d, eax
 jmp xchain486_n9_α
 xchain486_n6_β:
 jmp xchain486_n9_β
# IR_MATCH_DEFER
 xchain486_n7_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx498_0
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
 je .Lx498_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n8_β
.Lx498_1:
 jmp xchain486_n8_α
.Lx498_0:
 lea rdi, [rip + .S6]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain486_n8_β
 mov r14d, eax
 jmp xchain486_n8_α
 xchain486_n7_β:
 jmp xchain486_n8_β
# IR_MATCH_ALT_JOIN
 xchain486_n8_α:
 lea rax, [rip + .Lx500_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$14_γ
 xchain486_n8_β:
.Lx500_0:
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
 xchain486_n9_α:
 lea rax, [rip + .Lx502_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain486_n11_α
 xchain486_n9_β:
.Lx502_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n10_α
# IR_MATCH_DEFER
 xchain486_n10_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx503_0
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
 je .Lx503_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n12_β
.Lx503_1:
 jmp xchain486_n12_α
.Lx503_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain486_n12_β
 mov r14d, eax
 jmp xchain486_n12_α
 xchain486_n10_β:
 jmp xchain486_n12_β
# IR_MATCH_ALT_SAVE
 xchain486_n11_α:
 mov dword ptr [r12 + 64], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 68], eax
 jmp xchain486_n13_α
 xchain486_n11_β:
 jmp qword ptr [r12 + 72]
# IR_MATCH_ALT_JOIN
 xchain486_n12_α:
 lea rax, [rip + .Lx507_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain486_n11_α
 xchain486_n12_β:
.Lx507_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n3_β
# IR_MATCH_DEFER
 xchain486_n13_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx508_0
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
 je .Lx508_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n14_β
.Lx508_1:
 jmp xchain486_n14_α
.Lx508_0:
 lea rdi, [rip + .S6]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain486_n14_β
 mov r14d, eax
 jmp xchain486_n14_α
 xchain486_n13_β:
 jmp xchain486_n14_β
# IR_MATCH_ALT_JOIN
 xchain486_n14_α:
 lea rax, [rip + .Lx510_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain486_n3_α
 xchain486_n14_β:
.Lx510_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n15_α
# IR_MATCH_DEFER
 xchain486_n15_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx511_0
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
 je .Lx511_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n16_β
.Lx511_1:
 jmp xchain486_n16_α
.Lx511_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain486_n16_β
 mov r14d, eax
 jmp xchain486_n16_α
 xchain486_n15_β:
 jmp xchain486_n16_β
# IR_MATCH_ALT_JOIN
 xchain486_n16_α:
 lea rax, [rip + .Lx513_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain486_n3_α
 xchain486_n16_β:
.Lx513_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain486_n4_β
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
 xchain514_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain514_n1_α
 xchain514_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain514_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx517_0
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
 je .Lx517_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain514_n2_β
.Lx517_1:
 jmp xchain514_n2_α
.Lx517_0:
 lea rdi, [rip + .S7]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain514_n2_β
 mov r14d, eax
 jmp xchain514_n2_α
 xchain514_n1_β:
 jmp xchain514_n2_β
# IR_MATCH_ALT_JOIN
 xchain514_n2_α:
 lea rax, [rip + .Lx519_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$15_γ
 xchain514_n2_β:
.Lx519_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain514_n3_α
# IR_MATCH_LIT
 xchain514_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain514_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain514_n5_β
 add r14d, 1
 jmp xchain514_n4_α
 xchain514_n3_β:
 sub r14d, 1
 jmp xchain514_n5_β
# IR_MATCH_DEFER
 xchain514_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx522_0
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
 je .Lx522_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain514_n5_β
.Lx522_1:
 jmp xchain514_n6_α
.Lx522_0:
 lea rdi, [rip + .S9]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain514_n5_β
 mov r14d, eax
 jmp xchain514_n6_α
 xchain514_n4_β:
 jmp xchain514_n5_β
# IR_MATCH_ALT_JOIN
 xchain514_n5_α:
 lea rax, [rip + .Lx524_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$15_γ
 xchain514_n5_β:
.Lx524_0:
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
 xchain514_n6_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain514_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain514_n5_β
 add r14d, 1
 jmp xchain514_n5_α
 xchain514_n6_β:
 sub r14d, 1
 jmp xchain514_n5_β
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
 xchain527_n0_α:
 mov dword ptr [r12 + 16], 0
.Lx529_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jge .Lx529_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx529_1
 add dword ptr [r12 + 16], 1
 jmp .Lx529_0
.Lx529_1:
 mov eax, dword ptr [r12 + 16]
 test eax, eax
 jle proc_PAT$16_ω
 mov edx, r14d
 mov dword ptr [r12 + 20], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$16_γ
 xchain527_n0_β:
 xchain527_n0_β:
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
 xchain530_n0_α:
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
 jmp xchain530_n1_α
 xchain530_n0_β:
 sub r14d, 1
 jmp proc_PAT$17_ω
# IR_MATCH_ALT_SAVE
 xchain530_n1_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain530_n2_α
 xchain530_n1_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_ANY
 xchain530_n2_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain530_n3_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S2]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain530_n3_β
 add r14d, 1
 jmp xchain530_n3_α
 xchain530_n2_β:
 sub r14d, 1
 jmp xchain530_n3_β
# IR_MATCH_ALT_JOIN
 xchain530_n3_α:
 lea rax, [rip + .Lx536_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain530_n5_α
 xchain530_n3_β:
.Lx536_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain530_n4_α
# IR_MATCH_DEFER
 xchain530_n4_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx537_0
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
 je .Lx537_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain530_n6_β
.Lx537_1:
 jmp xchain530_n6_α
.Lx537_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain530_n6_β
 mov r14d, eax
 jmp xchain530_n6_α
 xchain530_n4_β:
 jmp xchain530_n6_β
# IR_MATCH_DEFER
 xchain530_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx538_0
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
 je .Lx538_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain530_n1_β
.Lx538_1:
 jmp proc_PAT$17_γ
.Lx538_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain530_n1_β
 mov r14d, eax
 jmp proc_PAT$17_γ
 xchain530_n5_β:
 jmp xchain530_n1_β
# IR_MATCH_ALT_JOIN
 xchain530_n6_α:
 lea rax, [rip + .Lx540_0]
 mov qword ptr [r12 + 24], rax
 jmp xchain530_n5_α
 xchain530_n6_β:
.Lx540_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$17_ω
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
 xchain541_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain541_n1_α
 xchain541_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain541_n1_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx544_0
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
 je .Lx544_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n3_β
.Lx544_1:
 jmp xchain541_n2_α
.Lx544_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain541_n3_β
 mov r14d, eax
 jmp xchain541_n2_α
 xchain541_n1_β:
 jmp xchain541_n3_β
# IR_MATCH_LIT
 xchain541_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain541_n3_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain541_n3_β
 add r14d, 1
 jmp xchain541_n4_α
 xchain541_n2_β:
 sub r14d, 1
 jmp xchain541_n3_β
# IR_MATCH_ALT_JOIN
 xchain541_n3_α:
 lea rax, [rip + .Lx548_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$18_γ
 xchain541_n3_β:
.Lx548_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n5_α
# IR_MATCH_ALT_SAVE
 xchain541_n4_α:
 mov dword ptr [r12 + 112], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 116], eax
 jmp xchain541_n6_α
 xchain541_n4_β:
 jmp qword ptr [r12 + 120]
# IR_MATCH_DEFER
 xchain541_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx551_0
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
 je .Lx551_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n8_β
.Lx551_1:
 jmp xchain541_n7_α
.Lx551_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain541_n8_β
 mov r14d, eax
 jmp xchain541_n7_α
 xchain541_n5_β:
 jmp xchain541_n8_β
# IR_MATCH_DEFER
 xchain541_n6_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx552_0
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
 je .Lx552_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n9_β
.Lx552_1:
 jmp xchain541_n9_α
.Lx552_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain541_n9_β
 mov r14d, eax
 jmp xchain541_n9_α
 xchain541_n6_β:
 jmp xchain541_n9_β
# IR_MATCH_DEFER
 xchain541_n7_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx553_0
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
 je .Lx553_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n8_β
.Lx553_1:
 jmp xchain541_n8_α
.Lx553_0:
 lea rdi, [rip + .S6]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain541_n8_β
 mov r14d, eax
 jmp xchain541_n8_α
 xchain541_n7_β:
 jmp xchain541_n8_β
# IR_MATCH_ALT_JOIN
 xchain541_n8_α:
 lea rax, [rip + .Lx555_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$18_γ
 xchain541_n8_β:
.Lx555_0:
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
 xchain541_n9_α:
 lea rax, [rip + .Lx557_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain541_n11_α
 xchain541_n9_β:
.Lx557_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n10_α
# IR_MATCH_DEFER
 xchain541_n10_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx558_0
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
 je .Lx558_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n12_β
.Lx558_1:
 jmp xchain541_n12_α
.Lx558_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain541_n12_β
 mov r14d, eax
 jmp xchain541_n12_α
 xchain541_n10_β:
 jmp xchain541_n12_β
# IR_MATCH_ALT_SAVE
 xchain541_n11_α:
 mov dword ptr [r12 + 64], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 68], eax
 jmp xchain541_n13_α
 xchain541_n11_β:
 jmp qword ptr [r12 + 72]
# IR_MATCH_ALT_JOIN
 xchain541_n12_α:
 lea rax, [rip + .Lx562_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain541_n11_α
 xchain541_n12_β:
.Lx562_0:
 mov r14d, dword ptr [r12 + 112]
 mov edi, dword ptr [r12 + 116]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n3_β
# IR_MATCH_DEFER
 xchain541_n13_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx563_0
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
 je .Lx563_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n14_β
.Lx563_1:
 jmp xchain541_n14_α
.Lx563_0:
 lea rdi, [rip + .S6]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain541_n14_β
 mov r14d, eax
 jmp xchain541_n14_α
 xchain541_n13_β:
 jmp xchain541_n14_β
# IR_MATCH_ALT_JOIN
 xchain541_n14_α:
 lea rax, [rip + .Lx565_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain541_n3_α
 xchain541_n14_β:
.Lx565_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n15_α
# IR_MATCH_DEFER
 xchain541_n15_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx566_0
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
 je .Lx566_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n16_β
.Lx566_1:
 jmp xchain541_n16_α
.Lx566_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain541_n16_β
 mov r14d, eax
 jmp xchain541_n16_α
 xchain541_n15_β:
 jmp xchain541_n16_β
# IR_MATCH_ALT_JOIN
 xchain541_n16_α:
 lea rax, [rip + .Lx568_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain541_n3_α
 xchain541_n16_β:
.Lx568_0:
 mov r14d, dword ptr [r12 + 64]
 mov edi, dword ptr [r12 + 68]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain541_n4_β
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
 xchain569_n0_α:
 mov dword ptr [r12 + 16], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 20], eax
 jmp xchain569_n1_α
 xchain569_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_DEFER
 xchain569_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx572_0
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
 je .Lx572_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain569_n2_β
.Lx572_1:
 jmp xchain569_n2_α
.Lx572_0:
 lea rdi, [rip + .S7]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain569_n2_β
 mov r14d, eax
 jmp xchain569_n2_α
 xchain569_n1_β:
 jmp xchain569_n2_β
# IR_MATCH_ALT_JOIN
 xchain569_n2_α:
 lea rax, [rip + .Lx574_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$19_γ
 xchain569_n2_β:
.Lx574_0:
 mov r14d, dword ptr [r12 + 16]
 mov edi, dword ptr [r12 + 20]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain569_n3_α
# IR_MATCH_LIT
 xchain569_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain569_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain569_n5_β
 add r14d, 1
 jmp xchain569_n4_α
 xchain569_n3_β:
 sub r14d, 1
 jmp xchain569_n5_β
# IR_MATCH_DEFER
 xchain569_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx577_0
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
 je .Lx577_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain569_n5_β
.Lx577_1:
 jmp xchain569_n6_α
.Lx577_0:
 lea rdi, [rip + .S9]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain569_n5_β
 mov r14d, eax
 jmp xchain569_n6_α
 xchain569_n4_β:
 jmp xchain569_n5_β
# IR_MATCH_ALT_JOIN
 xchain569_n5_α:
 lea rax, [rip + .Lx579_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$19_γ
 xchain569_n5_β:
.Lx579_0:
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
 xchain569_n6_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain569_n5_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain569_n5_β
 add r14d, 1
 jmp xchain569_n5_α
 xchain569_n6_β:
 sub r14d, 1
 jmp xchain569_n5_β
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
 xchain582_n0_α:
# BOX IR_CALL TABLE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
  .section .rodata
  .Lrkfn584: .string "TABLE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn584]
 lea rsi, [r12 + 96]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je xchain582_n2_α
 jmp xchain582_n1_α
 xchain582_n0_β:
 jmp xchain582_n2_α
# IR_ASSIGN gva
 xchain582_n1_α:
 mov rax, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 mov qword ptr [rbx + 144], rax
 mov qword ptr [rbx + 152], rdx
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain582_n2_α
 xchain582_n1_β:
 jmp xchain582_n2_α
# IR_LIT_STRING
 xchain582_n2_α:
 mov qword ptr [r12 + 1488], 1
 mov rax, qword ptr [rip + .Lx586_0]
 mov qword ptr [r12 + 1496], rax
 jmp xchain582_n3_α
 xchain582_n2_β:
 jmp xchain582_n5_α
.Lx586_0:
 .quad .Lx586_0_s
.Lx586_0_s:
 .string "PAT$0"
 xchain582_n3_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1488] -> [zr+1472]
 mov rax, qword ptr [r12 + 1488]
 mov qword ptr [r12 + 1472], rax
 mov rax, qword ptr [r12 + 1496]
 mov qword ptr [r12 + 1480], rax
  .section .rodata
  .Lrkfn588: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn588]
 lea rsi, [r12 + 1472]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1456], rax
 mov qword ptr [r12 + 1464], rdx
 cmp eax, 99
 je xchain582_n5_α
 jmp xchain582_n4_α
 xchain582_n3_β:
 jmp xchain582_n5_α
# IR_ASSIGN gva
 xchain582_n4_α:
 mov rax, qword ptr [r12 + 1456]
 mov rdx, qword ptr [r12 + 1464]
 mov qword ptr [rbx + 176], rax
 mov qword ptr [rbx + 184], rdx
 mov qword ptr [r12 + 1440], rax
 mov qword ptr [r12 + 1448], rdx
 jmp xchain582_n5_α
 xchain582_n4_β:
 jmp xchain582_n5_α
# IR_LIT_STRING
 xchain582_n5_α:
 mov qword ptr [r12 + 1552], 1
 mov rax, qword ptr [rip + .Lx590_0]
 mov qword ptr [r12 + 1560], rax
 jmp xchain582_n6_α
 xchain582_n5_β:
 jmp xchain582_n8_α
.Lx590_0:
 .quad .Lx590_0_s
.Lx590_0_s:
 .string "PAT$1"
 xchain582_n6_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1552] -> [zr+1536]
 mov rax, qword ptr [r12 + 1552]
 mov qword ptr [r12 + 1536], rax
 mov rax, qword ptr [r12 + 1560]
 mov qword ptr [r12 + 1544], rax
  .section .rodata
  .Lrkfn592: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn592]
 lea rsi, [r12 + 1536]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
 cmp eax, 99
 je xchain582_n8_α
 jmp xchain582_n7_α
 xchain582_n6_β:
 jmp xchain582_n8_α
# IR_ASSIGN gva
 xchain582_n7_α:
 mov rax, qword ptr [r12 + 1520]
 mov rdx, qword ptr [r12 + 1528]
 mov qword ptr [rbx + 192], rax
 mov qword ptr [rbx + 200], rdx
 mov qword ptr [r12 + 1504], rax
 mov qword ptr [r12 + 1512], rdx
 jmp xchain582_n8_α
 xchain582_n7_β:
 jmp xchain582_n8_α
# IR_LIT_STRING
 xchain582_n8_α:
 mov qword ptr [r12 + 1616], 1
 mov rax, qword ptr [rip + .Lx594_0]
 mov qword ptr [r12 + 1624], rax
 jmp xchain582_n9_α
 xchain582_n8_β:
 jmp xchain582_n11_α
.Lx594_0:
 .quad .Lx594_0_s
.Lx594_0_s:
 .string "PAT$2"
 xchain582_n9_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1616] -> [zr+1600]
 mov rax, qword ptr [r12 + 1616]
 mov qword ptr [r12 + 1600], rax
 mov rax, qword ptr [r12 + 1624]
 mov qword ptr [r12 + 1608], rax
  .section .rodata
  .Lrkfn596: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn596]
 lea rsi, [r12 + 1600]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 cmp eax, 99
 je xchain582_n11_α
 jmp xchain582_n10_α
 xchain582_n9_β:
 jmp xchain582_n11_α
# IR_ASSIGN gva
 xchain582_n10_α:
 mov rax, qword ptr [r12 + 1584]
 mov rdx, qword ptr [r12 + 1592]
 mov qword ptr [rbx + 208], rax
 mov qword ptr [rbx + 216], rdx
 mov qword ptr [r12 + 1568], rax
 mov qword ptr [r12 + 1576], rdx
 jmp xchain582_n11_α
 xchain582_n10_β:
 jmp xchain582_n11_α
# IR_LIT_INTEGER
 xchain582_n11_α:
 mov qword ptr [r12 + 1712], 6
 mov rax, qword ptr [rip + .Lx598_0]
 mov qword ptr [r12 + 1720], rax
 jmp xchain582_n12_α
 xchain582_n11_β:
 jmp xchain582_n17_α
.Lx598_0:
 .quad 42
# IR_LIT_STRING
 xchain582_n12_α:
 mov qword ptr [r12 + 1728], 1
 mov rax, qword ptr [rip + .Lx599_0]
 mov qword ptr [r12 + 1736], rax
 jmp xchain582_n13_α
 xchain582_n12_β:
 jmp xchain582_n17_α
.Lx599_0:
 .quad .Lx599_0_s
.Lx599_0_s:
 .string "*EXPR$0"
# IR_LIT_INTEGER
 xchain582_n13_α:
 mov qword ptr [r12 + 1792], 6
 mov rax, qword ptr [rip + .Lx600_0]
 mov qword ptr [r12 + 1800], rax
 jmp xchain582_n14_α
 xchain582_n13_β:
 jmp xchain582_n17_α
.Lx600_0:
 .quad 28
# IR_LIT_STRING
 xchain582_n14_α:
 mov qword ptr [r12 + 1808], 1
 mov rax, qword ptr [rip + .Lx601_0]
 mov qword ptr [r12 + 1816], rax
 jmp xchain582_n15_α
 xchain582_n14_β:
 jmp xchain582_n17_α
.Lx601_0:
 .quad .Lx601_0_s
.Lx601_0_s:
 .string "+-"
 xchain582_n15_α:
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
 je xchain582_n17_α
 jmp xchain582_n16_α
 xchain582_n15_β:
 jmp xchain582_n17_α
 xchain582_n16_α:
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
 je xchain582_n17_α
 jmp xchain582_n18_α
 xchain582_n16_β:
 jmp xchain582_n17_α
# IR_LIT_INTEGER
 xchain582_n17_α:
 mov qword ptr [r12 + 1904], 6
 mov rax, qword ptr [rip + .Lx604_0]
 mov qword ptr [r12 + 1912], rax
 jmp xchain582_n19_α
 xchain582_n17_β:
 jmp xchain582_n24_α
.Lx604_0:
 .quad 42
# IR_ASSIGN gva
 xchain582_n18_α:
 mov rax, qword ptr [r12 + 1648]
 mov rdx, qword ptr [r12 + 1656]
 mov qword ptr [rbx + 224], rax
 mov qword ptr [rbx + 232], rdx
 mov qword ptr [r12 + 1632], rax
 mov qword ptr [r12 + 1640], rdx
 jmp xchain582_n17_α
 xchain582_n18_β:
 jmp xchain582_n17_α
# IR_LIT_STRING
 xchain582_n19_α:
 mov qword ptr [r12 + 1920], 1
 mov rax, qword ptr [rip + .Lx606_0]
 mov qword ptr [r12 + 1928], rax
 jmp xchain582_n20_α
 xchain582_n19_β:
 jmp xchain582_n24_α
.Lx606_0:
 .quad .Lx606_0_s
.Lx606_0_s:
 .string "*EXPR$1"
# IR_LIT_INTEGER
 xchain582_n20_α:
 mov qword ptr [r12 + 1984], 6
 mov rax, qword ptr [rip + .Lx607_0]
 mov qword ptr [r12 + 1992], rax
 jmp xchain582_n21_α
 xchain582_n20_β:
 jmp xchain582_n24_α
.Lx607_0:
 .quad 28
# IR_LIT_STRING
 xchain582_n21_α:
 mov qword ptr [r12 + 2000], 1
 mov rax, qword ptr [rip + .Lx608_0]
 mov qword ptr [r12 + 2008], rax
 jmp xchain582_n22_α
 xchain582_n21_β:
 jmp xchain582_n24_α
.Lx608_0:
 .quad .Lx608_0_s
.Lx608_0_s:
 .string "*/"
 xchain582_n22_α:
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
 je xchain582_n24_α
 jmp xchain582_n23_α
 xchain582_n22_β:
 jmp xchain582_n24_α
 xchain582_n23_α:
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
 je xchain582_n24_α
 jmp xchain582_n25_α
 xchain582_n23_β:
 jmp xchain582_n24_α
# IR_LIT_INTEGER
 xchain582_n24_α:
 mov qword ptr [r12 + 2096], 6
 mov rax, qword ptr [rip + .Lx611_0]
 mov qword ptr [r12 + 2104], rax
 jmp xchain582_n26_α
 xchain582_n24_β:
 jmp xchain582_n29_α
.Lx611_0:
 .quad 42
# IR_ASSIGN gva
 xchain582_n25_α:
 mov rax, qword ptr [r12 + 1840]
 mov rdx, qword ptr [r12 + 1848]
 mov qword ptr [rbx + 240], rax
 mov qword ptr [rbx + 248], rdx
 mov qword ptr [r12 + 1824], rax
 mov qword ptr [r12 + 1832], rdx
 jmp xchain582_n24_α
 xchain582_n25_β:
 jmp xchain582_n24_α
# IR_LIT_STRING
 xchain582_n26_α:
 mov qword ptr [r12 + 2112], 1
 mov rax, qword ptr [rip + .Lx613_0]
 mov qword ptr [r12 + 2120], rax
 jmp xchain582_n27_α
 xchain582_n26_β:
 jmp xchain582_n29_α
.Lx613_0:
 .quad .Lx613_0_s
.Lx613_0_s:
 .string "*EXPR$2"
# IR_VAR
 xchain582_n27_α:
 mov rax, qword ptr [rbx + 208]
 mov rdx, qword ptr [rbx + 216]
 mov qword ptr [r12 + 2176], rax
 mov qword ptr [r12 + 2184], rdx
 jmp xchain582_n28_α
 xchain582_n27_β:
 jmp xchain582_n29_α
# IR_VAR
 xchain582_n28_α:
 mov rax, qword ptr [rbx + 176]
 mov rdx, qword ptr [rbx + 184]
 mov qword ptr [r12 + 2192], rax
 mov qword ptr [r12 + 2200], rdx
 jmp xchain582_n30_α
 xchain582_n28_β:
 jmp xchain582_n29_α
# IR_LIT_STRING
 xchain582_n29_α:
 mov qword ptr [r12 + 2256], 1
 mov rax, qword ptr [rip + .Lx616_0]
 mov qword ptr [r12 + 2264], rax
 jmp xchain582_n31_α
 xchain582_n29_β:
 jmp xchain582_n34_α
.Lx616_0:
 .quad .Lx616_0_s
.Lx616_0_s:
 .string "PAT$3"
 xchain582_n30_α:
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
 je xchain582_n29_α
 jmp xchain582_n32_α
 xchain582_n30_β:
 jmp xchain582_n29_α
 xchain582_n31_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2256] -> [zr+2240]
 mov rax, qword ptr [r12 + 2256]
 mov qword ptr [r12 + 2240], rax
 mov rax, qword ptr [r12 + 2264]
 mov qword ptr [r12 + 2248], rax
  .section .rodata
  .Lrkfn619: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn619]
 lea rsi, [r12 + 2240]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2224], rax
 mov qword ptr [r12 + 2232], rdx
 cmp eax, 99
 je xchain582_n34_α
 jmp xchain582_n33_α
 xchain582_n31_β:
 jmp xchain582_n34_α
 xchain582_n32_α:
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
 je xchain582_n29_α
 jmp xchain582_n35_α
 xchain582_n32_β:
 jmp xchain582_n29_α
# IR_ASSIGN gva
 xchain582_n33_α:
 mov rax, qword ptr [r12 + 2224]
 mov rdx, qword ptr [r12 + 2232]
 mov qword ptr [rbx + 272], rax
 mov qword ptr [rbx + 280], rdx
 mov qword ptr [r12 + 2208], rax
 mov qword ptr [r12 + 2216], rdx
 jmp xchain582_n34_α
 xchain582_n33_β:
 jmp xchain582_n34_α
# IR_VAR
 xchain582_n34_α:
 mov rax, qword ptr [rbx + 224]
 mov rdx, qword ptr [rbx + 232]
 mov qword ptr [r12 + 2352], rax
 mov qword ptr [r12 + 2360], rdx
 jmp xchain582_n36_α
 xchain582_n34_β:
 jmp xchain582_n37_α
# IR_ASSIGN gva
 xchain582_n35_α:
 mov rax, qword ptr [r12 + 2032]
 mov rdx, qword ptr [r12 + 2040]
 mov qword ptr [rbx + 256], rax
 mov qword ptr [rbx + 264], rdx
 mov qword ptr [r12 + 2016], rax
 mov qword ptr [r12 + 2024], rdx
 jmp xchain582_n29_α
 xchain582_n35_β:
 jmp xchain582_n29_α
# IR_LIT_INTEGER
 xchain582_n36_α:
 mov qword ptr [r12 + 2432], 6
 mov rax, qword ptr [rip + .Lx624_0]
 mov qword ptr [r12 + 2440], rax
 jmp xchain582_n38_α
 xchain582_n36_β:
 jmp xchain582_n37_α
.Lx624_0:
 .quad 42
# IR_LIT_STRING
 xchain582_n37_α:
 mov qword ptr [r12 + 2688], 1
 mov rax, qword ptr [rip + .Lx625_0]
 mov qword ptr [r12 + 2696], rax
 jmp xchain582_n39_α
 xchain582_n37_β:
 jmp xchain582_n42_α
.Lx625_0:
 .quad .Lx625_0_s
.Lx625_0_s:
 .string "EXPR$6"
# IR_LIT_STRING
 xchain582_n38_α:
 mov qword ptr [r12 + 2448], 1
 mov rax, qword ptr [rip + .Lx626_0]
 mov qword ptr [r12 + 2456], rax
 jmp xchain582_n40_α
 xchain582_n38_β:
 jmp xchain582_n37_α
.Lx626_0:
 .quad .Lx626_0_s
.Lx626_0_s:
 .string "*EXPR$3"
 xchain582_n39_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2688] -> [zr+2672]
 mov rax, qword ptr [r12 + 2688]
 mov qword ptr [r12 + 2672], rax
 mov rax, qword ptr [r12 + 2696]
 mov qword ptr [r12 + 2680], rax
  .section .rodata
  .Lrkfn628: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn628]
 lea rsi, [r12 + 2672]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2656], rax
 mov qword ptr [r12 + 2664], rdx
 cmp eax, 99
 je xchain582_n42_α
 jmp xchain582_n41_α
 xchain582_n39_β:
 jmp xchain582_n42_α
# IR_LIT_STRING
 xchain582_n40_α:
 mov qword ptr [r12 + 2496], 1
 mov rax, qword ptr [rip + .Lx629_0]
 mov qword ptr [r12 + 2504], rax
 jmp xchain582_n43_α
 xchain582_n40_β:
 jmp xchain582_n37_α
.Lx629_0:
 .quad .Lx629_0_s
.Lx629_0_s:
 .string "EXPR$4"
# IR_VAR
 xchain582_n41_α:
 mov rax, qword ptr [rbx + 240]
 mov rdx, qword ptr [rbx + 248]
 mov qword ptr [r12 + 2704], rax
 mov qword ptr [r12 + 2712], rdx
 jmp xchain582_n44_α
 xchain582_n41_β:
 jmp xchain582_n42_α
# IR_LIT_STRING
 xchain582_n42_α:
 mov qword ptr [r12 + 3040], 1
 mov rax, qword ptr [rip + .Lx631_0]
 mov qword ptr [r12 + 3048], rax
 jmp xchain582_n45_α
 xchain582_n42_β:
 jmp xchain582_n49_α
.Lx631_0:
 .quad .Lx631_0_s
.Lx631_0_s:
 .string "EXPR$10"
 xchain582_n43_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2496] -> [zr+2480]
 mov rax, qword ptr [r12 + 2496]
 mov qword ptr [r12 + 2480], rax
 mov rax, qword ptr [r12 + 2504]
 mov qword ptr [r12 + 2488], rax
  .section .rodata
  .Lrkfn633: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn633]
 lea rsi, [r12 + 2480]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2464], rax
 mov qword ptr [r12 + 2472], rdx
 cmp eax, 99
 je xchain582_n37_α
 jmp xchain582_n46_α
 xchain582_n43_β:
 jmp xchain582_n37_α
 xchain582_n44_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2656]
 mov rsi, qword ptr [r12 + 2664]
 mov rdx, qword ptr [r12 + 2704]
 mov rcx, qword ptr [r12 + 2712]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2640], rax
 mov qword ptr [r12 + 2648], rdx
 jmp xchain582_n47_α
 xchain582_n44_β:
 jmp xchain582_n42_α
 xchain582_n45_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3040] -> [zr+3024]
 mov rax, qword ptr [r12 + 3040]
 mov qword ptr [r12 + 3024], rax
 mov rax, qword ptr [r12 + 3048]
 mov qword ptr [r12 + 3032], rax
  .section .rodata
  .Lrkfn636: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn636]
 lea rsi, [r12 + 3024]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3008], rax
 mov qword ptr [r12 + 3016], rdx
 cmp eax, 99
 je xchain582_n49_α
 jmp xchain582_n48_α
 xchain582_n45_β:
 jmp xchain582_n49_α
 xchain582_n46_α:
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
 je xchain582_n37_α
 jmp xchain582_n50_α
 xchain582_n46_β:
 jmp xchain582_n37_α
# IR_LIT_INTEGER
 xchain582_n47_α:
 mov qword ptr [r12 + 2784], 6
 mov rax, qword ptr [rip + .Lx638_0]
 mov qword ptr [r12 + 2792], rax
 jmp xchain582_n51_α
 xchain582_n47_β:
 jmp xchain582_n42_α
.Lx638_0:
 .quad 42
# IR_VAR
 xchain582_n48_α:
 mov rax, qword ptr [rbx + 224]
 mov rdx, qword ptr [rbx + 232]
 mov qword ptr [r12 + 3056], rax
 mov qword ptr [r12 + 3064], rdx
 jmp xchain582_n52_α
 xchain582_n48_β:
 jmp xchain582_n49_α
# IR_LIT_STRING
 xchain582_n49_α:
 mov qword ptr [r12 + 3312], 1
 mov rax, qword ptr [rip + .Lx640_0]
 mov qword ptr [r12 + 3320], rax
 jmp xchain582_n53_α
 xchain582_n49_β:
 jmp xchain582_n61_α
.Lx640_0:
 .quad .Lx640_0_s
.Lx640_0_s:
 .string "TRIM"
 xchain582_n50_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2352]
 mov rsi, qword ptr [r12 + 2360]
 mov rdx, qword ptr [r12 + 2368]
 mov rcx, qword ptr [r12 + 2376]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2336], rax
 mov qword ptr [r12 + 2344], rdx
 jmp xchain582_n54_α
 xchain582_n50_β:
 jmp xchain582_n37_α
# IR_LIT_STRING
 xchain582_n51_α:
 mov qword ptr [r12 + 2800], 1
 mov rax, qword ptr [rip + .Lx642_0]
 mov qword ptr [r12 + 2808], rax
 jmp xchain582_n55_α
 xchain582_n51_β:
 jmp xchain582_n42_α
.Lx642_0:
 .quad .Lx642_0_s
.Lx642_0_s:
 .string "*EXPR$7"
 xchain582_n52_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 3008]
 mov rsi, qword ptr [r12 + 3016]
 mov rdx, qword ptr [r12 + 3056]
 mov rcx, qword ptr [r12 + 3064]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2992], rax
 mov qword ptr [r12 + 3000], rdx
 jmp xchain582_n56_α
 xchain582_n52_β:
 jmp xchain582_n49_α
# IR_LIT_INTEGER
 xchain582_n53_α:
 mov qword ptr [r12 + 3328], 6
 mov rax, qword ptr [rip + .Lx644_0]
 mov qword ptr [r12 + 3336], rax
 jmp xchain582_n57_α
 xchain582_n53_β:
 jmp xchain582_n61_α
.Lx644_0:
 .quad 1
# IR_LIT_STRING
 xchain582_n54_α:
 mov qword ptr [r12 + 2544], 1
 mov rax, qword ptr [rip + .Lx645_0]
 mov qword ptr [r12 + 2552], rax
 jmp xchain582_n58_α
 xchain582_n54_β:
 jmp xchain582_n37_α
.Lx645_0:
 .quad .Lx645_0_s
.Lx645_0_s:
 .string "EXPR$5"
# IR_LIT_STRING
 xchain582_n55_α:
 mov qword ptr [r12 + 2848], 1
 mov rax, qword ptr [rip + .Lx646_0]
 mov qword ptr [r12 + 2856], rax
 jmp xchain582_n59_α
 xchain582_n55_β:
 jmp xchain582_n42_α
.Lx646_0:
 .quad .Lx646_0_s
.Lx646_0_s:
 .string "EXPR$8"
# IR_LIT_INTEGER
 xchain582_n56_α:
 mov qword ptr [r12 + 3136], 6
 mov rax, qword ptr [rip + .Lx647_0]
 mov qword ptr [r12 + 3144], rax
 jmp xchain582_n60_α
 xchain582_n56_β:
 jmp xchain582_n49_α
.Lx647_0:
 .quad 42
 xchain582_n57_α:
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
  .Lrkfn649: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn649]
 lea rsi, [r12 + 3280]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3264], rax
 mov qword ptr [r12 + 3272], rdx
 cmp eax, 99
 je xchain582_n61_α
 jmp xchain582_n61_α
 xchain582_n57_β:
 jmp xchain582_n61_α
 xchain582_n58_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2544] -> [zr+2528]
 mov rax, qword ptr [r12 + 2544]
 mov qword ptr [r12 + 2528], rax
 mov rax, qword ptr [r12 + 2552]
 mov qword ptr [r12 + 2536], rax
  .section .rodata
  .Lrkfn651: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn651]
 lea rsi, [r12 + 2528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2512], rax
 mov qword ptr [r12 + 2520], rdx
 cmp eax, 99
 je xchain582_n37_α
 jmp xchain582_n62_α
 xchain582_n58_β:
 jmp xchain582_n37_α
 xchain582_n59_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2848] -> [zr+2832]
 mov rax, qword ptr [r12 + 2848]
 mov qword ptr [r12 + 2832], rax
 mov rax, qword ptr [r12 + 2856]
 mov qword ptr [r12 + 2840], rax
  .section .rodata
  .Lrkfn653: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn653]
 lea rsi, [r12 + 2832]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2816], rax
 mov qword ptr [r12 + 2824], rdx
 cmp eax, 99
 je xchain582_n42_α
 jmp xchain582_n63_α
 xchain582_n59_β:
 jmp xchain582_n42_α
# IR_LIT_STRING
 xchain582_n60_α:
 mov qword ptr [r12 + 3152], 1
 mov rax, qword ptr [rip + .Lx654_0]
 mov qword ptr [r12 + 3160], rax
 jmp xchain582_n64_α
 xchain582_n60_β:
 jmp xchain582_n49_α
.Lx654_0:
 .quad .Lx654_0_s
.Lx654_0_s:
 .string "*EXPR$11"
# IR_VAR
 xchain582_n61_α:
 mov rdi, qword ptr [rip + .Lx655_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 3360], rax
 mov qword ptr [r12 + 3368], rdx
 jmp xchain582_n65_α
 xchain582_n61_β:
 jmp main_γ
.Lx655_0:
 .quad .Lx655_0_s
.Lx655_0_s:
 .string "INPUT"
 xchain582_n62_α:
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
 je xchain582_n37_α
 jmp xchain582_n66_α
 xchain582_n62_β:
 jmp xchain582_n37_α
 xchain582_n63_α:
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
 je xchain582_n42_α
 jmp xchain582_n67_α
 xchain582_n63_β:
 jmp xchain582_n42_α
# IR_LIT_STRING
 xchain582_n64_α:
 mov qword ptr [r12 + 3200], 1
 mov rax, qword ptr [rip + .Lx658_0]
 mov qword ptr [r12 + 3208], rax
 jmp xchain582_n68_α
 xchain582_n64_β:
 jmp xchain582_n49_α
.Lx658_0:
 .quad .Lx658_0_s
.Lx658_0_s:
 .string "EXPR$12"
# IR_ASSIGN gva
 xchain582_n65_α:
 mov rax, qword ptr [r12 + 3360]
 mov rdx, qword ptr [r12 + 3368]
 mov qword ptr [rbx + 336], rax
 mov qword ptr [rbx + 344], rdx
 mov qword ptr [r12 + 3344], rax
 mov qword ptr [r12 + 3352], rdx
 jmp xchain582_n69_α
 xchain582_n65_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain582_n66_α:
 mov rax, qword ptr [r12 + 2288]
 mov rdx, qword ptr [r12 + 2296]
 mov qword ptr [rbx + 288], rax
 mov qword ptr [rbx + 296], rdx
 mov qword ptr [r12 + 2272], rax
 mov qword ptr [r12 + 2280], rdx
 jmp xchain582_n37_α
 xchain582_n66_β:
 jmp xchain582_n37_α
 xchain582_n67_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2640]
 mov rsi, qword ptr [r12 + 2648]
 mov rdx, qword ptr [r12 + 2720]
 mov rcx, qword ptr [r12 + 2728]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2624], rax
 mov qword ptr [r12 + 2632], rdx
 jmp xchain582_n70_α
 xchain582_n67_β:
 jmp xchain582_n42_α
 xchain582_n68_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3200] -> [zr+3184]
 mov rax, qword ptr [r12 + 3200]
 mov qword ptr [r12 + 3184], rax
 mov rax, qword ptr [r12 + 3208]
 mov qword ptr [r12 + 3192], rax
  .section .rodata
  .Lrkfn663: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn663]
 lea rsi, [r12 + 3184]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3168], rax
 mov qword ptr [r12 + 3176], rdx
 cmp eax, 99
 je xchain582_n49_α
 jmp xchain582_n71_α
 xchain582_n68_β:
 jmp xchain582_n49_α
# IR_VAR
 xchain582_n69_α:
 mov rax, qword ptr [rbx + 336]
 mov rdx, qword ptr [rbx + 344]
 mov qword ptr [r12 + 3440], rax
 mov qword ptr [r12 + 3448], rdx
 jmp xchain582_n72_α
 xchain582_n69_β:
 jmp xchain582_n73_α
# IR_LIT_STRING
 xchain582_n70_α:
 mov qword ptr [r12 + 2896], 1
 mov rax, qword ptr [rip + .Lx665_0]
 mov qword ptr [r12 + 2904], rax
 jmp xchain582_n74_α
 xchain582_n70_β:
 jmp xchain582_n42_α
.Lx665_0:
 .quad .Lx665_0_s
.Lx665_0_s:
 .string "EXPR$9"
 xchain582_n71_α:
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
 je xchain582_n49_α
 jmp xchain582_n75_α
 xchain582_n71_β:
 jmp xchain582_n49_α
# IR_MATCH_HEAD
 xchain582_n72_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 3384], rax
 mov qword ptr [r12 + 3392], rsp
 mov rdi, qword ptr [r12 + 3440]
 mov rsi, qword ptr [r12 + 3448]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 3376], 0
.Lx668_0:
 mov r14d, dword ptr [r12 + 3376]
 jmp xchain582_n76_α
 xchain582_n72_β:
 add dword ptr [r12 + 3376], 1
 mov eax, dword ptr [r12 + 3376]
 cmp eax, r15d
 jg .Lx668_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx668_1
 jmp .Lx668_0
.Lx668_1:
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
 jmp xchain582_n73_α
# IR_LIT_STRING
 xchain582_n73_α:
 mov qword ptr [r12 + 3504], 1
 mov rax, qword ptr [rip + .Lx669_0]
 mov qword ptr [r12 + 3512], rax
 jmp xchain582_n77_α
 xchain582_n73_β:
 jmp xchain582_n61_α
.Lx669_0:
 .quad .Lx669_0_s
.Lx669_0_s:
 .string "Bad input, try again"
 xchain582_n74_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2896] -> [zr+2880]
 mov rax, qword ptr [r12 + 2896]
 mov qword ptr [r12 + 2880], rax
 mov rax, qword ptr [r12 + 2904]
 mov qword ptr [r12 + 2888], rax
  .section .rodata
  .Lrkfn671: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn671]
 lea rsi, [r12 + 2880]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2864], rax
 mov qword ptr [r12 + 2872], rdx
 cmp eax, 99
 je xchain582_n42_α
 jmp xchain582_n78_α
 xchain582_n74_β:
 jmp xchain582_n42_α
 xchain582_n75_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2992]
 mov rsi, qword ptr [r12 + 3000]
 mov rdx, qword ptr [r12 + 3072]
 mov rcx, qword ptr [r12 + 3080]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2976], rax
 mov qword ptr [r12 + 2984], rdx
 jmp xchain582_n79_α
 xchain582_n75_β:
 jmp xchain582_n49_α
# IR_LIT_INTEGER
 xchain582_n76_α:
 mov qword ptr [r12 + 3424], 6
 mov rax, qword ptr [rip + .Lx673_0]
 mov qword ptr [r12 + 3432], rax
 jmp xchain582_n80_α
 xchain582_n76_β:
 jmp xchain582_n72_α
.Lx673_0:
 .quad 0
# IR_ASSIGN global
 xchain582_n77_α:
 mov rsi, qword ptr [r12 + 3504]
 mov rdx, qword ptr [r12 + 3512]
 mov rdi, qword ptr [rip + .Lx674_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 3488], rax
 mov qword ptr [r12 + 3496], rdx
 jmp xchain582_n61_α
 xchain582_n77_β:
 jmp xchain582_n61_α
.Lx674_0:
 .quad .Lx674_0_s
.Lx674_0_s:
 .string "OUTPUT"
 xchain582_n78_α:
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
 je xchain582_n42_α
 jmp xchain582_n81_α
 xchain582_n78_β:
 jmp xchain582_n42_α
# IR_LIT_STRING
 xchain582_n79_α:
 mov qword ptr [r12 + 3248], 1
 mov rax, qword ptr [rip + .Lx676_0]
 mov qword ptr [r12 + 3256], rax
 jmp xchain582_n82_α
 xchain582_n79_β:
 jmp xchain582_n49_α
.Lx676_0:
 .quad .Lx676_0_s
.Lx676_0_s:
 .string "EXPR$13"
# IR_MATCH_POS
 xchain582_n80_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain582_n72_β
 jmp xchain582_n83_α
 xchain582_n80_β:
 jmp xchain582_n72_β
# IR_ASSIGN gva
 xchain582_n81_α:
 mov rax, qword ptr [r12 + 2576]
 mov rdx, qword ptr [r12 + 2584]
 mov qword ptr [rbx + 304], rax
 mov qword ptr [rbx + 312], rdx
 mov qword ptr [r12 + 2560], rax
 mov qword ptr [r12 + 2568], rdx
 jmp xchain582_n42_α
 xchain582_n81_β:
 jmp xchain582_n42_α
 xchain582_n82_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3248] -> [zr+3232]
 mov rax, qword ptr [r12 + 3248]
 mov qword ptr [r12 + 3232], rax
 mov rax, qword ptr [r12 + 3256]
 mov qword ptr [r12 + 3240], rax
  .section .rodata
  .Lrkfn680: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn680]
 lea rsi, [r12 + 3232]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3216], rax
 mov qword ptr [r12 + 3224], rdx
 cmp eax, 99
 je xchain582_n49_α
 jmp xchain582_n84_α
 xchain582_n82_β:
 jmp xchain582_n49_α
# IR_MATCH_DEFER
 xchain582_n83_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx681_0
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
 je .Lx681_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain582_n72_β
.Lx681_1:
 jmp xchain582_n85_α
.Lx681_0:
 lea rdi, [rip + .S9]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain582_n72_β
 mov r14d, eax
 jmp xchain582_n85_α
 xchain582_n83_β:
 jmp xchain582_n72_β
 xchain582_n84_α:
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
 je xchain582_n49_α
 jmp xchain582_n86_α
 xchain582_n84_β:
 jmp xchain582_n49_α
# IR_LIT_INTEGER
 xchain582_n85_α:
 mov qword ptr [r12 + 3408], 6
 mov rax, qword ptr [rip + .Lx683_0]
 mov qword ptr [r12 + 3416], rax
 jmp xchain582_n87_α
 xchain582_n85_β:
 jmp xchain582_n72_α
.Lx683_0:
 .quad 0
# IR_ASSIGN gva
 xchain582_n86_α:
 mov rax, qword ptr [r12 + 2928]
 mov rdx, qword ptr [r12 + 2936]
 mov qword ptr [rbx + 320], rax
 mov qword ptr [rbx + 328], rdx
 mov qword ptr [r12 + 2912], rax
 mov qword ptr [r12 + 2920], rdx
 jmp xchain582_n49_α
 xchain582_n86_β:
 jmp xchain582_n49_α
# IR_MATCH_RPOS
 xchain582_n87_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain582_n72_β
 jmp xchain582_n88_α
 xchain582_n87_β:
 jmp xchain582_n72_β
# IR_MATCH_RELEASE
 xchain582_n88_α:
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
 jmp xchain582_n89_α
 xchain582_n89_α:
  .section .rodata
  .Lcall473_pname: .string "Pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcall473_pname]
 mov esi, 0
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 3472], rax
 mov qword ptr [r12 + 3480], rdx
 cmp eax, 99
 je xchain582_n61_α
 jmp xchain582_n90_α
xchain582_n89_β:
 jmp xchain582_n61_α
# IR_ASSIGN global
 xchain582_n90_α:
 mov rsi, qword ptr [r12 + 3472]
 mov rdx, qword ptr [r12 + 3480]
 mov rdi, qword ptr [rip + .Lx689_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 3456], rax
 mov qword ptr [r12 + 3464], rdx
 jmp xchain582_n61_α
 xchain582_n90_β:
 jmp xchain582_n61_α
.Lx689_0:
 .quad .Lx689_0_s
.Lx689_0_s:
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
