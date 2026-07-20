  .intel_syntax noprefix
  .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 176
  mov [rsp + 152], rcx
  mov [rsp + 160], rdx
  mov [rsp + 168], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 144
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 136], rsp
mov qword ptr [rbp + 144], r8
mov dword ptr [rbp + 136], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [rbp + 112], rax
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n0_α:
 jmp xchain0_n1_α
xchain0_n0_as:
 jmp proc_PAT$0_γ
 xchain0_n0_β:
 jmp xchain0_n2_β
xchain0_n0_af:
 jmp proc_PAT$0_ω
# IR_MATCH_BREAK
 xchain0_n1_α:
 sub rsp, 16
 lea rdi, [rip + .C0]
 mov dword ptr [rsp + 0], 0
.Lx4_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jl .Lx4_240
 add rsp, 16
 jmp xchain0_n0_af
.Lx4_240:
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 cmp byte ptr [rdi+rsi], 0
 jnz .Lx4_1
 add dword ptr [rsp + 0], 1
 jmp .Lx4_0
.Lx4_1:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 mov r14d, eax
 jmp xchain0_n2_α
 xchain0_n1_β:
 mov eax, r14d
 sub eax, dword ptr [rsp + 0]
 mov r14d, eax
 add rsp, 16
 jmp xchain0_n0_af
# IR_MATCH_SPAN
 xchain0_n2_α:
 sub rsp, 16
 lea rdi, [rip + .C0]
 mov dword ptr [rsp + 0], 0
.Lx6_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx6_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 cmp byte ptr [rdi+rsi], 0
 je .Lx6_1
 add dword ptr [rsp + 0], 1
 jmp .Lx6_0
.Lx6_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx6_240
 add rsp, 16
 jmp xchain0_n1_β
.Lx6_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp xchain0_n0_as
 xchain0_n2_β:
 xchain0_n2_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp xchain0_n1_β
proc_PAT$0_scanhit:
cmp qword ptr [rbp + 144], 1
jne 7f
mov ecx, dword ptr [rbp + 136]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$0_γ
proc_PAT$0_scanfail:
cmp qword ptr [rbp + 144], 1
jne 8f
mov eax, dword ptr [rbp + 136]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 136], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$0_attempt
8:
jmp proc_PAT$0_ω
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 112]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp + 152]
mov rbp, [rbp + 168]
jmp rax
proc_PAT$0_ω:
mov rax, [rbp + 160]
lea rsp, [rbp + 176]
mov rbp, [rbp + 168]
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
  mov esi, 144
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  add rsp, 8
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
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 5
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 5
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
  mov qword ptr [rsp + 1096], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain7_n0_α:
 mov qword ptr [rbp + 160], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain7_n1_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain7_n1_α:
 mov qword ptr [rbp + 192], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain7_n2_α
.Lx9_0:
 .quad 1
 xchain7_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rbp + 160]
 mov qword ptr [rbp + 112], rax
 mov rax, qword ptr [rbp + 168]
 mov qword ptr [rbp + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [rbp + 192]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 200]
 mov qword ptr [rbp + 136], rax
  .section .rodata
  .Lrkfn11: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn11]
 lea rsi, [rbp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain7_n3_α
 jmp xchain7_n3_α
 xchain7_n2_β:
 jmp xchain7_n3_α
# IR_LIT_STRING
 xchain7_n3_α:
 mov qword ptr [rbp + 240], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rbp + 248], rax
 jmp xchain7_n4_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "0123456789"
# IR_ASSIGN gva
 xchain7_n4_α:
 mov rax, qword ptr [rbp + 240]
 mov rdx, qword ptr [rbp + 248]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 jmp xchain7_n5_α
# IR_LIT_STRING
 xchain7_n5_α:
 mov qword ptr [rbp + 384], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 392], rax
 jmp xchain7_n6_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "'-"
# IR_VAR
 xchain7_n6_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 416], rax
 mov qword ptr [rbp + 424], rdx
 jmp xchain7_n7_α
 xchain7_n7_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 384]
 mov rsi, qword ptr [rbp + 392]
 mov rdx, qword ptr [rbp + 416]
 mov rcx, qword ptr [rbp + 424]
 call str_concat_d@PLT
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 jmp xchain7_n9_α
# IR_LIT_STRING
 xchain7_n8_α:
 mov qword ptr [rbp + 544], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rbp + 552], rax
 jmp xchain7_n10_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "PAT$0"
