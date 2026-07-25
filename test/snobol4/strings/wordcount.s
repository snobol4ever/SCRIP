  .intel_syntax noprefix
  .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 256
  mov [rsp + 232], rcx
  mov [rsp + 240], rdx
  mov [rsp + 248], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 192], 0
  mov qword ptr [rsp + 200], 0
  mov qword ptr [rsp + 208], 0
  mov qword ptr [rsp + 216], rsp
mov qword ptr [rbp + 224], r8
mov dword ptr [rbp + 216], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n4_β]
mov qword ptr [rbp + 192], rax
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052384]
 mov rdx, qword ptr [1879052392]
 mov qword ptr [rbp + 160], rax
 mov qword ptr [rbp + 168], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_PAT$0_scanfail
# IR_COERCE_STRING
 xchain0_n1_α:
 lea rdi, [rbp + 160]
 lea rsi, [rbp + 128]
 mov rdx, 12320956
 call rt_coerce_str_d@PLT
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_PAT$0_scanfail
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [1879052368]
 mov rdx, qword ptr [1879052376]
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_PAT$0_scanfail
# IR_COERCE_STRING
 xchain0_n3_α:
 lea rdi, [rbp + 96]
 lea rsi, [rbp + 64]
 mov rdx, 4522053
 call rt_coerce_str_d@PLT
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp proc_PAT$0_scanfail
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n4_α:
 jmp xchain0_n5_α
xchain0_n4_as:
 jmp proc_PAT$0_γ
 xchain0_n4_β:
 jmp xchain0_n6_β
xchain0_n4_af:
 jmp proc_PAT$0_ω
# IR_MATCH_BREAK
 xchain0_n5_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
 mov r8, qword ptr [rbp + 72]
 mov r9d, dword ptr [rbp + 68]
.Lx10_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jl .Lx10_240
 add rsp, 16
 jmp proc_PAT$0_ω
.Lx10_240:
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 mov edx, 0
.Lx10_2:
 cmp edx, r9d
 jge .Lx10_3
 movzx edi, byte ptr [r8 + rdx]
 cmp esi, edi
 je .Lx10_1
 add edx, 1
 jmp .Lx10_2
.Lx10_3:
 add dword ptr [rsp + 0], 1
 jmp .Lx10_0
.Lx10_1:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 mov dword ptr [rsp + 0], r14d
 mov r14d, eax
 jmp xchain0_n6_α
 xchain0_n5_β:
 mov r14d, dword ptr [rsp + 0]
 add rsp, 16
 jmp proc_PAT$0_ω
# IR_MATCH_SPAN
 xchain0_n6_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
 mov r8, qword ptr [rbp + 136]
 mov r9d, dword ptr [rbp + 132]
.Lx12_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx12_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 mov edx, 0
.Lx12_2:
 cmp edx, r9d
 jge .Lx12_1
 movzx edi, byte ptr [r8 + rdx]
 cmp esi, edi
 je .Lx12_3
 add edx, 1
 jmp .Lx12_2
.Lx12_3:
 add dword ptr [rsp + 0], 1
 jmp .Lx12_0
.Lx12_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx12_240
 add rsp, 16
 jmp xchain0_n5_β
.Lx12_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$0_γ
 xchain0_n6_β:
 xchain0_n6_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp xchain0_n5_β
proc_PAT$0_scanhit:
cmp qword ptr [rbp + 224], 1
jne 7f
mov ecx, dword ptr [rbp + 216]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$0_γ
proc_PAT$0_scanfail:
cmp qword ptr [rbp + 224], 1
jne 8f
mov eax, dword ptr [rbp + 216]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 216], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$0_attempt
8:
jmp proc_PAT$0_ω
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 192]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp + 232]
mov rbp, [rbp + 248]
jmp rax
proc_PAT$0_ω:
mov rax, [rbp + 240]
lea rsp, [rbp + 256]
mov rbp, [rbp + 248]
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
  mov esi, 224
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
  .Lgvan0: .string "NUMERALS"
  .Lgvan1: .string "WORD"
  .Lgvan2: .string "WPAT"
  .Lgvan3: .string "LINE"
  .Lgvan4: .string "N"
  .Lgvan5: .string "PAT$0$A0"
  .Lgvan6: .string "PAT$0$A1"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .quad .Lgvan6
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 7
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 7
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
  mov qword ptr [rsp + 1144], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain13_n0_α:
 mov qword ptr [rbp + 160], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain13_n1_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain13_n1_α:
 mov qword ptr [rbp + 192], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain13_n2_α
