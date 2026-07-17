  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "A"
  .Lgvan1: .string "I"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 2
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 2
  call gva_register@PLT
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
  sub rsp, 65544
  mov rdi, rsp
  mov ecx, 65544
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 1176], rsp
  mov r12, qword ptr [1879048192]
  mov rbp, rsp
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rsp + 160], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 5
 xchain0_n1_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lrkfn3: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn3]
 lea rsi, [rsp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [rsp + 208], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 216], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [rsp + 208]
 mov rdx, qword ptr [rsp + 216]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 192], rax
 mov qword ptr [rsp + 200], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 240], rax
 mov qword ptr [rsp + 248], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 496], rax
 mov qword ptr [rsp + 504], rdx
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n10_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n8_α:
 mov rdi, qword ptr [rsp + 240]
 mov rsi, qword ptr [rsp + 248]
 mov rdx, qword ptr [rsp + 272]
 mov rcx, qword ptr [rsp + 280]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [rsp + 304], rax
 mov qword ptr [rsp + 312], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n7_α
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [rsp + 528], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 536], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx11_0:
 .quad 1
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 688], rax
 mov qword ptr [rsp + 696], rdx
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n14_α
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 368], rax
 mov qword ptr [rsp + 376], rdx
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n7_α
 xchain0_n12_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 496]
 cmp eax, 100
 je .Lx14_0
 mov eax, dword ptr [rsp + 496]
 cmp eax, 6
 jne .Lx14_2
.Lx14_1:
 mov rax, qword ptr [rsp + 504]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rsp + 464], 6
 mov qword ptr [rsp + 472], rax
 jmp xchain0_n16_α
.Lx14_0:
 mov rdi, qword ptr [rsp + 496]
 mov rsi, qword ptr [rsp + 504]
 mov rdx, qword ptr [rsp + 528]
 mov rcx, qword ptr [rsp + 536]
 mov r8d, 0
 lea r9, [rsp + 464]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx14_3
.Lx14_2:
 mov rdi, qword ptr [rsp + 496]
 mov rsi, qword ptr [rsp + 504]
 mov rdx, qword ptr [rsp + 528]
 mov rcx, qword ptr [rsp + 536]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n10_α
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
.Lx14_3:
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n10_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [rsp + 656], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 664], rax
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx15_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [rsp + 736], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rsp + 744], rax
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n21_α
.Lx16_0:
 .quad 1
# IR_VAR
 xchain0_n15_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 400], rax
 mov qword ptr [rsp + 408], rdx
 jmp xchain0_n19_α
 xchain0_n15_β:
 jmp xchain0_n7_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [rsp + 464]
 mov rdx, qword ptr [rsp + 472]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 448], rax
 mov qword ptr [rsp + 456], rdx
 jmp xchain0_n10_α
 xchain0_n16_β:
 jmp xchain0_n10_α
# IR_COERCE_NUMERIC
 xchain0_n17_α:
 mov eax, dword ptr [rsp + 688]
 cmp eax, 7
 je .Lx20_1
 cmp eax, 6
 jne .Lx20_0
 mov eax, dword ptr [rsp + 656]
 cmp eax, 6
 jne .Lx20_0
.Lx20_1:
 mov rax, qword ptr [rsp + 688]
 mov qword ptr [rsp + 624], rax
 mov rax, qword ptr [rsp + 696]
 mov qword ptr [rsp + 632], rax
 jmp .Lx20_2
.Lx20_0:
 lea rdi, [rsp + 688]
 lea rsi, [rsp + 656]
 lea rdx, [rsp + 624]
 mov rcx, 118
 call rt_coerce_num2_d@PLT
.Lx20_2:
 jmp xchain0_n20_α
 xchain0_n17_β:
 jmp xchain0_n14_α
# IR_ASSIGN gva
 xchain0_n18_α:
 mov rax, qword ptr [rsp + 736]
 mov rdx, qword ptr [rsp + 744]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 720], rax
 mov qword ptr [rsp + 728], rdx
 jmp xchain0_n21_α
 xchain0_n18_β:
 jmp xchain0_n21_α
 xchain0_n19_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 368]
 cmp eax, 100
 je .Lx22_0
 mov eax, dword ptr [rsp + 400]
 cmp eax, 100
 je .Lx22_0
 mov eax, dword ptr [rsp + 368]
 cmp eax, 6
 jne .Lx22_2
 mov eax, dword ptr [rsp + 400]
 cmp eax, 6
 jne .Lx22_2
