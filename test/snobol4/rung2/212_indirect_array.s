  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "a"
  .Lgvan1: .string "output"
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
 .quad 3
xgvarg1_done:
bb2_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn4: .string "array"
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
# IR_LIT_S
bb4_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 72], rax
 jmp xiset6_val_done
 xiset6_val_β:
 jmp snoch0_n3_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "x"
xiset6_val_done:
# IR_LIT_I
bb5_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 88], rax
 jmp xiset6_key_done
 xiset6_key_β:
 jmp snoch0_n3_α
.Lx8_0:
 .quad 2
xiset6_key_done:
# IR_VAR gva
bb6_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xiset6_base_done
 xiset6_base_β:
 jmp snoch0_n3_α
xiset6_base_done:
bb7_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 96]
 cmp eax, 4
 jne .Lx12_0
 mov rax, qword ptr [r12 + 80]
 cmp eax, 6
 jne .Lx12_0
 mov rsi, qword ptr [r12 + 104]
 mov rcx, qword ptr [r12 + 88]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx12_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx12_0
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
 jmp snoch0_n3_α
.Lx12_0:
 mov rdi, qword ptr [r12 + 96]
 mov rsi, qword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 80]
 mov rcx, qword ptr [r12 + 88]
 mov r8, qword ptr [r12 + 64]
 mov r9, qword ptr [r12 + 72]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n3_α
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n3_α
snoch0_n3_α:
bb8_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx15_0
 movabs rcx, 2
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx15_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx15_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xgvarg13_done
.Lx15_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 2
 call subscript_get@PLT
 cmp eax, 99
 je snoch0_n5_α
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xgvarg13_done
 xgvarg13_β:
 jmp snoch0_n5_α
xgvarg13_done:
# IR_LIT_S
bb9_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 152], rax
 jmp xgvarg16_done
 xgvarg16_β:
 jmp snoch0_n5_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "x"
xgvarg16_done:
bb10_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+128] -> [r12+176]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 184], rax
# marshal arg1 = producer-box slot [r12+144] -> [r12+192]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lbynamefn19: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
snoch0_n3_β:
 jmp snoch0_n5_α
snoch0_n4_α:
# IR_LIT_scalar
bb11_α:
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
# IR_LIT_scalar
bb12_α:
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
bb13_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
bb14_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
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
.S0: .string "a"
.S1: .string "output"
.S2: .string "FAIL 212/001: $.var<index> indirect array"
.S3: .string "PASS 212_indirect_array (1/1)"
.text
