  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "a"
  .Lgvan1: .string "output"
  .Lgvan2: .string "b"
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
  mov qword ptr [rsp + 2344], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         data('node(val,lson,rson)')
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rbp + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 152], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "node(val,lson,rson)"
 xchain0_n1_α:
# BOX CALL data(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [rbp + 144]
 mov qword ptr [rbp + 112], rax
 mov rax, qword ptr [rbp + 152]
 mov qword ptr [rbp + 120], rax
  .section .rodata
  .Lbynamefn2: .string "data"
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
#         data('node(val,lson,rson)')
#         a = node('x', 'y', 'z')
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rbp + 272], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rbp + 280], rax
 jmp xchain0_n3_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "x"
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rbp + 304], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 312], rax
 jmp xchain0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "y"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rbp + 336], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rbp + 344], rax
 jmp xchain0_n5_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "z"
 xchain0_n5_α:
# BOX CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+272] -> [zr+208]
 mov rax, qword ptr [rbp + 272]
 mov qword ptr [rbp + 208], rax
 mov rax, qword ptr [rbp + 280]
 mov qword ptr [rbp + 216], rax
# marshal arg1 = producer-box slot [zr+304] -> [zr+224]
 mov rax, qword ptr [rbp + 304]
 mov qword ptr [rbp + 224], rax
 mov rax, qword ptr [rbp + 312]
 mov qword ptr [rbp + 232], rax
# marshal arg2 = producer-box slot [zr+336] -> [zr+240]
 mov rax, qword ptr [rbp + 336]
 mov qword ptr [rbp + 240], rax
 mov rax, qword ptr [rbp + 344]
 mov qword ptr [rbp + 248], rax
  .section .rodata
  .Lbynamefn6: .string "node"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [rbp + 208]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rbp + 192], rax
 mov qword ptr [rbp + 200], rdx
 cmp eax, 99
 je xchain0_n7_α
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
# IR_ASSIGN gva
 xchain0_n6_α:
 mov rax, qword ptr [rbp + 192]
 mov rdx, qword ptr [rbp + 200]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 176], rax
 mov qword ptr [rbp + 184], rdx
 jmp xchain0_n7_α
#         a = node('x', 'y', 'z')
#         differ(replace(datatype(a),&lcase,&ucase), 'NODE')   :f(e001)
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 560], rax
 mov qword ptr [rbp + 568], rdx
 jmp xchain0_n8_α
 xchain0_n8_α:
# BOX CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+560] -> [zr+528]
 mov rax, qword ptr [rbp + 560]
 mov qword ptr [rbp + 528], rax
 mov rax, qword ptr [rbp + 568]
 mov qword ptr [rbp + 536], rax
  .section .rodata
  .Lbynamefn9: .string "datatype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn9]
 lea rsi, [rbp + 528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 512], rax
 mov qword ptr [rbp + 520], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n9_α
#         differ(val(a), 'x')                   :f(e002)
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 816], rax
 mov qword ptr [rbp + 824], rdx
 jmp xchain0_n11_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n10_α:
 mov rdi, qword ptr [rip + .Lx11_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp xchain0_n13_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "lcase"
 xchain0_n11_α:
# BOX CALL val(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+816] -> [zr+784]
 mov rax, qword ptr [rbp + 816]
 mov qword ptr [rbp + 784], rax
 mov rax, qword ptr [rbp + 824]
 mov qword ptr [rbp + 792], rax
  .section .rodata
  .Lbynamefn12: .string "val"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [rbp + 784]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 768], rax
 mov qword ptr [rbp + 776], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n14_α
 xchain0_n11_β:
 jmp xchain0_n12_α
#         b = node()
 xchain0_n12_α:
# BOX CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn13: .string "node"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [rbp + 960]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [rbp + 944], rax
 mov qword ptr [rbp + 952], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp xchain0_n16_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n13_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rbp + 608], rax
 mov qword ptr [rbp + 616], rdx
 jmp xchain0_n17_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "ucase"
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rbp + 848], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rbp + 856], rax
 jmp xchain0_n18_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "x"
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [rbp + 944]
 mov rdx, qword ptr [rbp + 952]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 928], rax
 mov qword ptr [rbp + 936], rdx
 jmp xchain0_n16_α
