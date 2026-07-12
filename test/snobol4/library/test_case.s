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
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 4
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
 mov qword ptr [r12 + 1584], rax
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
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n20_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [r12 + 688], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n15_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "Hello"
 xchain0_n17_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+832] -> [zr+800]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 800], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 808], rax
  .section .rodata
  .Lbynamefn18: .string "icase"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 800]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
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
 mov rdi, qword ptr [r12 + 688]
 mov rsi, qword ptr [r12 + 696]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
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
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 632]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 640]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n15_α
# IR_ASSIGN gva
 xchain0_n19_α:
 mov rax, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n20_α
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 1088], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 1096], rax
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp xchain0_n27_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "hello"
# IR_MATCH_DEFER
 xchain0_n21_α:
 lea rdi, [rip + .S0]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 672], rcx
 test rax, rax
 jz .Lx24_0
 mov qword ptr [r12 + 672], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 680], rax
 mov rcx, qword ptr [r12 + 672]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx24_1
 mov rdi, qword ptr [r12 + 680]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 672], rax
 jmp xchain0_n18_β
.Lx24_1:
 jmp xchain0_n24_α
.Lx24_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S0]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx24_2:
 test rax, rax
 je .Lx24_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx24_2
.Lx24_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain0_n18_β
 mov r14d, eax
 jmp xchain0_n24_α
 xchain0_n21_β:
 mov rcx, qword ptr [r12 + 672]
 test rcx, rcx
 jz xchain0_n18_β
 mov rdi, qword ptr [r12 + 680]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx24_1
 mov rdi, qword ptr [r12 + 680]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 672], rax
 jmp xchain0_n18_β
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 944], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n25_α
 xchain0_n22_β:
 jmp xchain0_n20_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "HELLO"
 xchain0_n23_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1088] -> [zr+1056]
 mov rax, qword ptr [r12 + 1088]
 mov qword ptr [r12 + 1056], rax
 mov rax, qword ptr [r12 + 1096]
 mov qword ptr [r12 + 1064], rax
  .section .rodata
  .Lbynamefn24: .string "icase"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn24]
 lea rsi, [r12 + 1056]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 cmp eax, 99
 je xchain0_n27_α
 jmp xchain0_n26_α
 xchain0_n23_β:
 jmp xchain0_n27_α
# IR_MATCH_RELEASE
 xchain0_n24_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 632]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 640]
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rsp, rbp
 pop rbp
 push r14
 push r15
 push r13
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok_open@PLT
.Lx28_1:
 test rax, rax
 je .Lx28_2
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx28_1
.Lx28_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain0_n28_α
# IR_MATCH_HEAD
 xchain0_n25_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 888], rax
 mov qword ptr [r12 + 896], rsp
 mov rdi, qword ptr [r12 + 944]
 mov rsi, qword ptr [r12 + 952]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 880], 0
.Lx30_0:
 mov r14d, dword ptr [r12 + 880]
 jmp xchain0_n29_α
 xchain0_n25_β:
 add dword ptr [r12 + 880], 1
 mov eax, dword ptr [r12 + 880]
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
 mov rdi, qword ptr [r12 + 888]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 896]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n20_α
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [r12 + 1040]
 mov rdx, qword ptr [r12 + 1048]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 jmp xchain0_n30_α
 xchain0_n26_β:
 jmp xchain0_n27_α
# IR_LIT_STRING
 xchain0_n27_α:
 mov qword ptr [r12 + 1344], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 1352], rax
 jmp xchain0_n31_α
 xchain0_n27_β:
 jmp xchain0_n36_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n32_α
 xchain0_n28_β:
 jmp xchain0_n15_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "ok: icase hello"
# IR_MATCH_DEFER
 xchain0_n29_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 928], rcx
 test rax, rax
 jz .Lx34_0
 mov qword ptr [r12 + 928], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 936], rax
 mov rcx, qword ptr [r12 + 928]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx34_1
 mov rdi, qword ptr [r12 + 936]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 928], rax
 jmp xchain0_n25_β
.Lx34_1:
 jmp xchain0_n33_α
.Lx34_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S1]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx34_2:
 test rax, rax
 je .Lx34_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx34_2
