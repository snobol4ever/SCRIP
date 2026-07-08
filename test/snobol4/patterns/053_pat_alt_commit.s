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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 80], rax
 pop rsi
proc_PAT$0_α_body:
# IR_MATCH_ALT_SAVE
 xchain0_n0_α:
 mov dword ptr [r12 + 16], r14d
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp qword ptr [r12 + 24]
# IR_MATCH_LIT
 xchain0_n1_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n2_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n2_β
 add r14d, 1
 jmp xchain0_n2_α
 xchain0_n1_β:
 sub r14d, 1
 jmp xchain0_n2_β
# IR_MATCH_ALT_JOIN
 xchain0_n2_α:
 lea rax, [rip + .Lx6_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$0_γ
 xchain0_n2_β:
.Lx6_0:
 mov r14d, dword ptr [r12 + 16]
 jmp xchain0_n3_α
# IR_MATCH_LIT
 xchain0_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n4_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n4_β
 add r14d, 1
 jmp xchain0_n4_α
 xchain0_n3_β:
 sub r14d, 1
 jmp xchain0_n4_β
# IR_MATCH_ALT_JOIN
 xchain0_n4_α:
 lea rax, [rip + .Lx10_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$0_γ
 xchain0_n4_β:
.Lx10_0:
 mov r14d, dword ptr [r12 + 16]
 jmp xchain0_n5_α
# IR_MATCH_LIT
 xchain0_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n6_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S2]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n6_β
 add r14d, 1
 jmp xchain0_n6_α
 xchain0_n5_β:
 sub r14d, 1
 jmp xchain0_n6_β
# IR_MATCH_ALT_JOIN
 xchain0_n6_α:
 lea rax, [rip + .Lx14_0]
 mov qword ptr [r12 + 24], rax
 jmp proc_PAT$0_γ
 xchain0_n6_β:
.Lx14_0:
 mov r14d, dword ptr [r12 + 16]
 jmp proc_PAT$0_ω
proc_PAT$0_β:
jmp proc_PAT$0_ω
proc_PAT$0_γ:
mov eax, 1
xor edx, edx
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 80]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
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
  .Lgvan0: .string "P"
  .Lgvan1: .string "X"
  .Lgvan2: .string "V"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .bss
  .align 16
__gva: .space 48, 0
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
  mov edx, 3
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 240], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain15_n0_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain15_n1_α
 xchain15_n0_β:
 jmp xchain15_n3_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "PAT$0"
 xchain15_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+64] -> [r12+48]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn18: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn18]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je xchain15_n3_α
 jmp xchain15_n2_α
 xchain15_n1_β:
 jmp xchain15_n3_α
# IR_ASSIGN gva
 xchain15_n2_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain15_n3_α
 xchain15_n2_β:
 jmp xchain15_n3_α
# IR_LIT_STRING
 xchain15_n3_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain15_n4_α
 xchain15_n3_β:
 jmp xchain15_n5_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "b"
# IR_ASSIGN gva
 xchain15_n4_α:
 mov rax, qword ptr [r12 + 96]
 mov rdx, qword ptr [r12 + 104]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain15_n5_α
 xchain15_n4_β:
 jmp xchain15_n5_α
# IR_VAR gva
 xchain15_n5_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain15_n6_α
 xchain15_n5_β:
 jmp xchain15_n7_α
# IR_MATCH_HEAD
 xchain15_n6_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 120], rax
 call rt_zls2_mark@PLT
 mov qword ptr [r12 + 128], rax
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 112], 0
.Lx24_0:
 mov r14d, dword ptr [r12 + 112]
 jmp xchain15_n8_α
 xchain15_n6_β:
 add dword ptr [r12 + 112], 1
 mov eax, dword ptr [r12 + 112]
 cmp eax, r15d
 jg .Lx24_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx24_1
 jmp .Lx24_0
.Lx24_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 120]
 call rt_zls_release_to@PLT
 mov rdi, qword ptr [r12 + 128]
 call rt_zls2_release_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain15_n7_α
# IR_LIT_STRING
 xchain15_n7_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain15_n9_α
 xchain15_n7_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "no match"
# IR_MATCH_CAPTURE_SAVE push
 xchain15_n8_α:
 lea rdi, [r12 + 144]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain15_n10_α
 xchain15_n8_β:
 lea rdi, [r12 + 144]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain15_n6_β
# IR_ASSIGN global
 xchain15_n9_α:
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 mov rdi, qword ptr [rip + .Lx28_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp main_γ
 xchain15_n9_β:
 jmp main_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "OUTPUT"
# IR_MATCH_DEFER
 xchain15_n10_α:
 lea rdi, [rip + .S3]
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
 jmp xchain15_n8_β
.Lx29_1:
 jmp xchain15_n11_α
.Lx29_0:
 lea rdi, [rip + .S3]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain15_n8_β
 mov r14d, eax
 jmp xchain15_n11_α
 xchain15_n10_β:
 jmp xchain15_n8_β
# IR_MATCH_CAPTURE_COND
 xchain15_n11_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 144]
 call rt_cap_top@PLT
 lea rdi, [rip + .S4]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain15_n12_α
 xchain15_n11_β:
 jmp xchain15_n8_β
# IR_MATCH_RELEASE
 xchain15_n12_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 120]
 call rt_zls_release_to@PLT
 mov rdi, qword ptr [r12 + 128]
 call rt_zls2_release_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain15_n13_α
# IR_VAR gva
 xchain15_n13_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain15_n14_α
 xchain15_n13_β:
 jmp main_γ
# IR_ASSIGN global
 xchain15_n14_α:
 mov rsi, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov rdi, qword ptr [rip + .Lx35_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain15_n14_β:
 jmp main_γ
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 240]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
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
.S0: .string "a"
.S1: .string "b"
.S2: .string "c"
.S3: .string "P"
.S4: .string "V"
.text
