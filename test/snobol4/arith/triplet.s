  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "N"
  .Lgvan1: .string "S"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 2
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 2
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
  mov qword ptr [rsp + 1096], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 160], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [rsp + 192]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 200]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn4]
 lea rsi, [rsp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [rsp + 240], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 248], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad 0
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [rsp + 240]
 mov rdx, qword ptr [rsp + 248]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n5_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rsp + 288], rax
 mov qword ptr [rsp + 296], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp main_γ
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain0_n6_α:
 mov rax, qword ptr [rsp + 288]
 mov rdx, qword ptr [rsp + 296]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rsp + 432], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 440], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n11_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string " "
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [rsp + 528], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 536], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n11_α
.Lx10_0:
 .quad 80
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 608], rax
 mov qword ptr [rsp + 616], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n11_α
 xchain0_n10_α:
# BOX IR_CALL SIZE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+608] -> [zr+576]
 mov rax, qword ptr [rsp + 608]
 mov qword ptr [rsp + 576], rax
 mov rax, qword ptr [rsp + 616]
 mov qword ptr [rsp + 584], rax
  .section .rodata
  .Lrkfn13: .string "SIZE"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn13]
 lea rsi, [rsp + 576]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 560], rax
 mov qword ptr [rsp + 568], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 816], rax
 mov qword ptr [rsp + 824], rdx
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n14_α
 xchain0_n12_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 560]
 cmp eax, 100
 je .Lx15_0
 mov eax, dword ptr [rsp + 560]
 cmp eax, 6
 jne .Lx15_2
.Lx15_1:
 mov rax, 80
 mov rcx, qword ptr [rsp + 568]
 sub rax, rcx
 mov qword ptr [rsp + 496], 6
 mov qword ptr [rsp + 504], rax
 jmp xchain0_n15_α
.Lx15_0:
 mov rdi, qword ptr [rsp + 528]
 mov rsi, qword ptr [rsp + 536]
 mov rdx, qword ptr [rsp + 560]
 mov rcx, qword ptr [rsp + 568]
 mov r8d, 1
 lea r9, [rsp + 496]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx15_3
.Lx15_2:
 mov rdi, qword ptr [rsp + 528]
 mov rsi, qword ptr [rsp + 536]
 mov rdx, qword ptr [rsp + 560]
 mov rcx, qword ptr [rsp + 568]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [rsp + 496], rax
 mov qword ptr [rsp + 504], rdx
.Lx15_3:
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [rsp + 848], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rsp + 856], rax
 jmp xchain0_n16_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx16_0:
 .quad 1
# IR_VAR
 xchain0_n14_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 1056], rax
 mov qword ptr [rsp + 1064], rdx
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [rsp + 640], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 648], rax
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp xchain0_n11_α
.Lx18_0:
 .quad 2
 xchain0_n16_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 816]
 cmp eax, 100
 je .Lx19_0
 mov eax, dword ptr [rsp + 816]
 cmp eax, 6
 jne .Lx19_2
.Lx19_1:
 mov rax, qword ptr [rsp + 824]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rsp + 784], 6
 mov qword ptr [rsp + 792], rax
 jmp xchain0_n19_α
.Lx19_0:
 mov rdi, qword ptr [rsp + 816]
 mov rsi, qword ptr [rsp + 824]
 mov rdx, qword ptr [rsp + 848]
 mov rcx, qword ptr [rsp + 856]
 mov r8d, 0
 lea r9, [rsp + 784]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx19_3
.Lx19_2:
 mov rdi, qword ptr [rsp + 816]
 mov rsi, qword ptr [rsp + 824]
 mov rdx, qword ptr [rsp + 848]
 mov rcx, qword ptr [rsp + 856]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n14_α
 mov qword ptr [rsp + 784], rax
 mov qword ptr [rsp + 792], rdx
.Lx19_3:
 jmp xchain0_n19_α
 xchain0_n16_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n17_α:
 mov qword ptr [rsp + 1024], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rsp + 1032], rax
 jmp xchain0_n20_α
 xchain0_n17_β:
 jmp xchain0_n5_α
.Lx20_0:
 .quad 0
 xchain0_n18_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 496]
 cmp eax, 100
 je .Lx21_0
 mov eax, dword ptr [rsp + 496]
 cmp eax, 6
 jne .Lx21_2
.Lx21_1:
 mov rax, qword ptr [rsp + 504]
 mov rcx, 2
 cqo
 idiv rcx
 mov qword ptr [rsp + 464], 6
 mov qword ptr [rsp + 472], rax
 jmp xchain0_n21_α
.Lx21_0:
 mov rdi, qword ptr [rsp + 496]
 mov rsi, qword ptr [rsp + 504]
 mov rdx, qword ptr [rsp + 640]
 mov rcx, qword ptr [rsp + 648]
 mov r8d, 3
 lea r9, [rsp + 464]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx21_3
.Lx21_2:
 mov rdi, qword ptr [rsp + 496]
 mov rsi, qword ptr [rsp + 504]
 mov rdx, qword ptr [rsp + 640]
 mov rcx, qword ptr [rsp + 648]
 mov r8d, 3
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
.Lx21_3:
 jmp xchain0_n21_α
 xchain0_n18_β:
 jmp xchain0_n11_α
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [rsp + 880], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rsp + 888], rax
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n14_α
.Lx22_0:
 .quad 3