# IR_KEYWORD_SNOBOL4_read
 xchain7_n9_α:
 mov rdi, qword ptr [rip + .Lx18_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 jmp xchain7_n11_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "UCASE"
 xchain7_n10_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+544] -> [zr+512]
 mov rax, qword ptr [rbp + 544]
 mov qword ptr [rbp + 512], rax
 mov rax, qword ptr [rbp + 552]
 mov qword ptr [rbp + 520], rax
  .section .rodata
  .Lrkfn20: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn20]
 lea rsi, [rbp + 512]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 496], rax
 mov qword ptr [rbp + 504], rdx
 cmp eax, 99
 je xchain7_n13_α
 jmp xchain7_n12_α
 xchain7_n10_β:
 jmp xchain7_n13_α
 xchain7_n11_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 352]
 mov rsi, qword ptr [rbp + 360]
 mov rdx, qword ptr [rbp + 448]
 mov rcx, qword ptr [rbp + 456]
 call str_concat_d@PLT
 mov qword ptr [rbp + 320], rax
 mov qword ptr [rbp + 328], rdx
 jmp xchain7_n14_α
# IR_ASSIGN gva
 xchain7_n12_α:
 mov rax, qword ptr [rbp + 496]
 mov rdx, qword ptr [rbp + 504]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 480], rax
 mov qword ptr [rbp + 488], rdx
 jmp xchain7_n13_α
# IR_VAR
 xchain7_n13_α:
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je xchain7_n16_α
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp xchain7_n15_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "INPUT"
# IR_KEYWORD_SNOBOL4_read
 xchain7_n14_α:
 mov rdi, qword ptr [rip + .Lx24_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 jmp xchain7_n17_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "LCASE"
# IR_ASSIGN gva
 xchain7_n15_α:
 mov rax, qword ptr [rbp + 592]
 mov rdx, qword ptr [rbp + 600]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rbp + 576], rax
 mov qword ptr [rbp + 584], rdx
 jmp xchain7_n18_α
# IR_VAR
 xchain7_n16_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 1024], rax
 mov qword ptr [rbp + 1032], rdx
 jmp xchain7_n19_α
 xchain7_n17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 320]
 mov rsi, qword ptr [rbp + 328]
 mov rdx, qword ptr [rbp + 464]
 mov rcx, qword ptr [rbp + 472]
 call str_concat_d@PLT
 mov qword ptr [rbp + 288], rax
 mov qword ptr [rbp + 296], rdx
 jmp xchain7_n20_α
# IR_VAR
 xchain7_n18_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 800], rax
 mov qword ptr [rbp + 808], rdx
 jmp xchain7_n21_α
# IR_UNOP
 xchain7_n19_α:
 mov rdi, qword ptr [rbp + 1024]
 mov rsi, qword ptr [rbp + 1032]
 call rt_num_pos@PLT
 mov qword ptr [rbp + 992], rax
 mov qword ptr [rbp + 1000], rdx
 jmp xchain7_n22_α
# IR_ASSIGN gva
 xchain7_n20_α:
 mov rax, qword ptr [rbp + 288]
 mov rdx, qword ptr [rbp + 296]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 jmp xchain7_n8_α
# IR_MATCH_HEAD
 xchain7_n21_α:
 mov qword ptr [rbp + 680], rbp
 mov rdi, qword ptr [rbp + 800]
 mov rsi, qword ptr [rbp + 808]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 672], r12
 mov qword ptr [rbp + 656], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 648], rax
 mov dword ptr [rbp + 640], 0
.Lx32_0:
 mov r14d, dword ptr [rbp + 640]
 jmp xchain7_n23_α
 xchain7_n21_β:
 add dword ptr [rbp + 640], 1
 mov eax, dword ptr [rbp + 640]
 cmp eax, r15d
 jg .Lx32_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx32_1
 jmp .Lx32_0
.Lx32_1:
 mov rax, qword ptr [rbp + 648]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 656]
 mov r12, qword ptr [rbp + 672]
 mov rbp, qword ptr [rbp + 680]
 jmp xchain7_n13_α
# IR_LIT_STRING
 xchain7_n22_α:
 mov qword ptr [rbp + 1056], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [rbp + 1064], rax
 jmp xchain7_n24_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string " words"
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain7_n23_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 cmp eax, 3
 jne .Lx34_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx34_10
.Lx34_9:
 xor eax, eax
.Lx34_10:
 test rax, rax
 jz .Lx34_0
 mov r8d, 1
 lea rcx, [rip + .Lx34_4]
 lea rdx, [rip + .Lx34_5]
 jmp rax
.Lx34_4:
 lea rcx, [rip + g_scan_hit_start]
 mov rax, qword ptr [rcx]
 mov dword ptr [rbp + 640], eax
 jmp xchain7_n25_α
