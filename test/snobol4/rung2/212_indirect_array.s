  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "a"
  .Lgvan1: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .bss
  .align 16
__gva: .space 32, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
  call gva_register@PLT
  mov rbx, rax
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
main_α_body:
xchain0_n0_α:
# IR_LIT_INTEGER
bb1_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 3
xchain0_n1_α:
bb2_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+48]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn3: .string "array"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn3]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
xchain0_n1_β:
 jmp xchain0_n3_α
xchain0_n2_α:
# IR_ASSIGN gva
bb3_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
xchain0_n3_α:
# IR_VAR gva
bb4_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n7_α
xchain0_n4_α:
# IR_LIT_INTEGER
bb5_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n7_α
.Lx6_0:
 .quad 2
xchain0_n5_α:
# IR_SUBSCRIPT x[i] variable
bb6_α:
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
xchain0_n6_α:
# IR_LIT_STRING
bb7_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n7_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "x"
xchain0_n7_α:
# IR_VAR gva
bb8_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n12_α
xchain0_n8_α:
# IR_ASSIGN_VAR write through variable
bb9_α:
 mov rdi, qword ptr [r12 + 112]
 mov rsi, qword ptr [r12 + 120]
 mov rdx, qword ptr [r12 + 128]
 mov rcx, qword ptr [r12 + 136]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n7_α
 xchain0_n8_β:
 jmp xchain0_n7_α
xchain0_n9_α:
# IR_LIT_INTEGER
bb10_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n12_α
.Lx11_0:
 .quad 2
xchain0_n10_α:
# IR_SUBSCRIPT x[i] variable
bb11_α:
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n12_α
xchain0_n11_α:
# IR_DEREF variable -> value
bb12_α:
 mov rdi, qword ptr [r12 + 256]
 mov rsi, qword ptr [r12 + 264]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n12_α
xchain0_n12_α:
# IR_LIT_STRING
bb13_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "PASS 212_indirect_array (1/1)"
xchain0_n13_α:
# IR_LIT_STRING
bb14_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n12_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "x"
xchain0_n14_α:
# IR_ASSIGN gva
bb15_α:
 mov rax, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp main_γ
 xchain0_n14_β:
 jmp main_γ
xchain0_n15_α:
bb16_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+176]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 184], rax
# marshal arg1 = producer-box slot [r12+272] -> [r12+192]
 mov rax, qword ptr [r12 + 272]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 280]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lbynamefn18: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n16_α
xchain0_n15_β:
 jmp xchain0_n12_α
xchain0_n16_α:
# IR_LIT_STRING
bb17_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 212/001: $.var<index> indirect array"
xchain0_n17_α:
# IR_ASSIGN gva
bb18_α:
 mov rax, qword ptr [r12 + 304]
 mov rdx, qword ptr [r12 + 312]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
