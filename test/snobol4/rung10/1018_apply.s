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
# IR_LIT_S
bb1_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n2_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "eq"
xgvarg1_done:
# IR_LIT_I
bb2_α:
 mov qword ptr [r12 + 32], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg3_done
 xgvarg3_β:
 jmp snoch0_n2_α
.Lx4_0:
 .quad 1
xgvarg3_done:
# IR_LIT_I
bb3_α:
 mov qword ptr [r12 + 48], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 56], rax
 jmp xgvarg5_done
 xgvarg5_β:
 jmp snoch0_n2_α
.Lx6_0:
 .quad 2
xgvarg5_done:
bb4_α:
# BOX IR_CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+80]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [r12+32] -> [r12+96]
 mov rax, qword ptr [r12 + 32]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 40]
 mov qword ptr [r12 + 104], rax
# marshal arg2 = producer-box slot [r12+48] -> [r12+112]
 mov rax, qword ptr [r12 + 48]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 56]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lbynamefn8: .string "apply"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 80]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
# IR_LIT_scalar
bb5_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
# IR_LIT_S
bb6_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 136], rax
 jmp xgvarg10_done
 xgvarg10_β:
 jmp snoch0_n5_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "eq"
xgvarg10_done:
# IR_LIT_I
bb7_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 152], rax
 jmp xgvarg12_done
 xgvarg12_β:
 jmp snoch0_n5_α
.Lx13_0:
 .quad 1
xgvarg12_done:
# IR_LIT_I
bb8_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg14_done
 xgvarg14_β:
 jmp snoch0_n5_α
.Lx15_0:
 .quad 1
xgvarg14_done:
bb9_α:
# BOX IR_CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+128] -> [r12+192]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 200], rax
# marshal arg1 = producer-box slot [r12+144] -> [r12+208]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 216], rax
# marshal arg2 = producer-box slot [r12+160] -> [r12+224]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn17: .string "apply"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 192]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
snoch0_n2_β:
 jmp snoch0_n5_α
snoch0_n3_α:
bb10_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
# IR_LIT_S
bb11_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 248], rax
 jmp xgvarg20_done
 xgvarg20_β:
 jmp snoch0_n7_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "trim"
xgvarg20_done:
# IR_LIT_S
bb12_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg22_done
 xgvarg22_β:
 jmp snoch0_n7_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "abc   "
xgvarg22_done:
bb13_α:
# BOX IR_CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+240] -> [r12+288]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 296], rax
# marshal arg1 = producer-box slot [r12+256] -> [r12+304]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 312], rax
  .section .rodata
  .Lbynamefn25: .string "apply"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn25]
 lea rsi, [r12 + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je snoch0_n7_α
 jmp xgvarg19_done
xgvarg19_β:
 jmp snoch0_n7_α
xgvarg19_done:
# IR_LIT_S
bb14_α:
 mov qword ptr [r12 + 320], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 328], rax
 jmp xgvarg26_done
 xgvarg26_β:
 jmp snoch0_n7_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "abc"
xgvarg26_done:
bb15_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+272] -> [r12+352]
 mov rax, qword ptr [r12 + 272]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 280]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [r12+320] -> [r12+368]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn29: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn29]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je snoch0_n7_α
 jmp snoch0_n6_α
snoch0_n4_β:
 jmp snoch0_n7_α
snoch0_n5_α:
# IR_LIT_scalar
bb16_α:
 jmp snoch0_n8_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
# IR_LIT_scalar
bb17_α:
 jmp snoch0_n9_α
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_scalar
bb18_α:
 jmp snoch0_n10_α
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
bb19_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
bb20_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
bb21_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S4]
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
.S0: .string "output"
.S1: .string "FAIL 1018/001: apply(.eq,1,2) should fail"
.S2: .string "FAIL 1018/002: apply(.eq,1,1) should succeed"
.S3: .string "FAIL 1018/003: apply(.trim,...)"
.S4: .string "PASS 1018_apply (3/3)"
.text
