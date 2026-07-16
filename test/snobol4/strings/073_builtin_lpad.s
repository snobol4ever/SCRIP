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
  mov qword ptr [rsp + 248], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 176], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 184], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_γ
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hi"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 208], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 216], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_γ
.Lx2_0:
 .quad 6
 xchain0_n2_α:
# BOX IR_CALL LPAD(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+176] -> [zr+128]
 mov rax, qword ptr [rsp + 176]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 184]
 mov qword ptr [rsp + 136], rax
# marshal arg1 = producer-box slot [zr+208] -> [zr+144]
 mov rax, qword ptr [rsp + 208]
 mov qword ptr [rsp + 144], rax
 mov rax, qword ptr [rsp + 216]
 mov qword ptr [rsp + 152], rax
  .section .rodata
  .Lrkfn4: .string "LPAD"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn4]
 lea rsi, [rsp + 128]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n3_α:
 mov rsi, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov rdi, qword ptr [rip + .Lx5_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp main_γ
 xchain0_n3_β:
 jmp main_γ
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 248]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 248]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
