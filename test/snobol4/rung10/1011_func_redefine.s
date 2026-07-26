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
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         define('myfunc(n)')                            :(myfunc_end)
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rbp + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 152], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "myfunc(n)"
 xchain0_n1_α:
# BOX CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [rbp + 144]
 mov qword ptr [rbp + 112], rax
 mov rax, qword ptr [rbp + 152]
 mov qword ptr [rbp + 120], rax
  .section .rodata
  .Lbynamefn2: .string "define"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [rbp + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
#         ne(myfunc(3), 6)           :f(e001)
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [rbp + 400], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rbp + 408], rax
 jmp xchain0_n3_α
.Lx3_0:
 .quad 3
 xchain0_n3_α:
# BOX CALL myfunc(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+368]
 mov rax, qword ptr [rbp + 400]
 mov qword ptr [rbp + 368], rax
 mov rax, qword ptr [rbp + 408]
 mov qword ptr [rbp + 376], rax
  .section .rodata
  .Lbynamefn4: .string "myfunc"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn4]
 lea rsi, [rbp + 368]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rbp + 432], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rbp + 440], rax
 jmp xchain0_n6_α
.Lx5_0:
 .quad 6
#         differ(define('myfunc(myfunc)', 'myfunc2'))                   :f(e002)
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [rbp + 624], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rbp + 632], rax
 jmp xchain0_n7_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "myfunc(myfunc)"
 xchain0_n6_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+352] -> [zr+304]
 mov rax, qword ptr [rbp + 352]
 mov qword ptr [rbp + 304], rax
 mov rax, qword ptr [rbp + 360]
 mov qword ptr [rbp + 312], rax
# marshal arg1 = producer-box slot [zr+432] -> [zr+320]
 mov rax, qword ptr [rbp + 432]
 mov qword ptr [rbp + 320], rax
 mov rax, qword ptr [rbp + 440]
 mov qword ptr [rbp + 328], rax
  .section .rodata
  .Lbynamefn7: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn7]
 lea rsi, [rbp + 304]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 288], rax
 mov qword ptr [rbp + 296], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rbp + 656], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 664], rax
 jmp xchain0_n9_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "myfunc2"
#         output = 'FAIL 1011/001: first definition myfunc(3)=6' :(end)
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rbp + 480], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rbp + 488], rax
 jmp xchain0_n10_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 1011/001: first definition myfunc(3)=6"
 xchain0_n9_α:
# BOX CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+624] -> [zr+576]
 mov rax, qword ptr [rbp + 624]
 mov qword ptr [rbp + 576], rax
 mov rax, qword ptr [rbp + 632]
 mov qword ptr [rbp + 584], rax
# marshal arg1 = producer-box slot [zr+656] -> [zr+592]
 mov rax, qword ptr [rbp + 656]
 mov qword ptr [rbp + 592], rax
 mov rax, qword ptr [rbp + 664]
 mov qword ptr [rbp + 600], rax
  .section .rodata
  .Lbynamefn10: .string "define"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn10]
 lea rsi, [rbp + 576]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 560], rax
 mov qword ptr [rbp + 568], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n12_α
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [rbp + 480]
 mov rdx, qword ptr [rbp + 488]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 jmp main_γ
 xchain0_n11_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+560] -> [zr+528]
 mov rax, qword ptr [rbp + 560]
 mov qword ptr [rbp + 528], rax
 mov rax, qword ptr [rbp + 568]
 mov qword ptr [rbp + 536], rax
  .section .rodata
  .Lbynamefn12: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [rbp + 528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 512], rax
 mov qword ptr [rbp + 520], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n12_α
# e002    :(myfunc2_end)
#         ne(myfunc(4), 24)          :f(e003)
# IR_LIT_INTEGER
 xchain0_n12_α:
 mov qword ptr [rbp + 1232], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rbp + 1240], rax
 jmp xchain0_n14_α
.Lx13_0:
 .quad 4
#         output = 'FAIL 1011/002: define returns function name' :(end)
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [rbp + 704], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 712], rax
 jmp xchain0_n15_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "FAIL 1011/002: define returns function name"
 xchain0_n14_α:
# BOX CALL myfunc(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1232] -> [zr+1200]
 mov rax, qword ptr [rbp + 1232]
 mov qword ptr [rbp + 1200], rax
 mov rax, qword ptr [rbp + 1240]
 mov qword ptr [rbp + 1208], rax
  .section .rodata
  .Lbynamefn15: .string "myfunc"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn15]
 lea rsi, [rbp + 1200]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1184], rax
 mov qword ptr [rbp + 1192], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n17_α
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [rbp + 704]
 mov rdx, qword ptr [rbp + 712]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 688], rax
 mov qword ptr [rbp + 696], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [rbp + 1264], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rbp + 1272], rax
 jmp xchain0_n18_α
.Lx17_0:
 .quad 24
#         output = 'PASS 1011_func_redefine (3/3)'
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [rbp + 1360], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rbp + 1368], rax
 jmp xchain0_n19_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "PASS 1011_func_redefine (3/3)"
 xchain0_n18_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1184] -> [zr+1136]
 mov rax, qword ptr [rbp + 1184]
 mov qword ptr [rbp + 1136], rax
 mov rax, qword ptr [rbp + 1192]
 mov qword ptr [rbp + 1144], rax
# marshal arg1 = producer-box slot [zr+1264] -> [zr+1152]
 mov rax, qword ptr [rbp + 1264]
 mov qword ptr [rbp + 1152], rax
 mov rax, qword ptr [rbp + 1272]
 mov qword ptr [rbp + 1160], rax
  .section .rodata
  .Lbynamefn19: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [rbp + 1136]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1120], rax
 mov qword ptr [rbp + 1128], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n17_α
# IR_ASSIGN gva
 xchain0_n19_α:
 mov rax, qword ptr [rbp + 1360]
 mov rdx, qword ptr [rbp + 1368]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 1344], rax
 mov qword ptr [rbp + 1352], rdx
 jmp main_γ
#         output = 'FAIL 1011/003: redefined myfunc(4)=24'   :(end)
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [rbp + 1312], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rbp + 1320], rax
 jmp xchain0_n21_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "FAIL 1011/003: redefined myfunc(4)=24"
# IR_ASSIGN gva
 xchain0_n21_α:
 mov rax, qword ptr [rbp + 1312]
 mov rdx, qword ptr [rbp + 1320]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 1296], rax
 mov qword ptr [rbp + 1304], rdx
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 1400]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 1400]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
