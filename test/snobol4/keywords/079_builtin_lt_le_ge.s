  .intel_syntax noprefix
  .text
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
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
  mov qword ptr [rsp + 728], rsp
  mov r12, qword ptr [1879048192]
  mov rbp, rsp
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rsp + 224], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 232], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_γ
.Lx1_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_γ
.Lx2_0:
 .quad 5
# IR_COERCE_NUMERIC
 xchain0_n2_α:
 mov eax, dword ptr [rsp + 224]
 cmp eax, 7
 je .Lx4_1
 cmp eax, 6
 jne .Lx4_0
 mov eax, dword ptr [rsp + 192]
 cmp eax, 6
 jne .Lx4_0
.Lx4_1:
 mov rax, qword ptr [rsp + 224]
 mov qword ptr [rsp + 160], rax
 mov rax, qword ptr [rsp + 232]
 mov qword ptr [rsp + 168], rax
 jmp .Lx4_2
.Lx4_0:
 lea rdi, [rsp + 224]
 lea rsi, [rsp + 192]
 lea rdx, [rsp + 160]
 mov rcx, 147
 call rt_coerce_num2_d@PLT
.Lx4_2:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_γ
# IR_COERCE_NUMERIC
 xchain0_n3_α:
 mov eax, dword ptr [rsp + 192]
 cmp eax, 7
 je .Lx6_1
 cmp eax, 6
 jne .Lx6_0
 mov eax, dword ptr [rsp + 224]
 cmp eax, 6
 jne .Lx6_0
.Lx6_1:
 mov rax, qword ptr [rsp + 192]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 200]
 mov qword ptr [rsp + 136], rax
 jmp .Lx6_2
.Lx6_0:
 lea rdi, [rsp + 192]
 lea rsi, [rsp + 224]
 lea rdx, [rsp + 128]
 mov rcx, 148
 call rt_coerce_num2_d@PLT
.Lx6_2:
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n4_α:
 lea rdi, [rsp + 160]
 lea rsi, [rsp + 128]
 call rt_cmp_d@PLT
 test eax, eax
 jns main_γ
 mov qword ptr [rsp + 96], 0
 mov qword ptr [rsp + 104], 0
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [rsp + 272], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 280], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "3 < 5"
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [rsp + 272]
 mov rdx, qword ptr [rsp + 280]
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 256], rax
 mov qword ptr [rsp + 264], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n7_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rsp + 432], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 440], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp main_γ
.Lx11_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [rsp + 400], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rsp + 408], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp main_γ
.Lx12_0:
 .quad 5
# IR_COERCE_NUMERIC
 xchain0_n9_α:
 mov eax, dword ptr [rsp + 432]
 cmp eax, 7
 je .Lx14_1
 cmp eax, 6
 jne .Lx14_0
 mov eax, dword ptr [rsp + 400]
 cmp eax, 6
 jne .Lx14_0
.Lx14_1:
 mov rax, qword ptr [rsp + 432]
 mov qword ptr [rsp + 368], rax
 mov rax, qword ptr [rsp + 440]
 mov qword ptr [rsp + 376], rax
 jmp .Lx14_2
.Lx14_0:
 lea rdi, [rsp + 432]
 lea rsi, [rsp + 400]
 lea rdx, [rsp + 368]
 mov rcx, 118
 call rt_coerce_num2_d@PLT
.Lx14_2:
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp main_γ
# IR_COERCE_NUMERIC
 xchain0_n10_α:
 mov eax, dword ptr [rsp + 400]
 cmp eax, 7
 je .Lx16_1
 cmp eax, 6
 jne .Lx16_0
 mov eax, dword ptr [rsp + 432]
 cmp eax, 6
 jne .Lx16_0
.Lx16_1:
 mov rax, qword ptr [rsp + 400]
 mov qword ptr [rsp + 336], rax
 mov rax, qword ptr [rsp + 408]
 mov qword ptr [rsp + 344], rax
 jmp .Lx16_2
.Lx16_0:
 lea rdi, [rsp + 400]
 lea rsi, [rsp + 432]
 lea rdx, [rsp + 336]
 mov rcx, 119
 call rt_coerce_num2_d@PLT
.Lx16_2:
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n11_α:
 lea rdi, [rsp + 368]
 lea rsi, [rsp + 336]
 call rt_cmp_d@PLT
 test eax, eax
 jg main_γ
 mov qword ptr [rsp + 304], 0
 mov qword ptr [rsp + 312], 0
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [rsp + 480], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 488], rax
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "5 <= 5"
# IR_ASSIGN global
 xchain0_n13_α:
 mov rsi, qword ptr [rsp + 480]
 mov rdx, qword ptr [rsp + 488]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [rsp + 640], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rsp + 648], rax
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp main_γ
.Lx21_0:
 .quad 7
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [rsp + 608], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rsp + 616], rax
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp main_γ
.Lx22_0:
 .quad 5
# IR_COERCE_NUMERIC
 xchain0_n16_α:
 mov eax, dword ptr [rsp + 640]
 cmp eax, 7
 je .Lx24_1
 cmp eax, 6
 jne .Lx24_0
 mov eax, dword ptr [rsp + 608]
 cmp eax, 6
 jne .Lx24_0
.Lx24_1:
 mov rax, qword ptr [rsp + 640]
 mov qword ptr [rsp + 576], rax
 mov rax, qword ptr [rsp + 648]
 mov qword ptr [rsp + 584], rax
 jmp .Lx24_2
.Lx24_0:
 lea rdi, [rsp + 640]
 lea rsi, [rsp + 608]
 lea rdx, [rsp + 576]
 mov rcx, 109
 call rt_coerce_num2_d@PLT
.Lx24_2:
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_γ
# IR_COERCE_NUMERIC
 xchain0_n17_α:
 mov eax, dword ptr [rsp + 608]
 cmp eax, 7
 je .Lx26_1
 cmp eax, 6
 jne .Lx26_0
 mov eax, dword ptr [rsp + 640]
 cmp eax, 6
 jne .Lx26_0
.Lx26_1:
 mov rax, qword ptr [rsp + 608]
 mov qword ptr [rsp + 544], rax
 mov rax, qword ptr [rsp + 616]
 mov qword ptr [rsp + 552], rax
 jmp .Lx26_2
.Lx26_0:
 lea rdi, [rsp + 608]
 lea rsi, [rsp + 640]
 lea rdx, [rsp + 544]
 mov rcx, 110
 call rt_coerce_num2_d@PLT
.Lx26_2:
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n18_α:
 lea rdi, [rsp + 576]
 lea rsi, [rsp + 544]
 call rt_cmp_d@PLT
 test eax, eax
 js main_γ
 mov qword ptr [rsp + 512], 0
 mov qword ptr [rsp + 520], 0
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [rsp + 688], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [rsp + 696], rax
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "7 >= 5"
# IR_ASSIGN global
 xchain0_n20_α:
 mov rsi, qword ptr [rsp + 688]
 mov rdx, qword ptr [rsp + 696]
 mov rdi, qword ptr [rip + .Lx30_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 672], rax
 mov qword ptr [rsp + 680], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 728]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 728]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
