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
xargsub2_n0_α:
# IR_VAR
bb1_α:
 mov rdi, qword ptr [rip + .Lx4_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xargsub2_n1_α
 xargsub2_n0_β:
 jmp snoch0_n2_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "eq"
xargsub2_n1_α:
# IR_VAR
bb2_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xunop5_arg_done
 xunop5_arg_β:
 jmp snoch0_n2_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "eq"
xunop5_arg_done:
jmp xgvarg1_done
xargsub2_n1_β:
jmp snoch0_n2_α
xgvarg1_done:
# IR_LIT_I
bb3_α:
 mov qword ptr [r12 + 48], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 56], rax
 jmp xgvarg8_done
 xgvarg8_β:
 jmp snoch0_n2_α
.Lx9_0:
 .quad 1
xgvarg8_done:
# IR_LIT_I
bb4_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg10_done
 xgvarg10_β:
 jmp snoch0_n2_α
.Lx11_0:
 .quad 2
xgvarg10_done:
bb5_α:
# BOX IR_CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+96]
   lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
# marshal arg1 = producer-box slot [r12+48] -> [r12+112]
 mov rax, qword ptr [r12 + 48]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 56]
 mov qword ptr [r12 + 120], rax
# marshal arg2 = producer-box slot [r12+64] -> [r12+128]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn13: .string "apply"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 96]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
# IR_LIT_scalar
bb6_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
xargsub16_n0_α:
# IR_VAR
bb7_α:
 mov rdi, qword ptr [rip + .Lx18_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xargsub16_n1_α
 xargsub16_n0_β:
 jmp snoch0_n5_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "eq"
xargsub16_n1_α:
# IR_VAR
bb8_α:
 mov rdi, qword ptr [rip + .Lx21_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xunop19_arg_done
 xunop19_arg_β:
 jmp snoch0_n5_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "eq"
xunop19_arg_done:
jmp xgvarg15_done
xargsub16_n1_β:
jmp snoch0_n5_α
xgvarg15_done:
# IR_LIT_I
bb9_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 184], rax
 jmp xgvarg22_done
 xgvarg22_β:
 jmp snoch0_n5_α
.Lx23_0:
 .quad 1
xgvarg22_done:
# IR_LIT_I
bb10_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 200], rax
 jmp xgvarg24_done
 xgvarg24_β:
 jmp snoch0_n5_α
.Lx25_0:
 .quad 1
xgvarg24_done:
bb11_α:
# BOX IR_CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+224]
   lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
# marshal arg1 = producer-box slot [r12+176] -> [r12+240]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 248], rax
# marshal arg2 = producer-box slot [r12+192] -> [r12+256]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 264], rax
  .section .rodata
  .Lbynamefn27: .string "apply"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 224]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
snoch0_n2_β:
 jmp snoch0_n5_α
snoch0_n3_α:
bb12_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
xargsub31_n0_α:
# IR_VAR
bb13_α:
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xargsub31_n1_α
 xargsub31_n0_β:
 jmp snoch0_n7_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "trim"
xargsub31_n1_α:
# IR_VAR
bb14_α:
 mov rdi, qword ptr [rip + .Lx36_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xunop34_arg_done
 xunop34_arg_β:
 jmp snoch0_n7_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "trim"
xunop34_arg_done:
jmp xgvarg30_done
xargsub31_n1_β:
jmp snoch0_n7_α
xgvarg30_done:
# IR_LIT_S
bb15_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 312], rax
 jmp xgvarg37_done
 xgvarg37_β:
 jmp snoch0_n7_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "abc   "
xgvarg37_done:
bb16_α:
# BOX IR_CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+336]
   lea rdi, [rip + .S3]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
# marshal arg1 = producer-box slot [r12+304] -> [r12+352]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 360], rax
  .section .rodata
  .Lbynamefn40: .string "apply"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn40]
 lea rsi, [r12 + 336]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je snoch0_n7_α
 jmp xgvarg29_done
xgvarg29_β:
 jmp snoch0_n7_α
xgvarg29_done:
# IR_LIT_S
bb17_α:
 mov qword ptr [r12 + 368], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 376], rax
 jmp xgvarg41_done
 xgvarg41_β:
 jmp snoch0_n7_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "abc"
xgvarg41_done:
bb18_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+320] -> [r12+400]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 408], rax
# marshal arg1 = producer-box slot [r12+368] -> [r12+416]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 424], rax
  .section .rodata
  .Lbynamefn44: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn44]
 lea rsi, [r12 + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je snoch0_n7_α
 jmp snoch0_n6_α
snoch0_n4_β:
 jmp snoch0_n7_α
snoch0_n5_α:
# IR_LIT_scalar
bb19_α:
 jmp snoch0_n8_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
# IR_LIT_scalar
bb20_α:
 jmp snoch0_n9_α
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_scalar
bb21_α:
 jmp snoch0_n10_α
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
bb22_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
bb23_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
bb24_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S6]
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
.S0: .string "eq"
.S1: .string "output"
.S2: .string "FAIL 1018/001: apply(.eq,1,2) should fail"
.S3: .string "trim"
.S4: .string "FAIL 1018/002: apply(.eq,1,1) should succeed"
.S5: .string "FAIL 1018/003: apply(.trim,...)"
.S6: .string "PASS 1018_apply (3/3)"
.text
