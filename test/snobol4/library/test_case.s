  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "PATTMP$P0"
  .Lgvan1: .string "PATTMP$P1"
  .Lgvan2: .string "PATTMP$P2"
  .Lgvan3: .string "PATTMP$P3"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 4
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 4
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
  mov qword ptr [rsp + 1656], rsp
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
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rbp + 288], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rbp + 296], rax
 jmp xchain0_n4_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "HELLO WORLD"
 xchain0_n4_α:
# BOX CALL lwr(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+288] -> [zr+256]
 mov rax, qword ptr [rbp + 288]
 mov qword ptr [rbp + 256], rax
 mov rax, qword ptr [rbp + 296]
 mov qword ptr [rbp + 264], rax
  .section .rodata
  .Lbynamefn5: .string "lwr"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn5]
 lea rsi, [rbp + 256]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 240], rax
 mov qword ptr [rbp + 248], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_ASSIGN global
 xchain0_n5_α:
 mov rsi, qword ptr [rbp + 240]
 mov rdx, qword ptr [rbp + 248]
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 jmp xchain0_n6_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rbp + 384], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 392], rax
 jmp xchain0_n7_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "hello world"
 xchain0_n7_α:
# BOX CALL upr(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+352]
 mov rax, qword ptr [rbp + 384]
 mov qword ptr [rbp + 352], rax
 mov rax, qword ptr [rbp + 392]
 mov qword ptr [rbp + 360], rax
  .section .rodata
  .Lbynamefn8: .string "upr"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [rbp + 352]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 336], rax
 mov qword ptr [rbp + 344], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_ASSIGN global
 xchain0_n8_α:
 mov rsi, qword ptr [rbp + 336]
 mov rdx, qword ptr [rbp + 344]
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 320], rax
 mov qword ptr [rbp + 328], rdx
 jmp xchain0_n9_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rbp + 480], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rbp + 488], rax
 jmp xchain0_n10_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "hELLO wORLD"
 xchain0_n10_α:
# BOX CALL cap(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+480] -> [zr+448]
 mov rax, qword ptr [rbp + 480]
 mov qword ptr [rbp + 448], rax
 mov rax, qword ptr [rbp + 488]
 mov qword ptr [rbp + 456], rax
  .section .rodata
  .Lbynamefn11: .string "cap"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [rbp + 448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n12_α
# IR_ASSIGN global
 xchain0_n11_α:
 mov rsi, qword ptr [rbp + 432]
 mov rdx, qword ptr [rbp + 440]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 416], rax
 mov qword ptr [rbp + 424], rdx
 jmp xchain0_n12_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [rbp + 704], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 712], rax
 jmp xchain0_n13_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "Hello"
# IR_MATCH_HEAD
 xchain0_n13_α:
 mov qword ptr [rbp + 568], rbp
 mov rdi, qword ptr [rbp + 704]
 mov rsi, qword ptr [rbp + 712]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 560], r12
 mov qword ptr [rbp + 544], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 536], rax
 mov dword ptr [rbp + 528], 0
.Lx16_0:
 mov r14d, dword ptr [rbp + 528]
 jmp xchain0_n14_α
 xchain0_n13_β:
 add dword ptr [rbp + 528], 1
 mov eax, dword ptr [rbp + 528]
 cmp eax, r15d
 jg .Lx16_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx16_1
 jmp .Lx16_0
.Lx16_1:
 mov rax, qword ptr [rbp + 536]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 544]
 mov r12, qword ptr [rbp + 560]
 mov rbp, qword ptr [rbp + 568]
 jmp xchain0_n21_α
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rbp + 672], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rbp + 680], rax
 jmp xchain0_n15_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "hello"
 xchain0_n15_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+672] -> [zr+640]
 mov rax, qword ptr [rbp + 672]
 mov qword ptr [rbp + 640], rax
 mov rax, qword ptr [rbp + 680]
 mov qword ptr [rbp + 648], rax
  .section .rodata
  .Lbynamefn16: .string "icase"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn16]
 lea rsi, [rbp + 640]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 624], rax
 mov qword ptr [rbp + 632], rdx
 cmp eax, 99
 je xchain0_n13_α
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n13_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [rbp + 624]
 mov rdx, qword ptr [rbp + 632]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 608], rax
 mov qword ptr [rbp + 616], rdx
 jmp xchain0_n17_α
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain0_n17_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 cmp eax, 3
 jne .Lx20_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx20_10
.Lx20_9:
 xor eax, eax
.Lx20_10:
 test rax, rax
 jz .Lx20_0
 mov r8d, 0
 lea rcx, [rip + .Lx20_4]
 lea rdx, [rip + .Lx20_5]
 jmp rax
.Lx20_4:
 jmp xchain0_n18_α