#         differ(rson(b))            :f(e003)
# IR_VAR
 xchain0_n16_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 1072], rax
 mov qword ptr [rbp + 1080], rdx
 jmp xchain0_n19_α
 xchain0_n17_α:
# BOX CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+448]
 mov rax, qword ptr [rbp + 512]
 mov qword ptr [rbp + 448], rax
 mov rax, qword ptr [rbp + 520]
 mov qword ptr [rbp + 456], rax
# marshal arg1 = producer-box slot [zr+592] -> [zr+464]
 mov rax, qword ptr [rbp + 592]
 mov qword ptr [rbp + 464], rax
 mov rax, qword ptr [rbp + 600]
 mov qword ptr [rbp + 472], rax
# marshal arg2 = producer-box slot [zr+608] -> [zr+480]
 mov rax, qword ptr [rbp + 608]
 mov qword ptr [rbp + 480], rax
 mov rax, qword ptr [rbp + 616]
 mov qword ptr [rbp + 488], rax
  .section .rodata
  .Lbynamefn18: .string "replace"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [rbp + 448]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n21_α
 xchain0_n17_β:
 jmp xchain0_n9_α
 xchain0_n18_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+768] -> [zr+720]
 mov rax, qword ptr [rbp + 768]
 mov qword ptr [rbp + 720], rax
 mov rax, qword ptr [rbp + 776]
 mov qword ptr [rbp + 728], rax
# marshal arg1 = producer-box slot [zr+848] -> [zr+736]
 mov rax, qword ptr [rbp + 848]
 mov qword ptr [rbp + 736], rax
 mov rax, qword ptr [rbp + 856]
 mov qword ptr [rbp + 744], rax
  .section .rodata
  .Lbynamefn19: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [rbp + 720]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 704], rax
 mov qword ptr [rbp + 712], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n22_α
 xchain0_n18_β:
 jmp xchain0_n12_α
 xchain0_n19_α:
# BOX CALL rson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1072] -> [zr+1040]
 mov rax, qword ptr [rbp + 1072]
 mov qword ptr [rbp + 1040], rax
 mov rax, qword ptr [rbp + 1080]
 mov qword ptr [rbp + 1048], rax
  .section .rodata
  .Lbynamefn20: .string "rson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [rbp + 1040]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1024], rax
 mov qword ptr [rbp + 1032], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n23_α
 xchain0_n19_β:
 jmp xchain0_n20_α
#         lson(b) = a
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [rbp + 1152], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rbp + 1160], rax
 jmp xchain0_n24_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [rbp + 624], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rbp + 632], rax
 jmp xchain0_n25_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "NODE"
#         output = 'FAIL 1115/002: field accessor val'   :(end)
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [rbp + 896], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [rbp + 904], rax
 jmp xchain0_n26_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "FAIL 1115/002: field accessor val"
 xchain0_n23_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1024] -> [zr+992]
 mov rax, qword ptr [rbp + 1024]
 mov qword ptr [rbp + 992], rax
 mov rax, qword ptr [rbp + 1032]
 mov qword ptr [rbp + 1000], rax
  .section .rodata
  .Lbynamefn24: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn24]
 lea rsi, [rbp + 992]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 976], rax
 mov qword ptr [rbp + 984], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n27_α
 xchain0_n23_β:
 jmp xchain0_n20_α
 xchain0_n24_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1152] -> [zr+1200]
 mov rax, qword ptr [rbp + 1152]
 mov qword ptr [rbp + 1200], rax
 mov rax, qword ptr [rbp + 1160]
 mov qword ptr [rbp + 1208], rax
  .section .rodata
  .Lrkfn26: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn26]
 lea rsi, [rbp + 1200]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1184], rax
 mov qword ptr [rbp + 1192], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n28_α
 xchain0_n24_β:
 jmp xchain0_n29_α
 xchain0_n25_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+384]
 mov rax, qword ptr [rbp + 432]
 mov qword ptr [rbp + 384], rax
 mov rax, qword ptr [rbp + 440]
 mov qword ptr [rbp + 392], rax
