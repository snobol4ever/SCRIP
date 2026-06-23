  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
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
 .string "point(x,y)"
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
 .quad 10
xgvarg5_done:
# IR_LIT_I
bb4_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 88], rax
 jmp xgvarg7_done
 xgvarg7_β:
 jmp snoch0_n3_α
.Lx8_0:
 .quad 20
xgvarg7_done:
bb5_α:
# BOX IR_CALL point(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+112]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [r12+80] -> [r12+128]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn10: .string "point"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn10]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je snoch0_n3_α
 jmp snoch0_n2_α
snoch0_n1_β:
 jmp snoch0_n3_α
snoch0_n2_α:
bb6_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 96]
 mov rdx, qword ptr [r12 + 104]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n3_α
snoch0_n3_α:
# IR_VAR
bb7_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xgvarg12_done
 xgvarg12_β:
 jmp snoch0_n5_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "P"
xgvarg12_done:
bb8_α:
# BOX IR_CALL x(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+144] -> [r12+176]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 184], rax
  .section .rodata
  .Lbynamefn16: .string "x"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 176]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
snoch0_n3_β:
 jmp snoch0_n5_α
snoch0_n4_α:
bb9_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n5_α
snoch0_n5_α:
# IR_VAR
bb10_α:
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xgvarg18_done
 xgvarg18_β:
 jmp snoch0_n7_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "P"
xgvarg18_done:
bb11_α:
# BOX IR_CALL y(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+192] -> [r12+224]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn22: .string "y"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn22]
 lea rsi, [r12 + 224]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je snoch0_n7_α
 jmp snoch0_n6_α
snoch0_n5_β:
 jmp snoch0_n7_α
snoch0_n6_α:
bb12_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n7_α
snoch0_n7_α:
# IR_VAR
bb13_α:
 mov rdi, qword ptr [rip + .Lx26_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xgvarg24_done
 xgvarg24_β:
 jmp flat_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "P"
xgvarg24_done:
bb14_α:
# BOX IR_CALL x(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+240] -> [r12+272]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn28: .string "x"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 272]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n8_α
snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
bb15_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch0_n8_β:
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
.S0: .string "P"
.S1: .string "OUTPUT"
.text