.Lx20_5:
 jmp xchain0_n13_β
.Lx20_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx20_2:
 test rax, rax
 je .Lx20_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx20_7]
 lea rdx, [rip + .Lx20_8]
 jmp rax
.Lx20_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx20_2
.Lx20_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx20_2
.Lx20_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain0_n13_β
 mov r14d, eax
 lea rax, [rip + .Lx20_6]
 sub rsp, 8
 push rax
 jmp xchain0_n18_α
.Lx20_6:
 add rsp, 16
 jmp xchain0_n13_β
 xchain0_n17_β:
 jmp qword ptr [rsp]
# IR_MATCH_RELEASE
 xchain0_n18_α:
 mov rax, qword ptr [rbp + 536]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 544]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 592]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx22_1:
 test rax, rax
 je .Lx22_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx22_3]
 lea rdx, [rip + .Lx22_4]
 jmp rax
.Lx22_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx22_1
.Lx22_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx22_1
.Lx22_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 560]
 mov rbp, qword ptr [rbp + 568]
 jmp xchain0_n19_α
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [rbp + 752], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [rbp + 760], rax
 jmp xchain0_n20_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "ok: icase hello"
# IR_ASSIGN global
 xchain0_n20_α:
 mov rsi, qword ptr [rbp + 752]
 mov rdx, qword ptr [rbp + 760]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 736], rax
 mov qword ptr [rbp + 744], rdx
 jmp xchain0_n21_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [rbp + 976], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rbp + 984], rax
 jmp xchain0_n22_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "HELLO"
# IR_MATCH_HEAD
 xchain0_n22_α:
 mov qword ptr [rbp + 840], rbp
 mov rdi, qword ptr [rbp + 976]
 mov rsi, qword ptr [rbp + 984]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 832], r12
 mov qword ptr [rbp + 816], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 808], rax
 mov dword ptr [rbp + 800], 0
.Lx27_0:
 mov r14d, dword ptr [rbp + 800]
 jmp xchain0_n23_α
 xchain0_n22_β:
 add dword ptr [rbp + 800], 1
 mov eax, dword ptr [rbp + 800]
 cmp eax, r15d
 jg .Lx27_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx27_1
 jmp .Lx27_0
.Lx27_1:
 mov rax, qword ptr [rbp + 808]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 816]
 mov r12, qword ptr [rbp + 832]
 mov rbp, qword ptr [rbp + 840]
 jmp xchain0_n30_α
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [rbp + 944], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [rbp + 952], rax
 jmp xchain0_n24_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "hello"
 xchain0_n24_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+944] -> [zr+912]
 mov rax, qword ptr [rbp + 944]
 mov qword ptr [rbp + 912], rax
 mov rax, qword ptr [rbp + 952]
 mov qword ptr [rbp + 920], rax
  .section .rodata
  .Lbynamefn25: .string "icase"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn25]
 lea rsi, [rbp + 912]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 896], rax
 mov qword ptr [rbp + 904], rdx
 cmp eax, 99
 je xchain0_n22_α
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp xchain0_n22_α
# IR_ASSIGN gva
 xchain0_n25_α:
 mov rax, qword ptr [rbp + 896]
 mov rdx, qword ptr [rbp + 904]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 880], rax
 mov qword ptr [rbp + 888], rdx
 jmp xchain0_n26_α
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain0_n26_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 cmp eax, 3
 jne .Lx31_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx31_10
.Lx31_9:
 xor eax, eax
.Lx31_10:
 test rax, rax
 jz .Lx31_0
 mov r8d, 0
 lea rcx, [rip + .Lx31_4]
 lea rdx, [rip + .Lx31_5]
 jmp rax
.Lx31_4:
 jmp xchain0_n27_α
.Lx31_5:
 jmp xchain0_n22_β
.Lx31_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx31_2:
 test rax, rax
 je .Lx31_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx31_7]
 lea rdx, [rip + .Lx31_8]
 jmp rax
.Lx31_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx31_2
.Lx31_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx31_2
.Lx31_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain0_n22_β
 mov r14d, eax
 lea rax, [rip + .Lx31_6]
 sub rsp, 8
 push rax
 jmp xchain0_n27_α
.Lx31_6:
 add rsp, 16
 jmp xchain0_n22_β
 xchain0_n26_β:
 jmp qword ptr [rsp]
# IR_MATCH_RELEASE
 xchain0_n27_α:
 mov rax, qword ptr [rbp + 808]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 816]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 864]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx33_1:
 test rax, rax
 je .Lx33_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx33_3]
 lea rdx, [rip + .Lx33_4]
 jmp rax