.Lx15_0:
 .quad 1
 xchain13_n2_α:
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
  .Lrkfn17: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn17]
 lea rsi, [rbp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain13_n3_α
 jmp xchain13_n3_α
 xchain13_n2_β:
 jmp xchain13_n3_α
# IR_LIT_STRING
 xchain13_n3_α:
 mov qword ptr [rbp + 240], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rbp + 248], rax
 jmp xchain13_n4_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "0123456789"
# IR_ASSIGN gva
 xchain13_n4_α:
 mov rax, qword ptr [rbp + 240]
 mov rdx, qword ptr [rbp + 248]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 jmp xchain13_n5_α
# IR_LIT_STRING
 xchain13_n5_α:
 mov qword ptr [rbp + 384], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rbp + 392], rax
 jmp xchain13_n6_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "'-"
# IR_VAR
 xchain13_n6_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 416], rax
 mov qword ptr [rbp + 424], rdx
 jmp xchain13_n7_α
 xchain13_n7_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 384]
 mov rsi, qword ptr [rbp + 392]
 mov rdx, qword ptr [rbp + 416]
 mov rcx, qword ptr [rbp + 424]
 call str_concat_d@PLT
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 jmp xchain13_n9_α
# IR_VAR
 xchain13_n8_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 640], rax
 mov qword ptr [rbp + 648], rdx
 jmp xchain13_n10_α
# IR_KEYWORD_SNOBOL4_read
 xchain13_n9_α:
 mov rdi, qword ptr [rip + .Lx24_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 jmp xchain13_n12_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "UCASE"
# IR_ASSIGN gva
 xchain13_n10_α:
 mov rax, qword ptr [rbp + 640]
 mov rdx, qword ptr [rbp + 648]
 mov qword ptr [1879052384], rax
 mov qword ptr [1879052392], rdx
 mov qword ptr [rbp + 624], rax
 mov qword ptr [rbp + 632], rdx
 jmp xchain13_n13_α
# IR_VAR
 xchain13_n11_α:
 mov rdi, qword ptr [rip + .Lx26_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je xchain13_n15_α
 mov qword ptr [rbp + 688], rax
 mov qword ptr [rbp + 696], rdx
 jmp xchain13_n14_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "INPUT"
 xchain13_n12_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 352]
 mov rsi, qword ptr [rbp + 360]
 mov rdx, qword ptr [rbp + 448]
 mov rcx, qword ptr [rbp + 456]
 call str_concat_d@PLT
 mov qword ptr [rbp + 320], rax
 mov qword ptr [rbp + 328], rdx
 jmp xchain13_n16_α
# IR_VAR
 xchain13_n13_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp xchain13_n17_α
# IR_ASSIGN gva
 xchain13_n14_α:
 mov rax, qword ptr [rbp + 688]
 mov rdx, qword ptr [rbp + 696]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rbp + 672], rax
 mov qword ptr [rbp + 680], rdx
 jmp xchain13_n18_α
# IR_VAR
 xchain13_n15_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 1072], rax
 mov qword ptr [rbp + 1080], rdx
 jmp xchain13_n19_α
# IR_KEYWORD_SNOBOL4_read
 xchain13_n16_α:
 mov rdi, qword ptr [rip + .Lx31_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 jmp xchain13_n20_α
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "LCASE"
# IR_ASSIGN gva
 xchain13_n17_α:
 mov rax, qword ptr [rbp + 592]
 mov rdx, qword ptr [rbp + 600]
 mov qword ptr [1879052368], rax
 mov qword ptr [1879052376], rdx
 mov qword ptr [rbp + 576], rax
 mov qword ptr [rbp + 584], rdx
 jmp xchain13_n21_α
# IR_VAR
 xchain13_n18_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 848], rax
 mov qword ptr [rbp + 856], rdx
 jmp xchain13_n22_α
