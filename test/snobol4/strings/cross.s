  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "H"
  .Lgvan1: .string "V"
  .Lgvan2: .string "HC"
  .Lgvan3: .string "NH"
  .Lgvan4: .string "CROSS"
  .Lgvan5: .string "VC"
  .Lgvan6: .string "NV"
  .Lgvan7: .string "PRINTV"
  .Lgvan8: .string "C"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .quad .Lgvan6
  .quad .Lgvan7
  .quad .Lgvan8
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 9
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 9
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
  mov qword ptr [rsp + 1960], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rbp + 160], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
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
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn4]
 lea rsi, [rbp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_VAR
 xchain0_n3_α:
 mov rdi, qword ptr [rip + .Lx5_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rbp + 240], rax
 mov qword ptr [rbp + 248], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp main_γ
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [rbp + 240]
 mov rdx, qword ptr [rbp + 248]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp main_γ
# IR_VAR
 xchain0_n5_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rbp + 288], rax
 mov qword ptr [rbp + 296], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp main_γ
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain0_n6_α:
 mov rax, qword ptr [rbp + 288]
 mov rdx, qword ptr [rbp + 296]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp main_γ
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 336], rax
 mov qword ptr [rbp + 344], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n8_α:
 mov rax, qword ptr [rbp + 336]
 mov rdx, qword ptr [rbp + 344]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 320], rax
 mov qword ptr [rbp + 328], rdx
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n9_α
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 656], rax
 mov qword ptr [rbp + 664], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n3_α
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 624], rax
 mov qword ptr [rbp + 632], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n3_α
# IR_COERCE_STRING
 xchain0_n11_α:
 lea rdi, [rbp + 624]
 lea rsi, [rbp + 592]
 mov rdx, 3866683
 call rt_coerce_str_d@PLT
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n3_α
# IR_MATCH_HEAD
 xchain0_n12_α:
 mov qword ptr [rbp + 424], rbp
 mov rdi, qword ptr [rbp + 656]
 mov rsi, qword ptr [rbp + 664]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 416], r12
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
 mov dword ptr [rsp + 0], 0
.Lx16_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n13_α
 xchain0_n12_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx16_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx16_1
 jmp .Lx16_0
.Lx16_1:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 mov r12, qword ptr [rbp + 416]
 mov rbp, qword ptr [rbp + 424]
 jmp xchain0_n3_α
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n13_α:
 jmp xchain0_n15_α
xchain0_n13_as:
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n17_β
xchain0_n13_af:
 jmp xchain0_n12_β
# IR_MATCH_RELEASE
 xchain0_n14_α:
 mov rax, qword ptr [rsp + 24]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov eax, dword ptr [rsp + 16]
 mov dword ptr [rsp + 432], eax
 mov qword ptr [rsp + 456], r14
 mov rsp, qword ptr [rsp + 32]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 448]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx20_1:
 test rax, rax
 je .Lx20_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx20_3]
 lea rdx, [rip + .Lx20_4]
 jmp rax
.Lx20_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx20_1
.Lx20_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx20_1
.Lx20_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 416]
 jmp xchain0_n18_α
# IR_MATCH_ATP
 xchain0_n15_α:
 mov esi, r14d
 lea rdi, [rip + .S0]
 call rt_at_cursor@PLT
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n13_af
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n16_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n19_α
 xchain0_n16_β:
 add rsp, 16
 jmp xchain0_n15_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n17_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S1]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n13_as
 xchain0_n17_β:
 sub r12, 24
 jmp xchain0_n19_β
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rbp + 448], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rbp + 456], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n3_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "*"
# IR_MATCH_ANY
 xchain0_n19_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx27_239
 add rsp, 16
 jmp xchain0_n15_β
.Lx27_239:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 mov rdi, qword ptr [rbp + 600]
 call strchr@PLT
 test rax, rax
 jne .Lx27_240
 add rsp, 16
 jmp xchain0_n15_β
.Lx27_240:
 add r14d, 1
 jmp xchain0_n17_α
 xchain0_n19_β:
 sub r14d, 1
 add rsp, 16
 jmp xchain0_n15_β
# IR_MATCH_REPLACE
 xchain0_n20_α:
 mov rdi, qword ptr [rip + .Lx29_0]
 mov rsi, qword ptr [rbp + 656]
 mov rdx, qword ptr [rbp + 664]
 mov ecx, dword ptr [rbp + 384]
 mov r8, qword ptr [rbp + 408]
 lea r9, [rbp + 448]
 call rt_match_replace@PLT
 jmp .Lx29_1
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "HC"
.Lx29_1:
 mov rbp, qword ptr [rbp + 424]
 jmp xchain0_n21_α