.Lx33_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx33_1
.Lx33_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx33_1
.Lx33_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 832]
 mov rbp, qword ptr [rbp + 840]
 jmp xchain0_n28_α
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [rbp + 1024], 1
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [rbp + 1032], rax
 jmp xchain0_n29_α
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "ok: icase HELLO"
# IR_ASSIGN global
 xchain0_n29_α:
 mov rsi, qword ptr [rbp + 1024]
 mov rdx, qword ptr [rbp + 1032]
 mov rdi, qword ptr [rip + .Lx35_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 1008], rax
 mov qword ptr [rbp + 1016], rdx
 jmp xchain0_n30_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n30_α:
 mov qword ptr [rbp + 1248], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [rbp + 1256], rax
 jmp xchain0_n31_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "HeLLo"
# IR_MATCH_HEAD
 xchain0_n31_α:
 mov qword ptr [rbp + 1112], rbp
 mov rdi, qword ptr [rbp + 1248]
 mov rsi, qword ptr [rbp + 1256]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 1104], r12
 mov qword ptr [rbp + 1088], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 1080], rax
 mov dword ptr [rbp + 1072], 0
.Lx38_0:
 mov r14d, dword ptr [rbp + 1072]
 jmp xchain0_n32_α
 xchain0_n31_β:
 add dword ptr [rbp + 1072], 1
 mov eax, dword ptr [rbp + 1072]
 cmp eax, r15d
 jg .Lx38_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx38_1
 jmp .Lx38_0
.Lx38_1:
 mov rax, qword ptr [rbp + 1080]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 1088]
 mov r12, qword ptr [rbp + 1104]
 mov rbp, qword ptr [rbp + 1112]
 jmp xchain0_n39_α
# IR_LIT_STRING
 xchain0_n32_α:
 mov qword ptr [rbp + 1216], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [rbp + 1224], rax
 jmp xchain0_n33_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "hello"
 xchain0_n33_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1216] -> [zr+1184]
 mov rax, qword ptr [rbp + 1216]
 mov qword ptr [rbp + 1184], rax
 mov rax, qword ptr [rbp + 1224]
 mov qword ptr [rbp + 1192], rax
  .section .rodata
  .Lbynamefn34: .string "icase"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn34]
 lea rsi, [rbp + 1184]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1168], rax
 mov qword ptr [rbp + 1176], rdx
 cmp eax, 99
 je xchain0_n31_α
 jmp xchain0_n34_α
 xchain0_n33_β:
 jmp xchain0_n31_α
# IR_ASSIGN gva
 xchain0_n34_α:
 mov rax, qword ptr [rbp + 1168]
 mov rdx, qword ptr [rbp + 1176]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 1152], rax
 mov qword ptr [rbp + 1160], rdx
 jmp xchain0_n35_α
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain0_n35_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 cmp eax, 3
 jne .Lx42_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx42_10
.Lx42_9:
 xor eax, eax
.Lx42_10:
 test rax, rax
 jz .Lx42_0
 mov r8d, 0
 lea rcx, [rip + .Lx42_4]
 lea rdx, [rip + .Lx42_5]
 jmp rax
.Lx42_4:
 jmp xchain0_n36_α
.Lx42_5:
 jmp xchain0_n31_β
.Lx42_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx42_2:
 test rax, rax
 je .Lx42_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx42_7]
 lea rdx, [rip + .Lx42_8]
 jmp rax
.Lx42_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx42_2
.Lx42_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx42_2
.Lx42_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain0_n31_β
 mov r14d, eax
 lea rax, [rip + .Lx42_6]
 sub rsp, 8
 push rax
 jmp xchain0_n36_α
.Lx42_6:
 add rsp, 16
 jmp xchain0_n31_β
 xchain0_n35_β:
 jmp qword ptr [rsp]
# IR_MATCH_RELEASE
 xchain0_n36_α:
 mov rax, qword ptr [rbp + 1080]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 1088]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 1136]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx44_1:
 test rax, rax
 je .Lx44_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx44_3]
 lea rdx, [rip + .Lx44_4]
 jmp rax
.Lx44_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx44_1
.Lx44_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx44_1
.Lx44_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 1104]
 mov rbp, qword ptr [rbp + 1112]
 jmp xchain0_n37_α
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [rbp + 1296], 1
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [rbp + 1304], rax
 jmp xchain0_n38_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string "ok: icase HeLLo"
# IR_ASSIGN global
 xchain0_n38_α:
 mov rsi, qword ptr [rbp + 1296]
 mov rdx, qword ptr [rbp + 1304]
 mov rdi, qword ptr [rip + .Lx46_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 1280], rax
 mov qword ptr [rbp + 1288], rdx
 jmp xchain0_n39_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n39_α:
 mov qword ptr [rbp + 1520], 1
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [rbp + 1528], rax
 jmp xchain0_n40_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "world"
