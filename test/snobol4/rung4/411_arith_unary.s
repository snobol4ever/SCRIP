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
  mov qword ptr [rsp + 568], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rbp + 192], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad 5
# IR_UNOP
 xchain0_n1_α:
 mov rdi, qword ptr [rbp + 192]
 mov rsi, qword ptr [rbp + 200]
 call rt_num_neg@PLT
 mov qword ptr [rbp + 160], rax
 mov qword ptr [rbp + 168], rdx
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [rbp + 224], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rbp + 232], rax
 jmp xchain0_n4_α
.Lx3_0:
 .quad 18446744073709551611
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rbp + 400], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 408], rax
 jmp xchain0_n5_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "4"
 xchain0_n4_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rbp + 160]
 mov qword ptr [rbp + 112], rax
 mov rax, qword ptr [rbp + 168]
 mov qword ptr [rbp + 120], rax
# marshal arg1 = producer-box slot [zr+224] -> [zr+128]
 mov rax, qword ptr [rbp + 224]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 232]
 mov qword ptr [rbp + 136], rax
  .section .rodata
  .Lbynamefn5: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn5]
 lea rsi, [rbp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n3_α
# IR_UNOP
 xchain0_n5_α:
 mov rdi, qword ptr [rbp + 400]
 mov rsi, qword ptr [rbp + 408]
 call rt_num_pos@PLT
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 jmp xchain0_n7_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rbp + 272], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rbp + 280], rax
 jmp xchain0_n9_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "FAIL 411/001: unary minus"
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rbp + 432], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 440], rax
 jmp xchain0_n10_α
.Lx8_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rbp + 528], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rbp + 536], rax
 jmp xchain0_n11_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "PASS 411_arith_unary (2/2)"
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [rbp + 272]
 mov rdx, qword ptr [rbp + 280]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 256], rax
 mov qword ptr [rbp + 264], rdx
 jmp main_γ
 xchain0_n10_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+368] -> [zr+320]
 mov rax, qword ptr [rbp + 368]
 mov qword ptr [rbp + 320], rax
 mov rax, qword ptr [rbp + 376]
 mov qword ptr [rbp + 328], rax
# marshal arg1 = producer-box slot [zr+432] -> [zr+336]
 mov rax, qword ptr [rbp + 432]
 mov qword ptr [rbp + 336], rax
 mov rax, qword ptr [rbp + 440]
 mov qword ptr [rbp + 344], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [rbp + 320]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 304], rax
 mov qword ptr [rbp + 312], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n8_α
# IR_ASSIGN gva
 xchain0_n11_α:
 mov rax, qword ptr [rbp + 528]
 mov rdx, qword ptr [rbp + 536]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 512], rax
 mov qword ptr [rbp + 520], rdx
 jmp main_γ
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [rbp + 480], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rbp + 488], rax
 jmp xchain0_n13_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "FAIL 411/002: unary plus string->int"
# IR_ASSIGN gva
 xchain0_n13_α:
 mov rax, qword ptr [rbp + 480]
 mov rdx, qword ptr [rbp + 488]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 568]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 568]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
