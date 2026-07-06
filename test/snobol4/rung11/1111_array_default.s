  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "b"
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
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 3
xchain0_n1_α:
# IR_LIT_INTEGER
bb2_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 10
xchain0_n2_α:
bb3_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+80] -> [r12+48]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 56], rax
# marshal arg1 = producer-box slot [r12+96] -> [r12+64]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 72], rax
  .section .rodata
  .Lbynamefn4: .string "array"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 48]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
xchain0_n2_β:
 jmp xchain0_n4_α
xchain0_n3_α:
# IR_ASSIGN gva
bb4_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
xchain0_n4_α:
# IR_VAR gva
bb5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n8_α
xchain0_n5_α:
# IR_LIT_INTEGER
bb6_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n8_α
.Lx7_0:
 .quad 2
xchain0_n6_α:
# IR_SUBSCRIPT x[i] variable
bb7_α:
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n8_α
xchain0_n7_α:
# IR_DEREF variable -> value
bb8_α:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n8_α
xchain0_n8_α:
# IR_VAR gva
bb9_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n15_α
xchain0_n9_α:
# IR_LIT_INTEGER
bb10_α:
 mov qword ptr [r12 + 224], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n8_α
.Lx11_0:
 .quad 10
xchain0_n10_α:
# IR_LIT_INTEGER
bb11_α:
 mov qword ptr [r12 + 336], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n15_α
.Lx12_0:
 .quad 1
xchain0_n11_α:
bb12_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+128]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 136], rax
# marshal arg1 = producer-box slot [r12+224] -> [r12+144]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lbynamefn14: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn14]
 lea rsi, [r12 + 128]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n13_α
xchain0_n11_β:
 jmp xchain0_n8_α
xchain0_n12_α:
# IR_SUBSCRIPT x[i] variable
bb13_α:
 mov rdi, qword ptr [r12 + 320]
 mov rsi, qword ptr [r12 + 328]
 mov rdx, qword ptr [r12 + 336]
 mov rcx, qword ptr [r12 + 344]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n15_α
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n15_α
xchain0_n13_α:
# IR_LIT_STRING
bb14_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n16_α
 xchain0_n13_β:
 jmp main_γ
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "FAIL 1111/001: array default value"
xchain0_n14_α:
# IR_DEREF variable -> value
bb15_α:
 mov rdi, qword ptr [r12 + 352]
 mov rsi, qword ptr [r12 + 360]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n15_α
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp xchain0_n15_α
xchain0_n15_α:
# IR_LIT_STRING
bb16_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp main_γ
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "PASS 1111_array_default (2/2)"
xchain0_n16_α:
# IR_ASSIGN gva
bb17_α:
 mov rax, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
xchain0_n17_α:
# IR_LIT_INTEGER
bb18_α:
 mov qword ptr [r12 + 384], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n15_α
.Lx20_0:
 .quad 10
xchain0_n18_α:
# IR_ASSIGN gva
bb19_α:
 mov rax, qword ptr [r12 + 448]
 mov rdx, qword ptr [r12 + 456]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp main_γ
 xchain0_n18_β:
 jmp main_γ
xchain0_n19_α:
bb20_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+368] -> [r12+288]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 296], rax
# marshal arg1 = producer-box slot [r12+384] -> [r12+304]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 312], rax
  .section .rodata
  .Lbynamefn23: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je xchain0_n15_α
 jmp xchain0_n20_α
xchain0_n19_β:
 jmp xchain0_n15_α
xchain0_n20_α:
# IR_LIT_STRING
bb21_α:
 mov qword ptr [r12 + 416], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp main_γ
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "FAIL 1111/002: default fills all slots"
xchain0_n21_α:
# IR_ASSIGN gva
bb22_α:
 mov rax, qword ptr [r12 + 416]
 mov rdx, qword ptr [r12 + 424]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp main_γ
 xchain0_n21_β:
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
