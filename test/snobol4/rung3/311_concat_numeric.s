  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
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
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call flat_α
  xor eax, eax
  pop rbp
  ret
flat_α:
#=======================================================================================================================
    .global flat_α
    .global flat_β
    .global flat_γ
    .global flat_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
flat_α_body:
snoch0_n0_α:
xgvarg1_done:
# IR_LIT_S
bb1_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg2_done
 xgvarg2_β:
 jmp snoch0_n2_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "12"
xgvarg2_done:
bb2_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline concat 2 parts -> [r12+48]
 mov dword ptr [r12 + 80], 0
 lea rax, [rip + .S0]
 mov qword ptr [r12 + 88], rax
 mov dword ptr [r12 + 96], 0
 lea rax, [rip + .S1]
 mov qword ptr [r12 + 104], rax
 lea rdi, [r12 + 80]
 mov rsi, 2
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_concat_parts_d@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
# marshal arg1 = producer-box slot [r12+16] -> [r12+64]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 72], rax
  .section .rodata
  .Lbynamefn5: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn5]
 lea rsi, [r12 + 48]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
# IR_LIT_scalar
bb3_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
xgvarg7_done:
# IR_LIT_S
bb4_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg8_done
 xgvarg8_β:
 jmp snoch0_n5_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "222"
xgvarg8_done:
bb5_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline concat 3 parts -> [r12+144]
 mov dword ptr [r12 + 176], 0
 lea rax, [rip + .S1]
 mov qword ptr [r12 + 184], rax
 mov dword ptr [r12 + 192], 0
 lea rax, [rip + .S1]
 mov qword ptr [r12 + 200], rax
 mov dword ptr [r12 + 208], 0
 lea rax, [rip + .S1]
 mov qword ptr [r12 + 216], rax
 lea rdi, [r12 + 176]
 mov rsi, 3
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_concat_parts_d@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
# marshal arg1 = producer-box slot [r12+112] -> [r12+160]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
snoch0_n2_β:
 jmp snoch0_n5_α
snoch0_n3_α:
bb6_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
# IR_LIT_scalar
bb7_α:
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
xgvarg14_done:
# IR_LIT_S
bb8_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 232], rax
 jmp xgvarg15_done
 xgvarg15_β:
 jmp snoch0_n8_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "13.4"
xgvarg15_done:
bb9_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline concat 2 parts -> [r12+256]
 mov dword ptr [r12 + 288], 0
 lea rax, [rip + .S0]
 mov qword ptr [r12 + 296], rax
 mov dword ptr [r12 + 304], 0
 lea rax, [rip + .S4]
 mov qword ptr [r12 + 312], rax
 lea rdi, [r12 + 288]
 mov rsi, 2
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_concat_parts_d@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
# marshal arg1 = producer-box slot [r12+224] -> [r12+272]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn18: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
snoch0_n5_β:
 jmp snoch0_n8_α
snoch0_n6_α:
bb10_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_scalar
bb11_α:
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
# IR_LIT_scalar
bb12_α:
 jmp snoch0_n10_α
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
bb13_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
bb14_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n10_β:
 jmp flat_γ
flat_β:
jmp flat_ω
flat_γ:
mov eax, 1
xor edx, edx
pop r12
ret
flat_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
.section .rodata
.S0: .string "1"
.S1: .string "2"
.S2: .string "output"
.S3: .string "FAIL 311/001: int int concat"
.S4: .string "3.4"
.S5: .string "FAIL 311/002: three int concat"
.S6: .string "FAIL 311/003: int real concat"
.S7: .string "PASS 311_concat_numeric (3/3)"
.text
