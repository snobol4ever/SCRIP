  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "N"
  .Lgvan1: .string "S"
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
# IR_LIT_scalar
bb3_α:
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb4_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S1]
 movabs rsi, 0
 call rt_gvar_assign_int@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_VAR
bb5_α:
 mov rdi, qword ptr [rip + .Lx6_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp flat_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "INPUT"
snoch0_n5_α:
bb6_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
# IR_LIT_S
bb7_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg8_done
 xgvarg8_β:
 jmp snoch0_n8_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string " "
xgvarg8_done:
xgvarg10_done:
bb8_α:
# BOX IR_CALL DUPL(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
# marshal arg1 = inline gvar-arith subexpr -> [r12+64]
 movabs rax, 80
 mov qword ptr [r12 + 96], rax
# marshal arg0 = gvar NV_GET -> [r12+128]
   lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
  .section .rodata
  .Lcallfn12: .string "SIZE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcallfn12]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 mov rcx, qword ptr [r12 + 120]
 mov rax, qword ptr [r12 + 96]
 sub rax, rcx
 mov qword ptr [r12 + 80], rax
 mov rcx, 2
 mov rax, qword ptr [r12 + 80]
 cqo
 idiv rcx
 mov qword ptr [r12 + 64], 6
 mov qword ptr [r12 + 72], rax
  .section .rodata
  .Lbynamefn13: .string "DUPL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 48]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
snoch0_n6_β:
 jmp snoch0_n8_α
snoch0_n7_α:
# IR_VAR gva
bb9_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp snoch0_n8_α
snoch0_n8_α:
xgvarg16_done:
# IR_LIT_I
bb10_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg17_done
 xgvarg17_β:
 jmp snoch0_n11_α
.Lx18_0:
 .quad 3
xgvarg17_done:
bb11_α:
# BOX IR_CALL REMDR(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+192]
   lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
 mov qword ptr [r12 + 224], rax
 mov rcx, 1
 mov rax, qword ptr [r12 + 224]
 add rax, rcx
 mov qword ptr [r12 + 192], 6
 mov qword ptr [r12 + 200], rax
# marshal arg1 = producer-box slot [r12+160] -> [r12+208]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lbynamefn20: .string "REMDR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 192]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp snoch0_n10_α
snoch0_n8_β:
 jmp snoch0_n11_α
snoch0_n9_α:
bb12_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, qword ptr [r12 + 32]
 mov rsi, qword ptr [r12 + 40]
 mov rdx, qword ptr [r12 + 144]
 mov rcx, qword ptr [r12 + 152]
 call str_concat_d@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp snoch0_n12_α
 snoch0_n9_β:
 jmp snoch0_n8_α
snoch0_n10_α:
bb13_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rcx
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp snoch0_n11_α
snoch0_n11_α:
# IR_VAR gva
bb14_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xgvarg23_done
 xgvarg23_β:
 jmp snoch0_n4_α
xgvarg23_done:
# IR_LIT_I
bb15_α:
 mov qword ptr [r12 + 272], 6
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 280], rax
 jmp xgvarg26_done
 xgvarg26_β:
 jmp snoch0_n4_α
.Lx27_0:
 .quad 0
xgvarg26_done:
bb16_α:
# BOX IR_CALL EQ(...) inline integer relop [four-port, FAIL->ω]
 mov rdx, qword ptr [rbx + 0]
 cmp edx, 6
 jne .Lx29_0
 mov rax, qword ptr [rbx + 8]
 jmp .Lx29_1
.Lx29_0:
   lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
.Lx29_1:
 mov qword ptr [r12 + 304], rax
 mov rcx, 0
 mov qword ptr [r12 + 288], 0
 mov qword ptr [r12 + 296], 0
 mov rax, qword ptr [r12 + 304]
 cmp rax, rcx
 jne snoch0_n4_α
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp snoch0_n4_α
snoch0_n12_α:
bb17_α:
# IR_ASSIGN
 lea rdi, [rip + .S4]
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n8_α
 snoch0_n12_β:
 jmp snoch0_n8_α
snoch0_n13_α:
bb18_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S4]
 mov rsi, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n4_α
 snoch0_n13_β:
 jmp snoch0_n4_α
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
.S1: .string "N"
.S2: .string "S"
.S3: .string "INPUT"
.S4: .string "OUTPUT"
.text
