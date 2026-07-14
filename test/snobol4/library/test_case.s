  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "PATTMP$0"
  .Lgvan1: .string "PATTMP$1"
  .Lgvan2: .string "PATTMP$2"
  .Lgvan3: .string "PATTMP$3"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .bss
  .align 16
__gva: .space 64, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 4
  call gva_register@PLT
  mov rbx, rax
  sub rsp, 65536
  mov rdi, rsp
  mov ecx, 8192
  xor eax, eax
  rep stosq
  mov rdi, rsp
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 65536
  add rsp, 24
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
  mov qword ptr [r12 + 1656], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 1648], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n6_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "HELLO WORLD"
 xchain0_n4_α:
# BOX CALL lwr(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+288] -> [zr+256]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 264], rax
  .section .rodata
  .Lbynamefn5: .string "lwr"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn5]
 lea rsi, [r12 + 256]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_ASSIGN global
 xchain0_n5_α:
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n9_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "hello world"
 xchain0_n7_α:
# BOX CALL upr(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+352]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 360], rax
  .section .rodata
  .Lbynamefn8: .string "upr"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 352]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_ASSIGN global
 xchain0_n8_α:
 mov rsi, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n9_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n12_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "hELLO wORLD"
 xchain0_n10_α:
# BOX CALL cap(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+480] -> [zr+448]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lbynamefn11: .string "cap"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n12_α
# IR_ASSIGN global
 xchain0_n11_α:
 mov rsi, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n12_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n15_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "hello"
 xchain0_n13_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+576] -> [zr+544]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 552], rax
  .section .rodata
  .Lbynamefn14: .string "icase"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn14]
 lea rsi, [r12 + 544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je xchain0_n15_α
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n15_α
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n15_α
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [r12 + 848], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n20_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [r12 + 704], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n15_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "Hello"
 xchain0_n17_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+848] -> [zr+816]
 mov rax, qword ptr [r12 + 848]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 856]
 mov qword ptr [r12 + 824], rax
  .section .rodata
  .Lbynamefn18: .string "icase"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 816]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n20_α
# IR_MATCH_HEAD
 xchain0_n18_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 632], rax
 mov qword ptr [r12 + 640], rsp
 mov rdi, qword ptr [r12 + 704]
 mov rsi, qword ptr [r12 + 712]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [r12 + 664], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [r12 + 656], rbp
 mov dword ptr [r12 + 624], 0
.Lx21_0:
 mov r14d, dword ptr [r12 + 624]
 jmp xchain0_n21_α
 xchain0_n18_β:
 add dword ptr [r12 + 624], 1
 mov eax, dword ptr [r12 + 624]
 cmp eax, r15d
 jg .Lx21_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx21_1
 jmp .Lx21_0
.Lx21_1:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 632]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 640]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 664]
 jmp xchain0_n15_α
# IR_ASSIGN gva
 xchain0_n19_α:
 mov rax, qword ptr [r12 + 800]
 mov rdx, qword ptr [r12 + 808]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n20_α
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 1120], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp xchain0_n27_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "hello"
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain0_n21_α:
 lea rdi, [rip + .S0]
 xor esi, esi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, [rsp + 8]
 test rax, rax
 jz .Lx24_0
 lea rcx, [rip + .Lx24_4]
 lea rdx, [rip + .Lx24_5]
 jmp rax
.Lx24_4:
 jmp xchain0_n24_α
.Lx24_5:
 jmp xchain0_n18_β
.Lx24_0:
 push r14
 push r15
 push r13
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 lea rdi, [rip + .S0]
 xor esi, esi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_open@PLT
.Lx24_2:
 test rax, rax
 je .Lx24_3
 mov rcx, rsp
 sub rsp, rax
 sub rsp, 16
 and rsp, -16
 mov qword ptr [rsp + 0], rcx
 mov rdi, rsp
 add rdi, 16
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 add rdi, 16
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 mov rsp, qword ptr [rsp + 0]
 call rt_defer_step@PLT
 jmp .Lx24_2
.Lx24_3:
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, [rsp + 8]
 test eax, eax
 js xchain0_n18_β
 mov r14d, eax
 lea rax, [rip + .Lx24_6]
 sub rsp, 8
 push rax
 jmp xchain0_n24_α
.Lx24_6:
 add rsp, 16
 jmp xchain0_n18_β
 xchain0_n21_β:
 jmp qword ptr [rsp]
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 976], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n25_α
 xchain0_n22_β:
 jmp xchain0_n20_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "HELLO"
 xchain0_n23_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1120] -> [zr+1088]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1088], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1096], rax
  .section .rodata
  .Lbynamefn24: .string "icase"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn24]
 lea rsi, [r12 + 1088]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 cmp eax, 99
 je xchain0_n27_α
 jmp xchain0_n26_α
 xchain0_n23_β:
 jmp xchain0_n27_α