# IR_VAR
 xchain0_n21_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 704], rax
 mov qword ptr [rbp + 712], rdx
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n23_α
# IR_ASSIGN gva
 xchain0_n22_α:
 mov rax, qword ptr [rbp + 704]
 mov rdx, qword ptr [rbp + 712]
 mov qword ptr [1879052368], rax
 mov qword ptr [1879052376], rdx
 mov qword ptr [rbp + 688], rax
 mov qword ptr [rbp + 696], rdx
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n23_α
# IR_VAR
 xchain0_n23_α:
 mov rax, qword ptr [1879052368]
 mov rdx, qword ptr [1879052376]
 mov qword ptr [rbp + 928], rax
 mov qword ptr [rbp + 936], rdx
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp xchain0_n9_α
# IR_MATCH_HEAD
 xchain0_n24_α:
 mov qword ptr [rbp + 792], rbp
 mov rdi, qword ptr [rbp + 928]
 mov rsi, qword ptr [rbp + 936]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 784], r12
 mov qword ptr [rbp + 768], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 760], rax
 mov dword ptr [rbp + 752], 0
.Lx34_0:
 mov r14d, dword ptr [rbp + 752]
 jmp xchain0_n25_α
 xchain0_n24_β:
 add dword ptr [rbp + 752], 1
 mov eax, dword ptr [rbp + 752]
 cmp eax, r15d
 jg .Lx34_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx34_1
 jmp .Lx34_0
.Lx34_1:
 mov rax, qword ptr [rbp + 760]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 768]
 mov r12, qword ptr [rbp + 784]
 mov rbp, qword ptr [rbp + 792]
 jmp xchain0_n9_α
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain0_n25_α:
 mov dword ptr [rbp + 864], r14d
 jmp xchain0_n27_α
xchain0_n25_as:
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp xchain0_n28_β
xchain0_n25_af:
 jmp xchain0_n24_β
# IR_MATCH_RELEASE
 xchain0_n26_α:
 mov rax, qword ptr [rbp + 760]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov qword ptr [rbp + 776], r14
 mov rsp, qword ptr [rbp + 768]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 816]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx38_1:
 test rax, rax
 je .Lx38_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx38_3]
 lea rdx, [rip + .Lx38_4]
 jmp rax
.Lx38_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx38_1
.Lx38_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx38_1
.Lx38_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 784]
 jmp xchain0_n29_α
# IR_MATCH_ATP
 xchain0_n27_α:
 mov esi, r14d
 lea rdi, [rip + .S2]
 call rt_at_cursor@PLT
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n25_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain0_n28_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 cmp eax, 3
 jne .Lx40_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx40_10
.Lx40_9:
 xor eax, eax
.Lx40_10:
 test rax, rax
 jz .Lx40_0
 mov r8d, 0
 lea rcx, [rip + .Lx40_4]
 lea rdx, [rip + .Lx40_5]
 jmp rax
.Lx40_4:
 jmp xchain0_n25_as
.Lx40_5:
 jmp xchain0_n27_β
.Lx40_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx40_2:
 test rax, rax
 je .Lx40_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx40_7]
 lea rdx, [rip + .Lx40_8]
 jmp rax
.Lx40_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx40_2
.Lx40_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx40_2
.Lx40_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain0_n27_β
 mov r14d, eax
 lea rax, [rip + .Lx40_6]
 sub rsp, 8
 push rax
 jmp xchain0_n25_as
.Lx40_6:
 add rsp, 16
 jmp xchain0_n27_β
 xchain0_n28_β:
 jmp qword ptr [rsp]
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [rbp + 816], 1
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [rbp + 824], rax
 jmp xchain0_n30_α
 xchain0_n29_β:
 jmp xchain0_n9_α
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "#"
# IR_MATCH_REPLACE
 xchain0_n30_α:
 mov rdi, qword ptr [rip + .Lx43_0]
 mov rsi, qword ptr [rbp + 928]
 mov rdx, qword ptr [rbp + 936]
 mov ecx, dword ptr [rbp + 752]
 mov r8, qword ptr [rbp + 776]
 lea r9, [rbp + 816]
 call rt_match_replace@PLT
 jmp .Lx43_1
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "VC"
.Lx43_1:
 mov rbp, qword ptr [rbp + 792]
 jmp xchain0_n31_α
