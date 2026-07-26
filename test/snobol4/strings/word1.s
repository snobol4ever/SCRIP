  .intel_syntax noprefix
  .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 208
  mov [rsp + 184], rcx
  mov [rsp + 192], rdx
  mov [rsp + 200], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 80], 0
  mov qword ptr [rsp + 88], 0
  mov qword ptr [rsp + 144], 0
  mov qword ptr [rsp + 152], 0
  mov qword ptr [rsp + 160], 0
  mov qword ptr [rsp + 168], rsp
mov qword ptr [rbp + 176], r8
mov dword ptr [rbp + 168], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [rbp + 144], rax
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
 jg proc_PAT$0_ω
 movsxd rcx, r14d
 mov edx, dword ptr [r13+rcx]
 cmp edx, 1701344288
 jne proc_PAT$0_ω
 movzx eax, byte ptr [r13+rcx+4]
 cmp eax, 32
 jne proc_PAT$0_ω
 add r14d, 5
 jmp xchain0_n2_α
 xchain0_n1_β:
 sub r14d, 5
 jmp proc_PAT$0_ω
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n2_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n5_α
 xchain0_n2_β:
 add rsp, 16
 jmp xchain0_n1_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n3_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S0]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n4_α
 xchain0_n3_β:
 sub r12, 24
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
 jmp proc_PAT$0_γ
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
 add rsp, 16
 jmp xchain0_n1_β
# IR_MATCH_LIT
 xchain0_n6_α:
 mov eax, r14d
 add eax, 4
 cmp eax, r15d
 jg xchain0_n4_af
 movsxd rcx, r14d
 mov edx, dword ptr [r13+rcx]
 cmp edx, 543584032
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
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 32
 jne xchain0_n4_af
 movzx eax, byte ptr [r13+rcx+1]
 cmp eax, 97
 jne xchain0_n4_af
 movzx eax, byte ptr [r13+rcx+2]
 cmp eax, 32
 jne xchain0_n4_af
 add r14d, 3
 jmp xchain0_n4_s1
 xchain0_n7_β:
 sub r14d, 3
 jmp xchain0_n4_af
proc_PAT$0_scanhit:
cmp qword ptr [rbp + 176], 1
jne 7f
mov ecx, dword ptr [rbp + 168]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$0_γ
proc_PAT$0_scanfail:
cmp qword ptr [rbp + 176], 1
jne 8f
mov eax, dword ptr [rbp + 168]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 168], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$0_attempt
8:
jmp proc_PAT$0_ω
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 144]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp + 184]
mov rbp, [rbp + 200]
jmp rax
proc_PAT$0_ω:
mov rax, [rbp + 192]
lea rsp, [rbp + 208]
mov rbp, [rbp + 200]
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
  mov esi, 176
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_zstatic@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
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
  mov qword ptr [rsp + 392], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#       PAT      =  " the " ARB . OUTPUT (" of " | " a ")
# IR_LIT_STRING
 xchain17_n0_α:
 mov qword ptr [rbp + 160], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain17_n1_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "PAT$0"
 xchain17_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rbp + 160]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 168]
 mov qword ptr [rbp + 136], rax
  .section .rodata
  .Lrkfn20: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn20]
 lea rsi, [rbp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 cmp eax, 99
 je xchain17_n3_α
 jmp xchain17_n2_α
 xchain17_n1_β:
 jmp xchain17_n3_α
# IR_ASSIGN gva
 xchain17_n2_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain17_n3_α
# LOOP  LINE     =  INPUT                            :F(END)
# IR_VAR
 xchain17_n3_α:
 mov rdi, qword ptr [rip + .Lx22_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 jmp xchain17_n4_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain17_n4_α:
 mov rax, qword ptr [rbp + 208]
 mov rdx, qword ptr [rbp + 216]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 192], rax
 mov qword ptr [rbp + 200], rdx
 jmp xchain17_n5_α
#       LINE     ?  PAT                              :(LOOP)
# IR_VAR
 xchain17_n5_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 jmp xchain17_n6_α
# IR_MATCH_HEAD
 xchain17_n6_α:
 mov qword ptr [rbp + 296], rbp
 mov rdi, qword ptr [rbp + 352]
 mov rsi, qword ptr [rbp + 360]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 288], r12
 mov qword ptr [rbp + 272], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 264], rax
 mov dword ptr [rbp + 256], 0
.Lx26_0:
 mov r14d, dword ptr [rbp + 256]
 jmp xchain17_n7_α
 xchain17_n6_β:
 add dword ptr [rbp + 256], 1
 mov eax, dword ptr [rbp + 256]
 cmp eax, r15d
 jg .Lx26_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx26_1
 jmp .Lx26_0
.Lx26_1:
 mov rax, qword ptr [rbp + 264]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 272]
 mov r12, qword ptr [rbp + 288]
 mov rbp, qword ptr [rbp + 296]
 jmp xchain17_n3_α
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain17_n7_α:
 lea rsi, [rip + g_sno_defer_cells+0]
 mov rax, qword ptr [rsi + 0]
 test rax, rax
 jne .Lx27_11
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 cmp eax, 3
 jne .Lx27_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx27_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx27_10
.Lx27_9:
 xor eax, eax
.Lx27_10:
 lea rsi, [rip + g_sno_defer_cells+0]
 mov qword ptr [rsi + 0], rax
.Lx27_11:
 test rax, rax
 jz .Lx27_0
 mov r8d, 1
 lea rcx, [rip + .Lx27_4]
 lea rdx, [rip + .Lx27_5]
 jmp rax
.Lx27_4:
 lea rcx, [rip + g_scan_hit_start]
 mov rax, qword ptr [rcx]
 mov dword ptr [rbp + 256], eax
 jmp xchain17_n8_α
.Lx27_5:
 jmp xchain17_n6_β
.Lx27_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
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
 sub rsp, 8
 push rax
 jmp xchain17_n8_α
.Lx27_6:
 add rsp, 16
 jmp xchain17_n6_β
 xchain17_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_RELEASE
 xchain17_n8_α:
 mov rax, qword ptr [rbp + 264]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 272]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 320]
 mov rsi, r12
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
 mov r12, qword ptr [rbp + 288]
 mov rbp, qword ptr [rbp + 296]
 jmp xchain17_n3_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 392]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 392]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
.section .rodata
.S0: .string "OUTPUT"
.S1: .string "PAT"
.text
