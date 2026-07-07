  .intel_syntax noprefix
  .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
push r12
  mov r12, rdi
proc_PAT$0_α_body:
xchain0_n0_α:
# IR_MATCH_LIT
bb1_α:
 mov eax, r14d
 add eax, 5
 cmp eax, r15d
 jg proc_PAT$0_ω
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov edx, 5
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne proc_PAT$0_ω
 add r14d, 5
 jmp xchain0_n1_α
 xchain0_n0_β:
 sub r14d, 5
 jmp proc_PAT$0_ω
xchain0_n1_α:
# IR_MATCH_CAPTURE_SAVE push
bb2_α:
 lea rdi, [r12 + 64]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n2_α
 xchain0_n1_β:
 lea rdi, [r12 + 64]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$0_ω
xchain0_n2_α:
# IR_MATCH_ARB
bb3_α:
 mov dword ptr [r12 + 80], 0
 mov eax, r14d
 mov dword ptr [r12 + 84], eax
 jmp xchain0_n3_α
 xchain0_n2_β:
 add dword ptr [r12 + 80], 1
 mov eax, dword ptr [r12 + 84]
 add eax, dword ptr [r12 + 80]
 cmp eax, r15d
 jg .Lx6_0
 mov r14d, eax
 jmp xchain0_n3_α
.Lx6_0:
 mov r14d, dword ptr [r12 + 84]
 jmp xchain0_n1_β
xchain0_n3_α:
# IR_MATCH_CAPTURE_COND
bb4_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 64]
 call rt_cap_top@PLT
 lea rdi, [rip + .S1]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n2_β
xchain0_n4_α:
# IR_MATCH_ALT_SAVE
bb5_α:
 mov dword ptr [r12 + 16], r14d
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp qword ptr [r12 + 24]
xchain0_n5_α:
# IR_MATCH_LIT
bb6_α:
 mov eax, r14d
 add eax, 4
 cmp eax, r15d
 jg xchain0_n6_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S2]
 mov edx, 4
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n6_β
 add r14d, 4
 jmp xchain0_n6_α
 xchain0_n5_β:
 sub r14d, 4
 jmp xchain0_n6_β
xchain0_n6_α:
# IR_MATCH_ALT_JOIN
bb7_α:
 lea rax, [rip + .Lx14_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$0_γ
 xchain0_n6_β:
.Lx14_0:
 mov r14d, dword ptr [r12 + 16]
 jmp xchain0_n7_α
xchain0_n7_α:
# IR_MATCH_LIT
bb8_α:
 mov eax, r14d
 add eax, 3
 cmp eax, r15d
 jg xchain0_n8_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S3]
 mov edx, 3
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n8_β
 add r14d, 3
 jmp xchain0_n8_α
 xchain0_n7_β:
 sub r14d, 3
 jmp xchain0_n8_β
xchain0_n8_α:
# IR_MATCH_ALT_JOIN
bb9_α:
 lea rax, [rip + .Lx18_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$0_γ
 xchain0_n8_β:
.Lx18_0:
 mov r14d, dword ptr [r12 + 16]
 jmp proc_PAT$0_ω
proc_PAT$0_β:
jmp proc_PAT$0_ω
proc_PAT$0_γ:
mov eax, 1
xor edx, edx
pop r12
ret
proc_PAT$0_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
proc_startup:
  push rbp
  mov rbp, rsp
  .section .rodata
  .Lstartup_pname0: .string "PAT$0"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_PAT$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "PAT"
  .Lgvan1: .string "LINE"
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
  push rdi
  push rsi
  call proc_startup
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call main_α
  xor eax, eax
  mov rsp, rbp
  pop rbp
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
push r12
  mov r12, rdi
main_α_body:
xchain19_n0_α:
# IR_LIT_STRING
bb10_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain19_n1_α
 xchain19_n0_β:
 jmp xchain19_n3_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "PAT$0"
xchain19_n1_α:
bb11_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+64] -> [r12+48]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn22: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn22]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je xchain19_n3_α
 jmp xchain19_n2_α
 xchain19_n1_β:
 jmp xchain19_n3_α
xchain19_n2_α:
# IR_ASSIGN gva
bb12_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain19_n3_α
 xchain19_n2_β:
 jmp xchain19_n3_α
xchain19_n3_α:
# IR_VAR
bb13_α:
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain19_n4_α
 xchain19_n3_β:
 jmp main_γ
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "INPUT"
xchain19_n4_α:
# IR_ASSIGN gva
bb14_α:
 mov rax, qword ptr [r12 + 96]
 mov rdx, qword ptr [r12 + 104]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain19_n5_α
 xchain19_n4_β:
 jmp main_γ
xchain19_n5_α:
# IR_VAR gva
bb15_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain19_n6_α
 xchain19_n5_β:
 jmp xchain19_n3_α
xchain19_n6_α:
# IR_MATCH_HEAD
bb16_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 120], rax
 mov rdi, qword ptr [r12 + 128]
 mov rsi, qword ptr [r12 + 136]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 112], 0
.Lx28_0:
 mov r14d, dword ptr [r12 + 112]
 jmp xchain19_n7_α
 xchain19_n6_β:
 add dword ptr [r12 + 112], 1
 mov eax, dword ptr [r12 + 112]
 cmp eax, r15d
 jg .Lx28_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx28_1
 jmp .Lx28_0
.Lx28_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 120]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain19_n3_α
xchain19_n7_α:
# IR_MATCH_DEFER
bb17_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx29_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx29_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain19_n6_β
.Lx29_1:
 jmp xchain19_n8_α
.Lx29_0:
 lea rdi, [rip + .S4]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain19_n6_β
 mov r14d, eax
 jmp xchain19_n8_α
 xchain19_n7_β:
 jmp xchain19_n6_β
xchain19_n8_α:
# IR_MATCH_RELEASE
bb18_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 120]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain19_n3_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
.section .rodata
.S0: .string " the "
.S1: .string "OUTPUT"
.S2: .string " of "
.S3: .string " a "
.S4: .string "PAT"
.text
