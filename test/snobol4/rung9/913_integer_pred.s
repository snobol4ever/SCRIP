  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 1
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 1
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
  mov qword ptr [rsp + 536], rsp
  mov r12, qword ptr [1879048192]
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rbp + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "abc"
 xchain0_n1_α:
# BOX IR_CALL integer(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [rbp + 144]
 mov qword ptr [rbp + 112], rax
 mov rax, qword ptr [rbp + 152]
 mov qword ptr [rbp + 120], rax
  .section .rodata
  .Lrkfn3: .string "integer"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn3]
 lea rsi, [rbp + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rbp + 192], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp main_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "FAIL 913/001: integer(string) should fail"
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [rbp + 272], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rbp + 280], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n7_α
.Lx5_0:
 .quad 12
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [rbp + 192]
 mov rdx, qword ptr [rbp + 200]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 176], rax
 mov qword ptr [rbp + 184], rdx
 jmp main_γ
 xchain0_n4_β:
 jmp main_γ
 xchain0_n5_α:
# BOX IR_CALL integer(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+272] -> [zr+240]
 mov rax, qword ptr [rbp + 272]
 mov qword ptr [rbp + 240], rax
 mov rax, qword ptr [rbp + 280]
 mov qword ptr [rbp + 248], rax
  .section .rodata
  .Lrkfn8: .string "integer"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn8]
 lea rsi, [rbp + 240]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 cmp eax, 99
 je xchain0_n7_α
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rbp + 400], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rbp + 408], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "12"
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rbp + 320], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rbp + 328], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "FAIL 913/002: integer(12) should succeed"
 xchain0_n8_α:
# BOX IR_CALL integer(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+400] -> [zr+368]
 mov rax, qword ptr [rbp + 400]
 mov qword ptr [rbp + 368], rax
 mov rax, qword ptr [rbp + 408]
 mov qword ptr [rbp + 376], rax
  .section .rodata
  .Lrkfn12: .string "integer"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn12]
 lea rsi, [rbp + 368]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [rbp + 320]
 mov rdx, qword ptr [rbp + 328]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 304], rax
 mov qword ptr [rbp + 312], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [rbp + 496], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 504], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "PASS 913_integer_pred (3/3)"
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rbp + 448], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rbp + 456], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 913/003: integer(numeric-string) should succeed"
# IR_ASSIGN gva
 xchain0_n12_α:
 mov rax, qword ptr [rbp + 496]
 mov rdx, qword ptr [rbp + 504]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 480], rax
 mov qword ptr [rbp + 488], rdx
 jmp main_γ
 xchain0_n12_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n13_α:
 mov rax, qword ptr [rbp + 448]
 mov rdx, qword ptr [rbp + 456]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 jmp main_γ
 xchain0_n13_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 536]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
