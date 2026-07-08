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
 mov qword ptr [r12 + 160], rax
 pop rsi
proc_PAT$0_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_PAT$0_ω
.Lx1_0:
 .quad 0
# IR_MATCH_POS
 xchain0_n1_α:
 mov rax, 0
 cmp r14d, eax
 jne proc_PAT$0_ω
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_PAT$0_ω
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n2_α:
 lea rdi, [r12 + 112]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n3_α
 xchain0_n2_β:
 lea rdi, [r12 + 112]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$0_ω
# IR_MATCH_BREAK
 xchain0_n3_α:
 mov dword ptr [r12 + 128], 0
.Lx6_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 128]
 cmp eax, r15d
 jge xchain0_n2_β
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 jnz .Lx6_1
 add dword ptr [r12 + 128], 1
 jmp .Lx6_0
.Lx6_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 128]
 mov r14d, eax
 jmp xchain0_n4_α
 xchain0_n3_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 128]
 mov r14d, eax
 jmp xchain0_n2_β
# IR_MATCH_CAPTURE_COND
 xchain0_n4_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 112]
 call rt_cap_top@PLT
 lea rdi, [rip + .S1]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n2_β
# IR_MATCH_LIT
 xchain0_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg proc_PAT$0_ω
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne proc_PAT$0_ω
 add r14d, 1
 jmp xchain0_n6_α
 xchain0_n5_β:
 sub r14d, 1
 jmp proc_PAT$0_ω
# IR_MATCH_SPAN
 xchain0_n6_α:
 mov dword ptr [r12 + 96], 0
.Lx12_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 96]
 cmp eax, r15d
 jge .Lx12_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx12_1
 add dword ptr [r12 + 96], 1
 jmp .Lx12_0
.Lx12_1:
 mov eax, dword ptr [r12 + 96]
 test eax, eax
 jle xchain0_n4_β
 mov edx, r14d
 mov dword ptr [r12 + 100], edx
 add edx, eax
 mov r14d, edx
 jmp xchain0_n7_α
 xchain0_n6_β:
 xchain0_n6_β:
 mov r14d, dword ptr [r12 + 100]
 jmp xchain0_n4_β
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n7_α:
 lea rdi, [r12 + 64]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n8_α
 xchain0_n7_β:
 lea rdi, [r12 + 64]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$0_ω
# IR_MATCH_ARB
 xchain0_n8_α:
 lea rdi, [rip + g_zls2_cur]
 mov rax, qword ptr [rdi + 0]
 sub rax, 16
 mov qword ptr [rdi + 0], rax
 mov rcx, qword ptr [r12 + 88]
 mov qword ptr [rax + 0], rcx
 mov qword ptr [r12 + 88], rax
 mov dword ptr [r12 + 80], 0
 mov eax, r14d
 mov dword ptr [r12 + 84], eax
 jmp xchain0_n9_α
 xchain0_n8_β:
 add dword ptr [r12 + 80], 1
 mov eax, dword ptr [r12 + 84]
 add eax, dword ptr [r12 + 80]
 cmp eax, r15d
 jg .Lx16_0
 mov r14d, eax
 jmp xchain0_n9_α
.Lx16_0:
 mov r14d, dword ptr [r12 + 84]
 mov rax, qword ptr [r12 + 88]
 mov rcx, qword ptr [rax + 0]
 mov qword ptr [r12 + 88], rcx
 add rax, 16
 lea rdi, [rip + g_zls2_cur]
 mov qword ptr [rdi + 0], rax
 jmp xchain0_n7_β
# IR_MATCH_CAPTURE_COND
 xchain0_n9_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 64]
 call rt_cap_top@PLT
 lea rdi, [rip + .S2]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n8_β
# IR_MATCH_LIT
 xchain0_n10_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg proc_PAT$0_ω
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne proc_PAT$0_ω
 add r14d, 1
 jmp xchain0_n11_α
 xchain0_n10_β:
 sub r14d, 1
 jmp proc_PAT$0_ω
# IR_MATCH_SPAN
 xchain0_n11_α:
 mov dword ptr [r12 + 48], 0
.Lx22_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 48]
 cmp eax, r15d
 jge .Lx22_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx22_1
 add dword ptr [r12 + 48], 1
 jmp .Lx22_0
.Lx22_1:
 mov eax, dword ptr [r12 + 48]
 test eax, eax
 jle xchain0_n9_β
 mov edx, r14d
 mov dword ptr [r12 + 52], edx
 add edx, eax
 mov r14d, edx
 jmp xchain0_n12_α
 xchain0_n11_β:
 xchain0_n11_β:
 mov r14d, dword ptr [r12 + 52]
 jmp xchain0_n9_β
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n12_α:
 lea rdi, [r12 + 16]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n13_α
 xchain0_n12_β:
 lea rdi, [r12 + 16]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$0_ω
# IR_MATCH_REM
 xchain0_n13_α:
 mov dword ptr [r12 + 32], r14d
 mov r14d, r15d
 jmp xchain0_n14_α
 xchain0_n13_β:
 mov r14d, dword ptr [r12 + 32]
 jmp xchain0_n12_β
