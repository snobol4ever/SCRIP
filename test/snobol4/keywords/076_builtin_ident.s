  .intel_syntax noprefix
  .text
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  sub rsp, 65536
  mov rdi, rsp
  mov ecx, 8192
  xor eax, eax
  rep stosq
  mov rdi, rsp
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 65536
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
  mov qword ptr [r12 + 552], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 544], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "abc"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "abc"
 xchain0_n2_α:
# BOX IR_CALL IDENT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn4: .string "IDENT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n7_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "equal"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n7_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "not equal"
# IR_ASSIGN global
 xchain0_n5_α:
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n7_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov rdi, qword ptr [rip + .Lx8_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n7_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n11_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "abc"
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 416], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n11_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "xyz"
 xchain0_n9_α:
# BOX IR_CALL IDENT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+384] -> [zr+336]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 344], rax
# marshal arg1 = producer-box slot [zr+416] -> [zr+352]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 360], rax
  .section .rodata
  .Lrkfn12: .string "IDENT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn12]
 lea rsi, [r12 + 336]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "equal"
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "not equal"
# IR_ASSIGN global
 xchain0_n12_α:
 mov rsi, qword ptr [r12 + 464]
 mov rdx, qword ptr [r12 + 472]
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp main_γ
 xchain0_n12_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n13_α:
 mov rsi, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov rdi, qword ptr [rip + .Lx16_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp main_γ
 xchain0_n13_β:
 jmp main_γ
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 552]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 544]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 552]
pop r12
ret