.Lx22_1:
 mov rax, qword ptr [rsp + 376]
 mov rcx, qword ptr [rsp + 408]
 imul rax, rcx
 mov qword ptr [rsp + 336], 6
 mov qword ptr [rsp + 344], rax
 jmp xchain0_n22_α
.Lx22_0:
 mov rdi, qword ptr [rsp + 368]
 mov rsi, qword ptr [rsp + 376]
 mov rdx, qword ptr [rsp + 400]
 mov rcx, qword ptr [rsp + 408]
 mov r8d, 2
 lea r9, [rsp + 336]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx22_3
.Lx22_2:
 mov rdi, qword ptr [rsp + 368]
 mov rsi, qword ptr [rsp + 376]
 mov rdx, qword ptr [rsp + 400]
 mov rcx, qword ptr [rsp + 408]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
.Lx22_3:
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n7_α
# IR_COERCE_NUMERIC
 xchain0_n20_α:
 mov eax, dword ptr [rsp + 656]
 cmp eax, 7
 je .Lx24_1
 cmp eax, 6
 jne .Lx24_0
 mov eax, dword ptr [rsp + 688]
 cmp eax, 6
 jne .Lx24_0
.Lx24_1:
 mov rax, qword ptr [rsp + 656]
 mov qword ptr [rsp + 592], rax
 mov rax, qword ptr [rsp + 664]
 mov qword ptr [rsp + 600], rax
 jmp .Lx24_2
.Lx24_0:
 lea rdi, [rsp + 656]
 lea rsi, [rsp + 688]
 lea rdx, [rsp + 592]
 mov rcx, 119
 call rt_coerce_num2_d@PLT
.Lx24_2:
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp xchain0_n14_α
# IR_VAR
 xchain0_n21_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 784], rax
 mov qword ptr [rsp + 792], rdx
 jmp xchain0_n24_α
 xchain0_n21_β:
 jmp xchain0_n25_α
# IR_ASSIGN_VAR
 xchain0_n22_α:
 mov rdi, qword ptr [rsp + 304]
 mov rsi, qword ptr [rsp + 312]
 mov rdx, qword ptr [rsp + 336]
 mov rcx, qword ptr [rsp + 344]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 jmp xchain0_n7_α
 xchain0_n22_β:
 jmp xchain0_n7_α
# IR_CMP_TEST
 xchain0_n23_α:
 lea rdi, [rsp + 624]
 lea rsi, [rsp + 592]
 call rt_cmp_d@PLT
 test eax, eax
 jg xchain0_n14_α
 mov qword ptr [rsp + 560], 0
 mov qword ptr [rsp + 568], 0
 jmp xchain0_n5_α
 xchain0_n23_β:
 jmp xchain0_n14_α
# IR_VAR
 xchain0_n24_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 816], rax
 mov qword ptr [rsp + 824], rdx
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n25_α
# IR_VAR
 xchain0_n25_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 944], rax
 mov qword ptr [rsp + 952], rdx
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n28_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n26_α:
 mov rdi, qword ptr [rsp + 784]
 mov rsi, qword ptr [rsp + 792]
 mov rdx, qword ptr [rsp + 816]
 mov rcx, qword ptr [rsp + 824]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n25_α
 mov qword ptr [rsp + 848], rax
 mov qword ptr [rsp + 856], rdx
 jmp xchain0_n29_α
 xchain0_n26_β:
 jmp xchain0_n25_α
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [rsp + 976], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [rsp + 984], rax
 jmp xchain0_n30_α
 xchain0_n27_β:
 jmp xchain0_n28_α
.Lx32_0:
 .quad 1
# IR_VAR
 xchain0_n28_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 1136], rax
 mov qword ptr [rsp + 1144], rdx
 jmp xchain0_n31_α
 xchain0_n28_β:
 jmp main_γ
# IR_DEREF variable -> value
 xchain0_n29_α:
 mov rdi, qword ptr [rsp + 848]
 mov rsi, qword ptr [rsp + 856]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n25_α
 mov qword ptr [rsp + 880], rax
 mov qword ptr [rsp + 888], rdx
 jmp xchain0_n32_α
 xchain0_n29_β:
 jmp xchain0_n25_α
 xchain0_n30_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 944]
 cmp eax, 100
 je .Lx35_0
 mov eax, dword ptr [rsp + 944]
 cmp eax, 6
 jne .Lx35_2
