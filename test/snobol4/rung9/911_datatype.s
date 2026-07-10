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
 mov qword ptr [r12 + 672], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hello"
 xchain0_n1_α:
# BOX IR_CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+144] -> [zr+128]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn3: .string "datatype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn3]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
xchain0_n1_β:
 jmp xchain0_n3_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n3_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "string"
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n8_α
.Lx5_0:
 .quad 12
 xchain0_n4_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+112] -> [zr+80]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [zr+160] -> [zr+96]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn7: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 80]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n6_α
xchain0_n4_β:
 jmp xchain0_n3_α
 xchain0_n5_α:
# BOX IR_CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+288] -> [zr+272]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn9: .string "datatype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 272]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n7_α
xchain0_n5_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp main_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "FAIL 911/001: string literal datatype"
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "integer"
# IR_LIT_REAL
 xchain0_n8_α:
 mov qword ptr [r12 + 432], 7
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n14_α
.Lx12_0:
 .quad 4608668606677049672
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
 xchain0_n10_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+256] -> [zr+224]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 232], rax
# marshal arg1 = producer-box slot [zr+304] -> [zr+240]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lbynamefn15: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n12_α
xchain0_n10_β:
 jmp xchain0_n8_α
 xchain0_n11_α:
# BOX IR_CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+416]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 424], rax
  .section .rodata
  .Lbynamefn17: .string "datatype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 416]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
xchain0_n11_β:
 jmp xchain0_n14_α
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp main_γ
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "FAIL 911/002: integer datatype"
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n16_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "real"
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp xchain0_n20_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string ""
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp main_γ
 xchain0_n15_β:
 jmp main_γ
 xchain0_n16_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+368]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 376], rax
# marshal arg1 = producer-box slot [zr+448] -> [zr+384]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lbynamefn23: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 368]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n18_α
xchain0_n16_β:
 jmp xchain0_n14_α
 xchain0_n17_α:
# BOX IR_CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+576] -> [zr+560]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 568], rax
  .section .rodata
  .Lbynamefn25: .string "datatype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn25]
 lea rsi, [r12 + 560]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n19_α
xchain0_n17_β:
 jmp xchain0_n20_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n21_α
 xchain0_n18_β:
 jmp main_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "FAIL 911/003: real datatype"
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [r12 + 592], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 600], rax
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n20_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "string"
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 656], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp main_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "PASS 911_datatype (4/4)"
# IR_ASSIGN gva
 xchain0_n21_α:
 mov rax, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp main_γ
 xchain0_n21_β:
 jmp main_γ
 xchain0_n22_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+544] -> [zr+512]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 520], rax
# marshal arg1 = producer-box slot [zr+592] -> [zr+528]
 mov rax, qword ptr [r12 + 592]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 600]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn31: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn31]
 lea rsi, [r12 + 512]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n24_α
xchain0_n22_β:
 jmp xchain0_n20_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 656]
 mov rdx, qword ptr [r12 + 664]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 624], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 632], rax
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp main_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "FAIL 911/004: null is string"
# IR_ASSIGN gva
 xchain0_n25_α:
 mov rax, qword ptr [r12 + 624]
 mov rdx, qword ptr [r12 + 632]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp main_γ
 xchain0_n25_β:
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
 mov rdi, qword ptr [r12 + 672]
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
