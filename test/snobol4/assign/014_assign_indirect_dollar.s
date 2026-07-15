  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .bss
  .align 16
__gva: .space 16, 0
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
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
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
  mov qword ptr [rsp + 280], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "X"
 xchain0_n1_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [rsp + 144]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 152]
 mov qword ptr [rsp + 120], rax
  .section .rodata
  .Lrkfn3: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [rsp + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rsp + 176], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 184], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n3_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "hello"
# IR_VAR
 xchain0_n3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 240], rax
 mov qword ptr [rsp + 248], rdx
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp main_γ
# IR_ASSIGN_VAR
 xchain0_n4_α:
 mov rdi, qword ptr [rsp + 96]
 mov rsi, qword ptr [rsp + 104]
 mov rdx, qword ptr [rsp + 176]
 mov rcx, qword ptr [rsp + 184]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n3_α
 mov qword ptr [rsp + 208], rax
 mov qword ptr [rsp + 216], rdx
 jmp xchain0_n3_α
 xchain0_n4_β:
 jmp xchain0_n3_α
# IR_ASSIGN global
 xchain0_n5_α:
 mov rsi, qword ptr [rsp + 240]
 mov rdx, qword ptr [rsp + 248]
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
 jmp main_γ
 xchain0_n5_β:
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
mov rsp, qword ptr [rsp + 280]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 280]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