# marshal arg1 = producer-box slot [zr+624] -> [zr+400]
 mov rax, qword ptr [rbp + 624]
 mov qword ptr [rbp + 400], rax
 mov rax, qword ptr [rbp + 632]
 mov qword ptr [rbp + 408], rax
  .section .rodata
  .Lbynamefn26: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn26]
 lea rsi, [rbp + 384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n30_α
 xchain0_n25_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [rbp + 896]
 mov rdx, qword ptr [rbp + 904]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 880], rax
 mov qword ptr [rbp + 888], rdx
 jmp main_γ
#         output = 'FAIL 1115/003: unset field is null'  :(end)
# IR_LIT_STRING
 xchain0_n27_α:
 mov qword ptr [rbp + 1120], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [rbp + 1128], rax
 jmp xchain0_n31_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "FAIL 1115/003: unset field is null"
# IR_VAR
 xchain0_n28_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 1280], rax
 mov qword ptr [rbp + 1288], rdx
 jmp xchain0_n32_α
#         differ(rson(lson(b)), 'z')                   :f(e004)
# IR_VAR
 xchain0_n29_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 1520], rax
 mov qword ptr [rbp + 1528], rdx
 jmp xchain0_n33_α
#         output = 'FAIL 1115/001: datatype of node'            :(end)
# IR_LIT_STRING
 xchain0_n30_α:
 mov qword ptr [rbp + 672], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [rbp + 680], rax
 jmp xchain0_n35_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "FAIL 1115/001: datatype of node"
# IR_ASSIGN gva
 xchain0_n31_α:
 mov rax, qword ptr [rbp + 1120]
 mov rdx, qword ptr [rbp + 1128]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 1104], rax
 mov qword ptr [rbp + 1112], rdx
 jmp main_γ
 xchain0_n32_α:
# BOX CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1280] -> [zr+1248]
 mov rax, qword ptr [rbp + 1280]
 mov qword ptr [rbp + 1248], rax
 mov rax, qword ptr [rbp + 1288]
 mov qword ptr [rbp + 1256], rax
  .section .rodata
  .Lbynamefn33: .string "lson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn33]
 lea rsi, [rbp + 1248]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1232], rax
 mov qword ptr [rbp + 1240], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n36_α
 xchain0_n32_β:
 jmp xchain0_n29_α
 xchain0_n33_α:
# BOX CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1520] -> [zr+1488]
 mov rax, qword ptr [rbp + 1520]
 mov qword ptr [rbp + 1488], rax
 mov rax, qword ptr [rbp + 1528]
 mov qword ptr [rbp + 1496], rax
  .section .rodata
  .Lbynamefn34: .string "lson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn34]
 lea rsi, [rbp + 1488]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1472], rax
 mov qword ptr [rbp + 1480], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n37_α
 xchain0_n33_β:
 jmp xchain0_n34_α
#         differ(value('b'), b)                   :f(e005)
# IR_LIT_STRING
 xchain0_n34_α:
 mov qword ptr [rbp + 1744], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [rbp + 1752], rax
 jmp xchain0_n38_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "b"
# IR_ASSIGN gva
 xchain0_n35_α:
 mov rax, qword ptr [rbp + 672]
 mov rdx, qword ptr [rbp + 680]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 656], rax
 mov qword ptr [rbp + 664], rdx
 jmp main_γ
# IR_VAR
 xchain0_n36_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 1312], rax
 mov qword ptr [rbp + 1320], rdx
 jmp xchain0_n39_α
 xchain0_n37_α:
# BOX CALL rson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1472] -> [zr+1440]
 mov rax, qword ptr [rbp + 1472]
 mov qword ptr [rbp + 1440], rax
 mov rax, qword ptr [rbp + 1480]
 mov qword ptr [rbp + 1448], rax
  .section .rodata
  .Lbynamefn38: .string "rson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn38]
 lea rsi, [rbp + 1440]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1424], rax
 mov qword ptr [rbp + 1432], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n40_α
 xchain0_n37_β:
 jmp xchain0_n34_α
 xchain0_n38_α:
# BOX CALL value(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1744] -> [zr+1712]
 mov rax, qword ptr [rbp + 1744]
 mov qword ptr [rbp + 1712], rax
 mov rax, qword ptr [rbp + 1752]
 mov qword ptr [rbp + 1720], rax
  .section .rodata
  .Lbynamefn39: .string "value"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn39]
 lea rsi, [rbp + 1712]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1696], rax
 mov qword ptr [rbp + 1704], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n41_α
 xchain0_n38_β:
 jmp xchain0_n42_α
