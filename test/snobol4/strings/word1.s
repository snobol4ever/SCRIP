  .intel_syntax noprefix
  .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 272
  mov [rsp+8], rcx
  mov [rsp+16], rdx
  mov [rsp+24], r12
  lea r12, [rsp+32]
  mov rdi, r12
  mov ecx, 240
  xor eax, eax
  rep stosb
  mov qword ptr [r12 + 232], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 224], rax
 pop rsi
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [r12 + 208], rax
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n0_α:
 jmp xchain0_n1_α
xchain0_n0_as:
 jmp proc_PAT$0_γ
 xchain0_n0_β:
 jmp xchain0_n4_β
xchain0_n0_af:
 jmp proc_PAT$0_ω
# IR_MATCH_LIT
 xchain0_n1_α:
 mov eax, r14d
 add eax, 5
 cmp eax, r15d
 jg xchain0_n0_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov edx, 5
 call memcmp@PLT
 test eax, eax
 jne xchain0_n0_af
 add r14d, 5
 jmp xchain0_n2_α
 xchain0_n1_β:
 sub r14d, 5
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n2_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n5_α
 xchain0_n2_β:
 add rsp, 16
 jmp xchain0_n1_β
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n3_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S1]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n4_α
 xchain0_n3_β:
 sub rbp, 24
 jmp xchain0_n5_β
# IR_MATCH_ALT_NARY
 xchain0_n4_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 mov dword ptr [rsp + 4], 0
 jmp xchain0_n6_α
xchain0_n4_s0:
 jmp xchain0_n4_as
xchain0_n4_s1:
 jmp xchain0_n4_as
xchain0_n4_as:
 jmp xchain0_n0_as
 xchain0_n4_β:
 mov eax, dword ptr [rsp + 4]
 cmp eax, 0
 je xchain0_n6_β
 jmp xchain0_n7_β
xchain0_n4_af:
 add dword ptr [rsp + 4], 1
 mov r14d, dword ptr [rsp + 0]
 mov eax, dword ptr [rsp + 4]
 cmp eax, 1
 je xchain0_n7_α
 add rsp, 16
 jmp xchain0_n3_β
# IR_MATCH_ARB
 xchain0_n5_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
 mov eax, r14d
 mov dword ptr [rsp + 4], eax
 jmp xchain0_n3_α
 xchain0_n5_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 4]
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx12_0
 mov r14d, eax
 jmp xchain0_n3_α
.Lx12_0:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp xchain0_n2_β
# IR_MATCH_LIT
 xchain0_n6_α:
 mov eax, r14d
 add eax, 4
 cmp eax, r15d
 jg xchain0_n4_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S2]
 mov edx, 4
 call memcmp@PLT
 test eax, eax
 jne xchain0_n4_af
 add r14d, 4
 jmp xchain0_n4_s0
 xchain0_n6_β:
 sub r14d, 4
 jmp xchain0_n4_af
# IR_MATCH_LIT
 xchain0_n7_α:
 mov eax, r14d
 add eax, 3
 cmp eax, r15d
 jg xchain0_n4_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S3]
 mov edx, 3
 call memcmp@PLT
 test eax, eax
 jne xchain0_n4_af
 add r14d, 3
 jmp xchain0_n4_s1
 xchain0_n7_β:
 sub r14d, 3
 jmp xchain0_n4_af