# IR_MATCH_HEAD
 xchain0_n40_α:
 mov qword ptr [rbp + 1384], rbp
 mov rdi, qword ptr [rbp + 1520]
 mov rsi, qword ptr [rbp + 1528]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 1376], r12
 mov qword ptr [rbp + 1360], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 1352], rax
 mov dword ptr [rbp + 1344], 0
.Lx49_0:
 mov r14d, dword ptr [rbp + 1344]
 jmp xchain0_n41_α
 xchain0_n40_β:
 add dword ptr [rbp + 1344], 1
 mov eax, dword ptr [rbp + 1344]
 cmp eax, r15d
 jg .Lx49_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx49_1
 jmp .Lx49_0
.Lx49_1:
 mov rax, qword ptr [rbp + 1352]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 1360]
 mov r12, qword ptr [rbp + 1376]
 mov rbp, qword ptr [rbp + 1384]
 jmp xchain0_n48_α
# IR_LIT_STRING
 xchain0_n41_α:
 mov qword ptr [rbp + 1488], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [rbp + 1496], rax
 jmp xchain0_n42_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "hello"
 xchain0_n42_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1488] -> [zr+1456]
 mov rax, qword ptr [rbp + 1488]
 mov qword ptr [rbp + 1456], rax
 mov rax, qword ptr [rbp + 1496]
 mov qword ptr [rbp + 1464], rax
  .section .rodata
  .Lbynamefn43: .string "icase"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn43]
 lea rsi, [rbp + 1456]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1440], rax
 mov qword ptr [rbp + 1448], rdx
 cmp eax, 99
 je xchain0_n40_α
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp xchain0_n40_α
# IR_ASSIGN gva
 xchain0_n43_α:
 mov rax, qword ptr [rbp + 1440]
 mov rdx, qword ptr [rbp + 1448]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rbp + 1424], rax
 mov qword ptr [rbp + 1432], rdx
 jmp xchain0_n44_α
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain0_n44_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 cmp eax, 3
 jne .Lx53_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx53_10
.Lx53_9:
 xor eax, eax
.Lx53_10:
 test rax, rax
 jz .Lx53_0
 mov r8d, 0
 lea rcx, [rip + .Lx53_4]
 lea rdx, [rip + .Lx53_5]
 jmp rax
.Lx53_4:
 jmp xchain0_n45_α
.Lx53_5:
 jmp xchain0_n40_β
.Lx53_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S3]
 xor esi, esi
 call rt_defer_open@PLT
.Lx53_2:
 test rax, rax
 je .Lx53_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx53_7]
 lea rdx, [rip + .Lx53_8]
 jmp rax
.Lx53_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx53_2
.Lx53_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx53_2
.Lx53_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain0_n40_β
 mov r14d, eax
 lea rax, [rip + .Lx53_6]
 sub rsp, 8
 push rax
 jmp xchain0_n45_α
.Lx53_6:
 add rsp, 16
 jmp xchain0_n40_β
 xchain0_n44_β:
 jmp qword ptr [rsp]
# IR_MATCH_RELEASE
 xchain0_n45_α:
 mov rax, qword ptr [rbp + 1352]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 1360]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 1408]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx55_1:
 test rax, rax
 je .Lx55_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx55_3]
 lea rdx, [rip + .Lx55_4]
 jmp rax
.Lx55_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx55_1
.Lx55_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx55_1
.Lx55_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 1376]
 mov rbp, qword ptr [rbp + 1384]
 jmp xchain0_n46_α
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [rbp + 1616], 1
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [rbp + 1624], rax
 jmp xchain0_n47_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "FAIL: icase matched wrong string"
# IR_ASSIGN global
 xchain0_n47_α:
 mov rsi, qword ptr [rbp + 1616]
 mov rdx, qword ptr [rbp + 1624]
 mov rdi, qword ptr [rip + .Lx57_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 1600], rax
 mov qword ptr [rbp + 1608], rdx
 jmp main_γ
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n48_α:
 mov qword ptr [rbp + 1568], 1
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [rbp + 1576], rax
 jmp xchain0_n49_α
.Lx58_0:
 .quad .Lx58_0_s
.Lx58_0_s:
 .string "no match ok"
# IR_ASSIGN global
 xchain0_n49_α:
 mov rsi, qword ptr [rbp + 1568]
 mov rdx, qword ptr [rbp + 1576]
 mov rdi, qword ptr [rip + .Lx59_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 1552], rax
 mov qword ptr [rbp + 1560], rdx
 jmp main_γ
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 1656]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 1656]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
.section .rodata
.S0: .string "PATTMP$P0"
.S1: .string "PATTMP$P1"
.S2: .string "PATTMP$P2"
.S3: .string "PATTMP$P3"
.text