.Lx34_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain0_n25_β
 mov r14d, eax
 jmp xchain0_n33_α
 xchain0_n29_β:
 mov rcx, qword ptr [r12 + 928]
 test rcx, rcx
 jz xchain0_n25_β
 mov rdi, qword ptr [r12 + 936]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx34_1
 mov rdi, qword ptr [r12 + 936]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 928], rax
 jmp xchain0_n25_β
# IR_LIT_STRING
 xchain0_n30_α:
 mov qword ptr [r12 + 1200], 1
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 1208], rax
 jmp xchain0_n34_α
 xchain0_n30_β:
 jmp xchain0_n27_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "HeLLo"
 xchain0_n31_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1344] -> [zr+1312]
 mov rax, qword ptr [r12 + 1344]
 mov qword ptr [r12 + 1312], rax
 mov rax, qword ptr [r12 + 1352]
 mov qword ptr [r12 + 1320], rax
  .section .rodata
  .Lbynamefn32: .string "icase"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn32]
 lea rsi, [r12 + 1312]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 cmp eax, 99
 je xchain0_n36_α
 jmp xchain0_n35_α
 xchain0_n31_β:
 jmp xchain0_n36_α
# IR_ASSIGN global
 xchain0_n32_α:
 mov rsi, qword ptr [r12 + 736]
 mov rdx, qword ptr [r12 + 744]
 mov rdi, qword ptr [rip + .Lx37_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 jmp xchain0_n15_α
 xchain0_n32_β:
 jmp xchain0_n15_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n33_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 888]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 896]
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rsp, rbp
 pop rbp
 push r14
 push r15
 push r13
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok_open@PLT
.Lx39_1:
 test rax, rax
 je .Lx39_2
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx39_1
.Lx39_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain0_n37_α
# IR_MATCH_HEAD
 xchain0_n34_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 1144], rax
 mov qword ptr [r12 + 1152], rsp
 mov rdi, qword ptr [r12 + 1200]
 mov rsi, qword ptr [r12 + 1208]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 1136], 0
.Lx41_0:
 mov r14d, dword ptr [r12 + 1136]
 jmp xchain0_n38_α
 xchain0_n34_β:
 add dword ptr [r12 + 1136], 1
 mov eax, dword ptr [r12 + 1136]
 cmp eax, r15d
 jg .Lx41_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx41_1
 jmp .Lx41_0
.Lx41_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 1144]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 1152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n27_α
# IR_ASSIGN gva
 xchain0_n35_α:
 mov rax, qword ptr [r12 + 1296]
 mov rdx, qword ptr [r12 + 1304]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 jmp xchain0_n39_α
 xchain0_n35_β:
 jmp xchain0_n36_α
# IR_LIT_STRING
 xchain0_n36_α:
 mov qword ptr [r12 + 1504], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 1512], rax
 jmp xchain0_n40_α
 xchain0_n36_β:
 jmp main_γ
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "no match ok"
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [r12 + 992], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 1000], rax
 jmp xchain0_n41_α
 xchain0_n37_β:
 jmp xchain0_n20_α
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "ok: icase HELLO"
# IR_MATCH_DEFER
 xchain0_n38_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 1184], rcx
 test rax, rax
 jz .Lx45_0
 mov qword ptr [r12 + 1184], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 1192], rax
 mov rcx, qword ptr [r12 + 1184]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx45_1
 mov rdi, qword ptr [r12 + 1192]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 1184], rax
 jmp xchain0_n34_β
.Lx45_1:
 jmp xchain0_n42_α
.Lx45_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx45_2:
 test rax, rax
 je .Lx45_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx45_2
.Lx45_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain0_n34_β
 mov r14d, eax
 jmp xchain0_n42_α
 xchain0_n38_β:
 mov rcx, qword ptr [r12 + 1184]
 test rcx, rcx
 jz xchain0_n34_β
 mov rdi, qword ptr [r12 + 1192]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx45_1
 mov rdi, qword ptr [r12 + 1192]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 1184], rax
 jmp xchain0_n34_β
# IR_LIT_STRING
 xchain0_n39_α:
 mov qword ptr [r12 + 1456], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 1464], rax
 jmp xchain0_n43_α
 xchain0_n39_β:
 jmp xchain0_n36_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "world"
