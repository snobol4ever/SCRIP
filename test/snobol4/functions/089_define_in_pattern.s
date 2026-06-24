  .intel_syntax noprefix
  .text
upcase_α:
#=======================================================================================================================
    .global upcase_α
    .global upcase_β
    .global upcase_γ
    .global upcase_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
upcase_α_body:
snoch0_n0_α:
# IR_VAR
bb1_α:
 mov rdi, qword ptr [rip + .Lx3_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n2_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "s"
xgvarg1_done:
# IR_KEYWORD_read
bb2_α:
 mov rdi, qword ptr [rip + .Lx5_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xgvarg4_done
 xgvarg4_β:
 jmp snoch0_n2_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "LCASE"
xgvarg4_done:
# IR_KEYWORD_read
bb3_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp xgvarg6_done
 xgvarg6_β:
 jmp snoch0_n2_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "UCASE"
xgvarg6_done:
bb4_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn9: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn9]
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
bb5_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 64]
 mov rdx, qword ptr [r12 + 72]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
snoch0_n2_β:
jmp upcase_γ
jmp upcase_γ
upcase_β:
jmp upcase_ω
upcase_γ:
mov eax, 1
xor edx, edx
pop r12
ret
upcase_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
  .section .rodata
  .Lpn0: .string "upcase"
  .Lpp0_0: .string "s"
  .Lpnames0:
  .quad .Lpp0_0
  .quad 0
  .section .text
  .intel_syntax noprefix
proc_startup:
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  lea rdi, [rip + .Lpn0]
  lea rsi, [rip + .Lpnames0]
  mov edx, 1
  call rt_proc_register@PLT
  lea rdi, [rip + .Lpn0]
  lea rsi, [rip + upcase_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lpn0]
  mov esi, 128
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "upcase"
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
  call proc_startup
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
snoch11_n0_α:
bb6_α:
# IR_CALL_DEFINE
 lea rdi, [rip + .S1]
 call rt_proc_define@PLT
 jmp snoch11_n1_α
 snoch11_n0_β:
 jmp snoch11_n1_α
snoch11_n1_α:
# IR_LIT_S
bb7_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg13_done
 xgvarg13_β:
 jmp snoch11_n3_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "hello"
xgvarg13_done:
bb8_α:
# BOX IR_CALL upcase(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lprocfn16: .string "upcase"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn16]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch11_n3_α
 jmp snoch11_n2_α
snoch11_n1_β:
 jmp snoch11_n3_α
snoch11_n2_α:
bb9_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S2]
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 call rt_gvar_assign_descr@PLT
 jmp snoch11_n3_α
 snoch11_n2_β:
 jmp snoch11_n3_α
snoch11_n3_α:
# IR_LIT_S
bb10_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg18_done
 xgvarg18_β:
 jmp flat_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "world"
xgvarg18_done:
bb11_α:
# BOX IR_CALL upcase(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+96]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lprocfn21: .string "upcase"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn21]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch11_n4_α
snoch11_n3_β:
 jmp flat_γ
snoch11_n4_α:
bb12_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S2]
 mov rsi, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch11_n4_β:
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
.S0: .string "upcase"
.S1: .string "upcase(s)"
.S2: .string "OUTPUT"
.text
