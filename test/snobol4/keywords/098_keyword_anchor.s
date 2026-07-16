  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 1
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 1
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
  mov qword ptr [rsp + 696], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 160], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "ANCHOR"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [rsp + 192]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 200]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn4]
 lea rsi, [rsp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rsp + 240], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 248], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "hello world"
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [rsp + 240]
 mov rdx, qword ptr [rsp + 248]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 352], rax
 mov qword ptr [rsp + 360], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
# IR_MATCH_HEAD
 xchain0_n6_α:
 mov qword ptr [rsp + 328], rbp
 mov rdi, qword ptr [rsp + 352]
 mov rsi, qword ptr [rsp + 360]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov rax, qword ptr [1879048192]
 mov qword ptr [rsp + 320], rax
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
 mov dword ptr [rsp + 0], 0
.Lx9_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n8_α
 xchain0_n6_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx9_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx9_1
 jmp .Lx9_0
.Lx9_1:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 mov rax, qword ptr [rsp + 320]
 mov qword ptr [1879048192], rax
 mov rbp, qword ptr [rsp + 328]
 jmp xchain0_n7_α
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rsp + 608], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 616], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "wrong"
# IR_MATCH_LIT
 xchain0_n8_α:
 mov eax, r14d
 add eax, 5
 cmp eax, r15d
 jg xchain0_n6_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov edx, 5
 call memcmp@PLT
 test eax, eax
 jne xchain0_n6_β
 add r14d, 5
 jmp xchain0_n10_α
 xchain0_n8_β:
 sub r14d, 5
 jmp xchain0_n6_β
# IR_ASSIGN global
 xchain0_n9_α:
 mov rsi, qword ptr [rsp + 608]
 mov rdx, qword ptr [rsp + 616]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 592], rax
 mov qword ptr [rsp + 600], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n10_α:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 352]
 mov rsi, qword ptr [1879048192]
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx15_1:
 test rax, rax
 je .Lx15_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx15_3]
 lea rdx, [rip + .Lx15_4]
 jmp rax
.Lx15_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx15_1
.Lx15_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx15_1
.Lx15_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov rax, qword ptr [rsp + 320]
 mov qword ptr [1879048192], rax
 mov rbp, qword ptr [rsp + 328]
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rsp + 400], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rsp + 408], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n13_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "anchored match ok"
# IR_ASSIGN global
 xchain0_n12_α:
 mov rsi, qword ptr [rsp + 400]
 mov rdx, qword ptr [rsp + 408]
 mov rdi, qword ptr [rip + .Lx17_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n13_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "OUTPUT"
# IR_VAR
 xchain0_n13_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 512], rax
 mov qword ptr [rsp + 520], rdx
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n15_α
# IR_MATCH_HEAD
 xchain0_n14_α:
 mov qword ptr [rsp + 488], rbp
 mov rdi, qword ptr [rsp + 512]
 mov rsi, qword ptr [rsp + 520]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov rax, qword ptr [1879048192]
 mov qword ptr [rsp + 480], rax
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
 mov dword ptr [rsp + 0], 0
.Lx20_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n16_α
 xchain0_n14_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx20_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx20_1
 jmp .Lx20_0
.Lx20_1:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 mov rax, qword ptr [rsp + 480]
 mov qword ptr [1879048192], rax
 mov rbp, qword ptr [rsp + 488]
 jmp xchain0_n15_α
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [rsp + 656], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rsp + 664], rax
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "anchor prevented mid-string match"
# IR_MATCH_LIT
 xchain0_n16_α:
 mov eax, r14d
 add eax, 5
 cmp eax, r15d
 jg xchain0_n14_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 5
 call memcmp@PLT
 test eax, eax
 jne xchain0_n14_β
 add r14d, 5
 jmp xchain0_n18_α
 xchain0_n16_β:
 sub r14d, 5
 jmp xchain0_n14_β
# IR_ASSIGN global
 xchain0_n17_α:
 mov rsi, qword ptr [rsp + 656]
 mov rdx, qword ptr [rsp + 664]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 640], rax
 mov qword ptr [rsp + 648], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_γ
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n18_α:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 512]
 mov rsi, qword ptr [1879048192]
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx26_1:
 test rax, rax
 je .Lx26_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx26_3]
 lea rdx, [rip + .Lx26_4]
 jmp rax
.Lx26_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx26_1
.Lx26_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx26_1
.Lx26_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov rax, qword ptr [rsp + 480]
 mov qword ptr [1879048192], rax
 mov rbp, qword ptr [rsp + 488]
 jmp xchain0_n19_α
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [rsp + 560], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [rsp + 568], rax
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "should not reach"
# IR_ASSIGN global
 xchain0_n20_α:
 mov rsi, qword ptr [rsp + 560]
 mov rdx, qword ptr [rsp + 568]
 mov rdi, qword ptr [rip + .Lx28_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 544], rax
 mov qword ptr [rsp + 552], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 696]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 696]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
.section .rodata
.S0: .string "hello"
.S1: .string "world"
.text
