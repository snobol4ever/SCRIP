  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "qq"
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
# IR_LIT_STRING
bb1_α:
 mov qword ptr [r12 + 48], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 56], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "qq"
xchain0_n1_α:
bb2_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+48] -> [r12+32]
 mov rax, qword ptr [r12 + 48]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 56]
 mov qword ptr [r12 + 40], rax
  .section .rodata
  .Lrkfn3: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 32]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
xchain0_n2_α:
# IR_LIT_STRING
bb3_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n3_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "x"
xchain0_n3_α:
# IR_VAR gva
bb4_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n8_α
xchain0_n4_α:
# IR_ASSIGN_VAR write through variable
bb5_α:
 mov rdi, qword ptr [r12 + 16]
 mov rsi, qword ptr [r12 + 24]
 mov rdx, qword ptr [r12 + 64]
 mov rcx, qword ptr [r12 + 72]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n3_α
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain0_n3_α
 xchain0_n4_β:
 jmp xchain0_n3_α
xchain0_n5_α:
# IR_LIT_STRING
bb6_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n8_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "x"
xchain0_n6_α:
bb7_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+144] -> [r12+112]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [r12+160] -> [r12+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn9: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n7_α
xchain0_n6_β:
 jmp xchain0_n8_α
xchain0_n7_α:
# IR_LIT_STRING
bb8_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "FAIL 211/001: indirect assign sets named var"
xchain0_n8_α:
# IR_LIT_STRING
bb9_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n12_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "_no_such_var_"
xchain0_n9_α:
# IR_ASSIGN gva
bb10_α:
 mov rax, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
xchain0_n10_α:
bb11_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+288] -> [r12+272]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lrkfn14: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn14]
 lea rsi, [r12 + 272]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je xchain0_n12_α
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
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n12_α
xchain0_n12_α:
# IR_LIT_STRING
bb13_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp main_γ
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "PASS 211_indirect_assign (2/2)"
xchain0_n13_α:
bb14_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+240] -> [r12+224]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn18: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 224]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n15_α
xchain0_n13_β:
 jmp xchain0_n12_α
xchain0_n14_α:
# IR_ASSIGN gva
bb15_α:
 mov rax, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp main_γ
 xchain0_n14_β:
 jmp main_γ
xchain0_n15_α:
# IR_LIT_STRING
bb16_α:
 mov qword ptr [r12 + 320], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp main_γ
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "FAIL 211/002: undefined indirect is null"
xchain0_n16_α:
# IR_ASSIGN gva
bb17_α:
 mov rax, qword ptr [r12 + 320]
 mov rdx, qword ptr [r12 + 328]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp main_γ
 xchain0_n16_β:
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
