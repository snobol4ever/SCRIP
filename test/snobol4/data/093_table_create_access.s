  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "T"
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
bb1_α:
# BOX IR_CALL TABLE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn2: .string "TABLE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn2]
 lea rsi, [r12 + 0]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb2_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 0]
 mov rcx, qword ptr [r12 + 8]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rcx
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_LIT_S
bb3_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 24], rax
 jmp xiset4_val_done
 xiset4_val_β:
 jmp snoch0_n3_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "Alice"
xiset4_val_done:
# IR_LIT_S
bb4_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 40], rax
 jmp xiset4_key_done
 xiset4_key_β:
 jmp snoch0_n3_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "name"
xiset4_key_done:
# IR_VAR gva
bb5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp xiset4_base_done
 xiset4_base_β:
 jmp snoch0_n3_α
xiset4_base_done:
bb6_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 48]
 cmp eax, 4
 jne .Lx10_0
 mov rax, qword ptr [r12 + 32]
 cmp eax, 6
 jne .Lx10_0
 mov rsi, qword ptr [r12 + 56]
 mov rcx, qword ptr [r12 + 40]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx10_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx10_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 16]
 mov rdx, qword ptr [r12 + 24]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n3_α
.Lx10_0:
 mov rdi, qword ptr [r12 + 48]
 mov rsi, qword ptr [r12 + 56]
 mov rdx, qword ptr [r12 + 32]
 mov rcx, qword ptr [r12 + 40]
 mov r8, qword ptr [r12 + 16]
 mov r9, qword ptr [r12 + 24]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n3_α
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n3_α
snoch0_n3_α:
# IR_LIT_I
bb7_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 72], rax
 jmp xiset11_val_done
 xiset11_val_β:
 jmp snoch0_n4_α
.Lx12_0:
 .quad 30
xiset11_val_done:
# IR_LIT_S
bb8_α:
 mov qword ptr [r12 + 80], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 88], rax
 jmp xiset11_key_done
 xiset11_key_β:
 jmp snoch0_n4_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "age"
xiset11_key_done:
# IR_VAR gva
bb9_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xiset11_base_done
 xiset11_base_β:
 jmp snoch0_n4_α
xiset11_base_done:
bb10_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 96]
 cmp eax, 4
 jne .Lx17_0
 mov rax, qword ptr [r12 + 80]
 cmp eax, 6
 jne .Lx17_0
 mov rsi, qword ptr [r12 + 104]
 mov rcx, qword ptr [r12 + 88]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx17_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx17_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 64]
 mov rdx, qword ptr [r12 + 72]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n4_α
.Lx17_0:
 mov rdi, qword ptr [r12 + 96]
 mov rsi, qword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 80]
 mov rcx, qword ptr [r12 + 88]
 mov r8, qword ptr [r12 + 64]
 mov r9, qword ptr [r12 + 72]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n4_α
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_LIT_S
bb11_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 120], rax
 jmp xiset18_val_done
 xiset18_val_β:
 jmp snoch0_n5_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "SNOBOL4"
xiset18_val_done:
# IR_LIT_S
bb12_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 136], rax
 jmp xiset18_key_done
 xiset18_key_β:
 jmp snoch0_n5_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "lang"
xiset18_key_done:
# IR_VAR gva
bb13_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xiset18_base_done
 xiset18_base_β:
 jmp snoch0_n5_α
xiset18_base_done:
bb14_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 144]
 cmp eax, 4
 jne .Lx24_0
 mov rax, qword ptr [r12 + 128]
 cmp eax, 6
 jne .Lx24_0
 mov rsi, qword ptr [r12 + 152]
 mov rcx, qword ptr [r12 + 136]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx24_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx24_0
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
 jmp snoch0_n5_α
.Lx24_0:
 mov rdi, qword ptr [r12 + 144]
 mov rsi, qword ptr [r12 + 152]
 mov rdx, qword ptr [r12 + 128]
 mov rcx, qword ptr [r12 + 136]
 mov r8, qword ptr [r12 + 112]
 mov r9, qword ptr [r12 + 120]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n5_α
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n5_α
snoch0_n5_α:
bb15_α:
# IR_IDX: string-literal key — table-only, subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 1
 lea rcx, [rip + .S1]
 call subscript_get@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n7_α
snoch0_n6_α:
bb16_α:
# IR_ASSIGN
 lea rdi, [rip + .S2]
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n7_α
snoch0_n7_α:
bb17_α:
# IR_IDX: string-literal key — table-only, subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 1
 lea rcx, [rip + .S3]
 call subscript_get@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n9_α
snoch0_n8_α:
bb18_α:
# IR_ASSIGN
 lea rdi, [rip + .S2]
 mov rsi, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n9_α
 snoch0_n8_β:
 jmp snoch0_n9_α
snoch0_n9_α:
bb19_α:
# IR_IDX: string-literal key — table-only, subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 1
 lea rcx, [rip + .S4]
 call subscript_get@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp snoch0_n10_α
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
bb20_α:
# IR_ASSIGN
 lea rdi, [rip + .S2]
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 call rt_gvar_assign_descr@PLT
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
.S0: .string "T"
.S1: .string "name"
.S2: .string "OUTPUT"
.S3: .string "age"
.S4: .string "lang"
.text
