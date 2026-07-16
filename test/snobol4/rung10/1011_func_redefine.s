  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "myfunc"
  .Lgvan1: .string "n"
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
  mov qword ptr [rsp + 1400], rsp
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
 .string "myfunc(n)"
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
 mov qword ptr [rsp + 400], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rsp + 408], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n5_α
.Lx3_0:
 .quad 3
 xchain0_n3_α:
# BOX CALL myfunc(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+368]
 mov rax, qword ptr [rsp + 400]
 mov qword ptr [rsp + 368], rax
 mov rax, qword ptr [rsp + 408]
 mov qword ptr [rsp + 376], rax
  .section .rodata
  .Lbynamefn4: .string "myfunc"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn4]
 lea rsi, [rsp + 368]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 352], rax
 mov qword ptr [rsp + 360], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rsp + 432], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 440], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad 6
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [rsp + 624], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 632], rax
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n12_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "myfunc(myfunc)"
 xchain0_n6_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+352] -> [zr+304]
 mov rax, qword ptr [rsp + 352]
 mov qword ptr [rsp + 304], rax
 mov rax, qword ptr [rsp + 360]
 mov qword ptr [rsp + 312], rax
# marshal arg1 = producer-box slot [zr+432] -> [zr+320]
 mov rax, qword ptr [rsp + 432]
 mov qword ptr [rsp + 320], rax
 mov rax, qword ptr [rsp + 440]
 mov qword ptr [rsp + 328], rax
  .section .rodata
  .Lbynamefn7: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn7]
 lea rsi, [rsp + 304]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 288], rax
 mov qword ptr [rsp + 296], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rsp + 656], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 664], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n12_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "myfunc2"
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rsp + 480], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 488], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 1011/001: first definition myfunc(3)=6"
 xchain0_n9_α:
# BOX CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+624] -> [zr+576]
 mov rax, qword ptr [rsp + 624]
 mov qword ptr [rsp + 576], rax
 mov rax, qword ptr [rsp + 632]
 mov qword ptr [rsp + 584], rax
# marshal arg1 = producer-box slot [zr+656] -> [zr+592]
 mov rax, qword ptr [rsp + 656]
 mov qword ptr [rsp + 592], rax
 mov rax, qword ptr [rsp + 664]
 mov qword ptr [rsp + 600], rax
  .section .rodata
  .Lbynamefn10: .string "define"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn10]
 lea rsi, [rsp + 576]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 560], rax
 mov qword ptr [rsp + 568], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n12_α
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [rsp + 480]
 mov rdx, qword ptr [rsp + 488]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
 jmp main_γ
 xchain0_n10_β:
 jmp main_γ
 xchain0_n11_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+560] -> [zr+528]
 mov rax, qword ptr [rsp + 560]
 mov qword ptr [rsp + 528], rax
 mov rax, qword ptr [rsp + 568]
 mov qword ptr [rsp + 536], rax
  .section .rodata
  .Lbynamefn12: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [rsp + 528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 512], rax
 mov qword ptr [rsp + 520], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n12_α:
 mov qword ptr [rsp + 1232], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rsp + 1240], rax
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n17_α
.Lx13_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [rsp + 704], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 712], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "FAIL 1011/002: define returns function name"
 xchain0_n14_α:
# BOX CALL myfunc(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1232] -> [zr+1200]
 mov rax, qword ptr [rsp + 1232]
 mov qword ptr [rsp + 1200], rax
 mov rax, qword ptr [rsp + 1240]
 mov qword ptr [rsp + 1208], rax
  .section .rodata
  .Lbynamefn15: .string "myfunc"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn15]
 lea rsi, [rsp + 1200]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1184], rax
 mov qword ptr [rsp + 1192], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n17_α
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [rsp + 704]
 mov rdx, qword ptr [rsp + 712]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 688], rax
 mov qword ptr [rsp + 696], rdx
 jmp main_γ
 xchain0_n15_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [rsp + 1264], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rsp + 1272], rax
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx17_0:
 .quad 24
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [rsp + 1360], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 1368], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp main_γ
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "PASS 1011_func_redefine (3/3)"
 xchain0_n18_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1184] -> [zr+1136]
 mov rax, qword ptr [rsp + 1184]
 mov qword ptr [rsp + 1136], rax
 mov rax, qword ptr [rsp + 1192]
 mov qword ptr [rsp + 1144], rax
# marshal arg1 = producer-box slot [zr+1264] -> [zr+1152]
 mov rax, qword ptr [rsp + 1264]
 mov qword ptr [rsp + 1152], rax
 mov rax, qword ptr [rsp + 1272]
 mov qword ptr [rsp + 1160], rax
  .section .rodata
  .Lbynamefn19: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [rsp + 1136]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1120], rax
 mov qword ptr [rsp + 1128], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n17_α
# IR_ASSIGN gva
 xchain0_n19_α:
 mov rax, qword ptr [rsp + 1360]
 mov rdx, qword ptr [rsp + 1368]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 1344], rax
 mov qword ptr [rsp + 1352], rdx
 jmp main_γ
 xchain0_n19_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [rsp + 1312], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rsp + 1320], rax
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "FAIL 1011/003: redefined myfunc(4)=24"
# IR_ASSIGN gva
 xchain0_n21_α:
 mov rax, qword ptr [rsp + 1312]
 mov rdx, qword ptr [rsp + 1320]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 1296], rax
 mov qword ptr [rsp + 1304], rdx
 jmp main_γ
 xchain0_n21_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 1400]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 1400]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
