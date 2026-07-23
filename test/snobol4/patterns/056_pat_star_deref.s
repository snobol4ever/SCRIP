  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "PAT"
  .Lgvan1: .string "X"
  .Lgvan2: .string "V"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 3
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 3
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
  mov qword ptr [rsp + 472], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rbp + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 120], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hello"
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain0_n2_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rbp + 160], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain0_n3_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "say hello world"
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rbp + 160]
 mov rdx, qword ptr [rbp + 168]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 144], rax
 mov qword ptr [rbp + 152], rdx
 jmp xchain0_n4_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 336], rax
 mov qword ptr [rbp + 344], rdx
 jmp xchain0_n5_α
# IR_MATCH_HEAD
 xchain0_n5_α:
 mov qword ptr [rbp + 248], rbp
 mov rdi, qword ptr [rbp + 336]
 mov rsi, qword ptr [rbp + 344]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 240], r12
 mov qword ptr [rbp + 224], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 216], rax
 mov dword ptr [rbp + 208], 0
.Lx7_0:
 mov r14d, dword ptr [rbp + 208]
 jmp xchain0_n7_α
 xchain0_n5_β:
 add dword ptr [rbp + 208], 1
 mov eax, dword ptr [rbp + 208]
 cmp eax, r15d
 jg .Lx7_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx7_1
 jmp .Lx7_0
.Lx7_1:
 mov rax, qword ptr [rbp + 216]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 224]
 mov r12, qword ptr [rbp + 240]
 mov rbp, qword ptr [rbp + 248]
 jmp xchain0_n6_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rbp + 384], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 392], rax
 jmp xchain0_n8_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "fail"
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n7_α:
 lea rdi, [rbp + 288]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain0_n9_α
 xchain0_n7_β:
 lea rdi, [rbp + 288]
 call rt_cap_pop@PLT
 jmp xchain0_n5_β
# IR_ASSIGN global
 xchain0_n8_α:
 mov rsi, qword ptr [rbp + 384]
 mov rdx, qword ptr [rbp + 392]
 mov rdi, qword ptr [rip + .Lx11_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 jmp main_γ
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "OUTPUT"
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain0_n9_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 cmp eax, 3
 jne .Lx12_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx12_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx12_10
.Lx12_9:
 xor eax, eax
.Lx12_10:
 test rax, rax
 jz .Lx12_0
 mov r8d, 0
 lea rcx, [rip + .Lx12_4]
 lea rdx, [rip + .Lx12_5]
 jmp rax
.Lx12_4:
 jmp xchain0_n10_α
.Lx12_5:
 jmp xchain0_n7_β
.Lx12_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx12_2:
 test rax, rax
 je .Lx12_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx12_7]
 lea rdx, [rip + .Lx12_8]
 jmp rax
.Lx12_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx12_2
.Lx12_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx12_2
.Lx12_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain0_n7_β
 mov r14d, eax
 lea rax, [rip + .Lx12_6]
 sub rsp, 8
 push rax
 jmp xchain0_n10_α
.Lx12_6:
 add rsp, 16
 jmp xchain0_n7_β
 xchain0_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n10_α:
 lea rdi, [rbp + 288]
 call rt_cap_top@PLT
 lea rcx, [rip + .S1]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n11_α
 xchain0_n10_β:
 sub r12, 24
 jmp xchain0_n9_β
# IR_MATCH_RELEASE
 xchain0_n11_α:
 mov rax, qword ptr [rbp + 216]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 224]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 272]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx16_1:
 test rax, rax
 je .Lx16_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx16_3]
 lea rdx, [rip + .Lx16_4]
 jmp rax
.Lx16_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx16_1
.Lx16_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx16_1
.Lx16_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 240]
 mov rbp, qword ptr [rbp + 248]
 jmp xchain0_n12_α
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 jmp xchain0_n13_α
# IR_ASSIGN global
 xchain0_n13_α:
 mov rsi, qword ptr [rbp + 432]
 mov rdx, qword ptr [rbp + 440]
 mov rdi, qword ptr [rip + .Lx18_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 416], rax
 mov qword ptr [rbp + 424], rdx
 jmp main_γ
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 472]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 472]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
.section .rodata
.S0: .string "PAT"
.S1: .string "V"
.text
