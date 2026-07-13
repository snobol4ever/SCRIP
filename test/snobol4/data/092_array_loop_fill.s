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
  sub rsp, 8
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
  add rsp, 24
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
  mov qword ptr [r12 + 1176], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 1168], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 5
 xchain0_n1_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn3: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 208], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n10_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n8_α:
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 272]
 mov rcx, qword ptr [r12 + 280]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n7_α
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [r12 + 528], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx11_0:
 .quad 1
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n14_α
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n7_α
 xchain0_n12_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 496]
 cmp eax, 100
 je .Lx14_0
 mov eax, dword ptr [r12 + 496]
 cmp eax, 6
 jne .Lx14_2
.Lx14_1:
 mov rax, qword ptr [r12 + 504]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 464], 6
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n16_α
.Lx14_0:
 mov rdi, qword ptr [r12 + 496]
 mov rsi, qword ptr [r12 + 504]
 mov rdx, qword ptr [r12 + 528]
 mov rcx, qword ptr [r12 + 536]
 mov r8d, 0
 lea r9, [r12 + 464]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx14_3
.Lx14_2:
 mov rdi, qword ptr [r12 + 496]
 mov rsi, qword ptr [r12 + 504]
 mov rdx, qword ptr [r12 + 528]
 mov rcx, qword ptr [r12 + 536]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n10_α
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
.Lx14_3:
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n10_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 656], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx15_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [r12 + 736], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n21_α
.Lx16_0:
 .quad 1
# IR_VAR
 xchain0_n15_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain0_n19_α
 xchain0_n15_β:
 jmp xchain0_n7_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 464]
 mov rdx, qword ptr [r12 + 472]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain0_n10_α
 xchain0_n16_β:
 jmp xchain0_n10_α
# IR_COERCE_NUMERIC
 xchain0_n17_α:
 mov eax, dword ptr [r12 + 688]
 cmp eax, 7
 je .Lx20_1
 cmp eax, 6
 jne .Lx20_0
 mov eax, dword ptr [r12 + 656]
 cmp eax, 6
 jne .Lx20_0
.Lx20_1:
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 632], rax
 jmp .Lx20_2
.Lx20_0:
 lea rdi, [r12 + 688]
 lea rsi, [r12 + 656]
 lea rdx, [r12 + 624]
 mov rcx, 118
 call rt_coerce_num2_d@PLT
.Lx20_2:
 jmp xchain0_n20_α
 xchain0_n17_β:
 jmp xchain0_n14_α
# IR_ASSIGN gva
 xchain0_n18_α:
 mov rax, qword ptr [r12 + 736]
 mov rdx, qword ptr [r12 + 744]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 jmp xchain0_n21_α
 xchain0_n18_β:
 jmp xchain0_n21_α
 xchain0_n19_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 368]
 cmp eax, 100
 je .Lx22_0
 mov eax, dword ptr [r12 + 400]
 cmp eax, 100
 je .Lx22_0
 mov eax, dword ptr [r12 + 368]
 cmp eax, 6
 jne .Lx22_2
 mov eax, dword ptr [r12 + 400]
 cmp eax, 6
 jne .Lx22_2
.Lx22_1:
 mov rax, qword ptr [r12 + 376]
 mov rcx, qword ptr [r12 + 408]
 imul rax, rcx
 mov qword ptr [r12 + 336], 6
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n22_α
.Lx22_0:
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 mov rdx, qword ptr [r12 + 400]
 mov rcx, qword ptr [r12 + 408]
 mov r8d, 2
 lea r9, [r12 + 336]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx22_3
.Lx22_2:
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 mov rdx, qword ptr [r12 + 400]
 mov rcx, qword ptr [r12 + 408]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
.Lx22_3:
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n7_α
# IR_COERCE_NUMERIC
 xchain0_n20_α:
 mov eax, dword ptr [r12 + 656]
 cmp eax, 7
 je .Lx24_1
 cmp eax, 6
 jne .Lx24_0
 mov eax, dword ptr [r12 + 688]
 cmp eax, 6
 jne .Lx24_0
.Lx24_1:
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 600], rax
 jmp .Lx24_2
.Lx24_0:
 lea rdi, [r12 + 656]
 lea rsi, [r12 + 688]
 lea rdx, [r12 + 592]
 mov rcx, 119
 call rt_coerce_num2_d@PLT
.Lx24_2:
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp xchain0_n14_α
# IR_VAR
 xchain0_n21_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain0_n24_α
 xchain0_n21_β:
 jmp xchain0_n25_α
# IR_ASSIGN_VAR
 xchain0_n22_α:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 mov rdx, qword ptr [r12 + 336]
 mov rcx, qword ptr [r12 + 344]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain0_n7_α
 xchain0_n22_β:
 jmp xchain0_n7_α
# IR_CMP_TEST
 xchain0_n23_α:
 lea rdi, [r12 + 624]
 lea rsi, [r12 + 592]
 call rt_cmp_d@PLT
 test eax, eax
 jg xchain0_n14_α
 mov qword ptr [r12 + 560], 0
 mov qword ptr [r12 + 568], 0
 jmp xchain0_n5_α
 xchain0_n23_β:
 jmp xchain0_n14_α