# IR_ASSIGN_VAR
 xchain0_n39_α:
 mov rdi, qword ptr [rbp + 1232]
 mov rsi, qword ptr [rbp + 1240]
 mov rdx, qword ptr [rbp + 1312]
 mov rcx, qword ptr [rbp + 1320]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [rbp + 1344], rax
 mov qword ptr [rbp + 1352], rdx
 jmp xchain0_n29_α
# IR_LIT_STRING
 xchain0_n40_α:
 mov qword ptr [rbp + 1552], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [rbp + 1560], rax
 jmp xchain0_n43_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "z"
# IR_VAR
 xchain0_n41_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 1776], rax
 mov qword ptr [rbp + 1784], rdx
 jmp xchain0_n44_α
#         val(a) = 'new'
# IR_LIT_STRING
 xchain0_n42_α:
 mov qword ptr [rbp + 1856], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [rbp + 1864], rax
 jmp xchain0_n45_α
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string ""
 xchain0_n43_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1424] -> [zr+1376]
 mov rax, qword ptr [rbp + 1424]
 mov qword ptr [rbp + 1376], rax
 mov rax, qword ptr [rbp + 1432]
 mov qword ptr [rbp + 1384], rax
# marshal arg1 = producer-box slot [zr+1552] -> [zr+1392]
 mov rax, qword ptr [rbp + 1552]
 mov qword ptr [rbp + 1392], rax
 mov rax, qword ptr [rbp + 1560]
 mov qword ptr [rbp + 1400], rax
  .section .rodata
  .Lbynamefn44: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn44]
 lea rsi, [rbp + 1376]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1360], rax
 mov qword ptr [rbp + 1368], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n46_α
 xchain0_n43_β:
 jmp xchain0_n34_α
 xchain0_n44_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1696] -> [zr+1648]
 mov rax, qword ptr [rbp + 1696]
 mov qword ptr [rbp + 1648], rax
 mov rax, qword ptr [rbp + 1704]
 mov qword ptr [rbp + 1656], rax
# marshal arg1 = producer-box slot [zr+1776] -> [zr+1664]
 mov rax, qword ptr [rbp + 1776]
 mov qword ptr [rbp + 1664], rax
 mov rax, qword ptr [rbp + 1784]
 mov qword ptr [rbp + 1672], rax
  .section .rodata
  .Lbynamefn45: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn45]
 lea rsi, [rbp + 1648]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1632], rax
 mov qword ptr [rbp + 1640], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n47_α
 xchain0_n44_β:
 jmp xchain0_n42_α
 xchain0_n45_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1856] -> [zr+1904]
 mov rax, qword ptr [rbp + 1856]
 mov qword ptr [rbp + 1904], rax
 mov rax, qword ptr [rbp + 1864]
 mov qword ptr [rbp + 1912], rax
  .section .rodata
  .Lrkfn48: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn48]
 lea rsi, [rbp + 1904]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1888], rax
 mov qword ptr [rbp + 1896], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n48_α
 xchain0_n45_β:
 jmp xchain0_n49_α
#         output = 'FAIL 1115/004: nested accessor after mutate' :(end)
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [rbp + 1600], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [rbp + 1608], rax
 jmp xchain0_n50_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "FAIL 1115/004: nested accessor after mutate"
#         output = 'FAIL 1115/005: value() by variable name' :(end)
# IR_LIT_STRING
 xchain0_n47_α:
 mov qword ptr [rbp + 1824], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [rbp + 1832], rax
 jmp xchain0_n51_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "FAIL 1115/005: value() by variable name"
# IR_VAR
 xchain0_n48_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 1984], rax
 mov qword ptr [rbp + 1992], rdx
 jmp xchain0_n52_α
#         differ(val(a), 'new')                   :f(e006)
# IR_VAR
 xchain0_n49_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 2176], rax
 mov qword ptr [rbp + 2184], rdx
 jmp xchain0_n53_α
