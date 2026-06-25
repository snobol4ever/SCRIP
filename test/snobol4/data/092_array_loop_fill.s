  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "A"
  .Lgvan1: .string "I"
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
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 32]
 mov rcx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rcx
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
# IR_VAR gva
bb6_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n6_α
snoch0_n5_α:
# IR_VAR gva
bb7_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
# IR_VAR gva
bb8_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp snoch0_n8_α
 snoch0_n6_β:
 jmp snoch0_n9_α
snoch0_n7_α:
bb9_α:
# IR_BINOP_GVAR_ARITH
 mov rdx, qword ptr [rbx + 16]
 cmp edx, 6
 jne .Lx15_0
 mov rcx, qword ptr [rbx + 24]
 jmp .Lx15_1
.Lx15_0:
 lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
 mov rcx, rax
.Lx15_1:
 mov qword ptr [r12 + 112], rcx
 mov rdx, qword ptr [rbx + 16]
 cmp edx, 6
 jne .Lx15_2
 mov rax, qword ptr [rbx + 24]
 jmp .Lx15_3
.Lx15_2:
 lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
.Lx15_3:
 mov rcx, qword ptr [r12 + 112]
 imul rax, rcx
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
# IR_VAR gva
bb11_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 120], rax
 mov qword ptr [r12 + 128], rdx
 jmp xgvarg17_done
 xgvarg17_β:
 jmp snoch0_n12_α
xgvarg17_done:
# IR_LIT_I
bb12_α:
 mov qword ptr [r12 + 136], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 144], rax
 jmp xgvarg20_done
 xgvarg20_β:
 jmp snoch0_n12_α
.Lx21_0:
 .quad 5
xgvarg20_done:
bb13_α:
# BOX IR_CALL LE(...) inline integer relop [four-port, FAIL->ω]
 mov rdx, qword ptr [rbx + 16]
 cmp edx, 6
 jne .Lx23_0
 mov rax, qword ptr [rbx + 24]
 jmp .Lx23_1
.Lx23_0:
   lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
.Lx23_1:
 mov qword ptr [r12 + 168], rax
 mov rcx, 5
 mov qword ptr [r12 + 152], 0
 mov qword ptr [r12 + 160], 0
 mov rax, qword ptr [r12 + 168]
 cmp rax, rcx
 jg snoch0_n12_α
 jmp snoch0_n4_α
 snoch0_n9_β:
 jmp snoch0_n12_α
snoch0_n10_α:
bb14_α:
# IR_BINOP_GVAR_ARITH
 mov rdx, qword ptr [rbx + 16]
 cmp edx, 6
 jne .Lx26_0
 mov rcx, qword ptr [rbx + 24]
 jmp .Lx26_1
.Lx26_0:
 lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
 mov rcx, rax
.Lx26_1:
 mov qword ptr [r12 + 184], rcx
 mov rdx, qword ptr [rbx + 16]
 cmp edx, 6
 jne .Lx26_2
 mov rax, qword ptr [rbx + 24]
 jmp .Lx26_3
.Lx26_2:
 lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
.Lx26_3:
 mov rcx, qword ptr [r12 + 184]
 imul rax, rcx
 mov qword ptr [r12 + 184], rax
 jmp xgvidxv24_done
 xgvidxv24_β:
 jmp snoch0_n6_α
xgvidxv24_done:
bb15_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 lea rdi, [rip + .S1]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 mov rax, qword ptr [r12 + 112]
 movabs rdx, 6
 mov qword ptr [r12 + 112], rdx
 mov qword ptr [r12 + 120], rax
 mov rax, qword ptr [r12 + 192]
 cmp eax, 4
 jne .Lx28_0
 mov rax, qword ptr [r12 + 208]
 cmp eax, 6
 jne .Lx28_0
 mov rsi, qword ptr [r12 + 200]
 mov rcx, qword ptr [r12 + 216]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx28_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx28_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n6_α
.Lx28_0:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 208]
 mov rcx, qword ptr [r12 + 216]
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
 mov rdx, qword ptr [rbx + 16]
 cmp edx, 6
 jne .Lx30_0
 mov rax, qword ptr [rbx + 24]
 jmp .Lx30_1
.Lx30_0:
 lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
.Lx30_1:
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 224], rax
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
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [rbx + 16], 6
 mov qword ptr [rbx + 24], rax
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
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S1]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 232], rax
 mov qword ptr [r12 + 240], rdx
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx36_0
 mov r8, qword ptr [r12 + 232]
 cmp r8d, 6
 jne .Lx36_0
 mov rcx, qword ptr [r12 + 240]
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx36_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx36_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 248], rax
 mov qword ptr [r12 + 256], rdx
 jmp snoch0_n16_α
.Lx36_0:
 mov rdi, rax
 mov rsi, rdx
 mov rdx, qword ptr [r12 + 232]
 mov rcx, qword ptr [r12 + 240]
 call subscript_get@PLT
 mov qword ptr [r12 + 248], rax
 mov qword ptr [r12 + 256], rdx
 jmp snoch0_n16_α
 snoch0_n15_β:
 jmp snoch0_n17_α
snoch0_n16_α:
bb21_α:
# IR_ASSIGN
 lea rdi, [rip + .S2]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n17_α
 snoch0_n16_β:
 jmp snoch0_n17_α
snoch0_n17_α:
# IR_VAR gva
bb22_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 264], rax
 mov qword ptr [r12 + 272], rdx
 jmp snoch0_n18_α
 snoch0_n17_β:
 jmp snoch0_n19_α
snoch0_n18_α:
# IR_LIT_scalar
bb23_α:
 jmp snoch0_n20_α
 snoch0_n18_β:
 jmp snoch0_n19_α
snoch0_n19_α:
# IR_VAR gva
bb24_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 280], rax
 mov qword ptr [r12 + 288], rdx
 jmp xgvarg41_done
 xgvarg41_β:
 jmp flat_γ
xgvarg41_done:
# IR_LIT_I
bb25_α:
 mov qword ptr [r12 + 296], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 304], rax
 jmp xgvarg44_done
 xgvarg44_β:
 jmp flat_γ
.Lx45_0:
 .quad 5
xgvarg44_done:
bb26_α:
# BOX IR_CALL LE(...) inline integer relop [four-port, FAIL->ω]
 mov rdx, qword ptr [rbx + 16]
 cmp edx, 6
 jne .Lx47_0
 mov rax, qword ptr [rbx + 24]
 jmp .Lx47_1
.Lx47_0:
   lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
.Lx47_1:
 mov qword ptr [r12 + 328], rax
 mov rcx, 5
 mov qword ptr [r12 + 312], 0
 mov qword ptr [r12 + 320], 0
 mov rax, qword ptr [r12 + 328]
 cmp rax, rcx
 jg flat_γ
 jmp snoch0_n15_α
 snoch0_n19_β:
 jmp flat_γ
snoch0_n20_α:
bb27_α:
# IR_BINOP_GVAR_ARITH
 mov rdx, qword ptr [rbx + 16]
 cmp edx, 6
 jne .Lx49_0
 mov rax, qword ptr [rbx + 24]
 jmp .Lx49_1
.Lx49_0:
 lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
.Lx49_1:
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 344], rax
 jmp snoch0_n21_α
 snoch0_n20_β:
 jmp snoch0_n19_α
snoch0_n21_α:
bb28_α:
# IR_ASSIGN
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [rbx + 16], 6
 mov qword ptr [rbx + 24], rax
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