# IR_MATCH_RELEASE
 xchain0_n24_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 632]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 640]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 push r14
 push r15
 push r13
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [r12 + 656]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx28_1:
 test rax, rax
 je .Lx28_2
 mov rcx, rsp
 sub rsp, rax
 sub rsp, 16
 and rsp, -16
 mov qword ptr [rsp + 0], rcx
 mov rdi, rsp
 add rdi, 16
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 add rdi, 16
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 mov rsp, qword ptr [rsp + 0]
 call rt_dcap_step@PLT
 jmp .Lx28_1
.Lx28_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 664]
 jmp xchain0_n28_α
# IR_MATCH_HEAD
 xchain0_n25_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 904], rax
 mov qword ptr [r12 + 912], rsp
 mov rdi, qword ptr [r12 + 976]
 mov rsi, qword ptr [r12 + 984]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [r12 + 936], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [r12 + 928], rbp
 mov dword ptr [r12 + 896], 0
.Lx30_0:
 mov r14d, dword ptr [r12 + 896]
 jmp xchain0_n29_α
 xchain0_n25_β:
 add dword ptr [r12 + 896], 1
 mov eax, dword ptr [r12 + 896]
 cmp eax, r15d
 jg .Lx30_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx30_1
 jmp .Lx30_0
.Lx30_1:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 904]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 912]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 936]
 jmp xchain0_n20_α
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [r12 + 1072]
 mov rdx, qword ptr [r12 + 1080]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
 jmp xchain0_n30_α
 xchain0_n26_β:
 jmp xchain0_n27_α
# IR_LIT_STRING
 xchain0_n27_α:
 mov qword ptr [r12 + 1392], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 1400], rax
 jmp xchain0_n31_α
 xchain0_n27_β:
 jmp xchain0_n36_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 752], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain0_n32_α
 xchain0_n28_β:
 jmp xchain0_n15_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "ok: icase hello"
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain0_n29_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, [rsp + 8]
 test rax, rax
 jz .Lx34_0
 lea rcx, [rip + .Lx34_4]
 lea rdx, [rip + .Lx34_5]
 jmp rax
.Lx34_4:
 jmp xchain0_n33_α
.Lx34_5:
 jmp xchain0_n25_β
.Lx34_0:
 push r14
 push r15
 push r13
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 lea rdi, [rip + .S1]
 xor esi, esi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_open@PLT
.Lx34_2:
 test rax, rax
 je .Lx34_3
 mov rcx, rsp
 sub rsp, rax
 sub rsp, 16
 and rsp, -16
 mov qword ptr [rsp + 0], rcx
 mov rdi, rsp
 add rdi, 16
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 add rdi, 16
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 mov rsp, qword ptr [rsp + 0]
 call rt_defer_step@PLT
 jmp .Lx34_2
.Lx34_3:
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, [rsp + 8]
 test eax, eax
 js xchain0_n25_β
 mov r14d, eax
 lea rax, [rip + .Lx34_6]
 sub rsp, 8
 push rax
 jmp xchain0_n33_α
.Lx34_6:
 add rsp, 16
 jmp xchain0_n25_β
 xchain0_n29_β:
 jmp qword ptr [rsp]
# IR_LIT_STRING
 xchain0_n30_α:
 mov qword ptr [r12 + 1248], 1
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 1256], rax
 jmp xchain0_n34_α
 xchain0_n30_β:
 jmp xchain0_n27_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "HeLLo"
 xchain0_n31_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1392] -> [zr+1360]
 mov rax, qword ptr [r12 + 1392]
 mov qword ptr [r12 + 1360], rax
 mov rax, qword ptr [r12 + 1400]
 mov qword ptr [r12 + 1368], rax
  .section .rodata
  .Lbynamefn32: .string "icase"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn32]
 lea rsi, [r12 + 1360]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 cmp eax, 99
 je xchain0_n36_α
 jmp xchain0_n35_α
 xchain0_n31_β:
 jmp xchain0_n36_α
