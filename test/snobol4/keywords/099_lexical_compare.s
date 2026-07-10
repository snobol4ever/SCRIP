  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
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
 mov qword ptr [r12 + 512], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_γ
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "b"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_γ
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "a"
# IR_COERCE_STRING
 xchain0_n2_α:
 lea rdi, [r12 + 128]
 lea rsi, [r12 + 96]
 mov rdx, 126
 call rt_coerce_str_d@PLT
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_γ
# IR_COERCE_STRING
 xchain0_n3_α:
 lea rdi, [r12 + 112]
 lea rsi, [r12 + 80]
 mov rdx, 127
 call rt_coerce_str_d@PLT
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n4_α:
 lea rdi, [r12 + 96]
 lea rsi, [r12 + 80]
 call rt_cmp_d@PLT
 test eax, eax
 jle main_γ
 mov qword ptr [r12 + 64], 0
 mov qword ptr [r12 + 72], 0
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "b > a"
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n7_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp main_γ
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "a"
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp main_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "b"
# IR_COERCE_STRING
 xchain0_n9_α:
 lea rdi, [r12 + 240]
 lea rsi, [r12 + 208]
 mov rdx, 130
 call rt_coerce_str_d@PLT
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp main_γ
# IR_COERCE_STRING
 xchain0_n10_α:
 lea rdi, [r12 + 224]
 lea rsi, [r12 + 192]
 mov rdx, 131
 call rt_coerce_str_d@PLT
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n11_α:
 lea rdi, [r12 + 208]
 lea rsi, [r12 + 192]
 call rt_cmp_d@PLT
 test eax, eax
 jns main_γ
 mov qword ptr [r12 + 176], 0
 mov qword ptr [r12 + 184], 0
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "a < b"
# IR_ASSIGN global
 xchain0_n13_α:
 mov rsi, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "cat"
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "cat"
# IR_COERCE_STRING
 xchain0_n16_α:
 lea rdi, [r12 + 352]
 lea rsi, [r12 + 320]
 mov rdx, 122
 call rt_coerce_str_d@PLT
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_γ
# IR_COERCE_STRING
 xchain0_n17_α:
 lea rdi, [r12 + 336]
 lea rsi, [r12 + 304]
 mov rdx, 123
 call rt_coerce_str_d@PLT
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n18_α:
 lea rdi, [r12 + 320]
 lea rsi, [r12 + 304]
 call rt_cmp_d@PLT
 test eax, eax
 jne main_γ
 mov qword ptr [r12 + 288], 0
 mov qword ptr [r12 + 296], 0
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n21_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "cat = cat"
# IR_ASSIGN global
 xchain0_n20_α:
 mov rsi, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 mov rdi, qword ptr [rip + .Lx30_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n21_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp main_γ
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "cat"
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp main_γ
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "dog"
# IR_COERCE_STRING
 xchain0_n23_α:
 lea rdi, [r12 + 464]
 lea rsi, [r12 + 432]
 mov rdx, 132
 call rt_coerce_str_d@PLT
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp main_γ
# IR_COERCE_STRING
 xchain0_n24_α:
 lea rdi, [r12 + 448]
 lea rsi, [r12 + 416]
 mov rdx, 133
 call rt_coerce_str_d@PLT
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n25_α:
 lea rdi, [r12 + 432]
 lea rsi, [r12 + 416]
 call rt_cmp_d@PLT
 test eax, eax
 je main_γ
 mov qword ptr [r12 + 400], 0
 mov qword ptr [r12 + 408], 0
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n26_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp main_γ
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "cat != dog"
# IR_ASSIGN global
 xchain0_n27_α:
 mov rsi, qword ptr [r12 + 496]
 mov rdx, qword ptr [r12 + 504]
 mov rdi, qword ptr [rip + .Lx40_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp main_γ
 xchain0_n27_β:
 jmp main_γ
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
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
 mov rdi, qword ptr [r12 + 512]
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