# IR_MATCH_CAPTURE_COND
 xchain0_n14_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 16]
 call rt_cap_top@PLT
 lea rdi, [rip + .S4]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp proc_PAT$0_γ
 xchain0_n14_β:
 jmp xchain0_n12_β
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
 mov rdi, qword ptr [r12 + 160]
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
  .Lgvan0: .string "PAT"
  .Lgvan1: .string "LINE"
  .Lgvan2: .string "WHO"
  .Lgvan3: .string "WHAT"
  .Lgvan4: .string "WHEN"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .section .bss
  .align 16
__gva: .space 80, 0
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
  mov edx, 5
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
 mov qword ptr [r12 + 320], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain28_n0_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain28_n1_α
 xchain28_n0_β:
 jmp xchain28_n3_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "PAT$0"
 xchain28_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+64] -> [r12+48]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn31: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn31]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je xchain28_n3_α
 jmp xchain28_n2_α
 xchain28_n1_β:
 jmp xchain28_n3_α
# IR_ASSIGN gva
 xchain28_n2_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain28_n3_α
 xchain28_n2_β:
 jmp xchain28_n3_α
# IR_VAR
 xchain28_n3_α:
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain28_n4_α
 xchain28_n3_β:
 jmp main_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain28_n4_α:
 mov rax, qword ptr [r12 + 96]
 mov rdx, qword ptr [r12 + 104]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain28_n5_α
 xchain28_n4_β:
 jmp main_γ
# IR_VAR gva
 xchain28_n5_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain28_n6_α
 xchain28_n5_β:
 jmp xchain28_n3_α
# IR_MATCH_HEAD
 xchain28_n6_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 120], rax
 lea rdi, [rip + g_zls2_cur]
 mov rax, qword ptr [rdi + 0]
 mov qword ptr [r12 + 128], rax
 mov rdi, qword ptr [r12 + 144]
 mov rsi, qword ptr [r12 + 152]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 112], 0
.Lx37_0:
 mov r14d, dword ptr [r12 + 112]
 jmp xchain28_n7_α
 xchain28_n6_β:
 add dword ptr [r12 + 112], 1
 mov eax, dword ptr [r12 + 112]
 cmp eax, r15d
 jg .Lx37_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx37_1
 jmp .Lx37_0
.Lx37_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 120]
 call rt_zls_release_to@PLT
 lea rdi, [rip + g_zls2_cur]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [rdi + 0], rax
 mov rsp, rbp
 pop rbp
 jmp xchain28_n3_α
# IR_MATCH_DEFER
 xchain28_n7_α:
 lea rdi, [rip + .S5]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx38_0
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
 je .Lx38_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain28_n6_β
.Lx38_1:
 jmp xchain28_n8_α
.Lx38_0:
 lea rdi, [rip + .S5]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain28_n6_β
 mov r14d, eax
 jmp xchain28_n8_α
 xchain28_n7_β:
 jmp xchain28_n6_β
# IR_MATCH_RELEASE
 xchain28_n8_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 120]
 call rt_zls_release_to@PLT
 lea rdi, [rip + g_zls2_cur]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [rdi + 0], rax
 mov rsp, rbp
 pop rbp
 jmp xchain28_n9_α
# IR_VAR gva
 xchain28_n9_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain28_n10_α
 xchain28_n9_β:
 jmp xchain28_n3_α
# IR_LIT_STRING
 xchain28_n10_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain28_n11_α
 xchain28_n10_β:
 jmp xchain28_n3_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string " invented the "
 xchain28_n11_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call str_concat_d@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain28_n12_α
 xchain28_n11_β:
 jmp xchain28_n3_α
# IR_VAR gva
 xchain28_n12_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain28_n13_α
 xchain28_n12_β:
 jmp xchain28_n3_α
 xchain28_n13_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 272]
 mov rcx, qword ptr [r12 + 280]
 call str_concat_d@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain28_n14_α
 xchain28_n13_β:
 jmp xchain28_n3_α
# IR_LIT_STRING
 xchain28_n14_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain28_n15_α
 xchain28_n14_β:
 jmp xchain28_n3_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string " in "
 xchain28_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 288]
 mov rcx, qword ptr [r12 + 296]
 call str_concat_d@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain28_n16_α
 xchain28_n15_β:
 jmp xchain28_n3_α
# IR_VAR gva
 xchain28_n16_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain28_n17_α
 xchain28_n16_β:
 jmp xchain28_n3_α
 xchain28_n17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 304]
 mov rcx, qword ptr [r12 + 312]
 call str_concat_d@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain28_n18_α
 xchain28_n17_β:
 jmp xchain28_n3_α
# IR_ASSIGN global
 xchain28_n18_α:
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 mov rdi, qword ptr [rip + .Lx50_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain28_n3_α
 xchain28_n18_β:
 jmp xchain28_n3_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
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
 mov rdi, qword ptr [r12 + 320]
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
.S0: .string " "
.S1: .string "WHEN"
.S2: .string "WHO"
.S3: .string " :"
.S4: .string "WHAT"
.S5: .string "PAT"
.text
