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
# IR_LIT_S
bb1_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n1_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "complex(real,imag)"
xgvarg1_done:
bb2_α:
# BOX IR_CALL DATA(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn4: .string "DATA"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n1_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n1_α
snoch0_n1_α:
# IR_LIT_I
bb3_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg5_done
 xgvarg5_β:
 jmp snoch0_n3_α
.Lx6_0:
 .quad 3
xgvarg5_done:
xgvarg7_done:
bb4_α:
# BOX IR_CALL complex(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+96]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 104], rax
# marshal arg1 = inline gvar-unop subexpr -> [r12+112]
 movabs rax, 2
 neg rax
 mov qword ptr [r12 + 112], 6
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lbynamefn9: .string "complex"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 96]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je snoch0_n3_α
 jmp snoch0_n2_α
snoch0_n1_β:
 jmp snoch0_n3_α
snoch0_n2_α:
bb5_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 80]
 mov rcx, qword ptr [r12 + 88]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rcx
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n3_α
snoch0_n3_α:
# IR_VAR gva
bb6_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xgvarg11_done
 xgvarg11_β:
 jmp snoch0_n5_α
xgvarg11_done:
bb7_α:
# BOX IR_CALL real(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+128] -> [r12+160]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lrkfn15: .string "real"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn15]
 lea rsi, [r12 + 160]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n5_α
snoch0_n4_α:
bb8_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n5_α
snoch0_n5_α:
# IR_VAR gva
bb9_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xgvarg17_done
 xgvarg17_β:
 jmp flat_γ
xgvarg17_done:
bb10_α:
# BOX IR_CALL imag(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+176] -> [r12+208]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lbynamefn21: .string "imag"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 208]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n6_α
snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
bb11_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch0_n6_β:
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
.S0: .string "X"
.S1: .string "OUTPUT"
.text
