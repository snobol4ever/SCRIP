  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "eq"
  .Lgvan1: .string "output"
  .Lgvan2: .string "trim"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .bss
  .align 16
__gva: .space 48, 0
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
  mov edx, 3
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
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 1008], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "eq"
 xchain0_n1_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+224] -> [zr+192]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lrkfn3: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 192]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n3_α
.Lx4_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n8_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "eq"
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n3_α
.Lx6_0:
 .quad 2
 xchain0_n5_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+496] -> [zr+464]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 472], rax
  .section .rodata
  .Lrkfn8: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn8]
 lea rsi, [r12 + 464]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
 xchain0_n6_α:
# BOX CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+176] -> [zr+112]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+256] -> [zr+128]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 136], rax
# marshal arg2 = producer-box slot [zr+288] -> [zr+144]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lbynamefn7: .string "apply"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 112]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [r12 + 528], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx10_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 608], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp main_γ
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "FAIL 1018/002: apply(.eq,1,1) should succeed"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp main_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "FAIL 1018/001: apply(.eq,1,2) should fail"
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 560], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n8_α
.Lx13_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n11_α:
 mov rax, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp main_γ
 xchain0_n11_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n12_α:
 mov rax, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp main_γ
 xchain0_n12_β:
 jmp main_γ
 xchain0_n13_α:
# BOX CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+384]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 392], rax
# marshal arg1 = producer-box slot [zr+528] -> [zr+400]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 408], rax
# marshal arg2 = producer-box slot [zr+560] -> [zr+416]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 424], rax
  .section .rodata
  .Lbynamefn14: .string "apply"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn14]
 lea rsi, [r12 + 384]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 816], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 824], rax
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n17_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "trim"
 xchain0_n15_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+816] -> [zr+784]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 784], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 792], rax
  .section .rodata
  .Lrkfn19: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn19]
 lea rsi, [r12 + 784]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [r12 + 848], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "abc   "
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 976], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "PASS 1018_apply (3/3)"
 xchain0_n18_α:
# BOX CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+768] -> [zr+720]
 mov rax, qword ptr [r12 + 768]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 776]
 mov qword ptr [r12 + 728], rax
# marshal arg1 = producer-box slot [zr+848] -> [zr+736]
 mov rax, qword ptr [r12 + 848]
 mov qword ptr [r12 + 736], rax
 mov rax, qword ptr [r12 + 856]
 mov qword ptr [r12 + 744], rax
  .section .rodata
  .Lbynamefn19: .string "apply"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 720]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n17_α
# IR_ASSIGN gva
 xchain0_n19_α:
 mov rax, qword ptr [r12 + 976]
 mov rdx, qword ptr [r12 + 984]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 jmp main_γ
 xchain0_n19_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n17_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "abc"
 xchain0_n21_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+704] -> [zr+656]
 mov rax, qword ptr [r12 + 704]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 712]
 mov qword ptr [r12 + 664], rax
# marshal arg1 = producer-box slot [zr+880] -> [zr+672]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 680], rax
  .section .rodata
  .Lbynamefn22: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn22]
 lea rsi, [r12 + 656]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 928], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 936], rax
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp main_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "FAIL 1018/003: apply(.trim,...)"
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 928]
 mov rdx, qword ptr [r12 + 936]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp main_γ
 xchain0_n23_β:
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
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1008]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
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
