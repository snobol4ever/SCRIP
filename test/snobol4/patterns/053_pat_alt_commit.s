  .intel_syntax noprefix
  .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 160
  mov [rsp+8], rcx
  mov [rsp+16], rdx
  mov [rsp+24], rbp
  lea rbp, [rsp+32]
  mov rdi, rbp
  mov ecx, 128
  xor eax, eax
  rep stosb
  mov qword ptr [rbp + 120], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [rbp + 112], rax
 pop rsi
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [rbp + 96], rax
# IR_MATCH_ALT_NARY
 xchain0_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 mov dword ptr [rsp + 4], 0
 jmp xchain0_n1_α
xchain0_n0_s0:
 jmp xchain0_n0_as
xchain0_n0_s1:
 jmp xchain0_n0_as
xchain0_n0_s2:
 jmp xchain0_n0_as
xchain0_n0_as:
 jmp proc_PAT$0_γ
 xchain0_n0_β:
 mov eax, dword ptr [rsp + 4]
 cmp eax, 0
 je xchain0_n1_β
 cmp eax, 1
 je xchain0_n2_β
 jmp xchain0_n3_β
xchain0_n0_af:
 add dword ptr [rsp + 4], 1
 mov r14d, dword ptr [rsp + 0]
 mov eax, dword ptr [rsp + 4]
 cmp eax, 1
 je xchain0_n2_α
 cmp eax, 2
 je xchain0_n3_α
 add rsp, 16
 jmp proc_PAT$0_ω
# IR_MATCH_LIT
 xchain0_n1_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n0_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain0_n0_af
 add r14d, 1
 jmp xchain0_n0_s0
 xchain0_n1_β:
 sub r14d, 1
 jmp xchain0_n0_af
# IR_MATCH_LIT
 xchain0_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n0_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain0_n0_af
 add r14d, 1
 jmp xchain0_n0_s1
 xchain0_n2_β:
 sub r14d, 1
 jmp xchain0_n0_af
# IR_MATCH_LIT
 xchain0_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n0_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S2]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain0_n0_af
 add r14d, 1
 jmp xchain0_n0_s2
 xchain0_n3_β:
 sub r14d, 1
 jmp xchain0_n0_af
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 96]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp-24]
mov rbp, [rbp-8]
jmp rax
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [rbp + 112]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_PAT$0_ω:
mov rax, [rbp-16]
lea rsp, [rbp + 128]
mov rbp, [rbp-8]
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
  mov esi, 128
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
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
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
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
  mov qword ptr [rsp + 552], rsp
  mov r12, qword ptr [1879048192]
main_α_body:
# IR_LIT_STRING
 xchain9_n0_α:
 mov qword ptr [rsp + 160], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain9_n1_α
 xchain9_n0_β:
 jmp xchain9_n3_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "PAT$0"
 xchain9_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lrkfn12: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn12]
 lea rsi, [rsp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 cmp eax, 99
 je xchain9_n3_α
 jmp xchain9_n2_α
 xchain9_n1_β:
 jmp xchain9_n3_α
# IR_ASSIGN gva
 xchain9_n2_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain9_n3_α
 xchain9_n2_β:
 jmp xchain9_n3_α
# IR_LIT_STRING
 xchain9_n3_α:
 mov qword ptr [rsp + 208], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 216], rax
 jmp xchain9_n4_α
 xchain9_n3_β:
 jmp xchain9_n5_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "b"
# IR_ASSIGN gva
 xchain9_n4_α:
 mov rax, qword ptr [rsp + 208]
 mov rdx, qword ptr [rsp + 216]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 192], rax
 mov qword ptr [rsp + 200], rdx
 jmp xchain9_n5_α
 xchain9_n4_β:
 jmp xchain9_n5_α
# IR_VAR
 xchain9_n5_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 416], rax
 mov qword ptr [rsp + 424], rdx
 jmp xchain9_n6_α
 xchain9_n5_β:
 jmp xchain9_n7_α
