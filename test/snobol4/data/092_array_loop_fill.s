  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "A"
  .Lgvan1: .string "I"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .bss
  .align 16
__gva: .space 32, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
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
 mov qword ptr [r12 + 672], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 5
 xchain0_n1_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+112] -> [zr+96]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lrkfn3: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n10_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n8_α:
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n7_α
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [r12 + 320], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx11_0:
 .quad 1
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n14_α
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n7_α
 xchain0_n12_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 304]
 cmp eax, 100
 je .Lx14_0
 mov eax, dword ptr [r12 + 320]
 cmp eax, 100
 je .Lx14_0
 mov eax, dword ptr [r12 + 304]
 cmp eax, 6
 jne .Lx14_2
 mov eax, dword ptr [r12 + 320]
 cmp eax, 6
 jne .Lx14_2
.Lx14_1:
 mov rax, qword ptr [r12 + 312]
 mov rcx, qword ptr [r12 + 328]
 add rax, rcx
 mov qword ptr [r12 + 288], 6
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n16_α
.Lx14_0:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 mov rdx, qword ptr [r12 + 320]
 mov rcx, qword ptr [r12 + 328]
 mov r8d, 0
 lea r9, [r12 + 288]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx14_3
.Lx14_2:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 mov rdx, qword ptr [r12 + 320]
 mov rcx, qword ptr [r12 + 328]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n10_α
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
.Lx14_3:
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n10_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx15_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [r12 + 432], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n20_α
.Lx16_0:
 .quad 1
# IR_VAR
 xchain0_n15_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n19_α
 xchain0_n15_β:
 jmp xchain0_n7_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n10_α
 xchain0_n16_β:
 jmp xchain0_n10_α
 xchain0_n17_α:
# BOX IR_CALL LE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+384] -> [zr+352]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [zr+400] -> [zr+368]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lrkfn20: .string "LE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn20]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n5_α
 xchain0_n17_β:
 jmp xchain0_n14_α
# IR_ASSIGN gva
 xchain0_n18_α:
 mov rax, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n20_α
 xchain0_n19_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 224]
 cmp eax, 100
 je .Lx22_0
 mov eax, dword ptr [r12 + 240]
 cmp eax, 100
 je .Lx22_0
 mov eax, dword ptr [r12 + 224]
 cmp eax, 6
 jne .Lx22_2
 mov eax, dword ptr [r12 + 240]
 cmp eax, 6
 jne .Lx22_2
.Lx22_1:
 mov rax, qword ptr [r12 + 232]
 mov rcx, qword ptr [r12 + 248]
 imul rax, rcx
 mov qword ptr [r12 + 208], 6
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n21_α
.Lx22_0:
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 mov r8d, 2
 lea r9, [r12 + 208]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx22_3
.Lx22_2:
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
.Lx22_3:
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n20_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n23_α
# IR_ASSIGN_VAR
 xchain0_n21_α:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 208]
 mov rcx, qword ptr [r12 + 216]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n7_α
 xchain0_n21_β:
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n22_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n23_α
# IR_VAR
 xchain0_n23_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain0_n25_α
 xchain0_n23_β:
 jmp xchain0_n26_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n24_α:
 mov rdi, qword ptr [r12 + 464]
 mov rsi, qword ptr [r12 + 472]
 mov rdx, qword ptr [r12 + 480]
 mov rcx, qword ptr [r12 + 488]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n27_α
 xchain0_n24_β:
 jmp xchain0_n23_α
# IR_LIT_INTEGER
 xchain0_n25_α:
 mov qword ptr [r12 + 576], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n28_α
 xchain0_n25_β:
 jmp xchain0_n26_α
.Lx28_0:
 .quad 1
# IR_VAR
 xchain0_n26_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xchain0_n29_α
 xchain0_n26_β:
 jmp main_γ
# IR_DEREF variable -> value
 xchain0_n27_α:
 mov rdi, qword ptr [r12 + 496]
 mov rsi, qword ptr [r12 + 504]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain0_n30_α
 xchain0_n27_β:
 jmp xchain0_n23_α
 xchain0_n28_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 560]
 cmp eax, 100
 je .Lx31_0
 mov eax, dword ptr [r12 + 576]
 cmp eax, 100
 je .Lx31_0
 mov eax, dword ptr [r12 + 560]
 cmp eax, 6
 jne .Lx31_2
 mov eax, dword ptr [r12 + 576]
 cmp eax, 6
 jne .Lx31_2
.Lx31_1:
 mov rax, qword ptr [r12 + 568]
 mov rcx, qword ptr [r12 + 584]
 add rax, rcx
 mov qword ptr [r12 + 544], 6
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n31_α
.Lx31_0:
 mov rdi, qword ptr [r12 + 560]
 mov rsi, qword ptr [r12 + 568]
 mov rdx, qword ptr [r12 + 576]
 mov rcx, qword ptr [r12 + 584]
 mov r8d, 0
 lea r9, [r12 + 544]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx31_3
.Lx31_2:
 mov rdi, qword ptr [r12 + 560]
 mov rsi, qword ptr [r12 + 568]
 mov rdx, qword ptr [r12 + 576]
 mov rcx, qword ptr [r12 + 584]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n26_α
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
.Lx31_3:
 jmp xchain0_n31_α
 xchain0_n28_β:
 jmp xchain0_n26_α
# IR_LIT_INTEGER
 xchain0_n29_α:
 mov qword ptr [r12 + 656], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n32_α
 xchain0_n29_β:
 jmp main_γ
.Lx32_0:
 .quad 5
# IR_ASSIGN global
 xchain0_n30_α:
 mov rsi, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain0_n23_α
 xchain0_n30_β:
 jmp xchain0_n23_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "OUTPUT"
# IR_ASSIGN gva
 xchain0_n31_α:
 mov rax, qword ptr [r12 + 544]
 mov rdx, qword ptr [r12 + 552]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain0_n26_α
 xchain0_n31_β:
 jmp xchain0_n26_α
 xchain0_n32_α:
# BOX IR_CALL LE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+640] -> [zr+608]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 616], rax
# marshal arg1 = producer-box slot [zr+656] -> [zr+624]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 632], rax
  .section .rodata
  .Lrkfn36: .string "LE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn36]
 lea rsi, [r12 + 608]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n20_α
 xchain0_n32_β:
 jmp main_γ
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
 mov rdi, qword ptr [r12 + 672]
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