# IR_ASSIGN global
 xchain0_n40_α:
 mov rsi, qword ptr [r12 + 1504]
 mov rdx, qword ptr [r12 + 1512]
 mov rdi, qword ptr [rip + .Lx47_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1488], rax
 mov qword ptr [r12 + 1496], rdx
 jmp main_γ
 xchain0_n40_β:
 jmp main_γ
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n41_α:
 mov rsi, qword ptr [r12 + 992]
 mov rdx, qword ptr [r12 + 1000]
 mov rdi, qword ptr [rip + .Lx48_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 jmp xchain0_n20_α
 xchain0_n41_β:
 jmp xchain0_n20_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n42_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 1144]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 1152]
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rsp, rbp
 pop rbp
 push r14
 push r15
 push r13
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok_open@PLT
.Lx50_1:
 test rax, rax
 je .Lx50_2
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx50_1
.Lx50_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain0_n44_α
# IR_MATCH_HEAD
 xchain0_n43_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 1400], rax
 mov qword ptr [r12 + 1408], rsp
 mov rdi, qword ptr [r12 + 1456]
 mov rsi, qword ptr [r12 + 1464]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 1392], 0
.Lx52_0:
 mov r14d, dword ptr [r12 + 1392]
 jmp xchain0_n45_α
 xchain0_n43_β:
 add dword ptr [r12 + 1392], 1
 mov eax, dword ptr [r12 + 1392]
 cmp eax, r15d
 jg .Lx52_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx52_1
 jmp .Lx52_0
.Lx52_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 1400]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 1408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n36_α
# IR_LIT_STRING
 xchain0_n44_α:
 mov qword ptr [r12 + 1248], 1
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 1256], rax
 jmp xchain0_n46_α
 xchain0_n44_β:
 jmp xchain0_n27_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "ok: icase HeLLo"
# IR_MATCH_DEFER
 xchain0_n45_α:
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 1440], rcx
 test rax, rax
 jz .Lx54_0
 mov qword ptr [r12 + 1440], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 1448], rax
 mov rcx, qword ptr [r12 + 1440]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx54_1
 mov rdi, qword ptr [r12 + 1448]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 1440], rax
 jmp xchain0_n43_β
.Lx54_1:
 jmp xchain0_n47_α
.Lx54_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S3]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx54_2:
 test rax, rax
 je .Lx54_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx54_2
.Lx54_3:
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain0_n43_β
 mov r14d, eax
 jmp xchain0_n47_α
 xchain0_n45_β:
 mov rcx, qword ptr [r12 + 1440]
 test rcx, rcx
 jz xchain0_n43_β
 mov rdi, qword ptr [r12 + 1448]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx54_1
 mov rdi, qword ptr [r12 + 1448]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 1440], rax
 jmp xchain0_n43_β
# IR_ASSIGN global
 xchain0_n46_α:
 mov rsi, qword ptr [r12 + 1248]
 mov rdx, qword ptr [r12 + 1256]
 mov rdi, qword ptr [rip + .Lx55_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 jmp xchain0_n27_α
 xchain0_n46_β:
 jmp xchain0_n27_α
.Lx55_0:
 .quad .Lx55_0_s
.Lx55_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n47_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 1400]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 1408]
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rsp, rbp
 pop rbp
 push r14
 push r15
 push r13
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok_open@PLT
.Lx57_1:
 test rax, rax
 je .Lx57_2
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx57_1
.Lx57_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain0_n48_α
# IR_LIT_STRING
 xchain0_n48_α:
 mov qword ptr [r12 + 1552], 1
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 1560], rax
 jmp xchain0_n49_α
 xchain0_n48_β:
 jmp main_γ
.Lx58_0:
 .quad .Lx58_0_s
.Lx58_0_s:
 .string "FAIL: icase matched wrong string"
# IR_ASSIGN global
 xchain0_n49_α:
 mov rsi, qword ptr [r12 + 1552]
 mov rdx, qword ptr [r12 + 1560]
 mov rdi, qword ptr [rip + .Lx59_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1536], rax
 mov qword ptr [r12 + 1544], rdx
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
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 1584]
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
.S0: .string "PATTMP$0"
.S1: .string "PATTMP$1"
.S2: .string "PATTMP$2"
.S3: .string "PATTMP$3"
.text