# IR_MATCH_HEAD
 xchain9_n6_α:
 mov qword ptr [rsp + 296], rbp
 mov rbp, rsp
 mov rdi, qword ptr [rbp + 416]
 mov rsi, qword ptr [rbp + 424]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 288], r12
 mov qword ptr [rbp + 272], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 264], rax
 mov dword ptr [rbp + 256], 0
.Lx18_0:
 mov r14d, dword ptr [rbp + 256]
 jmp xchain9_n8_α
 xchain9_n6_β:
 add dword ptr [rbp + 256], 1
 mov eax, dword ptr [rbp + 256]
 cmp eax, r15d
 jg .Lx18_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx18_1
 jmp .Lx18_0
.Lx18_1:
 mov rax, qword ptr [rbp + 264]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 272]
 mov r12, qword ptr [rbp + 288]
 mov rbp, qword ptr [rbp + 296]
 jmp xchain9_n7_α
# IR_LIT_STRING
 xchain9_n7_α:
 mov qword ptr [rsp + 512], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 520], rax
 jmp xchain9_n9_α
 xchain9_n7_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "no match"
# IR_MATCH_CAPTURE_SAVE push
 xchain9_n8_α:
 lea rdi, [rbp + 336]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain9_n10_α
 xchain9_n8_β:
 lea rdi, [rbp + 336]
 call rt_cap_pop@PLT
 jmp xchain9_n6_β
# IR_ASSIGN global
 xchain9_n9_α:
 mov rsi, qword ptr [rsp + 512]
 mov rdx, qword ptr [rsp + 520]
 mov rdi, qword ptr [rip + .Lx22_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 496], rax
 mov qword ptr [rsp + 504], rdx
 jmp main_γ
 xchain9_n9_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "OUTPUT"
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain9_n10_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx23_0
 lea rcx, [rip + g_pat_main_rsp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rsp, qword ptr [rcx + 0]
 lea rcx, [rip + .Lx23_4]
 lea rdx, [rip + .Lx23_5]
 jmp rax
.Lx23_4:
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain9_n11_α
.Lx23_5:
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain9_n8_β
.Lx23_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S3]
 xor esi, esi
 call rt_defer_open@PLT
.Lx23_2:
 test rax, rax
 je .Lx23_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx23_7]
 lea rdx, [rip + .Lx23_8]
 jmp rax
.Lx23_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx23_2
.Lx23_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx23_2
.Lx23_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain9_n8_β
 mov r14d, eax
 lea rax, [rip + .Lx23_6]
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
 jmp xchain9_n11_α
.Lx23_6:
 add rsp, 16
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain9_n8_β
 xchain9_n10_β:
 lea rcx, [rip + g_pat_main_rsp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rsp, qword ptr [rcx + 0]
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain9_n11_α:
 lea rdi, [rbp + 336]
 call rt_cap_top@PLT
 lea rcx, [rip + .S4]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain9_n12_α
 xchain9_n11_β:
 sub r12, 24
 jmp xchain9_n10_β
# IR_MATCH_RELEASE
 xchain9_n12_α:
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
.Lx27_1:
 test rax, rax
 je .Lx27_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx27_3]
 lea rdx, [rip + .Lx27_4]
 jmp rax
.Lx27_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx27_1
.Lx27_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx27_1
.Lx27_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 288]
 mov rbp, qword ptr [rbp + 296]
 jmp xchain9_n13_α
# IR_VAR
 xchain9_n13_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
 jmp xchain9_n14_α
 xchain9_n13_β:
 jmp main_γ
# IR_ASSIGN global
 xchain9_n14_α:
 mov rsi, qword ptr [rsp + 464]
 mov rdx, qword ptr [rsp + 472]
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 448], rax
 mov qword ptr [rsp + 456], rdx
 jmp main_γ
 xchain9_n14_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 552]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 552]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
.section .rodata
.S0: .string "a"
.S1: .string "b"
.S2: .string "c"
.S3: .string "P"
.S4: .string "V"
.text