.Lx34_5:
 jmp xchain7_n21_β
.Lx34_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx34_2:
 test rax, rax
 je .Lx34_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx34_7]
 lea rdx, [rip + .Lx34_8]
 jmp rax
.Lx34_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx34_2
.Lx34_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx34_2
.Lx34_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain7_n21_β
 mov r14d, eax
 lea rax, [rip + .Lx34_6]
 sub rsp, 8
 push rax
 jmp xchain7_n25_α
.Lx34_6:
 add rsp, 16
 jmp xchain7_n21_β
 xchain7_n23_β:
 jmp qword ptr [rsp]
 xchain7_n24_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 992]
 mov rsi, qword ptr [rbp + 1000]
 mov rdx, qword ptr [rbp + 1056]
 mov rcx, qword ptr [rbp + 1064]
 call str_concat_d@PLT
 mov qword ptr [rbp + 960], rax
 mov qword ptr [rbp + 968], rdx
 jmp xchain7_n26_α
# IR_MATCH_RELEASE
 xchain7_n25_α:
 mov rax, qword ptr [rbp + 648]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov qword ptr [rbp + 664], r14
 mov rsp, qword ptr [rbp + 656]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 704]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx37_1:
 test rax, rax
 je .Lx37_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx37_3]
 lea rdx, [rip + .Lx37_4]
 jmp rax
.Lx37_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx37_1
.Lx37_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx37_1
.Lx37_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 672]
 jmp xchain7_n27_α
# IR_ASSIGN global
 xchain7_n26_α:
 mov rsi, qword ptr [rbp + 960]
 mov rdx, qword ptr [rbp + 968]
 mov rdi, qword ptr [rip + .Lx38_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 944], rax
 mov qword ptr [rbp + 952], rdx
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain7_n27_α:
 mov qword ptr [rbp + 704], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [rbp + 712], rax
 jmp xchain7_n28_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string ""
# IR_MATCH_REPLACE
 xchain7_n28_α:
 mov rdi, qword ptr [rip + .Lx41_0]
 mov rsi, qword ptr [rbp + 800]
 mov rdx, qword ptr [rbp + 808]
 mov ecx, dword ptr [rbp + 640]
 mov r8, qword ptr [rbp + 664]
 lea r9, [rbp + 704]
 call rt_match_replace@PLT
 jmp .Lx41_1
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "LINE"
.Lx41_1:
 mov rbp, qword ptr [rbp + 680]
 jmp xchain7_n29_α
# IR_VAR
 xchain7_n29_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 880], rax
 mov qword ptr [rbp + 888], rdx
 jmp xchain7_n30_α
# IR_LIT_INTEGER
 xchain7_n30_α:
 mov qword ptr [rbp + 912], 6
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [rbp + 920], rax
 jmp xchain7_n31_α
.Lx43_0:
 .quad 1
 xchain7_n31_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 880]
 cmp eax, 100
 je .Lx44_0
 mov eax, dword ptr [rbp + 880]
 cmp eax, 6
 jne .Lx44_2
.Lx44_1:
 mov rax, qword ptr [rbp + 888]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rbp + 848], 6
 mov qword ptr [rbp + 856], rax
 jmp xchain7_n32_α
.Lx44_0:
 mov rdi, qword ptr [rbp + 880]
 mov rsi, qword ptr [rbp + 888]
 mov rdx, qword ptr [rbp + 912]
 mov rcx, qword ptr [rbp + 920]
 mov r8d, 0
 lea r9, [rbp + 848]
 call rt_binop_overload@PLT
 test eax, eax
 jne xchain7_n32_α
.Lx44_2:
 mov rdi, qword ptr [rbp + 880]
 mov rsi, qword ptr [rbp + 888]
 mov rdx, qword ptr [rbp + 912]
 mov rcx, qword ptr [rbp + 920]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain7_n18_α
 mov qword ptr [rbp + 848], rax
 mov qword ptr [rbp + 856], rdx
 jmp xchain7_n32_α
 xchain7_n31_β:
 jmp xchain7_n18_α
# IR_ASSIGN gva
 xchain7_n32_α:
 mov rax, qword ptr [rbp + 848]
 mov rdx, qword ptr [rbp + 856]
 mov qword ptr [1879052352], rax
 mov qword ptr [1879052360], rdx
 mov qword ptr [rbp + 832], rax
 mov qword ptr [rbp + 840], rdx
 jmp xchain7_n18_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 1096]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 1096]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
.section .rodata
.S0: .string "WPAT"
.text
.section .rodata
.C0:
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0
 .byte 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0
 .byte 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
 .byte 1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0
 .byte 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
 .byte 1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.text