# IR_VAR
 xchain0_n24_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n25_α
# IR_VAR
 xchain0_n25_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n28_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n26_α:
 mov rdi, qword ptr [r12 + 784]
 mov rsi, qword ptr [r12 + 792]
 mov rdx, qword ptr [r12 + 816]
 mov rcx, qword ptr [r12 + 824]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n25_α
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp xchain0_n29_α
 xchain0_n26_β:
 jmp xchain0_n25_α
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [r12 + 976], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n30_α
 xchain0_n27_β:
 jmp xchain0_n28_α
.Lx32_0:
 .quad 1
# IR_VAR
 xchain0_n28_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
 jmp xchain0_n31_α
 xchain0_n28_β:
 jmp main_γ
# IR_DEREF variable -> value
 xchain0_n29_α:
 mov rdi, qword ptr [r12 + 848]
 mov rsi, qword ptr [r12 + 856]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n25_α
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 jmp xchain0_n32_α
 xchain0_n29_β:
 jmp xchain0_n25_α
 xchain0_n30_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 944]
 cmp eax, 100
 je .Lx35_0
 mov eax, dword ptr [r12 + 944]
 cmp eax, 6
 jne .Lx35_2
.Lx35_1:
 mov rax, qword ptr [r12 + 952]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 912], 6
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n33_α
.Lx35_0:
 mov rdi, qword ptr [r12 + 944]
 mov rsi, qword ptr [r12 + 952]
 mov rdx, qword ptr [r12 + 976]
 mov rcx, qword ptr [r12 + 984]
 mov r8d, 0
 lea r9, [r12 + 912]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx35_3
.Lx35_2:
 mov rdi, qword ptr [r12 + 944]
 mov rsi, qword ptr [r12 + 952]
 mov rdx, qword ptr [r12 + 976]
 mov rcx, qword ptr [r12 + 984]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n28_α
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
.Lx35_3:
 jmp xchain0_n33_α
 xchain0_n30_β:
 jmp xchain0_n28_α
# IR_LIT_INTEGER
 xchain0_n31_α:
 mov qword ptr [r12 + 1104], 6
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 1112], rax
 jmp xchain0_n34_α
 xchain0_n31_β:
 jmp main_γ
.Lx36_0:
 .quad 5
# IR_ASSIGN global
 xchain0_n32_α:
 mov rsi, qword ptr [r12 + 880]
 mov rdx, qword ptr [r12 + 888]
 mov rdi, qword ptr [rip + .Lx37_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xchain0_n25_α
 xchain0_n32_β:
 jmp xchain0_n25_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "OUTPUT"
# IR_ASSIGN gva
 xchain0_n33_α:
 mov rax, qword ptr [r12 + 912]
 mov rdx, qword ptr [r12 + 920]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp xchain0_n28_α
 xchain0_n33_β:
 jmp xchain0_n28_α
# IR_COERCE_NUMERIC
 xchain0_n34_α:
 mov eax, dword ptr [r12 + 1136]
 cmp eax, 7
 je .Lx40_1
 cmp eax, 6
 jne .Lx40_0
 mov eax, dword ptr [r12 + 1104]
 cmp eax, 6
 jne .Lx40_0
.Lx40_1:
 mov rax, qword ptr [r12 + 1136]
 mov qword ptr [r12 + 1072], rax
 mov rax, qword ptr [r12 + 1144]
 mov qword ptr [r12 + 1080], rax
 jmp .Lx40_2
.Lx40_0:
 lea rdi, [r12 + 1136]
 lea rsi, [r12 + 1104]
 lea rdx, [r12 + 1072]
 mov rcx, 118
 call rt_coerce_num2_d@PLT
.Lx40_2:
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp main_γ
# IR_COERCE_NUMERIC
 xchain0_n35_α:
 mov eax, dword ptr [r12 + 1104]
 cmp eax, 7
 je .Lx42_1
 cmp eax, 6
 jne .Lx42_0
 mov eax, dword ptr [r12 + 1136]
 cmp eax, 6
 jne .Lx42_0
.Lx42_1:
 mov rax, qword ptr [r12 + 1104]
 mov qword ptr [r12 + 1040], rax
 mov rax, qword ptr [r12 + 1112]
 mov qword ptr [r12 + 1048], rax
 jmp .Lx42_2
.Lx42_0:
 lea rdi, [r12 + 1104]
 lea rsi, [r12 + 1136]
 lea rdx, [r12 + 1040]
 mov rcx, 119
 call rt_coerce_num2_d@PLT
.Lx42_2:
 jmp xchain0_n36_α
 xchain0_n35_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n36_α:
 lea rdi, [r12 + 1072]
 lea rsi, [r12 + 1040]
 call rt_cmp_d@PLT
 test eax, eax
 jg main_γ
 mov qword ptr [r12 + 1008], 0
 mov qword ptr [r12 + 1016], 0
 jmp xchain0_n21_α
 xchain0_n36_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 1176]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1168]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 1176]
pop r12
ret
