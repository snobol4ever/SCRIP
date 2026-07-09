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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 48], rax
 pop rsi
proc_PAT$0_α_body:
# IR_MATCH_BREAK
 xchain0_n0_α:
 mov dword ptr [r12 + 32], 0
.Lx2_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 32]
 cmp eax, r15d
 jge proc_PAT$0_ω
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 jnz .Lx2_1
 add dword ptr [r12 + 32], 1
 jmp .Lx2_0
.Lx2_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 32]
 mov r14d, eax
 jmp xchain0_n1_α
 xchain0_n0_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 32]
 mov r14d, eax
 jmp proc_PAT$0_ω
# IR_MATCH_SPAN
 xchain0_n1_α:
 mov dword ptr [r12 + 16], 0
.Lx4_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jge .Lx4_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx4_1
 add dword ptr [r12 + 16], 1
 jmp .Lx4_0
.Lx4_1:
 mov eax, dword ptr [r12 + 16]
 test eax, eax
 jle proc_PAT$0_ω
 mov edx, r14d
 mov dword ptr [r12 + 20], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$0_γ
 xchain0_n1_β:
 xchain0_n1_β:
 mov r14d, dword ptr [r12 + 20]
 jmp proc_PAT$0_ω
proc_PAT$0_β:
jmp proc_PAT$0_ω
proc_PAT$0_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 48]
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
mov rsp, rbp
pop rbp
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
  .Lgvan0: .string "NUMERALS"
  .Lgvan1: .string "WORD"
  .Lgvan2: .string "WPAT"
  .Lgvan3: .string "LINE"
  .Lgvan4: .string "N"
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
  call core_lib_init@PLT
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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 560], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain5_n0_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain5_n1_α
 xchain5_n0_β:
 jmp xchain5_n3_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain5_n1_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain5_n2_α
 xchain5_n1_β:
 jmp xchain5_n3_α
.Lx7_0:
 .quad 1
 xchain5_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+64] -> [r12+32]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 40], rax
# marshal arg1 = producer-box slot [r12+80] -> [r12+48]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn9: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn9]
 lea rsi, [r12 + 32]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je xchain5_n3_α
 jmp xchain5_n3_α
 xchain5_n2_β:
 jmp xchain5_n3_α
# IR_LIT_STRING
 xchain5_n3_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain5_n4_α
 xchain5_n3_β:
 jmp xchain5_n5_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "0123456789"
# IR_ASSIGN gva
 xchain5_n4_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain5_n5_α
 xchain5_n4_β:
 jmp xchain5_n5_α
# IR_LIT_STRING
 xchain5_n5_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain5_n6_α
 xchain5_n5_β:
 jmp xchain5_n8_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "'-"
# IR_VAR gva
 xchain5_n6_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain5_n7_α
 xchain5_n6_β:
 jmp xchain5_n8_α
 xchain5_n7_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 208]
 mov rcx, qword ptr [r12 + 216]
 call str_concat_d@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain5_n9_α
 xchain5_n7_β:
 jmp xchain5_n8_α
# IR_LIT_STRING
 xchain5_n8_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain5_n10_α
 xchain5_n8_β:
 jmp xchain5_n13_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "PAT$0"
# IR_KEYWORD_SNOBOL4_read
 xchain5_n9_α:
 mov rdi, qword ptr [rip + .Lx16_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain5_n11_α
 xchain5_n9_β:
 jmp xchain5_n8_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "UCASE"
 xchain5_n10_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+304] -> [r12+288]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lrkfn18: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn18]
 lea rsi, [r12 + 288]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je xchain5_n13_α
 jmp xchain5_n12_α
 xchain5_n10_β:
 jmp xchain5_n13_α
 xchain5_n11_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 176]
 mov rsi, qword ptr [r12 + 184]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 call str_concat_d@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain5_n14_α
 xchain5_n11_β:
 jmp xchain5_n8_α
# IR_ASSIGN gva
 xchain5_n12_α:
 mov rax, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain5_n13_α
 xchain5_n12_β:
 jmp xchain5_n13_α