proc_PAT$0_res:
add rsp, 8
pop r12
proc_PAT$0_β:
jmp qword ptr [r12 + 208]
proc_PAT$0_γ:
push r12
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [r12-24]
mov r12, [r12-8]
jmp rax
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 224]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_PAT$0_ω:
mov rax, [r12-16]
lea rsp, [r12 + 240]
mov r12, [r12-8]
jmp rax
proc_startup:
  sub rsp, 8
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
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 240
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "PAT"
  .Lgvan1: .string "LINE"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 2
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 2
  call gva_register@PLT
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 24
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
  sub rsp, 65544
  mov rdi, rsp
  mov ecx, 65544
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 408], rsp
main_α_body:
# IR_LIT_STRING
 xchain17_n0_α:
 mov qword ptr [rsp + 160], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain17_n1_α
 xchain17_n0_β:
 jmp xchain17_n3_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "PAT$0"
 xchain17_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lrkfn20: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn20]
 lea rsi, [rsp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 cmp eax, 99
 je xchain17_n3_α
 jmp xchain17_n2_α
 xchain17_n1_β:
 jmp xchain17_n3_α
# IR_ASSIGN gva
 xchain17_n2_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain17_n3_α
 xchain17_n2_β:
 jmp xchain17_n3_α
# IR_VAR
 xchain17_n3_α:
 mov rdi, qword ptr [rip + .Lx22_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rsp + 208], rax
 mov qword ptr [rsp + 216], rdx
 jmp xchain17_n4_α
 xchain17_n3_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain17_n4_α:
 mov rax, qword ptr [rsp + 208]
 mov rdx, qword ptr [rsp + 216]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 192], rax
 mov qword ptr [rsp + 200], rdx
 jmp xchain17_n5_α
 xchain17_n4_β:
 jmp main_γ
# IR_VAR
 xchain17_n5_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 368], rax
 mov qword ptr [rsp + 376], rdx
 jmp xchain17_n6_α
 xchain17_n5_β:
 jmp xchain17_n3_α
# IR_MATCH_HEAD
 xchain17_n6_α:
 mov r12, rsp
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [r12 + 296], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [r12 + 288], rbp
 mov qword ptr [r12 + 272], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [r12 + 264], rax
 mov dword ptr [r12 + 256], 0
.Lx26_0:
 mov r14d, dword ptr [r12 + 256]
 jmp xchain17_n7_α
 xchain17_n6_β:
 add dword ptr [r12 + 256], 1
 mov eax, dword ptr [r12 + 256]
 cmp eax, r15d
 jg .Lx26_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx26_1
 jmp .Lx26_0
.Lx26_1:
 mov rax, qword ptr [r12 + 264]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [r12 + 272]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 296]
 jmp xchain17_n3_α
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain17_n7_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx27_0
 lea rcx, [rip + g_pat_main_rsp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rsp, qword ptr [rcx + 0]
 lea rcx, [rip + .Lx27_4]
 lea rdx, [rip + .Lx27_5]
 jmp rax
.Lx27_4:
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain17_n8_α
.Lx27_5:
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain17_n6_β
.Lx27_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx27_2:
 test rax, rax
 je .Lx27_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx27_7]
 lea rdx, [rip + .Lx27_8]
 jmp rax
.Lx27_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx27_2
.Lx27_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx27_2
.Lx27_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain17_n6_β
 mov r14d, eax
 lea rax, [rip + .Lx27_6]
 lea rcx, [rip + g_pat_main_rsp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rsp, qword ptr [rcx + 0]
 sub rsp, 8
 push rax
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain17_n8_α
.Lx27_6:
 add rsp, 16
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain17_n6_β
 xchain17_n7_β:
 lea rcx, [rip + g_pat_main_rsp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rsp, qword ptr [rcx + 0]
 jmp qword ptr [rsp]
# IR_MATCH_RELEASE
 xchain17_n8_α:
 mov rax, qword ptr [r12 + 264]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [r12 + 272]
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [rsp + 320]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx29_1:
 test rax, rax
 je .Lx29_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx29_3]
 lea rdx, [rip + .Lx29_4]
 jmp rax
.Lx29_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx29_1
.Lx29_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx29_1
.Lx29_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 296]
 jmp xchain17_n3_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 408]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 408]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
.section .rodata
.S0: .string " the "
.S1: .string "OUTPUT"
.S2: .string " of "
.S3: .string " a "
.S4: .string "PAT"
.text
