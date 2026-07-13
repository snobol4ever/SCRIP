  .intel_syntax noprefix
  .text
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
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
  mov qword ptr [r12 + 616], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 608], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 224], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 42
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 42
# IR_COERCE_NUMERIC
 xchain0_n2_α:
 mov eax, dword ptr [r12 + 224]
 cmp eax, 7
 je .Lx4_1
 cmp eax, 6
 jne .Lx4_0
 mov eax, dword ptr [r12 + 192]
 cmp eax, 6
 jne .Lx4_0
.Lx4_1:
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 168], rax
 jmp .Lx4_2
.Lx4_0:
 lea rdi, [r12 + 224]
 lea rsi, [r12 + 192]
 lea rdx, [r12 + 160]
 mov rcx, 101
 call rt_coerce_num2_d@PLT
.Lx4_2:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_COERCE_NUMERIC
 xchain0_n3_α:
 mov eax, dword ptr [r12 + 192]
 cmp eax, 7
 je .Lx6_1
 cmp eax, 6
 jne .Lx6_0
 mov eax, dword ptr [r12 + 224]
 cmp eax, 6
 jne .Lx6_0
.Lx6_1:
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 136], rax
 jmp .Lx6_2
.Lx6_0:
 lea rdi, [r12 + 192]
 lea rsi, [r12 + 224]
 lea rdx, [r12 + 128]
 mov rcx, 102
 call rt_coerce_num2_d@PLT
.Lx6_2:
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 320], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n8_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "wrong"
# IR_CMP_TEST
 xchain0_n5_α:
 lea rdi, [r12 + 160]
 lea rsi, [r12 + 128]
 call rt_cmp_d@PLT
 test eax, eax
 jne xchain0_n4_α
 mov qword ptr [r12 + 96], 0
 mov qword ptr [r12 + 104], 0
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [r12 + 320]
 mov rdx, qword ptr [r12 + 328]
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n8_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "42 = 42"
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [r12 + 480], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n13_α
.Lx12_0:
 .quad 42
# IR_ASSIGN global
 xchain0_n9_α:
 mov rsi, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n8_α
 xchain0_n9_β:
 jmp xchain0_n8_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 448], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n13_α
.Lx14_0:
 .quad 99
# IR_COERCE_NUMERIC
 xchain0_n11_α:
 mov eax, dword ptr [r12 + 480]
 cmp eax, 7
 je .Lx16_1
 cmp eax, 6
 jne .Lx16_0
 mov eax, dword ptr [r12 + 448]
 cmp eax, 6
 jne .Lx16_0
.Lx16_1:
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 424], rax
 jmp .Lx16_2
.Lx16_0:
 lea rdi, [r12 + 480]
 lea rsi, [r12 + 448]
 lea rdx, [r12 + 416]
 mov rcx, 149
 call rt_coerce_num2_d@PLT
.Lx16_2:
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n13_α
# IR_COERCE_NUMERIC
 xchain0_n12_α:
 mov eax, dword ptr [r12 + 448]
 cmp eax, 7
 je .Lx18_1
 cmp eax, 6
 jne .Lx18_0
 mov eax, dword ptr [r12 + 480]
 cmp eax, 6
 jne .Lx18_0
.Lx18_1:
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 392], rax
 jmp .Lx18_2
.Lx18_0:
 lea rdi, [r12 + 448]
 lea rsi, [r12 + 480]
 lea rdx, [r12 + 384]
 mov rcx, 150
 call rt_coerce_num2_d@PLT
.Lx18_2:
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n13_α
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "wrong"
# IR_CMP_TEST
 xchain0_n14_α:
 lea rdi, [r12 + 416]
 lea rsi, [r12 + 384]
 call rt_cmp_d@PLT
 test eax, eax
 je xchain0_n13_α
 mov qword ptr [r12 + 352], 0
 mov qword ptr [r12 + 360], 0
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n13_α
# IR_ASSIGN global
 xchain0_n15_α:
 mov rsi, qword ptr [r12 + 576]
 mov rdx, qword ptr [r12 + 584]
 mov rdi, qword ptr [rip + .Lx22_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp main_γ
 xchain0_n15_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [r12 + 528], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "42 != 99"
# IR_ASSIGN global
 xchain0_n17_α:
 mov rsi, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_γ
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 616]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 608]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 616]
pop r12
ret