# IR_UNOP
 xchain13_n19_α:
 mov rdi, qword ptr [rbp + 1072]
 mov rsi, qword ptr [rbp + 1080]
 call rt_num_pos@PLT
 mov qword ptr [rbp + 1040], rax
 mov qword ptr [rbp + 1048], rdx
 jmp xchain13_n23_α
 xchain13_n20_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 320]
 mov rsi, qword ptr [rbp + 328]
 mov rdx, qword ptr [rbp + 464]
 mov rcx, qword ptr [rbp + 472]
 call str_concat_d@PLT
 mov qword ptr [rbp + 288], rax
 mov qword ptr [rbp + 296], rdx
 jmp xchain13_n24_α
# IR_LIT_STRING
 xchain13_n21_α:
 mov qword ptr [rbp + 544], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [rbp + 552], rax
 jmp xchain13_n25_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "PAT$0"
# IR_MATCH_HEAD
 xchain13_n22_α:
 mov qword ptr [rbp + 776], rbp
 mov rdi, qword ptr [rbp + 848]
 mov rsi, qword ptr [rbp + 856]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 768], r12
 mov qword ptr [rbp + 752], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 744], rax
 mov dword ptr [rbp + 736], 0
.Lx38_0:
 mov r14d, dword ptr [rbp + 736]
 jmp xchain13_n26_α
 xchain13_n22_β:
 add dword ptr [rbp + 736], 1
 mov eax, dword ptr [rbp + 736]
 cmp eax, r15d
 jg .Lx38_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx38_1
 jmp .Lx38_0
.Lx38_1:
 mov rax, qword ptr [rbp + 744]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 752]
 mov r12, qword ptr [rbp + 768]
 mov rbp, qword ptr [rbp + 776]
 jmp xchain13_n11_α
# IR_LIT_STRING
 xchain13_n23_α:
 mov qword ptr [rbp + 1104], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [rbp + 1112], rax
 jmp xchain13_n27_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string " words"
# IR_ASSIGN gva
 xchain13_n24_α:
 mov rax, qword ptr [rbp + 288]
 mov rdx, qword ptr [rbp + 296]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 jmp xchain13_n8_α
 xchain13_n25_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+544] -> [zr+512]
 mov rax, qword ptr [rbp + 544]
 mov qword ptr [rbp + 512], rax
 mov rax, qword ptr [rbp + 552]
 mov qword ptr [rbp + 520], rax
  .section .rodata
  .Lrkfn42: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn42]
 lea rsi, [rbp + 512]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 496], rax
 mov qword ptr [rbp + 504], rdx
 cmp eax, 99
 je xchain13_n11_α
 jmp xchain13_n28_α
 xchain13_n25_β:
 jmp xchain13_n11_α
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain13_n26_α:
 lea rsi, [rip + g_sno_defer_cells+0]
 mov rax, qword ptr [rsi + 0]
 test rax, rax
 jne .Lx43_11
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 cmp eax, 3
 jne .Lx43_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx43_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx43_10
.Lx43_9:
 xor eax, eax
.Lx43_10:
 lea rsi, [rip + g_sno_defer_cells+0]
 mov qword ptr [rsi + 0], rax
.Lx43_11:
 test rax, rax
 jz .Lx43_0
 mov r8d, 1
 lea rcx, [rip + .Lx43_4]
 lea rdx, [rip + .Lx43_5]
 jmp rax
.Lx43_4:
 lea rcx, [rip + g_scan_hit_start]
 mov rax, qword ptr [rcx]
 mov dword ptr [rbp + 736], eax
 jmp xchain13_n29_α
.Lx43_5:
 jmp xchain13_n22_β
.Lx43_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx43_2:
 test rax, rax
 je .Lx43_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx43_7]
 lea rdx, [rip + .Lx43_8]
 jmp rax
.Lx43_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx43_2
.Lx43_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx43_2
.Lx43_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain13_n22_β
 mov r14d, eax
 lea rax, [rip + .Lx43_6]
 sub rsp, 8
 push rax
 jmp xchain13_n29_α
