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
# IR_LIT_I
bb1_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n2_α
.Lx2_0:
 .quad 5
xgvarg1_done:
bb2_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn4: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb3_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_LIT_scalar
bb4_α:
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb5_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S1]
 movabs rsi, 1
 call rt_gvar_assign_int@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_VAR
bb6_α:
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n6_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "I"
snoch0_n5_α:
# IR_VAR
bb7_α:
 mov rdi, qword ptr [rip + .Lx11_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp snoch0_n6_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "I"
snoch0_n6_α:
# IR_VAR
bb8_α:
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp snoch0_n8_α
 snoch0_n6_β:
 jmp snoch0_n9_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "I"
snoch0_n7_α:
bb9_α:
# IR_BINOP_GVAR_ARITH
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S1]
 mov rdx, 2
 call rt_gvar_arith@PLT
 mov qword ptr [r12 + 112], rax
 jmp snoch0_n10_α
 snoch0_n7_β:
 jmp snoch0_n6_α
snoch0_n8_α:
# IR_LIT_scalar
bb10_α:
 jmp snoch0_n11_α
 snoch0_n8_β:
 jmp snoch0_n9_α
snoch0_n9_α:
# IR_VAR
bb11_α:
 mov rdi, qword ptr [rip + .Lx18_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 120], rax
 mov qword ptr [r12 + 128], rdx
 jmp xgvarg16_done
 xgvarg16_β:
 jmp snoch0_n12_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "I"
xgvarg16_done:
# IR_LIT_I
bb12_α:
 mov qword ptr [r12 + 136], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 144], rax
 jmp xgvarg19_done
 xgvarg19_β:
 jmp snoch0_n12_α
.Lx20_0:
 .quad 5
xgvarg19_done:
bb13_α:
# BOX IR_CALL LE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+120] -> [r12+168]
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 168], rax
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 176], rax
# marshal arg1 = producer-box slot [r12+136] -> [r12+184]
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 184], rax
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 192], rax
  .section .rodata
  .Lbynamefn22: .string "LE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn22]
 lea rsi, [r12 + 168]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 152], rax
 mov qword ptr [r12 + 160], rdx
 cmp eax, 99
 je snoch0_n12_α
 jmp snoch0_n4_α
snoch0_n9_β:
 jmp snoch0_n12_α
snoch0_n10_α:
bb14_α:
# IR_BINOP_GVAR_ARITH
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S1]
 mov rdx, 2
 call rt_gvar_arith@PLT
 mov qword ptr [r12 + 200], rax
 jmp xgvidxv23_done
 xgvidxv23_β:
 jmp snoch0_n6_α
xgvidxv23_done:
bb15_α:
# IR_IDX_SET (subscript_set, by-name)
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 lea rdi, [rip + .S1]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 mov rax, qword ptr [r12 + 112]
 movabs rdx, 6
 mov qword ptr [r12 + 112], rdx
 mov qword ptr [r12 + 120], rax
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 mov r8, qword ptr [r12 + 112]
 mov r9, qword ptr [r12 + 120]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n6_α
 jmp snoch0_n6_α
 snoch0_n10_β:
 jmp snoch0_n6_α
snoch0_n11_α:
bb16_α:
# IR_BINOP_GVAR_ARITH
 lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 240], rax
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp snoch0_n9_α
snoch0_n12_α:
# IR_LIT_scalar
bb17_α:
 jmp snoch0_n14_α
 snoch0_n12_β:
 jmp snoch0_n15_α
snoch0_n13_α:
bb18_α:
# IR_ASSIGN
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 240]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n9_α
 snoch0_n13_β:
 jmp snoch0_n9_α
snoch0_n14_α:
bb19_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S1]
 movabs rsi, 1
 call rt_gvar_assign_int@PLT
 jmp snoch0_n15_α
 snoch0_n14_β:
 jmp snoch0_n15_α
snoch0_n15_α:
bb20_α:
# IR_IDX (subscript_get, by-name)
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 248], rax
 mov qword ptr [r12 + 256], rdx
 lea rdi, [rip + .S1]
 call NV_GET_fn@PLT
 mov rcx, rdx
 mov rdx, rax
 mov rdi, qword ptr [r12 + 248]
 mov rsi, qword ptr [r12 + 256]
 call subscript_get@PLT
 mov qword ptr [r12 + 264], rax
 mov qword ptr [r12 + 272], rdx
 jmp snoch0_n16_α
 snoch0_n15_β:
 jmp snoch0_n17_α
snoch0_n16_α:
bb21_α:
# IR_ASSIGN
 lea rdi, [rip + .S2]
 mov rsi, qword ptr [r12 + 264]
 mov rdx, qword ptr [r12 + 272]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n17_α
 snoch0_n16_β:
 jmp snoch0_n17_α
snoch0_n17_α:
# IR_VAR
bb22_α:
 mov rdi, qword ptr [rip + .Lx34_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 280], rax
 mov qword ptr [r12 + 288], rdx
 jmp snoch0_n18_α
 snoch0_n17_β:
 jmp snoch0_n19_α
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "I"
snoch0_n18_α:
# IR_LIT_scalar
bb23_α:
 jmp snoch0_n20_α
 snoch0_n18_β:
 jmp snoch0_n19_α
snoch0_n19_α:
# IR_VAR
bb24_α:
 mov rdi, qword ptr [rip + .Lx38_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 296], rax
 mov qword ptr [r12 + 304], rdx
 jmp xgvarg36_done
 xgvarg36_β:
 jmp flat_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "I"
xgvarg36_done:
# IR_LIT_I
bb25_α:
 mov qword ptr [r12 + 312], 6
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 320], rax
 jmp xgvarg39_done
 xgvarg39_β:
 jmp flat_γ
.Lx40_0:
 .quad 5
xgvarg39_done:
bb26_α:
# BOX IR_CALL LE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+296] -> [r12+344]
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 344], rax
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 352], rax
# marshal arg1 = producer-box slot [r12+312] -> [r12+360]
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 360], rax
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 368], rax
  .section .rodata
  .Lbynamefn42: .string "LE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn42]
 lea rsi, [r12 + 344]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 328], rax
 mov qword ptr [r12 + 336], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n15_α
snoch0_n19_β:
 jmp flat_γ
snoch0_n20_α:
bb27_α:
# IR_BINOP_GVAR_ARITH
 lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 376], rax
 jmp snoch0_n21_α
 snoch0_n20_β:
 jmp snoch0_n19_α
snoch0_n21_α:
bb28_α:
# IR_ASSIGN
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 376]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n19_α
 snoch0_n21_β:
 jmp snoch0_n19_α
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
.S0: .string "A"
.S1: .string "I"
.S2: .string "OUTPUT"
.text
