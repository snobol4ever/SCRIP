  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "P"
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
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rcx
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n3_α
snoch0_n3_α:
# IR_VAR gva
bb7_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xgvarg12_done
 xgvarg12_β:
 jmp snoch0_n5_α
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
# IR_VAR gva
bb10_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xgvarg18_done
 xgvarg18_β:
 jmp snoch0_n7_α
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
# IR_LIT_I
bb13_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 248], rax
 jmp xiset24_val_done
 xiset24_val_β:
 jmp snoch0_n8_α
.Lx25_0:
 .quad 99
xiset24_val_done:
# IR_LIT_S
bb14_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 264], rax
 jmp xiset24_key_done
 xiset24_key_β:
 jmp snoch0_n8_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "x"
xiset24_key_done:
# IR_VAR gva
bb15_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xiset24_base_done
 xiset24_base_β:
 jmp snoch0_n8_α
xiset24_base_done:
bb16_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 272]
 cmp eax, 4
 jne .Lx30_0
 mov rax, qword ptr [r12 + 256]
 cmp eax, 6
 jne .Lx30_0
 mov rsi, qword ptr [r12 + 280]
 mov rcx, qword ptr [r12 + 264]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx30_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx30_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n8_α
.Lx30_0:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 mov r8, qword ptr [r12 + 240]
 mov r9, qword ptr [r12 + 248]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n8_α
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n8_α
snoch0_n8_α:
# IR_VAR gva
bb17_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xgvarg31_done
 xgvarg31_β:
 jmp flat_γ
xgvarg31_done:
bb18_α:
# BOX IR_CALL x(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+288] -> [r12+320]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lbynamefn35: .string "x"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 320]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n9_α
snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
bb19_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 304]
 mov rdx, qword ptr [r12 + 312]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch0_n9_β:
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