# IR_LIT_STRING
 xchain0_n31_α:
 mov qword ptr [rbp + 976], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [rbp + 984], rax
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp xchain0_n33_α
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string ""
# IR_ASSIGN global
 xchain0_n32_α:
 mov rsi, qword ptr [rbp + 976]
 mov rdx, qword ptr [rbp + 984]
 mov rdi, qword ptr [rip + .Lx45_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 960], rax
 mov qword ptr [rbp + 968], rdx
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n33_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string "OUTPUT"
# IR_VAR
 xchain0_n33_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 1024], rax
 mov qword ptr [rbp + 1032], rdx
 jmp xchain0_n34_α
 xchain0_n33_β:
 jmp xchain0_n35_α
# IR_ASSIGN gva
 xchain0_n34_α:
 mov rax, qword ptr [rbp + 1024]
 mov rdx, qword ptr [rbp + 1032]
 mov qword ptr [1879052400], rax
 mov qword ptr [1879052408], rdx
 mov qword ptr [rbp + 1008], rax
 mov qword ptr [rbp + 1016], rdx
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n35_α
# IR_VAR
 xchain0_n35_α:
 mov rax, qword ptr [1879052400]
 mov rdx, qword ptr [1879052408]
 mov qword ptr [rbp + 1296], rax
 mov qword ptr [rbp + 1304], rdx
 jmp xchain0_n36_α
 xchain0_n35_β:
 jmp xchain0_n37_α
# IR_VAR
 xchain0_n36_α:
 mov rax, qword ptr [1879052384]
 mov rdx, qword ptr [1879052392]
 mov qword ptr [rbp + 1264], rax
 mov qword ptr [rbp + 1272], rdx
 jmp xchain0_n38_α
 xchain0_n36_β:
 jmp xchain0_n37_α
# IR_VAR
 xchain0_n37_α:
 mov rax, qword ptr [1879052400]
 mov rdx, qword ptr [1879052408]
 mov qword ptr [rbp + 1504], rax
 mov qword ptr [rbp + 1512], rdx
 jmp xchain0_n39_α
 xchain0_n37_β:
 jmp xchain0_n23_α
# IR_COERCE_INTEGER
 xchain0_n38_α:
 lea rdi, [rbp + 1264]
 lea rsi, [rbp + 1232]
 mov rdx, 10682530
 call rt_coerce_int_d@PLT
 jmp xchain0_n40_α
 xchain0_n38_β:
 jmp xchain0_n37_α
# IR_MATCH_HEAD
 xchain0_n39_α:
 mov qword ptr [rbp + 1384], rbp
 mov rdi, qword ptr [rbp + 1504]
 mov rsi, qword ptr [rbp + 1512]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 1376], r12
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
 mov dword ptr [rsp + 0], 0
.Lx54_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n41_α
 xchain0_n39_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx54_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx54_1
 jmp .Lx54_0
.Lx54_1:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 mov r12, qword ptr [rbp + 1376]
 mov rbp, qword ptr [rbp + 1384]
 jmp xchain0_n23_α
# IR_MATCH_HEAD
 xchain0_n40_α:
 mov qword ptr [rbp + 1112], rbp
 mov rdi, qword ptr [rbp + 1296]
 mov rsi, qword ptr [rbp + 1304]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 1104], r12
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
 mov dword ptr [rsp + 0], 0
.Lx56_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n42_α
 xchain0_n40_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx56_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx56_1
 jmp .Lx56_0
.Lx56_1:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 mov r12, qword ptr [rbp + 1104]
 mov rbp, qword ptr [rbp + 1112]
 jmp xchain0_n37_α
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n41_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n43_α
 xchain0_n41_β:
 add rsp, 16
 jmp xchain0_n39_β
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n42_α:
 jmp xchain0_n45_α
xchain0_n42_as:
 jmp xchain0_n44_α
 xchain0_n42_β:
 jmp xchain0_n46_β
xchain0_n42_af:
 jmp xchain0_n40_β
# IR_MATCH_LEN
 xchain0_n43_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jle .Lx61_240
 add rsp, 16
 jmp xchain0_n39_β
.Lx61_240:
 add r14d, 1
 jmp xchain0_n47_α
 xchain0_n43_β:
 sub r14d, 1
 add rsp, 16
 jmp xchain0_n39_β
# IR_MATCH_RELEASE
 xchain0_n44_α:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov eax, dword ptr [rsp + 0]
 mov dword ptr [rsp + 1104], eax
 mov qword ptr [rsp + 1128], r14
 mov rsp, qword ptr [rsp + 16]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 1136]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx63_1:
 test rax, rax
 je .Lx63_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx63_3]
 lea rdx, [rip + .Lx63_4]
 jmp rax
