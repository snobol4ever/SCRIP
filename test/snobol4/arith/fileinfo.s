  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "CHARS"
  .Lgvan1: .string "LINES"
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
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
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
# IR_LIT_scalar
bb1_α:
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb2_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 movabs rsi, 1
 call rt_gvar_assign_int@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_VAR gva
bb3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
# IR_VAR
bb4_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xgvarg5_done
 xgvarg5_β:
 jmp snoch0_n4_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "INPUT"
xgvarg5_done:
bb5_α:
# BOX IR_CALL SIZE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn9: .string "SIZE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n5_α
snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
bb6_α:
# IR_ASSIGN_CONCAT 4 parts
 mov dword ptr [r12 + 64], 1
 lea rax, [rip + .S2]
 mov qword ptr [r12 + 72], rax
 mov dword ptr [r12 + 80], 0
 lea rax, [rip + .S3]
 mov qword ptr [r12 + 88], rax
 mov dword ptr [r12 + 96], 1
 lea rax, [rip + .S4]
 mov qword ptr [r12 + 104], rax
 mov dword ptr [r12 + 112], 0
 lea rax, [rip + .S5]
 mov qword ptr [r12 + 120], rax
 lea rdi, [rip + .S1]
 lea rsi, [r12 + 64]
 mov edx, 4
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_gvar_assign_concat_parts@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp flat_γ
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
bb7_α:
# IR_BINOP_GVAR_ARITH_SLOT
 lea rdi, [rip + .S2]
 call rt_gvar_get_int@PLT
 mov rcx, qword ptr [r12 + 40]
 add rax, rcx
 mov qword ptr [r12 + 128], rax
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n4_α
snoch0_n6_α:
bb8_α:
# IR_ASSIGN
 mov rax, qword ptr [r12 + 128]
 mov rcx, qword ptr [r12 + 136]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rcx
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n4_α
snoch0_n7_α:
# IR_VAR gva
bb9_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n2_α
snoch0_n8_α:
# IR_LIT_scalar
bb10_α:
 jmp snoch0_n9_α
 snoch0_n8_β:
 jmp snoch0_n2_α
snoch0_n9_α:
bb11_α:
# IR_BINOP_GVAR_ARITH
 mov rdx, qword ptr [rbx + 16]
 cmp edx, 6
 jne .Lx18_0
 mov rax, qword ptr [rbx + 24]
 jmp .Lx18_1
.Lx18_0:
 lea rdi, [rip + .S4]
 call rt_gvar_get_int@PLT
.Lx18_1:
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 160], rax
 jmp snoch0_n10_α
 snoch0_n9_β:
 jmp snoch0_n2_α
snoch0_n10_α:
bb12_α:
# IR_ASSIGN
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [rbx + 16], 6
 mov qword ptr [rbx + 24], rax
 jmp snoch0_n2_α
 snoch0_n10_β:
 jmp snoch0_n2_α
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
.S0: .string "TRIM"
.S1: .string "OUTPUT"
.S2: .string "CHARS"
.S3: .string " characters, "
.S4: .string "LINES"
.S5: .string " lines read"
.text