# IR_COERCE_NUMERIC
 xchain0_n20_α:
 mov eax, dword ptr [rsp + 1056]
 cmp eax, 7
 je .Lx24_1
 cmp eax, 6
 jne .Lx24_0
 mov eax, dword ptr [rsp + 1024]
 cmp eax, 6
 jne .Lx24_0
.Lx24_1:
 mov rax, qword ptr [rsp + 1056]
 mov qword ptr [rsp + 992], rax
 mov rax, qword ptr [rsp + 1064]
 mov qword ptr [rsp + 1000], rax
 jmp .Lx24_2
.Lx24_0:
 lea rdi, [rsp + 1056]
 lea rsi, [rsp + 1024]
 lea rdx, [rsp + 992]
 mov rcx, 101
 call rt_coerce_num2_d@PLT
.Lx24_2:
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp xchain0_n5_α
 xchain0_n21_α:
# BOX IR_CALL DUPL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+432] -> [zr+384]
 mov rax, qword ptr [rsp + 432]
 mov qword ptr [rsp + 384], rax
 mov rax, qword ptr [rsp + 440]
 mov qword ptr [rsp + 392], rax
# marshal arg1 = producer-box slot [zr+464] -> [zr+400]
 mov rax, qword ptr [rsp + 464]
 mov qword ptr [rsp + 400], rax
 mov rax, qword ptr [rsp + 472]
 mov qword ptr [rsp + 408], rax
  .section .rodata
  .Lrkfn26: .string "DUPL"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn26]
 lea rsi, [rsp + 384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 368], rax
 mov qword ptr [rsp + 376], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n24_α
 xchain0_n21_β:
 jmp xchain0_n11_α
 xchain0_n22_α:
# BOX IR_CALL REMDR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+784] -> [zr+736]
 mov rax, qword ptr [rsp + 784]
 mov qword ptr [rsp + 736], rax
 mov rax, qword ptr [rsp + 792]
 mov qword ptr [rsp + 744], rax
# marshal arg1 = producer-box slot [zr+880] -> [zr+752]
 mov rax, qword ptr [rsp + 880]
 mov qword ptr [rsp + 752], rax
 mov rax, qword ptr [rsp + 888]
 mov qword ptr [rsp + 760], rax
  .section .rodata
  .Lrkfn28: .string "REMDR"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn28]
 lea rsi, [rsp + 736]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 720], rax
 mov qword ptr [rsp + 728], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n25_α
 xchain0_n22_β:
 jmp xchain0_n14_α
# IR_COERCE_NUMERIC
 xchain0_n23_α:
 mov eax, dword ptr [rsp + 1024]
 cmp eax, 7
 je .Lx30_1
 cmp eax, 6
 jne .Lx30_0
 mov eax, dword ptr [rsp + 1056]
 cmp eax, 6
 jne .Lx30_0
.Lx30_1:
 mov rax, qword ptr [rsp + 1024]
 mov qword ptr [rsp + 960], rax
 mov rax, qword ptr [rsp + 1032]
 mov qword ptr [rsp + 968], rax
 jmp .Lx30_2
.Lx30_0:
 lea rdi, [rsp + 1024]
 lea rsi, [rsp + 1056]
 lea rdx, [rsp + 960]
 mov rcx, 102
 call rt_coerce_num2_d@PLT
.Lx30_2:
 jmp xchain0_n26_α
 xchain0_n23_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n24_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 672], rax
 mov qword ptr [rsp + 680], rdx
 jmp xchain0_n27_α
 xchain0_n24_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n25_α:
 mov rax, qword ptr [rsp + 720]
 mov rdx, qword ptr [rsp + 728]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 704], rax
 mov qword ptr [rsp + 712], rdx
 jmp xchain0_n14_α
 xchain0_n25_β:
 jmp xchain0_n14_α
# IR_CMP_TEST
 xchain0_n26_α:
 lea rdi, [rsp + 992]
 lea rsi, [rsp + 960]
 call rt_cmp_d@PLT
 test eax, eax
 jne xchain0_n5_α
 mov qword ptr [rsp + 928], 0
 mov qword ptr [rsp + 936], 0
 jmp xchain0_n28_α
 xchain0_n26_β:
 jmp xchain0_n5_α
 xchain0_n27_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 368]
 mov rsi, qword ptr [rsp + 376]
 mov rdx, qword ptr [rsp + 672]
 mov rcx, qword ptr [rsp + 680]
 call str_concat_d@PLT
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n11_α
# IR_ASSIGN global
 xchain0_n28_α:
 mov rsi, qword ptr [rsp + 928]
 mov rdx, qword ptr [rsp + 936]
 mov rdi, qword ptr [rip + .Lx36_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 912], rax
 mov qword ptr [rsp + 920], rdx
 jmp xchain0_n5_α
 xchain0_n28_β:
 jmp xchain0_n5_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n29_α:
 mov rsi, qword ptr [rsp + 336]
 mov rdx, qword ptr [rsp + 344]
 mov rdi, qword ptr [rip + .Lx37_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp xchain0_n11_α
 xchain0_n29_β:
 jmp xchain0_n11_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 1096]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 1096]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
