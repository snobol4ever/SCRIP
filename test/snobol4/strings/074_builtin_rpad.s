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
  mov qword ptr [rsp + 296], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 224], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 232], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_γ
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hi"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 256], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 264], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_γ
.Lx2_0:
 .quad 6
 xchain0_n2_α:
# BOX IR_CALL RPAD(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+224] -> [zr+176]
 mov rax, qword ptr [rsp + 224]
 mov qword ptr [rsp + 176], rax
 mov rax, qword ptr [rsp + 232]
 mov qword ptr [rsp + 184], rax
# marshal arg1 = producer-box slot [zr+256] -> [zr+192]
 mov rax, qword ptr [rsp + 256]
 mov qword ptr [rsp + 192], rax
 mov rax, qword ptr [rsp + 264]
 mov qword ptr [rsp + 200], rax
  .section .rodata
  .Lrkfn4: .string "RPAD"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [rsp + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 160], rax
 mov qword ptr [rsp + 168], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_γ
 xchain0_n3_α:
# BOX IR_CALL SIZE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lrkfn6: .string "SIZE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn6]
 lea rsi, [rsp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n4_α:
 mov rsi, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp main_γ
 xchain0_n4_β:
 jmp main_γ
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 296]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 296]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