# IR_VAR
 xchain5_n13_α:
 mov rdi, qword ptr [rip + .Lx21_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je xchain5_n16_α
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain5_n15_α
 xchain5_n13_β:
 jmp xchain5_n16_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "INPUT"
# IR_KEYWORD_SNOBOL4_read
 xchain5_n14_α:
 mov rdi, qword ptr [rip + .Lx22_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain5_n17_α
 xchain5_n14_β:
 jmp xchain5_n8_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "LCASE"
# IR_ASSIGN gva
 xchain5_n15_α:
 mov rax, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain5_n18_α
 xchain5_n15_β:
 jmp xchain5_n16_α
# IR_VAR gva
 xchain5_n16_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain5_n19_α
 xchain5_n16_β:
 jmp main_γ
 xchain5_n17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 call str_concat_d@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain5_n20_α
 xchain5_n17_β:
 jmp xchain5_n8_α
# IR_VAR gva
 xchain5_n18_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain5_n21_α
 xchain5_n18_β:
 jmp xchain5_n13_α
# IR_UNOP
 xchain5_n19_α:
 mov rdi, qword ptr [r12 + 528]
 mov rsi, qword ptr [r12 + 536]
 call rt_num_pos@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain5_n22_α
 xchain5_n19_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain5_n20_α:
 mov rax, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain5_n8_α
 xchain5_n20_β:
 jmp xchain5_n8_α
# IR_MATCH_HEAD
 xchain5_n21_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 360], rax
 mov qword ptr [r12 + 368], rsp
 mov rdi, qword ptr [r12 + 400]
 mov rsi, qword ptr [r12 + 408]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 352], 0
.Lx30_0:
 mov r14d, dword ptr [r12 + 352]
 jmp xchain5_n23_α
 xchain5_n21_β:
 add dword ptr [r12 + 352], 1
 mov eax, dword ptr [r12 + 352]
 cmp eax, r15d
 jg .Lx30_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx30_1
 jmp .Lx30_0
.Lx30_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 360]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 368]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain5_n13_α
# IR_LIT_STRING
 xchain5_n22_α:
 mov qword ptr [r12 + 544], 1
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain5_n24_α
 xchain5_n22_β:
 jmp main_γ
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string " words"
# IR_MATCH_DEFER
 xchain5_n23_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx32_0
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
 je .Lx32_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain5_n21_β
.Lx32_1:
 jmp xchain5_n25_α
.Lx32_0:
 lea rdi, [rip + .S1]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain5_n21_β
 mov r14d, eax
 jmp xchain5_n25_α
 xchain5_n23_β:
 jmp xchain5_n21_β
 xchain5_n24_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 mov rdx, qword ptr [r12 + 544]
 mov rcx, qword ptr [r12 + 552]
 call str_concat_d@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain5_n26_α
 xchain5_n24_β:
 jmp main_γ
# IR_MATCH_RELEASE
 xchain5_n25_α:
 mov qword ptr [r12 + 376], r14
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 360]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 368]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain5_n27_α
# IR_ASSIGN global
 xchain5_n26_α:
 mov rsi, qword ptr [r12 + 496]
 mov rdx, qword ptr [r12 + 504]
 mov rdi, qword ptr [rip + .Lx36_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp main_γ
 xchain5_n26_β:
 jmp main_γ
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain5_n27_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain5_n28_α
 xchain5_n27_β:
 jmp xchain5_n13_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string ""
# IR_MATCH_REPLACE
 xchain5_n28_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx39_0]
 mov rsi, qword ptr [r12 + 400]
 mov rdx, qword ptr [r12 + 408]
 mov ecx, dword ptr [r12 + 352]
 mov r8, qword ptr [r12 + 376]
 lea r9, [r12 + 384]
 call rt_match_replace@PLT
 mov rsp, rbp
 pop rbp
 jmp .Lx39_1
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "LINE"
.Lx39_1:
 jmp xchain5_n29_α
# IR_VAR gva
 xchain5_n29_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain5_n30_α
 xchain5_n29_β:
 jmp xchain5_n18_α
# IR_LIT_INTEGER
 xchain5_n30_α:
 mov qword ptr [r12 + 464], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain5_n31_α
 xchain5_n30_β:
 jmp xchain5_n18_α
.Lx41_0:
 .quad 1
 xchain5_n31_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 448]
 cmp eax, 100
 je .Lx42_0
 mov eax, dword ptr [r12 + 464]
 cmp eax, 100
 je .Lx42_0
 mov eax, dword ptr [r12 + 448]
 cmp eax, 6
 jne .Lx42_2
 mov eax, dword ptr [r12 + 464]
 cmp eax, 6
 jne .Lx42_2
.Lx42_1:
 mov rax, qword ptr [r12 + 456]
 mov rcx, qword ptr [r12 + 472]
 add rax, rcx
 mov qword ptr [r12 + 432], 6
 mov qword ptr [r12 + 440], rax
 jmp xchain5_n32_α
.Lx42_0:
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 mov r8d, 0
 lea r9, [r12 + 432]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx42_3
.Lx42_2:
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain5_n18_α
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
.Lx42_3:
 jmp xchain5_n32_α
 xchain5_n31_β:
 jmp xchain5_n18_α
# IR_ASSIGN gva
 xchain5_n32_α:
 mov rax, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov qword ptr [rbx + 64], rax
 mov qword ptr [rbx + 72], rdx
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain5_n18_α
 xchain5_n32_β:
 jmp xchain5_n18_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 560]
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
mov rsp, rbp
pop rbp
pop r12
ret
.section .rodata
.S0: .string "'-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
.S1: .string "WPAT"
.text
