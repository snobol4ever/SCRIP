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
  mov qword ptr [rsp + 456], rsp
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
 .string "42"
 xchain0_n1_α:
# BOX IR_CALL INTEGER(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [rsp + 144]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 152]
 mov qword ptr [rsp + 120], rax
  .section .rodata
  .Lrkfn3: .string "INTEGER"
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
 mov qword ptr [rsp + 192], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 200], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n6_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "numeric"
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rsp + 240], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 248], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n6_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "not numeric"
# IR_ASSIGN global
 xchain0_n4_α:
 mov rsi, qword ptr [rsp + 192]
 mov rdx, qword ptr [rsp + 200]
 mov rdi, qword ptr [rip + .Lx6_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 176], rax
 mov qword ptr [rsp + 184], rdx
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n6_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n5_α:
 mov rsi, qword ptr [rsp + 240]
 mov rdx, qword ptr [rsp + 248]
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rsp + 320], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 328], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n9_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "abc"
 xchain0_n7_α:
# BOX IR_CALL INTEGER(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+320] -> [zr+288]
 mov rax, qword ptr [rsp + 320]
 mov qword ptr [rsp + 288], rax
 mov rax, qword ptr [rsp + 328]
 mov qword ptr [rsp + 296], rax
  .section .rodata
  .Lrkfn10: .string "INTEGER"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn10]
 lea rsi, [rsp + 288]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rsp + 368], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 376], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp main_γ
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "numeric"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rsp + 416], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rsp + 424], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp main_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "not numeric"
# IR_ASSIGN global
 xchain0_n10_α:
 mov rsi, qword ptr [rsp + 368]
 mov rdx, qword ptr [rsp + 376]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 352], rax
 mov qword ptr [rsp + 360], rdx
 jmp main_γ
 xchain0_n10_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n11_α:
 mov rsi, qword ptr [rsp + 416]
 mov rdx, qword ptr [rsp + 424]
 mov rdi, qword ptr [rip + .Lx14_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 400], rax
 mov qword ptr [rsp + 408], rdx
 jmp main_γ
 xchain0_n11_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 456]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 456]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