# IR_ASSIGN global
 xchain0_n32_α:
 mov rsi, qword ptr [r12 + 752]
 mov rdx, qword ptr [r12 + 760]
 mov rdi, qword ptr [rip + .Lx37_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp xchain0_n15_α
 xchain0_n32_β:
 jmp xchain0_n15_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n33_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 904]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 912]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 push r14
 push r15
 push r13
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [r12 + 928]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx39_1:
 test rax, rax
 je .Lx39_2
 mov rcx, rsp
 sub rsp, rax
 sub rsp, 16
 and rsp, -16
 mov qword ptr [rsp + 0], rcx
 mov rdi, rsp
 add rdi, 16
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 add rdi, 16
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 mov rsp, qword ptr [rsp + 0]
 call rt_dcap_step@PLT
 jmp .Lx39_1
.Lx39_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 936]
 jmp xchain0_n37_α
# IR_MATCH_HEAD
 xchain0_n34_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 1176], rax
 mov qword ptr [r12 + 1184], rsp
 mov rdi, qword ptr [r12 + 1248]
 mov rsi, qword ptr [r12 + 1256]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [r12 + 1208], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [r12 + 1200], rbp
 mov dword ptr [r12 + 1168], 0
.Lx41_0:
 mov r14d, dword ptr [r12 + 1168]
 jmp xchain0_n38_α
 xchain0_n34_β:
 add dword ptr [r12 + 1168], 1
 mov eax, dword ptr [r12 + 1168]
 cmp eax, r15d
 jg .Lx41_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx41_1
 jmp .Lx41_0
.Lx41_1:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1176]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 1184]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 1200]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 1208]
 jmp xchain0_n27_α
# IR_ASSIGN gva
 xchain0_n35_α:
 mov rax, qword ptr [r12 + 1344]
 mov rdx, qword ptr [r12 + 1352]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 1328], rax
 mov qword ptr [r12 + 1336], rdx
 jmp xchain0_n39_α
 xchain0_n35_β:
 jmp xchain0_n36_α
# IR_LIT_STRING
 xchain0_n36_α:
 mov qword ptr [r12 + 1568], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 1576], rax
 jmp xchain0_n40_α
 xchain0_n36_β:
 jmp main_γ
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "no match ok"
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [r12 + 1024], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 1032], rax
 jmp xchain0_n41_α
 xchain0_n37_β:
 jmp xchain0_n20_α
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "ok: icase HELLO"
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain0_n38_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, [rsp + 8]
 test rax, rax
 jz .Lx45_0
 lea rcx, [rip + .Lx45_4]
 lea rdx, [rip + .Lx45_5]
 jmp rax
.Lx45_4:
 jmp xchain0_n42_α
.Lx45_5:
 jmp xchain0_n34_β
.Lx45_0:
 push r14
 push r15
 push r13
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 lea rdi, [rip + .S2]
 xor esi, esi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_open@PLT
.Lx45_2:
 test rax, rax
 je .Lx45_3
 mov rcx, rsp
 sub rsp, rax
 sub rsp, 16
 and rsp, -16
 mov qword ptr [rsp + 0], rcx
 mov rdi, rsp
 add rdi, 16
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 add rdi, 16
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 mov rsp, qword ptr [rsp + 0]
 call rt_defer_step@PLT
 jmp .Lx45_2
.Lx45_3:
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, [rsp + 8]
 test eax, eax
 js xchain0_n34_β
 mov r14d, eax
 lea rax, [rip + .Lx45_6]
 sub rsp, 8
 push rax
 jmp xchain0_n42_α
.Lx45_6:
 add rsp, 16
 jmp xchain0_n34_β
 xchain0_n38_β:
 jmp qword ptr [rsp]
# IR_LIT_STRING
 xchain0_n39_α:
 mov qword ptr [r12 + 1520], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 1528], rax
 jmp xchain0_n43_α
 xchain0_n39_β:
 jmp xchain0_n36_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "world"
# IR_ASSIGN global
 xchain0_n40_α:
 mov rsi, qword ptr [r12 + 1568]
 mov rdx, qword ptr [r12 + 1576]
 mov rdi, qword ptr [rip + .Lx47_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1552], rax
 mov qword ptr [r12 + 1560], rdx
 jmp main_γ
 xchain0_n40_β:
 jmp main_γ
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n41_α:
 mov rsi, qword ptr [r12 + 1024]
 mov rdx, qword ptr [r12 + 1032]
 mov rdi, qword ptr [rip + .Lx48_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp xchain0_n20_α
 xchain0_n41_β:
 jmp xchain0_n20_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n42_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1176]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 1184]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 push r14
 push r15
 push r13
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [r12 + 1200]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx50_1:
 test rax, rax
 je .Lx50_2
 mov rcx, rsp
 sub rsp, rax
 sub rsp, 16
 and rsp, -16
 mov qword ptr [rsp + 0], rcx
 mov rdi, rsp
 add rdi, 16
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 add rdi, 16
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 mov rsp, qword ptr [rsp + 0]
 call rt_dcap_step@PLT
 jmp .Lx50_1