.Lx43_6:
 add rsp, 16
 jmp xchain13_n22_β
 xchain13_n26_β:
 jmp qword ptr [rsp]
 xchain13_n27_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 1040]
 mov rsi, qword ptr [rbp + 1048]
 mov rdx, qword ptr [rbp + 1104]
 mov rcx, qword ptr [rbp + 1112]
 call str_concat_d@PLT
 mov qword ptr [rbp + 1008], rax
 mov qword ptr [rbp + 1016], rdx
 jmp xchain13_n30_α
# IR_ASSIGN gva
 xchain13_n28_α:
 mov rax, qword ptr [rbp + 496]
 mov rdx, qword ptr [rbp + 504]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 480], rax
 mov qword ptr [rbp + 488], rdx
 jmp xchain13_n11_α
# IR_MATCH_RELEASE
 xchain13_n29_α:
 mov rax, qword ptr [rbp + 744]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov qword ptr [rbp + 760], r14
 mov rsp, qword ptr [rbp + 752]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 800]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx47_1:
 test rax, rax
 je .Lx47_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx47_3]
 lea rdx, [rip + .Lx47_4]
 jmp rax
.Lx47_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx47_1
.Lx47_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx47_1
.Lx47_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 768]
 jmp xchain13_n31_α
# IR_ASSIGN global
 xchain13_n30_α:
 mov rsi, qword ptr [rbp + 1008]
 mov rdx, qword ptr [rbp + 1016]
 mov rdi, qword ptr [rip + .Lx48_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 992], rax
 mov qword ptr [rbp + 1000], rdx
 jmp main_γ
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain13_n31_α:
 mov qword ptr [rbp + 800], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [rbp + 808], rax
 jmp xchain13_n32_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string ""
# IR_MATCH_REPLACE
 xchain13_n32_α:
 mov rdi, qword ptr [rip + .Lx51_0]
 mov rsi, qword ptr [rbp + 848]
 mov rdx, qword ptr [rbp + 856]
 mov ecx, dword ptr [rbp + 736]
 mov r8, qword ptr [rbp + 760]
 lea r9, [rbp + 800]
 call rt_match_replace@PLT
 jmp .Lx51_1
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "LINE"
.Lx51_1:
 mov rbp, qword ptr [rbp + 776]
 jmp xchain13_n33_α
# IR_VAR
 xchain13_n33_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 928], rax
 mov qword ptr [rbp + 936], rdx
 jmp xchain13_n34_α
# IR_LIT_INTEGER
 xchain13_n34_α:
 mov qword ptr [rbp + 960], 6
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [rbp + 968], rax
 jmp xchain13_n35_α
.Lx53_0:
 .quad 1
 xchain13_n35_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 928]
 cmp eax, 100
 je .Lx54_0
 mov eax, dword ptr [rbp + 928]
 cmp eax, 6
 jne .Lx54_2
.Lx54_1:
 mov rax, qword ptr [rbp + 936]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rbp + 896], 6
 mov qword ptr [rbp + 904], rax
 jmp xchain13_n36_α
.Lx54_0:
 mov rdi, qword ptr [rbp + 928]
 mov rsi, qword ptr [rbp + 936]
 mov rdx, qword ptr [rbp + 960]
 mov rcx, qword ptr [rbp + 968]
 mov r8d, 0
 lea r9, [rbp + 896]
 call rt_binop_overload@PLT
 test eax, eax
 jne xchain13_n36_α
.Lx54_2:
 mov rdi, qword ptr [rbp + 928]
 mov rsi, qword ptr [rbp + 936]
 mov rdx, qword ptr [rbp + 960]
 mov rcx, qword ptr [rbp + 968]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain13_n18_α
 mov qword ptr [rbp + 896], rax
 mov qword ptr [rbp + 904], rdx
 jmp xchain13_n36_α
 xchain13_n35_β:
 jmp xchain13_n18_α
# IR_ASSIGN gva
 xchain13_n36_α:
 mov rax, qword ptr [rbp + 896]
 mov rdx, qword ptr [rbp + 904]
 mov qword ptr [1879052352], rax
 mov qword ptr [1879052360], rdx
 mov qword ptr [rbp + 880], rax
 mov qword ptr [rbp + 888], rdx
 jmp xchain13_n18_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 1144]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 1144]
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
