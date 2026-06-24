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
xargsub8_n0_α:
# IR_LIT_I
bb4_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 88], rax
 jmp xargsub8_n1_α
 xargsub8_n0_β:
 jmp snoch0_n3_α
.Lx9_0:
 .quad 2
xargsub8_n1_α:
bb5_α:
# IR_UNOP_GVAR_SLOT
 mov rax, 2
 neg rax
 mov qword ptr [r12 + 96], rax
 jmp xgvarg7_done
 xargsub8_n1_β:
 jmp snoch0_n3_α
xgvarg7_done:
bb6_α:
# BOX IR_CALL complex(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+120]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 120], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 128], rax
# marshal arg1 = producer-box slot [r12+96] -> [r12+136]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 136], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 144], rax
  .section .rodata
  .Lbynamefn12: .string "complex"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 120]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 104], rax
 mov qword ptr [r12 + 112], rdx
 cmp eax, 99
 je snoch0_n3_α
 jmp snoch0_n2_α
snoch0_n1_β:
 jmp snoch0_n3_α
snoch0_n2_α:
bb7_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 104]
 mov rcx, qword ptr [r12 + 112]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rcx
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n3_α
snoch0_n3_α:
# IR_VAR gva
bb8_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 152], rax
 mov qword ptr [r12 + 160], rdx
 jmp xgvarg14_done
 xgvarg14_β:
 jmp snoch0_n5_α
xgvarg14_done:
bb9_α:
# BOX IR_CALL real(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+152] -> [r12+184]
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 184], rax
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 192], rax
  .section .rodata
  .Lrkfn18: .string "real"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn18]
 lea rsi, [r12 + 184]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 168], rax
 mov qword ptr [r12 + 176], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n5_α
snoch0_n4_α:
bb10_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 176]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n5_α
snoch0_n5_α:
# IR_VAR gva
bb11_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 200], rax
 mov qword ptr [r12 + 208], rdx
 jmp xgvarg20_done
 xgvarg20_β:
 jmp flat_γ
xgvarg20_done:
bb12_α:
# BOX IR_CALL imag(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+200] -> [r12+232]
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 232], rax
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 240], rax
  .section .rodata
  .Lbynamefn24: .string "imag"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn24]
 lea rsi, [r12 + 232]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 216], rax
 mov qword ptr [r12 + 224], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n6_α
snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
bb13_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 224]
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
