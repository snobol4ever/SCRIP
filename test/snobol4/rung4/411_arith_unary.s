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
xargsub2_n0_α:
# IR_LIT_I
bb1_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 24], rax
 jmp xargsub2_n1_α
 xargsub2_n0_β:
 jmp snoch0_n2_α
.Lx3_0:
 .quad 5
xargsub2_n1_α:
bb2_α:
# IR_UNOP_GVAR_SLOT
 mov rax, 5
 neg rax
 mov qword ptr [r12 + 32], rax
 jmp xgvarg1_done
 xargsub2_n1_β:
 jmp snoch0_n2_α
xgvarg1_done:
xgvarg5_done:
bb3_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+32] -> [r12+56]
 mov rax, qword ptr [r12 + 32]
 mov qword ptr [r12 + 56], rax
 mov rax, qword ptr [r12 + 40]
 mov qword ptr [r12 + 64], rax
# marshal arg1 = inline gvar-arith subexpr -> [r12+72]
 movabs rax, 0
 mov qword ptr [r12 + 88], rax
 mov rcx, 5
 mov rax, qword ptr [r12 + 88]
 sub rax, rcx
 mov qword ptr [r12 + 72], 6
 mov qword ptr [r12 + 80], rax
  .section .rodata
  .Lbynamefn7: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 56]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 40], rax
 mov qword ptr [r12 + 48], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
# IR_LIT_scalar
bb4_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
xargsub10_n0_α:
# IR_LIT_S
bb5_α:
 mov qword ptr [r12 + 104], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 112], rax
 jmp xargsub10_n1_α
 xargsub10_n0_β:
 jmp snoch0_n5_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "4"
xargsub10_n1_α:
bb6_α:
# IR_UNOP_GVAR_SLOT
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg9_done
 xargsub10_n1_β:
 jmp snoch0_n5_α
xgvarg9_done:
# IR_LIT_I
bb7_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 136], rax
 jmp xgvarg13_done
 xgvarg13_β:
 jmp snoch0_n5_α
.Lx14_0:
 .quad 4
xgvarg13_done:
bb8_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+120] -> [r12+160]
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 168], rax
# marshal arg1 = producer-box slot [r12+128] -> [r12+176]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 184], rax
  .section .rodata
  .Lbynamefn16: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 160]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
snoch0_n2_β:
 jmp snoch0_n5_α
snoch0_n3_α:
bb9_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
# IR_LIT_scalar
bb10_α:
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
# IR_LIT_scalar
bb11_α:
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
bb12_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
bb13_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n7_β:
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
.S0: .string "output"
.S1: .string "FAIL 411/001: unary minus"
.S2: .string "FAIL 411/002: unary plus string->int"
.S3: .string "PASS 411_arith_unary (2/2)"
.text
