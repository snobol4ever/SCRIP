  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 1
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 1
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
  mov qword ptr [rsp + 680], rsp
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
 .string "ab"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [rbp + 192], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain0_n2_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "ab"
 xchain0_n2_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn3: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn3]
 lea rsi, [rbp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
#         output = 'FAIL 310/001: two-string concat'      :(end)
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rbp + 240], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 248], rax
 jmp xchain0_n5_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "FAIL 310/001: two-string concat"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rbp + 336], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rbp + 344], rax
 jmp xchain0_n6_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "abc"
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [rbp + 240]
 mov rdx, qword ptr [rbp + 248]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 jmp main_γ
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rbp + 368], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rbp + 376], rax
 jmp xchain0_n7_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "abc"
 xchain0_n7_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+288]
 mov rax, qword ptr [rbp + 336]
 mov qword ptr [rbp + 288], rax
 mov rax, qword ptr [rbp + 344]
 mov qword ptr [rbp + 296], rax
# marshal arg1 = producer-box slot [zr+368] -> [zr+304]
 mov rax, qword ptr [rbp + 368]
 mov qword ptr [rbp + 304], rax
 mov rax, qword ptr [rbp + 376]
 mov qword ptr [rbp + 312], rax
  .section .rodata
  .Lbynamefn8: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [rbp + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
#         output = 'FAIL 310/002: three-string concat'    :(end)
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rbp + 416], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rbp + 424], rax
 jmp xchain0_n10_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 310/002: three-string concat"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rbp + 512], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rbp + 520], rax
 jmp xchain0_n11_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "hello world"
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [rbp + 416]
 mov rdx, qword ptr [rbp + 424]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 jmp main_γ
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rbp + 544], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rbp + 552], rax
 jmp xchain0_n12_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "hello world"
 xchain0_n12_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+464]
 mov rax, qword ptr [rbp + 512]
 mov qword ptr [rbp + 464], rax
 mov rax, qword ptr [rbp + 520]
 mov qword ptr [rbp + 472], rax
# marshal arg1 = producer-box slot [zr+544] -> [zr+480]
 mov rax, qword ptr [rbp + 544]
 mov qword ptr [rbp + 480], rax
 mov rax, qword ptr [rbp + 552]
 mov qword ptr [rbp + 488], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [rbp + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
#         output = 'FAIL 310/003: left-associative concat' :(end)
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [rbp + 592], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 600], rax
 jmp xchain0_n15_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "FAIL 310/003: left-associative concat"
#         output = 'PASS 310_concat_strings (3/3)'
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rbp + 640], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rbp + 648], rax
 jmp xchain0_n16_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "PASS 310_concat_strings (3/3)"
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [rbp + 592]
 mov rdx, qword ptr [rbp + 600]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 576], rax
 mov qword ptr [rbp + 584], rdx
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [rbp + 640]
 mov rdx, qword ptr [rbp + 648]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 624], rax
 mov qword ptr [rbp + 632], rdx
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 680]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 680]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
