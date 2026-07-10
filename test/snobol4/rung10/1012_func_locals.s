  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "a"
  .Lgvan1: .string "output"
  .Lgvan2: .string "b"
  .Lgvan3: .string "d"
  .Lgvan4: .string "lfunc"
  .Lgvan5: .string "x"
  .Lgvan6: .string "checklocal"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .quad .Lgvan6
  .section .bss
  .align 16
__gva: .space 112, 0
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
  mov edx, 7
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
 mov qword ptr [r12 + 1376], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "lfunc(a,b,c)d,e,f"
 xchain0_n1_α:
# BOX IR_CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+96] -> [zr+80]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 88], rax
  .section .rodata
  .Lbynamefn2: .string "define"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [r12 + 80]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [r12 + 624], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 632], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "global_a"
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 624]
 mov rdx, qword ptr [r12 + 632]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 656], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "global_d"
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [r12 + 656]
 mov rdx, qword ptr [r12 + 664]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "p"
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 800], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 808], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n11_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "q"
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 816], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 824], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n11_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "r"
 xchain0_n9_α:
# BOX IR_CALL lfunc(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+784] -> [zr+736]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 736], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 744], rax
# marshal arg1 = producer-box slot [zr+800] -> [zr+752]
 mov rax, qword ptr [r12 + 800]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 808]
 mov qword ptr [r12 + 760], rax
# marshal arg2 = producer-box slot [zr+816] -> [zr+768]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lbynamefn10: .string "lfunc"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn10]
 lea rsi, [r12 + 736]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "aabbdd"
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n14_α
 xchain0_n12_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+720] -> [zr+688]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 688], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 696], rax
# marshal arg1 = producer-box slot [zr+832] -> [zr+704]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 712], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 688]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [r12 + 944], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n16_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "global_a"
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 976], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 1012/005: global a not clobbered"
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [r12 + 864], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 872], rax
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp main_γ
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "FAIL 1012/004: lfunc return value"
 xchain0_n16_α:
# BOX IR_CALL ident(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+928] -> [zr+896]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [r12 + 896], rax
 mov rax, qword ptr [r12 + 936]
 mov qword ptr [r12 + 904], rax
# marshal arg1 = producer-box slot [zr+944] -> [zr+912]
 mov rax, qword ptr [r12 + 944]
 mov qword ptr [r12 + 912], rax
 mov rax, qword ptr [r12 + 952]
 mov qword ptr [r12 + 920], rax
  .section .rodata
  .Lbynamefn17: .string "ident"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 896]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n19_α
 xchain0_n16_β:
 jmp xchain0_n14_α
# IR_ASSIGN gva
 xchain0_n17_α:
 mov rax, qword ptr [r12 + 976]
 mov rdx, qword ptr [r12 + 984]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n18_α:
 mov rax, qword ptr [r12 + 864]
 mov rdx, qword ptr [r12 + 872]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp main_γ
 xchain0_n18_β:
 jmp main_γ
# IR_VAR
 xchain0_n19_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n21_α
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 1056], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 1064], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n21_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "global_d"
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 1088], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 1096], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "FAIL 1012/006: global d not clobbered"
 xchain0_n22_α:
# BOX IR_CALL ident(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1040] -> [zr+1008]
 mov rax, qword ptr [r12 + 1040]
 mov qword ptr [r12 + 1008], rax
 mov rax, qword ptr [r12 + 1048]
 mov qword ptr [r12 + 1016], rax
# marshal arg1 = producer-box slot [zr+1056] -> [zr+1024]
 mov rax, qword ptr [r12 + 1056]
 mov qword ptr [r12 + 1024], rax
 mov rax, qword ptr [r12 + 1064]
 mov qword ptr [r12 + 1032], rax
  .section .rodata
  .Lbynamefn23: .string "ident"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 1008]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 992], rax
 mov qword ptr [r12 + 1000], rdx
 cmp eax, 99
 je xchain0_n21_α
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n21_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 1088]
 mov rdx, qword ptr [r12 + 1096]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 1136], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 1144], rax
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp xchain0_n26_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "checklocal()x"
 xchain0_n25_α:
# BOX IR_CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1136] -> [zr+1120]
 mov rax, qword ptr [r12 + 1136]
 mov qword ptr [r12 + 1120], rax
 mov rax, qword ptr [r12 + 1144]
 mov qword ptr [r12 + 1128], rax
  .section .rodata
  .Lbynamefn26: .string "define"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 1120]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp xchain0_n26_α
 xchain0_n26_α:
# BOX IR_CALL checklocal(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn27: .string "checklocal"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 1312]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 cmp eax, 99
 je xchain0_n28_α
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n28_α
 xchain0_n27_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1296] -> [zr+1280]
 mov rax, qword ptr [r12 + 1296]
 mov qword ptr [r12 + 1280], rax
 mov rax, qword ptr [r12 + 1304]
 mov qword ptr [r12 + 1288], rax
  .section .rodata
  .Lbynamefn28: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 1280]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 cmp eax, 99
 je xchain0_n28_α
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n28_α
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 1360], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 1368], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "PASS 1012_func_locals (7/7)"
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [r12 + 1328], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 1336], rax
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp main_γ
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "FAIL 1012/007: local null on fresh call"
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 1360]
 mov rdx, qword ptr [r12 + 1368]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n31_α:
 mov rax, qword ptr [r12 + 1328]
 mov rdx, qword ptr [r12 + 1336]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 jmp main_γ
 xchain0_n31_β:
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
 mov rdi, qword ptr [r12 + 1376]
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