.Lx63_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx63_1
.Lx63_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx63_1
.Lx63_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 1104]
 jmp xchain0_n48_α
# IR_MATCH_POS
 xchain0_n45_α:
 mov rax, qword ptr [rbp + 1240]
 cmp r14d, eax
 jne xchain0_n42_af
 jmp xchain0_n46_α
 xchain0_n45_β:
 jmp xchain0_n42_af
# IR_MATCH_LEN
 xchain0_n46_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n42_af
 add r14d, 1
 jmp xchain0_n42_as
 xchain0_n46_β:
 sub r14d, 1
 jmp xchain0_n42_af
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n47_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S3]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n49_α
 xchain0_n47_β:
 sub r12, 24
 jmp xchain0_n43_β
# IR_LIT_STRING
 xchain0_n48_α:
 mov qword ptr [rbp + 1136], 1
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [rbp + 1144], rax
 jmp xchain0_n50_α
 xchain0_n48_β:
 jmp xchain0_n37_α
.Lx68_0:
 .quad .Lx68_0_s
.Lx68_0_s:
 .string "#"
# IR_MATCH_RELEASE
 xchain0_n49_α:
 mov rax, qword ptr [rsp + 24]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov eax, dword ptr [rsp + 16]
 mov dword ptr [rsp + 1392], eax
 mov qword ptr [rsp + 1416], r14
 mov rsp, qword ptr [rsp + 32]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 1408]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx70_1:
 test rax, rax
 je .Lx70_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx70_3]
 lea rdx, [rip + .Lx70_4]
 jmp rax
.Lx70_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx70_1
.Lx70_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx70_1
.Lx70_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 1376]
 jmp xchain0_n51_α
# IR_MATCH_REPLACE
 xchain0_n50_α:
 mov rdi, qword ptr [rip + .Lx72_0]
 mov rsi, qword ptr [rbp + 1296]
 mov rdx, qword ptr [rbp + 1304]
 mov ecx, dword ptr [rbp + 1072]
 mov r8, qword ptr [rbp + 1096]
 lea r9, [rbp + 1136]
 call rt_match_replace@PLT
 jmp .Lx72_1
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string "PRINTV"
.Lx72_1:
 mov rbp, qword ptr [rbp + 1112]
 jmp xchain0_n37_α
# IR_LIT_STRING
 xchain0_n51_α:
 mov qword ptr [rbp + 1408], 1
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [rbp + 1416], rax
 jmp xchain0_n52_α
 xchain0_n51_β:
 jmp xchain0_n23_α
.Lx73_0:
 .quad .Lx73_0_s
.Lx73_0_s:
 .string ""
# IR_MATCH_REPLACE
 xchain0_n52_α:
 mov rdi, qword ptr [rip + .Lx75_0]
 mov rsi, qword ptr [rbp + 1504]
 mov rdx, qword ptr [rbp + 1512]
 mov ecx, dword ptr [rbp + 1344]
 mov r8, qword ptr [rbp + 1368]
 lea r9, [rbp + 1408]
 call rt_match_replace@PLT
 jmp .Lx75_1
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "PRINTV"
.Lx75_1:
 mov rbp, qword ptr [rbp + 1384]
 jmp xchain0_n53_α
# IR_VAR
 xchain0_n53_α:
 mov rax, qword ptr [1879052416]
 mov rdx, qword ptr [1879052424]
 mov qword ptr [rbp + 1680], rax
 mov qword ptr [rbp + 1688], rdx
 jmp xchain0_n54_α
 xchain0_n53_β:
 jmp xchain0_n55_α
# IR_LIT_STRING
 xchain0_n54_α:
 mov qword ptr [rbp + 1712], 1
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [rbp + 1720], rax
 jmp xchain0_n56_α
 xchain0_n54_β:
 jmp xchain0_n55_α
.Lx77_0:
 .quad .Lx77_0_s
.Lx77_0_s:
 .string "#"
# IR_VAR
 xchain0_n55_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 1920], rax
 mov qword ptr [rbp + 1928], rdx
 jmp xchain0_n57_α
 xchain0_n55_β:
 jmp xchain0_n37_α
 xchain0_n56_α:
# BOX IR_CALL DIFFER(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1680] -> [zr+1632]
 mov rax, qword ptr [rbp + 1680]
 mov qword ptr [rbp + 1632], rax
 mov rax, qword ptr [rbp + 1688]
 mov qword ptr [rbp + 1640], rax
