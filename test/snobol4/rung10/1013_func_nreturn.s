  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "ref_a"
  .Lgvan1: .string "a"
  .Lgvan2: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 3
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 3
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
  mov qword ptr [rsp + 936], rsp
  mov r12, qword ptr [1879048192]
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "ref_a()"
 xchain0_n1_α:
# BOX CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [rsp + 144]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 152]
 mov qword ptr [rsp + 120], rax
  .section .rodata
  .Lbynamefn2: .string "define"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [rsp + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [rsp + 288], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rsp + 296], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx3_0:
 .quad 27
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rsp + 288]
 mov rdx, qword ptr [rsp + 296]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
 xchain0_n4_α:
# BOX CALL ref_a(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn5: .string "ref_a"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn5]
 lea rsi, [rsp + 400]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_LIT_INTEGER
 xchain0_n5_α:
 mov qword ptr [rsp + 416], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 424], rax
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n6_α
.Lx6_0:
 .quad 27
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rsp + 496], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rsp + 504], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string ""
 xchain0_n7_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+336]
 mov rax, qword ptr [rsp + 384]
 mov qword ptr [rsp + 336], rax
 mov rax, qword ptr [rsp + 392]
 mov qword ptr [rsp + 344], rax
# marshal arg1 = producer-box slot [zr+416] -> [zr+352]
 mov rax, qword ptr [rsp + 416]
 mov qword ptr [rsp + 352], rax
 mov rax, qword ptr [rsp + 424]
 mov qword ptr [rsp + 360], rax
  .section .rodata
  .Lbynamefn8: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [rsp + 336]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n6_α
 xchain0_n8_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+496] -> [zr+544]
 mov rax, qword ptr [rsp + 496]
 mov qword ptr [rsp + 544], rax
 mov rax, qword ptr [rsp + 504]
 mov qword ptr [rsp + 552], rax
  .section .rodata
  .Lrkfn10: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn10]
 lea rsi, [rsp + 544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 528], rax
 mov qword ptr [rsp + 536], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rsp + 464], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 472], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp main_γ
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "FAIL 1013/001: nreturn read gives value"
 xchain0_n10_α:
# BOX CALL ref_a(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn11: .string "ref_a"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [rsp + 592]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [rsp + 576], rax
 mov qword ptr [rsp + 584], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rsp + 672], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rsp + 680], rax
 jmp xchain0_n14_α
 xchain0_n11_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "FAIL 1013/002: nreturn lvalue assign failed"
# IR_ASSIGN gva
 xchain0_n12_α:
 mov rax, qword ptr [rsp + 464]
 mov rdx, qword ptr [rsp + 472]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 448], rax
 mov qword ptr [rsp + 456], rdx
 jmp main_γ
 xchain0_n12_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [rsp + 608], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 616], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n11_α
.Lx15_0:
 .quad 26
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [rsp + 672]
 mov rdx, qword ptr [rsp + 680]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 656], rax
 mov qword ptr [rsp + 664], rdx
 jmp main_γ
 xchain0_n14_β:
 jmp main_γ
# IR_ASSIGN_VAR
 xchain0_n15_α:
 mov rdi, qword ptr [rsp + 576]
 mov rsi, qword ptr [rsp + 584]
 mov rdx, qword ptr [rsp + 608]
 mov rcx, qword ptr [rsp + 616]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [rsp + 640], rax
 mov qword ptr [rsp + 648], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n11_α
# IR_VAR
 xchain0_n16_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 768], rax
 mov qword ptr [rsp + 776], rdx
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n17_α:
 mov qword ptr [rsp + 800], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 808], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx19_0:
 .quad 26
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rsp + 896], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rsp + 904], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp main_γ
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "PASS 1013_func_nreturn (3/3)"
 xchain0_n19_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+768] -> [zr+720]
 mov rax, qword ptr [rsp + 768]
 mov qword ptr [rsp + 720], rax
 mov rax, qword ptr [rsp + 776]
 mov qword ptr [rsp + 728], rax
# marshal arg1 = producer-box slot [zr+800] -> [zr+736]
 mov rax, qword ptr [rsp + 800]
 mov qword ptr [rsp + 736], rax
 mov rax, qword ptr [rsp + 808]
 mov qword ptr [rsp + 744], rax
  .section .rodata
  .Lbynamefn20: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [rsp + 720]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 704], rax
 mov qword ptr [rsp + 712], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [rsp + 896]
 mov rdx, qword ptr [rsp + 904]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 880], rax
 mov qword ptr [rsp + 888], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [rsp + 848], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [rsp + 856], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "FAIL 1013/003: a updated via nreturn"
# IR_ASSIGN gva
 xchain0_n22_α:
 mov rax, qword ptr [rsp + 848]
 mov rdx, qword ptr [rsp + 856]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 832], rax
 mov qword ptr [rsp + 840], rdx
 jmp main_γ
 xchain0_n22_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 936]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 936]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