.Lx50_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 1200]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 1208]
 jmp xchain0_n44_α
# IR_MATCH_HEAD
 xchain0_n43_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 1448], rax
 mov qword ptr [r12 + 1456], rsp
 mov rdi, qword ptr [r12 + 1520]
 mov rsi, qword ptr [r12 + 1528]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [r12 + 1480], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [r12 + 1472], rbp
 mov dword ptr [r12 + 1440], 0
.Lx52_0:
 mov r14d, dword ptr [r12 + 1440]
 jmp xchain0_n45_α
 xchain0_n43_β:
 add dword ptr [r12 + 1440], 1
 mov eax, dword ptr [r12 + 1440]
 cmp eax, r15d
 jg .Lx52_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx52_1
 jmp .Lx52_0
.Lx52_1:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1448]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 1456]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 1472]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 1480]
 jmp xchain0_n36_α
# IR_LIT_STRING
 xchain0_n44_α:
 mov qword ptr [r12 + 1296], 1
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain0_n46_α
 xchain0_n44_β:
 jmp xchain0_n27_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "ok: icase HeLLo"
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain0_n45_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, [rsp + 8]
 test rax, rax
 jz .Lx54_0
 lea rcx, [rip + .Lx54_4]
 lea rdx, [rip + .Lx54_5]
 jmp rax
.Lx54_4:
 jmp xchain0_n47_α
.Lx54_5:
 jmp xchain0_n43_β
.Lx54_0:
 push r14
 push r15
 push r13
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 lea rdi, [rip + .S3]
 xor esi, esi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_open@PLT
.Lx54_2:
 test rax, rax
 je .Lx54_3
 mov rcx, rsp
 sub rsp, rax
 sub rsp, 16
 and rsp, -16
 mov qword ptr [rsp + 0], rcx
 mov rdi, rsp
 add rdi, 16
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 add rdi, 16
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 mov rsp, qword ptr [rsp + 0]
 call rt_defer_step@PLT
 jmp .Lx54_2
.Lx54_3:
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, [rsp + 8]
 test eax, eax
 js xchain0_n43_β
 mov r14d, eax
 lea rax, [rip + .Lx54_6]
 sub rsp, 8
 push rax
 jmp xchain0_n47_α
.Lx54_6:
 add rsp, 16
 jmp xchain0_n43_β
 xchain0_n45_β:
 jmp qword ptr [rsp]
# IR_ASSIGN global
 xchain0_n46_α:
 mov rsi, qword ptr [r12 + 1296]
 mov rdx, qword ptr [r12 + 1304]
 mov rdi, qword ptr [rip + .Lx55_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 jmp xchain0_n27_α
 xchain0_n46_β:
 jmp xchain0_n27_α
.Lx55_0:
 .quad .Lx55_0_s
.Lx55_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n47_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1448]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 1456]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 push r14
 push r15
 push r13
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [r12 + 1472]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx57_1:
 test rax, rax
 je .Lx57_2
 mov rcx, rsp
 sub rsp, rax
 sub rsp, 16
 and rsp, -16
 mov qword ptr [rsp + 0], rcx
 mov rdi, rsp
 add rdi, 16
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 add rdi, 16
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 mov rsp, qword ptr [rsp + 0]
 call rt_dcap_step@PLT
 jmp .Lx57_1
.Lx57_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 1472]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 1480]
 jmp xchain0_n48_α
# IR_LIT_STRING
 xchain0_n48_α:
 mov qword ptr [r12 + 1616], 1
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 1624], rax
 jmp xchain0_n49_α
 xchain0_n48_β:
 jmp main_γ
.Lx58_0:
 .quad .Lx58_0_s
.Lx58_0_s:
 .string "FAIL: icase matched wrong string"
# IR_ASSIGN global
 xchain0_n49_α:
 mov rsi, qword ptr [r12 + 1616]
 mov rdx, qword ptr [r12 + 1624]
 mov rdi, qword ptr [rip + .Lx59_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1600], rax
 mov qword ptr [r12 + 1608], rdx
 jmp main_γ
 xchain0_n49_β:
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
mov rsp, qword ptr [r12 + 1656]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1648]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 1656]
pop r12
ret
.section .rodata
.S0: .string "PATTMP$0"
.S1: .string "PATTMP$1"
.S2: .string "PATTMP$2"
.S3: .string "PATTMP$3"
.text