# marshal arg1 = producer-box slot [zr+1712] -> [zr+1648]
 mov rax, qword ptr [rbp + 1712]
 mov qword ptr [rbp + 1648], rax
 mov rax, qword ptr [rbp + 1720]
 mov qword ptr [rbp + 1656], rax
  .section .rodata
  .Lrkfn80: .string "DIFFER"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn80]
 lea rsi, [rbp + 1632]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1616], rax
 mov qword ptr [rbp + 1624], rdx
 cmp eax, 99
 je xchain0_n55_α
 jmp xchain0_n58_α
 xchain0_n56_β:
 jmp xchain0_n55_α
# IR_ASSIGN global
 xchain0_n57_α:
 mov rsi, qword ptr [rbp + 1920]
 mov rdx, qword ptr [rbp + 1928]
 mov rdi, qword ptr [rip + .Lx81_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 1904], rax
 mov qword ptr [rbp + 1912], rdx
 jmp xchain0_n37_α
 xchain0_n57_β:
 jmp xchain0_n37_α
.Lx81_0:
 .quad .Lx81_0_s
.Lx81_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n58_α:
 mov qword ptr [rbp + 1808], 1
 mov rax, qword ptr [rip + .Lx82_0]
 mov qword ptr [rbp + 1816], rax
 jmp xchain0_n59_α
 xchain0_n58_β:
 jmp xchain0_n55_α
.Lx82_0:
 .quad .Lx82_0_s
.Lx82_0_s:
 .string " "
# IR_VAR
 xchain0_n59_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 1840], rax
 mov qword ptr [rbp + 1848], rdx
 jmp xchain0_n60_α
 xchain0_n59_β:
 jmp xchain0_n55_α
 xchain0_n60_α:
# BOX IR_CALL DUPL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1808] -> [zr+1760]
 mov rax, qword ptr [rbp + 1808]
 mov qword ptr [rbp + 1760], rax
 mov rax, qword ptr [rbp + 1816]
 mov qword ptr [rbp + 1768], rax
# marshal arg1 = producer-box slot [zr+1840] -> [zr+1776]
 mov rax, qword ptr [rbp + 1840]
 mov qword ptr [rbp + 1776], rax
 mov rax, qword ptr [rbp + 1848]
 mov qword ptr [rbp + 1784], rax
  .section .rodata
  .Lrkfn85: .string "DUPL"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn85]
 lea rsi, [rbp + 1760]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1744], rax
 mov qword ptr [rbp + 1752], rdx
 cmp eax, 99
 je xchain0_n55_α
 jmp xchain0_n61_α
 xchain0_n60_β:
 jmp xchain0_n55_α
 xchain0_n61_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 1616]
 mov rsi, qword ptr [rbp + 1624]
 mov rdx, qword ptr [rbp + 1744]
 mov rcx, qword ptr [rbp + 1752]
 call str_concat_d@PLT
 mov qword ptr [rbp + 1584], rax
 mov qword ptr [rbp + 1592], rdx
 jmp xchain0_n62_α
 xchain0_n61_β:
 jmp xchain0_n55_α
# IR_VAR
 xchain0_n62_α:
 mov rax, qword ptr [1879052416]
 mov rdx, qword ptr [1879052424]
 mov qword ptr [rbp + 1872], rax
 mov qword ptr [rbp + 1880], rdx
 jmp xchain0_n63_α
 xchain0_n62_β:
 jmp xchain0_n55_α
 xchain0_n63_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 1584]
 mov rsi, qword ptr [rbp + 1592]
 mov rdx, qword ptr [rbp + 1872]
 mov rcx, qword ptr [rbp + 1880]
 call str_concat_d@PLT
 mov qword ptr [rbp + 1552], rax
 mov qword ptr [rbp + 1560], rdx
 jmp xchain0_n64_α
 xchain0_n63_β:
 jmp xchain0_n55_α
# IR_ASSIGN global
 xchain0_n64_α:
 mov rsi, qword ptr [rbp + 1552]
 mov rdx, qword ptr [rbp + 1560]
 mov rdi, qword ptr [rip + .Lx89_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 1536], rax
 mov qword ptr [rbp + 1544], rdx
 jmp xchain0_n37_α
 xchain0_n64_β:
 jmp xchain0_n55_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 1960]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 1960]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
.section .rodata
.S0: .string "NH"
.S1: .string "CROSS"
.S2: .string "NV"
.S3: .string "C"
.text