# IR_ASSIGN gva
 xchain0_n50_α:
 mov rax, qword ptr [rbp + 1600]
 mov rdx, qword ptr [rbp + 1608]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 1584], rax
 mov qword ptr [rbp + 1592], rdx
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n51_α:
 mov rax, qword ptr [rbp + 1824]
 mov rdx, qword ptr [rbp + 1832]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 1808], rax
 mov qword ptr [rbp + 1816], rdx
 jmp main_γ
 xchain0_n52_α:
# BOX CALL val(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1984] -> [zr+1952]
 mov rax, qword ptr [rbp + 1984]
 mov qword ptr [rbp + 1952], rax
 mov rax, qword ptr [rbp + 1992]
 mov qword ptr [rbp + 1960], rax
  .section .rodata
  .Lbynamefn53: .string "val"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn53]
 lea rsi, [rbp + 1952]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1936], rax
 mov qword ptr [rbp + 1944], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n55_α
 xchain0_n52_β:
 jmp xchain0_n49_α
 xchain0_n53_α:
# BOX CALL val(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2176] -> [zr+2144]
 mov rax, qword ptr [rbp + 2176]
 mov qword ptr [rbp + 2144], rax
 mov rax, qword ptr [rbp + 2184]
 mov qword ptr [rbp + 2152], rax
  .section .rodata
  .Lbynamefn54: .string "val"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn54]
 lea rsi, [rbp + 2144]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2128], rax
 mov qword ptr [rbp + 2136], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n56_α
 xchain0_n53_β:
 jmp xchain0_n54_α
#         output = 'PASS 1115_data_basic (6/6)'
# IR_LIT_STRING
 xchain0_n54_α:
 mov qword ptr [rbp + 2304], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [rbp + 2312], rax
 jmp xchain0_n57_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "PASS 1115_data_basic (6/6)"
# IR_LIT_STRING
 xchain0_n55_α:
 mov qword ptr [rbp + 2016], 1
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [rbp + 2024], rax
 jmp xchain0_n58_α
.Lx58_0:
 .quad .Lx58_0_s
.Lx58_0_s:
 .string "new"
# IR_LIT_STRING
 xchain0_n56_α:
 mov qword ptr [rbp + 2208], 1
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [rbp + 2216], rax
 jmp xchain0_n59_α
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "new"
# IR_ASSIGN gva
 xchain0_n57_α:
 mov rax, qword ptr [rbp + 2304]
 mov rdx, qword ptr [rbp + 2312]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 2288], rax
 mov qword ptr [rbp + 2296], rdx
 jmp main_γ
# IR_ASSIGN_VAR
 xchain0_n58_α:
 mov rdi, qword ptr [rbp + 1936]
 mov rsi, qword ptr [rbp + 1944]
 mov rdx, qword ptr [rbp + 2016]
 mov rcx, qword ptr [rbp + 2024]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n49_α
 mov qword ptr [rbp + 2048], rax
 mov qword ptr [rbp + 2056], rdx
 jmp xchain0_n49_α
 xchain0_n59_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2128] -> [zr+2080]
 mov rax, qword ptr [rbp + 2128]
 mov qword ptr [rbp + 2080], rax
 mov rax, qword ptr [rbp + 2136]
 mov qword ptr [rbp + 2088], rax
# marshal arg1 = producer-box slot [zr+2208] -> [zr+2096]
 mov rax, qword ptr [rbp + 2208]
 mov qword ptr [rbp + 2096], rax
 mov rax, qword ptr [rbp + 2216]
 mov qword ptr [rbp + 2104], rax
  .section .rodata
  .Lbynamefn60: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn60]
 lea rsi, [rbp + 2080]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2064], rax
 mov qword ptr [rbp + 2072], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n60_α
 xchain0_n59_β:
 jmp xchain0_n54_α
#         output = 'FAIL 1115/006: mutate field and read back' :(end)
# IR_LIT_STRING
 xchain0_n60_α:
 mov qword ptr [rbp + 2256], 1
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [rbp + 2264], rax
 jmp xchain0_n61_α
.Lx63_0:
 .quad .Lx63_0_s
.Lx63_0_s:
 .string "FAIL 1115/006: mutate field and read back"
# IR_ASSIGN gva
 xchain0_n61_α:
 mov rax, qword ptr [rbp + 2256]
 mov rdx, qword ptr [rbp + 2264]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 2240], rax
 mov qword ptr [rbp + 2248], rdx
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 2344]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 2344]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