.Lx35_1:
 mov rax, qword ptr [rsp + 952]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rsp + 912], 6
 mov qword ptr [rsp + 920], rax
 jmp xchain0_n33_α
.Lx35_0:
 mov rdi, qword ptr [rsp + 944]
 mov rsi, qword ptr [rsp + 952]
 mov rdx, qword ptr [rsp + 976]
 mov rcx, qword ptr [rsp + 984]
 mov r8d, 0
 lea r9, [rsp + 912]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx35_3
.Lx35_2:
 mov rdi, qword ptr [rsp + 944]
 mov rsi, qword ptr [rsp + 952]
 mov rdx, qword ptr [rsp + 976]
 mov rcx, qword ptr [rsp + 984]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n28_α
 mov qword ptr [rsp + 912], rax
 mov qword ptr [rsp + 920], rdx
.Lx35_3:
 jmp xchain0_n33_α
 xchain0_n30_β:
 jmp xchain0_n28_α
# IR_LIT_INTEGER
 xchain0_n31_α:
 mov qword ptr [rsp + 1104], 6
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [rsp + 1112], rax
 jmp xchain0_n34_α
 xchain0_n31_β:
 jmp main_γ
.Lx36_0:
 .quad 5
# IR_ASSIGN global
 xchain0_n32_α:
 mov rsi, qword ptr [rsp + 880]
 mov rdx, qword ptr [rsp + 888]
 mov rdi, qword ptr [rip + .Lx37_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 768], rax
 mov qword ptr [rsp + 776], rdx
 jmp xchain0_n25_α
 xchain0_n32_β:
 jmp xchain0_n25_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "OUTPUT"
# IR_ASSIGN gva
 xchain0_n33_α:
 mov rax, qword ptr [rsp + 912]
 mov rdx, qword ptr [rsp + 920]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 896], rax
 mov qword ptr [rsp + 904], rdx
 jmp xchain0_n28_α
 xchain0_n33_β:
 jmp xchain0_n28_α
# IR_COERCE_NUMERIC
 xchain0_n34_α:
 mov eax, dword ptr [rsp + 1136]
 cmp eax, 7
 je .Lx40_1
 cmp eax, 6
 jne .Lx40_0
 mov eax, dword ptr [rsp + 1104]
 cmp eax, 6
 jne .Lx40_0
.Lx40_1:
 mov rax, qword ptr [rsp + 1136]
 mov qword ptr [rsp + 1072], rax
 mov rax, qword ptr [rsp + 1144]
 mov qword ptr [rsp + 1080], rax
 jmp .Lx40_2
.Lx40_0:
 lea rdi, [rsp + 1136]
 lea rsi, [rsp + 1104]
 lea rdx, [rsp + 1072]
 mov rcx, 118
 call rt_coerce_num2_d@PLT
.Lx40_2:
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp main_γ
# IR_COERCE_NUMERIC
 xchain0_n35_α:
 mov eax, dword ptr [rsp + 1104]
 cmp eax, 7
 je .Lx42_1
 cmp eax, 6
 jne .Lx42_0
 mov eax, dword ptr [rsp + 1136]
 cmp eax, 6
 jne .Lx42_0
.Lx42_1:
 mov rax, qword ptr [rsp + 1104]
 mov qword ptr [rsp + 1040], rax
 mov rax, qword ptr [rsp + 1112]
 mov qword ptr [rsp + 1048], rax
 jmp .Lx42_2
.Lx42_0:
 lea rdi, [rsp + 1104]
 lea rsi, [rsp + 1136]
 lea rdx, [rsp + 1040]
 mov rcx, 119
 call rt_coerce_num2_d@PLT
.Lx42_2:
 jmp xchain0_n36_α
 xchain0_n35_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n36_α:
 lea rdi, [rsp + 1072]
 lea rsi, [rsp + 1040]
 call rt_cmp_d@PLT
 test eax, eax
 jg main_γ
 mov qword ptr [rsp + 1008], 0
 mov qword ptr [rsp + 1016], 0
 jmp xchain0_n21_α
 xchain0_n36_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 1176]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 1176]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
