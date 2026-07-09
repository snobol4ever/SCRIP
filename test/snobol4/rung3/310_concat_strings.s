  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .bss
  .align 16
__gva: .space 16, 0
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
  mov edx, 1
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
 mov qword ptr [r12 + 544], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 80], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "a"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "b"
 xchain0_n2_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 call str_concat_d@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "ab"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "a"
 xchain0_n5_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+32]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 40], rax
# marshal arg1 = producer-box slot [r12+112] -> [r12+48]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn7: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 32]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n7_α
xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "b"
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 310/001: two-string concat"
 xchain0_n8_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call str_concat_d@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "c"
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 416], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n18_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "hello"
 xchain0_n12_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 272]
 mov rcx, qword ptr [r12 + 280]
 call str_concat_d@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n18_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string " "
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n11_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "abc"
 xchain0_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 416]
 mov rsi, qword ptr [r12 + 424]
 mov rdx, qword ptr [r12 + 432]
 mov rcx, qword ptr [r12 + 440]
 call str_concat_d@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n18_α
 xchain0_n16_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+176]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 184], rax
# marshal arg1 = producer-box slot [r12+288] -> [r12+192]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lbynamefn19: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n19_α
xchain0_n16_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n20_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "world"
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 528], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n21_α
 xchain0_n18_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "PASS 310_concat_strings (3/3)"
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [r12 + 320], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "FAIL 310/002: three-string concat"
 xchain0_n20_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 400]
 mov rsi, qword ptr [r12 + 408]
 mov rdx, qword ptr [r12 + 448]
 mov rcx, qword ptr [r12 + 456]
 call str_concat_d@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n21_α:
 mov rax, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp main_γ
 xchain0_n21_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n22_α:
 mov rax, qword ptr [r12 + 320]
 mov rdx, qword ptr [r12 + 328]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp main_γ
 xchain0_n22_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp xchain0_n18_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "hello world"
 xchain0_n24_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+384] -> [r12+352]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [r12+464] -> [r12+368]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn28: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n25_α
xchain0_n24_β:
 jmp xchain0_n18_α
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "FAIL 310/003: left-associative concat"
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [r12 + 496]
 mov rdx, qword ptr [r12 + 504]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp main_γ
 xchain0_n26_β:
 jmp main_γ
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
 mov rdi, qword ptr [r12 + 544]
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
