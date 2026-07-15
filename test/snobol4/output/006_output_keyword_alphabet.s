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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [rsp + 184], rsp
main_α_body:
# IR_KEYWORD_SNOBOL4_read
 xchain0_n0_α:
 mov rdi, qword ptr [rip + .Lx1_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rsp + 160], rax
 mov qword ptr [rsp + 168], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_γ
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "ALPHABET"
 xchain0_n1_α:
# BOX IR_CALL SIZE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lrkfn3: .string "SIZE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [rsp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n2_α:
 mov rsi, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov rdi, qword ptr [rip + .Lx4_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp main_γ
 xchain0_n2_β:
 jmp main_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 184]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 184]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
