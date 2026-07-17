  .intel_syntax noprefix
  .text
  .globl proc_Push_α
proc_Push_α:
#=======================================================================================================================
    .global proc_Push_α
    .global proc_Push_β
    .global proc_Push_γ
    .global proc_Push_ω
  sub rsp, 5808
  mov [rsp + 5784], rcx
  mov [rsp + 5792], rdx
  mov [rsp + 5800], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 5776
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 5768], rsp
proc_Push_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 144], rax
 mov qword ptr [rbp + 152], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 176], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 184], rax
 jmp xchain0_n3_α
 xchain0_n1_β:
 jmp xchain0_n2_α
.Lx2_0:
 .quad 0
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n5_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n3_α:
 mov rdi, qword ptr [rbp + 144]
 mov rsi, qword ptr [rbp + 152]
 mov rdx, qword ptr [rbp + 176]
 mov rcx, qword ptr [rbp + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 jmp xchain0_n6_α
 xchain0_n3_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 480], rax
 mov qword ptr [rbp + 488], rdx
 jmp xchain0_n7_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 672], rax
 mov qword ptr [rbp + 680], rdx
 jmp xchain0_n8_α
 xchain0_n5_β:
 jmp xchain0_n9_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 jmp xchain0_n10_α
 xchain0_n6_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rbp + 512], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 520], rax
 jmp xchain0_n11_α
 xchain0_n7_β:
 jmp xchain0_n5_α
.Lx8_0:
 .quad 0
 xchain0_n8_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+672] -> [zr+640]
 mov rax, qword ptr [rbp + 672]
 mov qword ptr [rbp + 640], rax
 mov rax, qword ptr [rbp + 680]
 mov qword ptr [rbp + 648], rax
  .section .rodata
  .Lrkfn10: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn10]
 lea rsi, [rbp + 640]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 624], rax
 mov qword ptr [rbp + 632], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n12_α
 xchain0_n8_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rbp + 16], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rbp + 24], rax
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp proc_Push_ω
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string ""
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [rbp + 304], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rbp + 312], rax
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n2_α
.Lx12_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain0_n11_α:
 mov rdi, qword ptr [rbp + 480]
 mov rsi, qword ptr [rbp + 488]
 mov rdx, qword ptr [rbp + 512]
 mov rcx, qword ptr [rbp + 520]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rbp + 544], rax
 mov qword ptr [rbp + 552], rdx
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 704], rax
 mov qword ptr [rbp + 712], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n9_α
 xchain0_n13_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+64]
 mov rax, qword ptr [rbp + 16]
 mov qword ptr [rbp + 64], rax
 mov rax, qword ptr [rbp + 24]
 mov qword ptr [rbp + 72], rax
  .section .rodata
  .Lrkfn16: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn16]
 lea rsi, [rbp + 64]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 48], rax
 mov qword ptr [rbp + 56], rdx
 cmp eax, 99
 je proc_Push_ω
 jmp proc_Push_γ
 xchain0_n13_β:
 jmp proc_Push_ω
# IR_SUBSCRIPT x[i] variable
 xchain0_n14_α:
 mov rdi, qword ptr [rbp + 272]
 mov rsi, qword ptr [rbp + 280]
 mov rdx, qword ptr [rbp + 304]
 mov rcx, qword ptr [rbp + 312]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 336], rax
 mov qword ptr [rbp + 344], rdx
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp xchain0_n2_α
# IR_DEREF variable -> value
 xchain0_n15_α:
 mov rdi, qword ptr [rbp + 544]
 mov rsi, qword ptr [rbp + 552]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rbp + 576], rax
 mov qword ptr [rbp + 584], rdx
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp xchain0_n5_α
# IR_ASSIGN_VAR
 xchain0_n16_α:
 mov rdi, qword ptr [rbp + 624]
 mov rsi, qword ptr [rbp + 632]
 mov rdx, qword ptr [rbp + 704]
 mov rcx, qword ptr [rbp + 712]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n9_α
 mov qword ptr [rbp + 736], rax
 mov qword ptr [rbp + 744], rdx
 jmp xchain0_n9_α
 xchain0_n16_β:
 jmp xchain0_n9_α
# IR_DEREF variable -> value
 xchain0_n17_α:
 mov rdi, qword ptr [rbp + 336]
 mov rsi, qword ptr [rbp + 344]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n2_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n18_α:
 mov rdi, qword ptr [rbp + 448]
 mov rsi, qword ptr [rbp + 456]
 mov rdx, qword ptr [rbp + 576]
 mov rcx, qword ptr [rbp + 584]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [rbp + 384], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rbp + 392], rax
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n2_α
.Lx22_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [rbp + 592]
 mov rdx, qword ptr [rbp + 600]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 jmp xchain0_n5_α
 xchain0_n20_β:
 jmp xchain0_n5_α
 xchain0_n21_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 368]
 cmp eax, 100
 je .Lx24_0
 mov eax, dword ptr [rbp + 368]
 cmp eax, 6
 jne .Lx24_2
.Lx24_1:
 mov rax, qword ptr [rbp + 376]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rbp + 240], 6
 mov qword ptr [rbp + 248], rax
 jmp xchain0_n22_α
.Lx24_0:
 mov rdi, qword ptr [rbp + 368]
 mov rsi, qword ptr [rbp + 376]
 mov rdx, qword ptr [rbp + 384]
 mov rcx, qword ptr [rbp + 392]
 mov r8d, 0
 lea r9, [rbp + 240]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx24_3
.Lx24_2:
 mov rdi, qword ptr [rbp + 368]
 mov rsi, qword ptr [rbp + 376]
 mov rdx, qword ptr [rbp + 384]
 mov rcx, qword ptr [rbp + 392]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 240], rax
 mov qword ptr [rbp + 248], rdx
.Lx24_3:
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n2_α
# IR_ASSIGN_VAR
 xchain0_n22_α:
 mov rdi, qword ptr [rbp + 208]
 mov rsi, qword ptr [rbp + 216]
 mov rdx, qword ptr [rbp + 240]
 mov rcx, qword ptr [rbp + 248]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 416], rax
 mov qword ptr [rbp + 424], rdx
 jmp xchain0_n2_α
 xchain0_n22_β:
 jmp xchain0_n2_α
proc_Push_res:
add rsp, 8
pop rbp
proc_Push_β:
jmp proc_Push_ω
proc_Push_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 5784]
lea rsp, [rbp + 5808]
mov rbp, [rbp + 5800]
jmp rax
proc_Push_ω:
mov rax, [rbp + 5792]
lea rsp, [rbp + 5808]
mov rbp, [rbp + 5800]
jmp rax
  .globl proc_Pop_α
proc_Pop_α:
#=======================================================================================================================
    .global proc_Pop_α
    .global proc_Pop_β
    .global proc_Pop_γ
    .global proc_Pop_ω
  sub rsp, 5808
  mov [rsp + 5784], rcx
  mov [rsp + 5792], rdx
  mov [rsp + 5800], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 5776
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 5768], rsp
proc_Pop_α_body:
# IR_VAR
 xchain26_n0_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 768], rax
 mov qword ptr [rbp + 776], rdx
 jmp xchain26_n1_α
 xchain26_n0_β:
 jmp xchain26_n2_α
# IR_VAR
 xchain26_n1_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 800], rax
 mov qword ptr [rbp + 808], rdx
 jmp xchain26_n3_α
 xchain26_n1_β:
 jmp xchain26_n2_α
# IR_VAR
 xchain26_n2_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 960], rax
 mov qword ptr [rbp + 968], rdx
 jmp xchain26_n4_α
 xchain26_n2_β:
 jmp proc_Pop_γ
# IR_LIT_INTEGER
 xchain26_n3_α:
 mov qword ptr [rbp + 832], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rbp + 840], rax
 jmp xchain26_n5_α
 xchain26_n3_β:
 jmp xchain26_n2_α
.Lx30_0:
 .quad 0
# IR_LIT_INTEGER
 xchain26_n4_α:
 mov qword ptr [rbp + 992], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [rbp + 1000], rax
 jmp xchain26_n6_α
 xchain26_n4_β:
 jmp proc_Pop_γ
.Lx31_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain26_n5_α:
 mov rdi, qword ptr [rbp + 800]
 mov rsi, qword ptr [rbp + 808]
 mov rdx, qword ptr [rbp + 832]
 mov rcx, qword ptr [rbp + 840]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 864], rax
 mov qword ptr [rbp + 872], rdx
 jmp xchain26_n7_α
 xchain26_n5_β:
 jmp xchain26_n2_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n6_α:
 mov rdi, qword ptr [rbp + 960]
 mov rsi, qword ptr [rbp + 968]
 mov rdx, qword ptr [rbp + 992]
 mov rcx, qword ptr [rbp + 1000]
 call rt_subscript_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 1024], rax
 mov qword ptr [rbp + 1032], rdx
 jmp xchain26_n8_α
 xchain26_n6_β:
 jmp proc_Pop_γ
# IR_DEREF variable -> value
 xchain26_n7_α:
 mov rdi, qword ptr [rbp + 864]
 mov rsi, qword ptr [rbp + 872]
 call rt_deref@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 896], rax
 mov qword ptr [rbp + 904], rdx
 jmp xchain26_n9_α
 xchain26_n7_β:
 jmp xchain26_n2_α
# IR_VAR
 xchain26_n8_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 1088], rax
 mov qword ptr [rbp + 1096], rdx
 jmp xchain26_n10_α
 xchain26_n8_β:
 jmp proc_Pop_γ
# IR_SUBSCRIPT x[i] variable
 xchain26_n9_α:
 mov rdi, qword ptr [rbp + 768]
 mov rsi, qword ptr [rbp + 776]
 mov rdx, qword ptr [rbp + 896]
 mov rcx, qword ptr [rbp + 904]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 912], rax
 mov qword ptr [rbp + 920], rdx
 jmp xchain26_n11_α
 xchain26_n9_β:
 jmp xchain26_n2_α
# IR_LIT_INTEGER
 xchain26_n10_α:
 mov qword ptr [rbp + 1120], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [rbp + 1128], rax
 jmp xchain26_n12_α
 xchain26_n10_β:
 jmp proc_Pop_γ
.Lx37_0:
 .quad 0
# IR_DEREF variable -> value
 xchain26_n11_α:
 mov rdi, qword ptr [rbp + 912]
 mov rsi, qword ptr [rbp + 920]
 call rt_deref@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 944], rax
 mov qword ptr [rbp + 952], rdx
 jmp xchain26_n13_α
 xchain26_n11_β:
 jmp xchain26_n2_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n12_α:
 mov rdi, qword ptr [rbp + 1088]
 mov rsi, qword ptr [rbp + 1096]
 mov rdx, qword ptr [rbp + 1120]
 mov rcx, qword ptr [rbp + 1128]
 call rt_subscript_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 1152], rax
 mov qword ptr [rbp + 1160], rdx
 jmp xchain26_n14_α
 xchain26_n12_β:
 jmp proc_Pop_γ
# IR_ASSIGN gva
 xchain26_n13_α:
 mov rax, qword ptr [rbp + 944]
 mov rdx, qword ptr [rbp + 952]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 752], rax
 mov qword ptr [rbp + 760], rdx
 jmp xchain26_n2_α
 xchain26_n13_β:
 jmp xchain26_n2_α
# IR_DEREF variable -> value
 xchain26_n14_α:
 mov rdi, qword ptr [rbp + 1152]
 mov rsi, qword ptr [rbp + 1160]
 call rt_deref@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 1184], rax
 mov qword ptr [rbp + 1192], rdx
 jmp xchain26_n15_α
 xchain26_n14_β:
 jmp proc_Pop_γ
# IR_LIT_INTEGER
 xchain26_n15_α:
 mov qword ptr [rbp + 1200], 6
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [rbp + 1208], rax
 jmp xchain26_n16_α
 xchain26_n15_β:
 jmp proc_Pop_γ
.Lx42_0:
 .quad 1
 xchain26_n16_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 1184]
 cmp eax, 100
 je .Lx43_0
 mov eax, dword ptr [rbp + 1184]
 cmp eax, 6
 jne .Lx43_2
.Lx43_1:
 mov rax, qword ptr [rbp + 1192]
 mov rcx, 1
 sub rax, rcx
 mov qword ptr [rbp + 1056], 6
 mov qword ptr [rbp + 1064], rax
 jmp xchain26_n17_α
.Lx43_0:
 mov rdi, qword ptr [rbp + 1184]
 mov rsi, qword ptr [rbp + 1192]
 mov rdx, qword ptr [rbp + 1200]
 mov rcx, qword ptr [rbp + 1208]
 mov r8d, 1
 lea r9, [rbp + 1056]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx43_3
.Lx43_2:
 mov rdi, qword ptr [rbp + 1184]
 mov rsi, qword ptr [rbp + 1192]
 mov rdx, qword ptr [rbp + 1200]
 mov rcx, qword ptr [rbp + 1208]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 1056], rax
 mov qword ptr [rbp + 1064], rdx
.Lx43_3:
 jmp xchain26_n17_α
 xchain26_n16_β:
 jmp proc_Pop_γ
# IR_ASSIGN_VAR
 xchain26_n17_α:
 mov rdi, qword ptr [rbp + 1024]
 mov rsi, qword ptr [rbp + 1032]
 mov rdx, qword ptr [rbp + 1056]
 mov rcx, qword ptr [rbp + 1064]
 call rt_assign_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 1232], rax
 mov qword ptr [rbp + 1240], rdx
 jmp proc_Pop_γ
 xchain26_n17_β:
 jmp proc_Pop_γ
proc_Pop_res:
add rsp, 8
pop rbp
proc_Pop_β:
jmp proc_Pop_ω
proc_Pop_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 5784]
lea rsp, [rbp + 5808]
mov rbp, [rbp + 5800]
jmp rax
proc_Pop_ω:
mov rax, [rbp + 5792]
lea rsp, [rbp + 5808]
mov rbp, [rbp + 5800]
jmp rax
  .globl proc_Unary_α
proc_Unary_α:
#=======================================================================================================================
    .global proc_Unary_α
    .global proc_Unary_β
    .global proc_Unary_γ
    .global proc_Unary_ω
  sub rsp, 5808
  mov [rsp + 5784], rcx
  mov [rsp + 5792], rdx
  mov [rsp + 5800], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 5776
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 5768], rsp
proc_Unary_α_body:
 xchain45_n0_α:
 mov rdi, qword ptr [rip + .Lx47_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx47_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx47_3]
 lea rdx, [rip + .Lx47_4]
 jmp rax
.Lx47_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx47_2
.Lx47_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx47_2
.Lx47_1:
 call rt_faildescr@PLT
.Lx47_2:
 mov qword ptr [rbp + 1264], rax
 mov qword ptr [rbp + 1272], rdx
 cmp eax, 99
 je xchain45_n2_α
 jmp xchain45_n1_α
 xchain45_n0_β:
 jmp xchain45_n2_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain45_n1_α:
 mov rax, qword ptr [rbp + 1264]
 mov rdx, qword ptr [rbp + 1272]
 mov qword ptr [1879052352], rax
 mov qword ptr [1879052360], rdx
 mov qword ptr [rbp + 1248], rax
 mov qword ptr [rbp + 1256], rdx
 jmp xchain45_n2_α
 xchain45_n1_β:
 jmp xchain45_n2_α
 xchain45_n2_α:
 mov rdi, qword ptr [rip + .Lx50_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx50_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx50_3]
 lea rdx, [rip + .Lx50_4]
 jmp rax
.Lx50_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx50_2
.Lx50_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx50_2
.Lx50_1:
 call rt_faildescr@PLT
.Lx50_2:
 mov qword ptr [rbp + 1312], rax
 mov qword ptr [rbp + 1320], rdx
 cmp eax, 99
 je xchain45_n4_α
 jmp xchain45_n3_α
 xchain45_n2_β:
 jmp xchain45_n4_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain45_n3_α:
 mov rax, qword ptr [rbp + 1312]
 mov rdx, qword ptr [rbp + 1320]
 mov qword ptr [1879052368], rax
 mov qword ptr [1879052376], rdx
 mov qword ptr [rbp + 1296], rax
 mov qword ptr [rbp + 1304], rdx
 jmp xchain45_n4_α
 xchain45_n3_β:
 jmp xchain45_n4_α
# IR_LIT_STRING
 xchain45_n4_α:
 mov qword ptr [rbp + 1344], 1
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [rbp + 1352], rax
 jmp xchain45_n5_α
 xchain45_n4_β:
 jmp xchain45_n7_α
.Lx52_0:
 .quad .Lx52_0_s
.Lx52_0_s:
 .string ""
 xchain45_n5_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1344] -> [zr+1392]
 mov rax, qword ptr [rbp + 1344]
 mov qword ptr [rbp + 1392], rax
 mov rax, qword ptr [rbp + 1352]
 mov qword ptr [rbp + 1400], rax
  .section .rodata
  .Lrkfn54: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn54]
 lea rsi, [rbp + 1392]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1376], rax
 mov qword ptr [rbp + 1384], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n6_α
 xchain45_n5_β:
 jmp xchain45_n7_α
 xchain45_n6_α:
 mov rdi, qword ptr [rip + .Lx56_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx56_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx56_3]
 lea rdx, [rip + .Lx56_4]
 jmp rax
.Lx56_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx56_2
.Lx56_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx56_2
.Lx56_1:
 call rt_faildescr@PLT
.Lx56_2:
 mov qword ptr [rbp + 1424], rax
 mov qword ptr [rbp + 1432], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n8_α
 xchain45_n6_β:
 jmp xchain45_n7_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "Push"
# IR_LIT_STRING
 xchain45_n7_α:
 mov qword ptr [rbp + 1680], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [rbp + 1688], rax
 jmp xchain45_n9_α
 xchain45_n7_β:
 jmp xchain45_n12_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "dummy"
# IR_VAR
 xchain45_n8_α:
 mov rax, qword ptr [1879052368]
 mov rdx, qword ptr [1879052376]
 mov qword ptr [rbp + 1536], rax
 mov qword ptr [rbp + 1544], rdx
 jmp xchain45_n10_α
 xchain45_n8_β:
 jmp xchain45_n7_α
 xchain45_n9_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1680] -> [zr+1648]
 mov rax, qword ptr [rbp + 1680]
 mov qword ptr [rbp + 1648], rax
 mov rax, qword ptr [rbp + 1688]
 mov qword ptr [rbp + 1656], rax
  .section .rodata
  .Lrkfn60: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn60]
 lea rsi, [rbp + 1648]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1632], rax
 mov qword ptr [rbp + 1640], rdx
 cmp eax, 99
 je xchain45_n12_α
 jmp xchain45_n11_α
 xchain45_n9_β:
 jmp xchain45_n12_α
# IR_VAR
 xchain45_n10_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 1568], rax
 mov qword ptr [rbp + 1576], rdx
 jmp xchain45_n13_α
 xchain45_n10_β:
 jmp xchain45_n7_α
# IR_ASSIGN gva
 xchain45_n11_α:
 mov rax, qword ptr [rbp + 1632]
 mov rdx, qword ptr [rbp + 1640]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rbp + 1616], rax
 mov qword ptr [rbp + 1624], rdx
 jmp xchain45_n12_α
 xchain45_n11_β:
 jmp xchain45_n12_α
# IR_LIT_STRING
 xchain45_n12_α:
 mov qword ptr [rbp + 16], 1
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [rbp + 24], rax
 jmp xchain45_n14_α
 xchain45_n12_β:
 jmp proc_Unary_ω
.Lx63_0:
 .quad .Lx63_0_s
.Lx63_0_s:
 .string ""
 xchain45_n13_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 1536]
 mov rsi, qword ptr [rbp + 1544]
 mov rdx, qword ptr [rbp + 1568]
 mov rcx, qword ptr [rbp + 1576]
 call str_concat_d@PLT
 mov qword ptr [rbp + 1504], rax
 mov qword ptr [rbp + 1512], rdx
 jmp xchain45_n15_α
 xchain45_n13_β:
 jmp xchain45_n7_α
 xchain45_n14_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+64]
 mov rax, qword ptr [rbp + 16]
 mov qword ptr [rbp + 64], rax
 mov rax, qword ptr [rbp + 24]
 mov qword ptr [rbp + 72], rax
  .section .rodata
  .Lrkfn66: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn66]
 lea rsi, [rbp + 64]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 48], rax
 mov qword ptr [rbp + 56], rdx
 cmp eax, 99
 je proc_Unary_ω
 jmp proc_Unary_γ
 xchain45_n14_β:
 jmp proc_Unary_ω
 xchain45_n15_α:
# BOX IR_CALL EVAL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1504] -> [zr+1472]
 mov rax, qword ptr [rbp + 1504]
 mov qword ptr [rbp + 1472], rax
 mov rax, qword ptr [rbp + 1512]
 mov qword ptr [rbp + 1480], rax
  .section .rodata
  .Lrkfn68: .string "EVAL"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn68]
 lea rsi, [rbp + 1472]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1456], rax
 mov qword ptr [rbp + 1464], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n16_α
 xchain45_n15_β:
 jmp xchain45_n7_α
# IR_ASSIGN_VAR
 xchain45_n16_α:
 mov rdi, qword ptr [rbp + 1424]
 mov rsi, qword ptr [rbp + 1432]
 mov rdx, qword ptr [rbp + 1456]
 mov rcx, qword ptr [rbp + 1464]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain45_n7_α
 mov qword ptr [rbp + 1600], rax
 mov qword ptr [rbp + 1608], rdx
 jmp xchain45_n7_α
 xchain45_n16_β:
 jmp xchain45_n7_α
proc_Unary_res:
add rsp, 8
pop rbp
proc_Unary_β:
jmp proc_Unary_ω
proc_Unary_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 5784]
lea rsp, [rbp + 5808]
mov rbp, [rbp + 5800]
jmp rax
proc_Unary_ω:
mov rax, [rbp + 5792]
lea rsp, [rbp + 5808]
mov rbp, [rbp + 5800]
jmp rax
  .globl proc_Binary_α
proc_Binary_α:
#=======================================================================================================================
    .global proc_Binary_α
    .global proc_Binary_β
    .global proc_Binary_γ
    .global proc_Binary_ω
  sub rsp, 5808
  mov [rsp + 5784], rcx
  mov [rsp + 5792], rdx
  mov [rsp + 5800], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 5776
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 5768], rsp
proc_Binary_α_body:
 xchain70_n0_α:
 mov rdi, qword ptr [rip + .Lx72_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx72_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx72_3]
 lea rdx, [rip + .Lx72_4]
 jmp rax
.Lx72_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx72_2
.Lx72_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx72_2
.Lx72_1:
 call rt_faildescr@PLT
.Lx72_2:
 mov qword ptr [rbp + 1728], rax
 mov qword ptr [rbp + 1736], rdx
 cmp eax, 99
 je xchain70_n2_α
 jmp xchain70_n1_α
 xchain70_n0_β:
 jmp xchain70_n2_α
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain70_n1_α:
 mov rax, qword ptr [rbp + 1728]
 mov rdx, qword ptr [rbp + 1736]
 mov qword ptr [1879052416], rax
 mov qword ptr [1879052424], rdx
 mov qword ptr [rbp + 1712], rax
 mov qword ptr [rbp + 1720], rdx
 jmp xchain70_n2_α
 xchain70_n1_β:
 jmp xchain70_n2_α
 xchain70_n2_α:
 mov rdi, qword ptr [rip + .Lx75_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx75_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx75_3]
 lea rdx, [rip + .Lx75_4]
 jmp rax
.Lx75_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx75_2
.Lx75_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx75_2
.Lx75_1:
 call rt_faildescr@PLT
.Lx75_2:
 mov qword ptr [rbp + 1776], rax
 mov qword ptr [rbp + 1784], rdx
 cmp eax, 99
 je xchain70_n4_α
 jmp xchain70_n3_α
 xchain70_n2_β:
 jmp xchain70_n4_α
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain70_n3_α:
 mov rax, qword ptr [rbp + 1776]
 mov rdx, qword ptr [rbp + 1784]
 mov qword ptr [1879052368], rax
 mov qword ptr [1879052376], rdx
 mov qword ptr [rbp + 1760], rax
 mov qword ptr [rbp + 1768], rdx
 jmp xchain70_n4_α
 xchain70_n3_β:
 jmp xchain70_n4_α
 xchain70_n4_α:
 mov rdi, qword ptr [rip + .Lx78_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx78_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx78_3]
 lea rdx, [rip + .Lx78_4]
 jmp rax
.Lx78_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx78_2
.Lx78_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx78_2
.Lx78_1:
 call rt_faildescr@PLT
.Lx78_2:
 mov qword ptr [rbp + 1824], rax
 mov qword ptr [rbp + 1832], rdx
 cmp eax, 99
 je xchain70_n6_α
 jmp xchain70_n5_α
 xchain70_n4_β:
 jmp xchain70_n6_α
.Lx78_0:
 .quad .Lx78_0_s
.Lx78_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain70_n5_α:
 mov rax, qword ptr [rbp + 1824]
 mov rdx, qword ptr [rbp + 1832]
 mov qword ptr [1879052400], rax
 mov qword ptr [1879052408], rdx
 mov qword ptr [rbp + 1808], rax
 mov qword ptr [rbp + 1816], rdx
 jmp xchain70_n6_α
 xchain70_n5_β:
 jmp xchain70_n6_α
# IR_LIT_STRING
 xchain70_n6_α:
 mov qword ptr [rbp + 1856], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [rbp + 1864], rax
 jmp xchain70_n7_α
 xchain70_n6_β:
 jmp xchain70_n9_α
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string ""
 xchain70_n7_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1856] -> [zr+1904]
 mov rax, qword ptr [rbp + 1856]
 mov qword ptr [rbp + 1904], rax
 mov rax, qword ptr [rbp + 1864]
 mov qword ptr [rbp + 1912], rax
  .section .rodata
  .Lrkfn82: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn82]
 lea rsi, [rbp + 1904]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1888], rax
 mov qword ptr [rbp + 1896], rdx
 cmp eax, 99
 je xchain70_n9_α
 jmp xchain70_n8_α
 xchain70_n7_β:
 jmp xchain70_n9_α
 xchain70_n8_α:
 mov rdi, qword ptr [rip + .Lx84_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx84_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx84_3]
 lea rdx, [rip + .Lx84_4]
 jmp rax
.Lx84_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx84_2
.Lx84_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx84_2
.Lx84_1:
 call rt_faildescr@PLT
.Lx84_2:
 mov qword ptr [rbp + 1936], rax
 mov qword ptr [rbp + 1944], rdx
 cmp eax, 99
 je xchain70_n9_α
 jmp xchain70_n10_α
 xchain70_n8_β:
 jmp xchain70_n9_α
.Lx84_0:
 .quad .Lx84_0_s
.Lx84_0_s:
 .string "Push"
# IR_LIT_STRING
 xchain70_n9_α:
 mov qword ptr [rbp + 2384], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [rbp + 2392], rax
 jmp xchain70_n11_α
 xchain70_n9_β:
 jmp xchain70_n14_α
.Lx85_0:
 .quad .Lx85_0_s
.Lx85_0_s:
 .string "dummy"
# IR_VAR
 xchain70_n10_α:
 mov rax, qword ptr [1879052400]
 mov rdx, qword ptr [1879052408]
 mov qword ptr [rbp + 2144], rax
 mov qword ptr [rbp + 2152], rdx
 jmp xchain70_n12_α
 xchain70_n10_β:
 jmp xchain70_n9_α
 xchain70_n11_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2384] -> [zr+2352]
 mov rax, qword ptr [rbp + 2384]
 mov qword ptr [rbp + 2352], rax
 mov rax, qword ptr [rbp + 2392]
 mov qword ptr [rbp + 2360], rax
  .section .rodata
  .Lrkfn88: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn88]
 lea rsi, [rbp + 2352]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2336], rax
 mov qword ptr [rbp + 2344], rdx
 cmp eax, 99
 je xchain70_n14_α
 jmp xchain70_n13_α
 xchain70_n11_β:
 jmp xchain70_n14_α
# IR_LIT_STRING
 xchain70_n12_α:
 mov qword ptr [rbp + 2176], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [rbp + 2184], rax
 jmp xchain70_n15_α
 xchain70_n12_β:
 jmp xchain70_n9_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string " "
# IR_ASSIGN gva
 xchain70_n13_α:
 mov rax, qword ptr [rbp + 2336]
 mov rdx, qword ptr [rbp + 2344]
 mov qword ptr [1879052384], rax
 mov qword ptr [1879052392], rdx
 mov qword ptr [rbp + 2320], rax
 mov qword ptr [rbp + 2328], rdx
 jmp xchain70_n14_α
 xchain70_n13_β:
 jmp xchain70_n14_α
# IR_LIT_STRING
 xchain70_n14_α:
 mov qword ptr [rbp + 16], 1
 mov rax, qword ptr [rip + .Lx91_0]
 mov qword ptr [rbp + 24], rax
 jmp xchain70_n16_α
 xchain70_n14_β:
 jmp proc_Binary_ω
.Lx91_0:
 .quad .Lx91_0_s
.Lx91_0_s:
 .string ""
 xchain70_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 2144]
 mov rsi, qword ptr [rbp + 2152]
 mov rdx, qword ptr [rbp + 2176]
 mov rcx, qword ptr [rbp + 2184]
 call str_concat_d@PLT
 mov qword ptr [rbp + 2112], rax
 mov qword ptr [rbp + 2120], rdx
 jmp xchain70_n17_α
 xchain70_n15_β:
 jmp xchain70_n9_α
 xchain70_n16_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+64]
 mov rax, qword ptr [rbp + 16]
 mov qword ptr [rbp + 64], rax
 mov rax, qword ptr [rbp + 24]
 mov qword ptr [rbp + 72], rax
  .section .rodata
  .Lrkfn94: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn94]
 lea rsi, [rbp + 64]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 48], rax
 mov qword ptr [rbp + 56], rdx
 cmp eax, 99
 je proc_Binary_ω
 jmp proc_Binary_γ
 xchain70_n16_β:
 jmp proc_Binary_ω
# IR_VAR
 xchain70_n17_α:
 mov rax, qword ptr [1879052368]
 mov rdx, qword ptr [1879052376]
 mov qword ptr [rbp + 2208], rax
 mov qword ptr [rbp + 2216], rdx
 jmp xchain70_n18_α
 xchain70_n17_β:
 jmp xchain70_n9_α
 xchain70_n18_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 2112]
 mov rsi, qword ptr [rbp + 2120]
 mov rdx, qword ptr [rbp + 2208]
 mov rcx, qword ptr [rbp + 2216]
 call str_concat_d@PLT
 mov qword ptr [rbp + 2080], rax
 mov qword ptr [rbp + 2088], rdx
 jmp xchain70_n19_α
 xchain70_n18_β:
 jmp xchain70_n9_α
# IR_LIT_STRING
 xchain70_n19_α:
 mov qword ptr [rbp + 2240], 1
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [rbp + 2248], rax
 jmp xchain70_n20_α
 xchain70_n19_β:
 jmp xchain70_n9_α
.Lx97_0:
 .quad .Lx97_0_s
.Lx97_0_s:
 .string " "
 xchain70_n20_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 2080]
 mov rsi, qword ptr [rbp + 2088]
 mov rdx, qword ptr [rbp + 2240]
 mov rcx, qword ptr [rbp + 2248]
 call str_concat_d@PLT
 mov qword ptr [rbp + 2048], rax
 mov qword ptr [rbp + 2056], rdx
 jmp xchain70_n21_α
 xchain70_n20_β:
 jmp xchain70_n9_α
# IR_VAR
 xchain70_n21_α:
 mov rax, qword ptr [1879052416]
 mov rdx, qword ptr [1879052424]
 mov qword ptr [rbp + 2272], rax
 mov qword ptr [rbp + 2280], rdx
 jmp xchain70_n22_α
 xchain70_n21_β:
 jmp xchain70_n9_α
 xchain70_n22_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 2048]
 mov rsi, qword ptr [rbp + 2056]
 mov rdx, qword ptr [rbp + 2272]
 mov rcx, qword ptr [rbp + 2280]
 call str_concat_d@PLT
 mov qword ptr [rbp + 2016], rax
 mov qword ptr [rbp + 2024], rdx
 jmp xchain70_n23_α
 xchain70_n22_β:
 jmp xchain70_n9_α
 xchain70_n23_α:
# BOX IR_CALL EVAL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2016] -> [zr+1984]
 mov rax, qword ptr [rbp + 2016]
 mov qword ptr [rbp + 1984], rax
 mov rax, qword ptr [rbp + 2024]
 mov qword ptr [rbp + 1992], rax
  .section .rodata
  .Lrkfn102: .string "EVAL"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn102]
 lea rsi, [rbp + 1984]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1968], rax
 mov qword ptr [rbp + 1976], rdx
 cmp eax, 99
 je xchain70_n9_α
 jmp xchain70_n24_α
 xchain70_n23_β:
 jmp xchain70_n9_α
# IR_ASSIGN_VAR
 xchain70_n24_α:
 mov rdi, qword ptr [rbp + 1936]
 mov rsi, qword ptr [rbp + 1944]
 mov rdx, qword ptr [rbp + 1968]
 mov rcx, qword ptr [rbp + 1976]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain70_n9_α
 mov qword ptr [rbp + 2304], rax
 mov qword ptr [rbp + 2312], rdx
 jmp xchain70_n9_α
 xchain70_n24_β:
 jmp xchain70_n9_α
proc_Binary_res:
add rsp, 8
pop rbp
proc_Binary_β:
jmp proc_Binary_ω
proc_Binary_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 5784]
lea rsp, [rbp + 5808]
mov rbp, [rbp + 5800]
jmp rax
proc_Binary_ω:
mov rax, [rbp + 5792]
lea rsp, [rbp + 5808]
mov rbp, [rbp + 5800]
jmp rax
  .globl proc_EXPR$0_α
proc_EXPR$0_α:
#=======================================================================================================================
    .global proc_EXPR$0_α
    .global proc_EXPR$0_β
    .global proc_EXPR$0_γ
    .global proc_EXPR$0_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$0_α_body:
 xchain104_n0_α:
 mov rdi, qword ptr [rip + .Lx106_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx106_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx106_3]
 lea rdx, [rip + .Lx106_4]
 jmp rax
.Lx106_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx106_2
.Lx106_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx106_2
.Lx106_1:
 call rt_faildescr@PLT
.Lx106_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$0_ω
 jmp xchain104_n1_α
 xchain104_n0_β:
 jmp proc_EXPR$0_ω
.Lx106_0:
 .quad .Lx106_0_s
.Lx106_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain104_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052640], rax
 mov qword ptr [1879052648], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$0_γ
 xchain104_n1_β:
 jmp proc_EXPR$0_ω
proc_EXPR$0_res:
add rsp, 8
pop rbp
proc_EXPR$0_β:
jmp proc_EXPR$0_ω
proc_EXPR$0_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$0_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$1_α
proc_EXPR$1_α:
#=======================================================================================================================
    .global proc_EXPR$1_α
    .global proc_EXPR$1_β
    .global proc_EXPR$1_γ
    .global proc_EXPR$1_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$1_α_body:
 xchain108_n0_α:
 mov rdi, qword ptr [rip + .Lx110_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx110_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx110_3]
 lea rdx, [rip + .Lx110_4]
 jmp rax
.Lx110_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx110_2
.Lx110_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx110_2
.Lx110_1:
 call rt_faildescr@PLT
.Lx110_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$1_ω
 jmp xchain108_n1_α
 xchain108_n0_β:
 jmp proc_EXPR$1_ω
.Lx110_0:
 .quad .Lx110_0_s
.Lx110_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain108_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052656], rax
 mov qword ptr [1879052664], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$1_γ
 xchain108_n1_β:
 jmp proc_EXPR$1_ω
proc_EXPR$1_res:
add rsp, 8
pop rbp
proc_EXPR$1_β:
jmp proc_EXPR$1_ω
proc_EXPR$1_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$1_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$2_α
proc_EXPR$2_α:
#=======================================================================================================================
    .global proc_EXPR$2_α
    .global proc_EXPR$2_β
    .global proc_EXPR$2_γ
    .global proc_EXPR$2_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$2_α_body:
 xchain112_n0_α:
 mov rdi, qword ptr [rip + .Lx114_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx114_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx114_3]
 lea rdx, [rip + .Lx114_4]
 jmp rax
.Lx114_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx114_2
.Lx114_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx114_2
.Lx114_1:
 call rt_faildescr@PLT
.Lx114_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$2_ω
 jmp xchain112_n1_α
 xchain112_n0_β:
 jmp proc_EXPR$2_ω
.Lx114_0:
 .quad .Lx114_0_s
.Lx114_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain112_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052672], rax
 mov qword ptr [1879052680], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$2_γ
 xchain112_n1_β:
 jmp proc_EXPR$2_ω
proc_EXPR$2_res:
add rsp, 8
pop rbp
proc_EXPR$2_β:
jmp proc_EXPR$2_ω
proc_EXPR$2_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$2_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$3_α
proc_EXPR$3_α:
#=======================================================================================================================
    .global proc_EXPR$3_α
    .global proc_EXPR$3_β
    .global proc_EXPR$3_γ
    .global proc_EXPR$3_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$3_α_body:
 xchain116_n0_α:
 mov rdi, qword ptr [rip + .Lx118_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx118_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx118_3]
 lea rdx, [rip + .Lx118_4]
 jmp rax
.Lx118_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx118_2
.Lx118_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx118_2
.Lx118_1:
 call rt_faildescr@PLT
.Lx118_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$3_ω
 jmp xchain116_n1_α
 xchain116_n0_β:
 jmp proc_EXPR$3_ω
.Lx118_0:
 .quad .Lx118_0_s
.Lx118_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain116_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052688], rax
 mov qword ptr [1879052696], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$3_γ
 xchain116_n1_β:
 jmp proc_EXPR$3_ω
proc_EXPR$3_res:
add rsp, 8
pop rbp
proc_EXPR$3_β:
jmp proc_EXPR$3_ω
proc_EXPR$3_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$3_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$4_α
proc_EXPR$4_α:
#=======================================================================================================================
    .global proc_EXPR$4_α
    .global proc_EXPR$4_β
    .global proc_EXPR$4_γ
    .global proc_EXPR$4_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$4_α_body:
# IR_VAR
 xchain120_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain120_n1_α
 xchain120_n0_β:
 jmp proc_EXPR$4_ω
# IR_ASSIGN gva
 xchain120_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052704], rax
 mov qword ptr [1879052712], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$4_γ
 xchain120_n1_β:
 jmp proc_EXPR$4_ω
proc_EXPR$4_res:
add rsp, 8
pop rbp
proc_EXPR$4_β:
jmp proc_EXPR$4_ω
proc_EXPR$4_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$4_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$5_α
proc_EXPR$5_α:
#=======================================================================================================================
    .global proc_EXPR$5_α
    .global proc_EXPR$5_β
    .global proc_EXPR$5_γ
    .global proc_EXPR$5_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$5_α_body:
# IR_VAR
 xchain123_n0_α:
 mov rax, qword ptr [1879052560]
 mov rdx, qword ptr [1879052568]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain123_n1_α
 xchain123_n0_β:
 jmp proc_EXPR$5_ω
# IR_ASSIGN gva
 xchain123_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052720], rax
 mov qword ptr [1879052728], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$5_γ
 xchain123_n1_β:
 jmp proc_EXPR$5_ω
proc_EXPR$5_res:
add rsp, 8
pop rbp
proc_EXPR$5_β:
jmp proc_EXPR$5_ω
proc_EXPR$5_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$5_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$6_α
proc_EXPR$6_α:
#=======================================================================================================================
    .global proc_EXPR$6_α
    .global proc_EXPR$6_β
    .global proc_EXPR$6_γ
    .global proc_EXPR$6_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$6_α_body:
# IR_VAR
 xchain126_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain126_n1_α
 xchain126_n0_β:
 jmp proc_EXPR$6_ω
# IR_ASSIGN gva
 xchain126_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052736], rax
 mov qword ptr [1879052744], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$6_γ
 xchain126_n1_β:
 jmp proc_EXPR$6_ω
proc_EXPR$6_res:
add rsp, 8
pop rbp
proc_EXPR$6_β:
jmp proc_EXPR$6_ω
proc_EXPR$6_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$6_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$7_α
proc_EXPR$7_α:
#=======================================================================================================================
    .global proc_EXPR$7_α
    .global proc_EXPR$7_β
    .global proc_EXPR$7_γ
    .global proc_EXPR$7_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$7_α_body:
 xchain129_n0_α:
 mov rdi, qword ptr [rip + .Lx131_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx131_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx131_3]
 lea rdx, [rip + .Lx131_4]
 jmp rax
.Lx131_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx131_2
.Lx131_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx131_2
.Lx131_1:
 call rt_faildescr@PLT
.Lx131_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$7_ω
 jmp xchain129_n1_α
 xchain129_n0_β:
 jmp proc_EXPR$7_ω
.Lx131_0:
 .quad .Lx131_0_s
.Lx131_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain129_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052752], rax
 mov qword ptr [1879052760], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$7_γ
 xchain129_n1_β:
 jmp proc_EXPR$7_ω
proc_EXPR$7_res:
add rsp, 8
pop rbp
proc_EXPR$7_β:
jmp proc_EXPR$7_ω
proc_EXPR$7_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$7_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$8_α
proc_EXPR$8_α:
#=======================================================================================================================
    .global proc_EXPR$8_α
    .global proc_EXPR$8_β
    .global proc_EXPR$8_γ
    .global proc_EXPR$8_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$8_α_body:
# IR_VAR
 xchain133_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain133_n1_α
 xchain133_n0_β:
 jmp proc_EXPR$8_ω
# IR_ASSIGN gva
 xchain133_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052768], rax
 mov qword ptr [1879052776], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$8_γ
 xchain133_n1_β:
 jmp proc_EXPR$8_ω
proc_EXPR$8_res:
add rsp, 8
pop rbp
proc_EXPR$8_β:
jmp proc_EXPR$8_ω
proc_EXPR$8_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$8_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$9_α
proc_EXPR$9_α:
#=======================================================================================================================
    .global proc_EXPR$9_α
    .global proc_EXPR$9_β
    .global proc_EXPR$9_γ
    .global proc_EXPR$9_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$9_α_body:
# IR_VAR
 xchain136_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain136_n1_α
 xchain136_n0_β:
 jmp proc_EXPR$9_ω
# IR_ASSIGN gva
 xchain136_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052784], rax
 mov qword ptr [1879052792], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$9_γ
 xchain136_n1_β:
 jmp proc_EXPR$9_ω
proc_EXPR$9_res:
add rsp, 8
pop rbp
proc_EXPR$9_β:
jmp proc_EXPR$9_ω
proc_EXPR$9_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$9_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$10_α
proc_EXPR$10_α:
#=======================================================================================================================
    .global proc_EXPR$10_α
    .global proc_EXPR$10_β
    .global proc_EXPR$10_γ
    .global proc_EXPR$10_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$10_α_body:
# IR_VAR
 xchain139_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain139_n1_α
 xchain139_n0_β:
 jmp proc_EXPR$10_ω
# IR_ASSIGN gva
 xchain139_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052800], rax
 mov qword ptr [1879052808], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$10_γ
 xchain139_n1_β:
 jmp proc_EXPR$10_ω
proc_EXPR$10_res:
add rsp, 8
pop rbp
proc_EXPR$10_β:
jmp proc_EXPR$10_ω
proc_EXPR$10_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$10_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$11_α
proc_EXPR$11_α:
#=======================================================================================================================
    .global proc_EXPR$11_α
    .global proc_EXPR$11_β
    .global proc_EXPR$11_γ
    .global proc_EXPR$11_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$11_α_body:
 xchain142_n0_α:
 mov rdi, qword ptr [rip + .Lx144_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx144_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx144_3]
 lea rdx, [rip + .Lx144_4]
 jmp rax
.Lx144_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx144_2
.Lx144_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx144_2
.Lx144_1:
 call rt_faildescr@PLT
.Lx144_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$11_ω
 jmp xchain142_n1_α
 xchain142_n0_β:
 jmp proc_EXPR$11_ω
.Lx144_0:
 .quad .Lx144_0_s
.Lx144_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain142_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052816], rax
 mov qword ptr [1879052824], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$11_γ
 xchain142_n1_β:
 jmp proc_EXPR$11_ω
proc_EXPR$11_res:
add rsp, 8
pop rbp
proc_EXPR$11_β:
jmp proc_EXPR$11_ω
proc_EXPR$11_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$11_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$12_α
proc_EXPR$12_α:
#=======================================================================================================================
    .global proc_EXPR$12_α
    .global proc_EXPR$12_β
    .global proc_EXPR$12_γ
    .global proc_EXPR$12_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$12_α_body:
# IR_VAR
 xchain146_n0_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain146_n1_α
 xchain146_n0_β:
 jmp proc_EXPR$12_ω
# IR_ASSIGN gva
 xchain146_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052832], rax
 mov qword ptr [1879052840], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$12_γ
 xchain146_n1_β:
 jmp proc_EXPR$12_ω
proc_EXPR$12_res:
add rsp, 8
pop rbp
proc_EXPR$12_β:
jmp proc_EXPR$12_ω
proc_EXPR$12_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$12_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$13_α
proc_EXPR$13_α:
#=======================================================================================================================
    .global proc_EXPR$13_α
    .global proc_EXPR$13_β
    .global proc_EXPR$13_γ
    .global proc_EXPR$13_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$13_α_body:
# IR_VAR
 xchain149_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain149_n1_α
 xchain149_n0_β:
 jmp proc_EXPR$13_ω
# IR_ASSIGN gva
 xchain149_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052848], rax
 mov qword ptr [1879052856], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$13_γ
 xchain149_n1_β:
 jmp proc_EXPR$13_ω
proc_EXPR$13_res:
add rsp, 8
pop rbp
proc_EXPR$13_β:
jmp proc_EXPR$13_ω
proc_EXPR$13_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$13_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$14_α
proc_EXPR$14_α:
#=======================================================================================================================
    .global proc_EXPR$14_α
    .global proc_EXPR$14_β
    .global proc_EXPR$14_γ
    .global proc_EXPR$14_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$14_α_body:
 xchain152_n0_α:
 mov rdi, qword ptr [rip + .Lx154_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx154_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx154_3]
 lea rdx, [rip + .Lx154_4]
 jmp rax
.Lx154_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx154_2
.Lx154_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx154_2
.Lx154_1:
 call rt_faildescr@PLT
.Lx154_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$14_ω
 jmp xchain152_n1_α
 xchain152_n0_β:
 jmp proc_EXPR$14_ω
.Lx154_0:
 .quad .Lx154_0_s
.Lx154_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain152_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052864], rax
 mov qword ptr [1879052872], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$14_γ
 xchain152_n1_β:
 jmp proc_EXPR$14_ω
proc_EXPR$14_res:
add rsp, 8
pop rbp
proc_EXPR$14_β:
jmp proc_EXPR$14_ω
proc_EXPR$14_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$14_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$15_α
proc_EXPR$15_α:
#=======================================================================================================================
    .global proc_EXPR$15_α
    .global proc_EXPR$15_β
    .global proc_EXPR$15_γ
    .global proc_EXPR$15_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$15_α_body:
 xchain156_n0_α:
 mov rdi, qword ptr [rip + .Lx158_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx158_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx158_3]
 lea rdx, [rip + .Lx158_4]
 jmp rax
.Lx158_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx158_2
.Lx158_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx158_2
.Lx158_1:
 call rt_faildescr@PLT
.Lx158_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$15_ω
 jmp xchain156_n1_α
 xchain156_n0_β:
 jmp proc_EXPR$15_ω
.Lx158_0:
 .quad .Lx158_0_s
.Lx158_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain156_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052880], rax
 mov qword ptr [1879052888], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$15_γ
 xchain156_n1_β:
 jmp proc_EXPR$15_ω
proc_EXPR$15_res:
add rsp, 8
pop rbp
proc_EXPR$15_β:
jmp proc_EXPR$15_ω
proc_EXPR$15_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$15_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$16_α
proc_EXPR$16_α:
#=======================================================================================================================
    .global proc_EXPR$16_α
    .global proc_EXPR$16_β
    .global proc_EXPR$16_γ
    .global proc_EXPR$16_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$16_α_body:
 xchain160_n0_α:
 mov rdi, qword ptr [rip + .Lx162_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx162_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx162_3]
 lea rdx, [rip + .Lx162_4]
 jmp rax
.Lx162_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx162_2
.Lx162_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx162_2
.Lx162_1:
 call rt_faildescr@PLT
.Lx162_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$16_ω
 jmp xchain160_n1_α
 xchain160_n0_β:
 jmp proc_EXPR$16_ω
.Lx162_0:
 .quad .Lx162_0_s
.Lx162_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain160_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052896], rax
 mov qword ptr [1879052904], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$16_γ
 xchain160_n1_β:
 jmp proc_EXPR$16_ω
proc_EXPR$16_res:
add rsp, 8
pop rbp
proc_EXPR$16_β:
jmp proc_EXPR$16_ω
proc_EXPR$16_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$16_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$17_α
proc_EXPR$17_α:
#=======================================================================================================================
    .global proc_EXPR$17_α
    .global proc_EXPR$17_β
    .global proc_EXPR$17_γ
    .global proc_EXPR$17_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$17_α_body:
 xchain164_n0_α:
 mov rdi, qword ptr [rip + .Lx166_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx166_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx166_3]
 lea rdx, [rip + .Lx166_4]
 jmp rax
.Lx166_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx166_2
.Lx166_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx166_2
.Lx166_1:
 call rt_faildescr@PLT
.Lx166_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$17_ω
 jmp xchain164_n1_α
 xchain164_n0_β:
 jmp proc_EXPR$17_ω
.Lx166_0:
 .quad .Lx166_0_s
.Lx166_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain164_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052912], rax
 mov qword ptr [1879052920], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$17_γ
 xchain164_n1_β:
 jmp proc_EXPR$17_ω
proc_EXPR$17_res:
add rsp, 8
pop rbp
proc_EXPR$17_β:
jmp proc_EXPR$17_ω
proc_EXPR$17_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$17_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$18_α
proc_EXPR$18_α:
#=======================================================================================================================
    .global proc_EXPR$18_α
    .global proc_EXPR$18_β
    .global proc_EXPR$18_γ
    .global proc_EXPR$18_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$18_α_body:
# IR_VAR
 xchain168_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain168_n1_α
 xchain168_n0_β:
 jmp proc_EXPR$18_ω
# IR_ASSIGN gva
 xchain168_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052928], rax
 mov qword ptr [1879052936], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$18_γ
 xchain168_n1_β:
 jmp proc_EXPR$18_ω
proc_EXPR$18_res:
add rsp, 8
pop rbp
proc_EXPR$18_β:
jmp proc_EXPR$18_ω
proc_EXPR$18_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$18_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$19_α
proc_EXPR$19_α:
#=======================================================================================================================
    .global proc_EXPR$19_α
    .global proc_EXPR$19_β
    .global proc_EXPR$19_γ
    .global proc_EXPR$19_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$19_α_body:
# IR_VAR
 xchain171_n0_α:
 mov rax, qword ptr [1879052560]
 mov rdx, qword ptr [1879052568]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain171_n1_α
 xchain171_n0_β:
 jmp proc_EXPR$19_ω
# IR_ASSIGN gva
 xchain171_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052944], rax
 mov qword ptr [1879052952], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$19_γ
 xchain171_n1_β:
 jmp proc_EXPR$19_ω
proc_EXPR$19_res:
add rsp, 8
pop rbp
proc_EXPR$19_β:
jmp proc_EXPR$19_ω
proc_EXPR$19_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$19_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$20_α
proc_EXPR$20_α:
#=======================================================================================================================
    .global proc_EXPR$20_α
    .global proc_EXPR$20_β
    .global proc_EXPR$20_γ
    .global proc_EXPR$20_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$20_α_body:
# IR_VAR
 xchain174_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain174_n1_α
 xchain174_n0_β:
 jmp proc_EXPR$20_ω
# IR_ASSIGN gva
 xchain174_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052960], rax
 mov qword ptr [1879052968], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$20_γ
 xchain174_n1_β:
 jmp proc_EXPR$20_ω
proc_EXPR$20_res:
add rsp, 8
pop rbp
proc_EXPR$20_β:
jmp proc_EXPR$20_ω
proc_EXPR$20_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$20_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$21_α
proc_EXPR$21_α:
#=======================================================================================================================
    .global proc_EXPR$21_α
    .global proc_EXPR$21_β
    .global proc_EXPR$21_γ
    .global proc_EXPR$21_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$21_α_body:
 xchain177_n0_α:
 mov rdi, qword ptr [rip + .Lx179_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx179_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx179_3]
 lea rdx, [rip + .Lx179_4]
 jmp rax
.Lx179_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx179_2
.Lx179_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx179_2
.Lx179_1:
 call rt_faildescr@PLT
.Lx179_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$21_ω
 jmp xchain177_n1_α
 xchain177_n0_β:
 jmp proc_EXPR$21_ω
.Lx179_0:
 .quad .Lx179_0_s
.Lx179_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain177_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052976], rax
 mov qword ptr [1879052984], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$21_γ
 xchain177_n1_β:
 jmp proc_EXPR$21_ω
proc_EXPR$21_res:
add rsp, 8
pop rbp
proc_EXPR$21_β:
jmp proc_EXPR$21_ω
proc_EXPR$21_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$21_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$22_α
proc_EXPR$22_α:
#=======================================================================================================================
    .global proc_EXPR$22_α
    .global proc_EXPR$22_β
    .global proc_EXPR$22_γ
    .global proc_EXPR$22_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$22_α_body:
# IR_VAR
 xchain181_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain181_n1_α
 xchain181_n0_β:
 jmp proc_EXPR$22_ω
# IR_ASSIGN gva
 xchain181_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052992], rax
 mov qword ptr [1879053000], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$22_γ
 xchain181_n1_β:
 jmp proc_EXPR$22_ω
proc_EXPR$22_res:
add rsp, 8
pop rbp
proc_EXPR$22_β:
jmp proc_EXPR$22_ω
proc_EXPR$22_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$22_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$23_α
proc_EXPR$23_α:
#=======================================================================================================================
    .global proc_EXPR$23_α
    .global proc_EXPR$23_β
    .global proc_EXPR$23_γ
    .global proc_EXPR$23_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$23_α_body:
# IR_VAR
 xchain184_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain184_n1_α
 xchain184_n0_β:
 jmp proc_EXPR$23_ω
# IR_ASSIGN gva
 xchain184_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053008], rax
 mov qword ptr [1879053016], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$23_γ
 xchain184_n1_β:
 jmp proc_EXPR$23_ω
proc_EXPR$23_res:
add rsp, 8
pop rbp
proc_EXPR$23_β:
jmp proc_EXPR$23_ω
proc_EXPR$23_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$23_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$24_α
proc_EXPR$24_α:
#=======================================================================================================================
    .global proc_EXPR$24_α
    .global proc_EXPR$24_β
    .global proc_EXPR$24_γ
    .global proc_EXPR$24_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$24_α_body:
# IR_VAR
 xchain187_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain187_n1_α
 xchain187_n0_β:
 jmp proc_EXPR$24_ω
# IR_ASSIGN gva
 xchain187_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053024], rax
 mov qword ptr [1879053032], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$24_γ
 xchain187_n1_β:
 jmp proc_EXPR$24_ω
proc_EXPR$24_res:
add rsp, 8
pop rbp
proc_EXPR$24_β:
jmp proc_EXPR$24_ω
proc_EXPR$24_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$24_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$25_α
proc_EXPR$25_α:
#=======================================================================================================================
    .global proc_EXPR$25_α
    .global proc_EXPR$25_β
    .global proc_EXPR$25_γ
    .global proc_EXPR$25_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$25_α_body:
 xchain190_n0_α:
 mov rdi, qword ptr [rip + .Lx192_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx192_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx192_3]
 lea rdx, [rip + .Lx192_4]
 jmp rax
.Lx192_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx192_2
.Lx192_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx192_2
.Lx192_1:
 call rt_faildescr@PLT
.Lx192_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$25_ω
 jmp xchain190_n1_α
 xchain190_n0_β:
 jmp proc_EXPR$25_ω
.Lx192_0:
 .quad .Lx192_0_s
.Lx192_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain190_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053040], rax
 mov qword ptr [1879053048], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$25_γ
 xchain190_n1_β:
 jmp proc_EXPR$25_ω
proc_EXPR$25_res:
add rsp, 8
pop rbp
proc_EXPR$25_β:
jmp proc_EXPR$25_ω
proc_EXPR$25_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$25_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$26_α
proc_EXPR$26_α:
#=======================================================================================================================
    .global proc_EXPR$26_α
    .global proc_EXPR$26_β
    .global proc_EXPR$26_γ
    .global proc_EXPR$26_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$26_α_body:
# IR_VAR
 xchain194_n0_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain194_n1_α
 xchain194_n0_β:
 jmp proc_EXPR$26_ω
# IR_ASSIGN gva
 xchain194_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053056], rax
 mov qword ptr [1879053064], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$26_γ
 xchain194_n1_β:
 jmp proc_EXPR$26_ω
proc_EXPR$26_res:
add rsp, 8
pop rbp
proc_EXPR$26_β:
jmp proc_EXPR$26_ω
proc_EXPR$26_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$26_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$27_α
proc_EXPR$27_α:
#=======================================================================================================================
    .global proc_EXPR$27_α
    .global proc_EXPR$27_β
    .global proc_EXPR$27_γ
    .global proc_EXPR$27_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$27_α_body:
# IR_VAR
 xchain197_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain197_n1_α
 xchain197_n0_β:
 jmp proc_EXPR$27_ω
# IR_ASSIGN gva
 xchain197_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053072], rax
 mov qword ptr [1879053080], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$27_γ
 xchain197_n1_β:
 jmp proc_EXPR$27_ω
proc_EXPR$27_res:
add rsp, 8
pop rbp
proc_EXPR$27_β:
jmp proc_EXPR$27_ω
proc_EXPR$27_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$27_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$28_α
proc_EXPR$28_α:
#=======================================================================================================================
    .global proc_EXPR$28_α
    .global proc_EXPR$28_β
    .global proc_EXPR$28_γ
    .global proc_EXPR$28_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$28_α_body:
 xchain200_n0_α:
 mov rdi, qword ptr [rip + .Lx202_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx202_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx202_3]
 lea rdx, [rip + .Lx202_4]
 jmp rax
.Lx202_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx202_2
.Lx202_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx202_2
.Lx202_1:
 call rt_faildescr@PLT
.Lx202_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$28_ω
 jmp xchain200_n1_α
 xchain200_n0_β:
 jmp proc_EXPR$28_ω
.Lx202_0:
 .quad .Lx202_0_s
.Lx202_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain200_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053088], rax
 mov qword ptr [1879053096], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$28_γ
 xchain200_n1_β:
 jmp proc_EXPR$28_ω
proc_EXPR$28_res:
add rsp, 8
pop rbp
proc_EXPR$28_β:
jmp proc_EXPR$28_ω
proc_EXPR$28_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$28_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$29_α
proc_EXPR$29_α:
#=======================================================================================================================
    .global proc_EXPR$29_α
    .global proc_EXPR$29_β
    .global proc_EXPR$29_γ
    .global proc_EXPR$29_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$29_α_body:
 xchain204_n0_α:
 mov rdi, qword ptr [rip + .Lx206_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx206_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx206_3]
 lea rdx, [rip + .Lx206_4]
 jmp rax
.Lx206_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx206_2
.Lx206_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx206_2
.Lx206_1:
 call rt_faildescr@PLT
.Lx206_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$29_ω
 jmp xchain204_n1_α
 xchain204_n0_β:
 jmp proc_EXPR$29_ω
.Lx206_0:
 .quad .Lx206_0_s
.Lx206_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain204_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053104], rax
 mov qword ptr [1879053112], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$29_γ
 xchain204_n1_β:
 jmp proc_EXPR$29_ω
proc_EXPR$29_res:
add rsp, 8
pop rbp
proc_EXPR$29_β:
jmp proc_EXPR$29_ω
proc_EXPR$29_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$29_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$30_α
proc_EXPR$30_α:
#=======================================================================================================================
    .global proc_EXPR$30_α
    .global proc_EXPR$30_β
    .global proc_EXPR$30_γ
    .global proc_EXPR$30_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$30_α_body:
 xchain208_n0_α:
 mov rdi, qword ptr [rip + .Lx210_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx210_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx210_3]
 lea rdx, [rip + .Lx210_4]
 jmp rax
.Lx210_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx210_2
.Lx210_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx210_2
.Lx210_1:
 call rt_faildescr@PLT
.Lx210_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$30_ω
 jmp xchain208_n1_α
 xchain208_n0_β:
 jmp proc_EXPR$30_ω
.Lx210_0:
 .quad .Lx210_0_s
.Lx210_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain208_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053120], rax
 mov qword ptr [1879053128], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$30_γ
 xchain208_n1_β:
 jmp proc_EXPR$30_ω
proc_EXPR$30_res:
add rsp, 8
pop rbp
proc_EXPR$30_β:
jmp proc_EXPR$30_ω
proc_EXPR$30_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$30_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$31_α
proc_EXPR$31_α:
#=======================================================================================================================
    .global proc_EXPR$31_α
    .global proc_EXPR$31_β
    .global proc_EXPR$31_γ
    .global proc_EXPR$31_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$31_α_body:
 xchain212_n0_α:
 mov rdi, qword ptr [rip + .Lx214_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx214_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx214_3]
 lea rdx, [rip + .Lx214_4]
 jmp rax
.Lx214_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx214_2
.Lx214_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx214_2
.Lx214_1:
 call rt_faildescr@PLT
.Lx214_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$31_ω
 jmp xchain212_n1_α
 xchain212_n0_β:
 jmp proc_EXPR$31_ω
.Lx214_0:
 .quad .Lx214_0_s
.Lx214_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain212_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053136], rax
 mov qword ptr [1879053144], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$31_γ
 xchain212_n1_β:
 jmp proc_EXPR$31_ω
proc_EXPR$31_res:
add rsp, 8
pop rbp
proc_EXPR$31_β:
jmp proc_EXPR$31_ω
proc_EXPR$31_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$31_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$32_α
proc_EXPR$32_α:
#=======================================================================================================================
    .global proc_EXPR$32_α
    .global proc_EXPR$32_β
    .global proc_EXPR$32_γ
    .global proc_EXPR$32_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$32_α_body:
# IR_VAR
 xchain216_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain216_n1_α
 xchain216_n0_β:
 jmp proc_EXPR$32_ω
# IR_ASSIGN gva
 xchain216_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053152], rax
 mov qword ptr [1879053160], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$32_γ
 xchain216_n1_β:
 jmp proc_EXPR$32_ω
proc_EXPR$32_res:
add rsp, 8
pop rbp
proc_EXPR$32_β:
jmp proc_EXPR$32_ω
proc_EXPR$32_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$32_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$33_α
proc_EXPR$33_α:
#=======================================================================================================================
    .global proc_EXPR$33_α
    .global proc_EXPR$33_β
    .global proc_EXPR$33_γ
    .global proc_EXPR$33_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$33_α_body:
# IR_VAR
 xchain219_n0_α:
 mov rax, qword ptr [1879052560]
 mov rdx, qword ptr [1879052568]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain219_n1_α
 xchain219_n0_β:
 jmp proc_EXPR$33_ω
# IR_ASSIGN gva
 xchain219_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053168], rax
 mov qword ptr [1879053176], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$33_γ
 xchain219_n1_β:
 jmp proc_EXPR$33_ω
proc_EXPR$33_res:
add rsp, 8
pop rbp
proc_EXPR$33_β:
jmp proc_EXPR$33_ω
proc_EXPR$33_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$33_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$34_α
proc_EXPR$34_α:
#=======================================================================================================================
    .global proc_EXPR$34_α
    .global proc_EXPR$34_β
    .global proc_EXPR$34_γ
    .global proc_EXPR$34_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$34_α_body:
# IR_VAR
 xchain222_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain222_n1_α
 xchain222_n0_β:
 jmp proc_EXPR$34_ω
# IR_ASSIGN gva
 xchain222_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053184], rax
 mov qword ptr [1879053192], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$34_γ
 xchain222_n1_β:
 jmp proc_EXPR$34_ω
proc_EXPR$34_res:
add rsp, 8
pop rbp
proc_EXPR$34_β:
jmp proc_EXPR$34_ω
proc_EXPR$34_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$34_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$35_α
proc_EXPR$35_α:
#=======================================================================================================================
    .global proc_EXPR$35_α
    .global proc_EXPR$35_β
    .global proc_EXPR$35_γ
    .global proc_EXPR$35_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$35_α_body:
 xchain225_n0_α:
 mov rdi, qword ptr [rip + .Lx227_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx227_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx227_3]
 lea rdx, [rip + .Lx227_4]
 jmp rax
.Lx227_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx227_2
.Lx227_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx227_2
.Lx227_1:
 call rt_faildescr@PLT
.Lx227_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$35_ω
 jmp xchain225_n1_α
 xchain225_n0_β:
 jmp proc_EXPR$35_ω
.Lx227_0:
 .quad .Lx227_0_s
.Lx227_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain225_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053200], rax
 mov qword ptr [1879053208], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$35_γ
 xchain225_n1_β:
 jmp proc_EXPR$35_ω
proc_EXPR$35_res:
add rsp, 8
pop rbp
proc_EXPR$35_β:
jmp proc_EXPR$35_ω
proc_EXPR$35_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$35_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$36_α
proc_EXPR$36_α:
#=======================================================================================================================
    .global proc_EXPR$36_α
    .global proc_EXPR$36_β
    .global proc_EXPR$36_γ
    .global proc_EXPR$36_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$36_α_body:
# IR_VAR
 xchain229_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain229_n1_α
 xchain229_n0_β:
 jmp proc_EXPR$36_ω
# IR_ASSIGN gva
 xchain229_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053216], rax
 mov qword ptr [1879053224], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$36_γ
 xchain229_n1_β:
 jmp proc_EXPR$36_ω
proc_EXPR$36_res:
add rsp, 8
pop rbp
proc_EXPR$36_β:
jmp proc_EXPR$36_ω
proc_EXPR$36_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$36_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$37_α
proc_EXPR$37_α:
#=======================================================================================================================
    .global proc_EXPR$37_α
    .global proc_EXPR$37_β
    .global proc_EXPR$37_γ
    .global proc_EXPR$37_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$37_α_body:
# IR_VAR
 xchain232_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain232_n1_α
 xchain232_n0_β:
 jmp proc_EXPR$37_ω
# IR_ASSIGN gva
 xchain232_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053232], rax
 mov qword ptr [1879053240], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$37_γ
 xchain232_n1_β:
 jmp proc_EXPR$37_ω
proc_EXPR$37_res:
add rsp, 8
pop rbp
proc_EXPR$37_β:
jmp proc_EXPR$37_ω
proc_EXPR$37_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$37_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$38_α
proc_EXPR$38_α:
#=======================================================================================================================
    .global proc_EXPR$38_α
    .global proc_EXPR$38_β
    .global proc_EXPR$38_γ
    .global proc_EXPR$38_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$38_α_body:
# IR_VAR
 xchain235_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain235_n1_α
 xchain235_n0_β:
 jmp proc_EXPR$38_ω
# IR_ASSIGN gva
 xchain235_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053248], rax
 mov qword ptr [1879053256], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$38_γ
 xchain235_n1_β:
 jmp proc_EXPR$38_ω
proc_EXPR$38_res:
add rsp, 8
pop rbp
proc_EXPR$38_β:
jmp proc_EXPR$38_ω
proc_EXPR$38_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$38_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$39_α
proc_EXPR$39_α:
#=======================================================================================================================
    .global proc_EXPR$39_α
    .global proc_EXPR$39_β
    .global proc_EXPR$39_γ
    .global proc_EXPR$39_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$39_α_body:
 xchain238_n0_α:
 mov rdi, qword ptr [rip + .Lx240_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx240_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx240_3]
 lea rdx, [rip + .Lx240_4]
 jmp rax
.Lx240_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx240_2
.Lx240_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx240_2
.Lx240_1:
 call rt_faildescr@PLT
.Lx240_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$39_ω
 jmp xchain238_n1_α
 xchain238_n0_β:
 jmp proc_EXPR$39_ω
.Lx240_0:
 .quad .Lx240_0_s
.Lx240_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain238_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053264], rax
 mov qword ptr [1879053272], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$39_γ
 xchain238_n1_β:
 jmp proc_EXPR$39_ω
proc_EXPR$39_res:
add rsp, 8
pop rbp
proc_EXPR$39_β:
jmp proc_EXPR$39_ω
proc_EXPR$39_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$39_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$40_α
proc_EXPR$40_α:
#=======================================================================================================================
    .global proc_EXPR$40_α
    .global proc_EXPR$40_β
    .global proc_EXPR$40_γ
    .global proc_EXPR$40_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$40_α_body:
# IR_VAR
 xchain242_n0_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain242_n1_α
 xchain242_n0_β:
 jmp proc_EXPR$40_ω
# IR_ASSIGN gva
 xchain242_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053280], rax
 mov qword ptr [1879053288], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$40_γ
 xchain242_n1_β:
 jmp proc_EXPR$40_ω
proc_EXPR$40_res:
add rsp, 8
pop rbp
proc_EXPR$40_β:
jmp proc_EXPR$40_ω
proc_EXPR$40_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$40_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$41_α
proc_EXPR$41_α:
#=======================================================================================================================
    .global proc_EXPR$41_α
    .global proc_EXPR$41_β
    .global proc_EXPR$41_γ
    .global proc_EXPR$41_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$41_α_body:
# IR_VAR
 xchain245_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain245_n1_α
 xchain245_n0_β:
 jmp proc_EXPR$41_ω
# IR_ASSIGN gva
 xchain245_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053296], rax
 mov qword ptr [1879053304], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$41_γ
 xchain245_n1_β:
 jmp proc_EXPR$41_ω
proc_EXPR$41_res:
add rsp, 8
pop rbp
proc_EXPR$41_β:
jmp proc_EXPR$41_ω
proc_EXPR$41_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$41_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$42_α
proc_EXPR$42_α:
#=======================================================================================================================
    .global proc_EXPR$42_α
    .global proc_EXPR$42_β
    .global proc_EXPR$42_γ
    .global proc_EXPR$42_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$42_α_body:
 xchain248_n0_α:
 mov rdi, qword ptr [rip + .Lx250_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx250_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx250_3]
 lea rdx, [rip + .Lx250_4]
 jmp rax
.Lx250_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx250_2
.Lx250_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx250_2
.Lx250_1:
 call rt_faildescr@PLT
.Lx250_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$42_ω
 jmp xchain248_n1_α
 xchain248_n0_β:
 jmp proc_EXPR$42_ω
.Lx250_0:
 .quad .Lx250_0_s
.Lx250_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain248_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053312], rax
 mov qword ptr [1879053320], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$42_γ
 xchain248_n1_β:
 jmp proc_EXPR$42_ω
proc_EXPR$42_res:
add rsp, 8
pop rbp
proc_EXPR$42_β:
jmp proc_EXPR$42_ω
proc_EXPR$42_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$42_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$43_α
proc_EXPR$43_α:
#=======================================================================================================================
    .global proc_EXPR$43_α
    .global proc_EXPR$43_β
    .global proc_EXPR$43_γ
    .global proc_EXPR$43_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$43_α_body:
 xchain252_n0_α:
 mov rdi, qword ptr [rip + .Lx254_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx254_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx254_3]
 lea rdx, [rip + .Lx254_4]
 jmp rax
.Lx254_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx254_2
.Lx254_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx254_2
.Lx254_1:
 call rt_faildescr@PLT
.Lx254_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$43_ω
 jmp xchain252_n1_α
 xchain252_n0_β:
 jmp proc_EXPR$43_ω
.Lx254_0:
 .quad .Lx254_0_s
.Lx254_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain252_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053328], rax
 mov qword ptr [1879053336], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$43_γ
 xchain252_n1_β:
 jmp proc_EXPR$43_ω
proc_EXPR$43_res:
add rsp, 8
pop rbp
proc_EXPR$43_β:
jmp proc_EXPR$43_ω
proc_EXPR$43_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$43_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$44_α
proc_EXPR$44_α:
#=======================================================================================================================
    .global proc_EXPR$44_α
    .global proc_EXPR$44_β
    .global proc_EXPR$44_γ
    .global proc_EXPR$44_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$44_α_body:
 xchain256_n0_α:
 mov rdi, qword ptr [rip + .Lx258_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx258_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx258_3]
 lea rdx, [rip + .Lx258_4]
 jmp rax
.Lx258_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx258_2
.Lx258_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx258_2
.Lx258_1:
 call rt_faildescr@PLT
.Lx258_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$44_ω
 jmp xchain256_n1_α
 xchain256_n0_β:
 jmp proc_EXPR$44_ω
.Lx258_0:
 .quad .Lx258_0_s
.Lx258_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain256_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053344], rax
 mov qword ptr [1879053352], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$44_γ
 xchain256_n1_β:
 jmp proc_EXPR$44_ω
proc_EXPR$44_res:
add rsp, 8
pop rbp
proc_EXPR$44_β:
jmp proc_EXPR$44_ω
proc_EXPR$44_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$44_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$45_α
proc_EXPR$45_α:
#=======================================================================================================================
    .global proc_EXPR$45_α
    .global proc_EXPR$45_β
    .global proc_EXPR$45_γ
    .global proc_EXPR$45_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$45_α_body:
 xchain260_n0_α:
 mov rdi, qword ptr [rip + .Lx262_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx262_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx262_3]
 lea rdx, [rip + .Lx262_4]
 jmp rax
.Lx262_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx262_2
.Lx262_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx262_2
.Lx262_1:
 call rt_faildescr@PLT
.Lx262_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$45_ω
 jmp xchain260_n1_α
 xchain260_n0_β:
 jmp proc_EXPR$45_ω
.Lx262_0:
 .quad .Lx262_0_s
.Lx262_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain260_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053360], rax
 mov qword ptr [1879053368], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$45_γ
 xchain260_n1_β:
 jmp proc_EXPR$45_ω
proc_EXPR$45_res:
add rsp, 8
pop rbp
proc_EXPR$45_β:
jmp proc_EXPR$45_ω
proc_EXPR$45_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$45_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$46_α
proc_EXPR$46_α:
#=======================================================================================================================
    .global proc_EXPR$46_α
    .global proc_EXPR$46_β
    .global proc_EXPR$46_γ
    .global proc_EXPR$46_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$46_α_body:
# IR_VAR
 xchain264_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain264_n1_α
 xchain264_n0_β:
 jmp proc_EXPR$46_ω
# IR_ASSIGN gva
 xchain264_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053376], rax
 mov qword ptr [1879053384], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$46_γ
 xchain264_n1_β:
 jmp proc_EXPR$46_ω
proc_EXPR$46_res:
add rsp, 8
pop rbp
proc_EXPR$46_β:
jmp proc_EXPR$46_ω
proc_EXPR$46_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$46_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$47_α
proc_EXPR$47_α:
#=======================================================================================================================
    .global proc_EXPR$47_α
    .global proc_EXPR$47_β
    .global proc_EXPR$47_γ
    .global proc_EXPR$47_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$47_α_body:
# IR_VAR
 xchain267_n0_α:
 mov rax, qword ptr [1879052560]
 mov rdx, qword ptr [1879052568]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain267_n1_α
 xchain267_n0_β:
 jmp proc_EXPR$47_ω
# IR_ASSIGN gva
 xchain267_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053392], rax
 mov qword ptr [1879053400], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$47_γ
 xchain267_n1_β:
 jmp proc_EXPR$47_ω
proc_EXPR$47_res:
add rsp, 8
pop rbp
proc_EXPR$47_β:
jmp proc_EXPR$47_ω
proc_EXPR$47_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$47_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$48_α
proc_EXPR$48_α:
#=======================================================================================================================
    .global proc_EXPR$48_α
    .global proc_EXPR$48_β
    .global proc_EXPR$48_γ
    .global proc_EXPR$48_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$48_α_body:
# IR_VAR
 xchain270_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain270_n1_α
 xchain270_n0_β:
 jmp proc_EXPR$48_ω
# IR_ASSIGN gva
 xchain270_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053408], rax
 mov qword ptr [1879053416], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$48_γ
 xchain270_n1_β:
 jmp proc_EXPR$48_ω
proc_EXPR$48_res:
add rsp, 8
pop rbp
proc_EXPR$48_β:
jmp proc_EXPR$48_ω
proc_EXPR$48_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$48_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$49_α
proc_EXPR$49_α:
#=======================================================================================================================
    .global proc_EXPR$49_α
    .global proc_EXPR$49_β
    .global proc_EXPR$49_γ
    .global proc_EXPR$49_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$49_α_body:
 xchain273_n0_α:
 mov rdi, qword ptr [rip + .Lx275_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx275_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx275_3]
 lea rdx, [rip + .Lx275_4]
 jmp rax
.Lx275_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx275_2
.Lx275_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx275_2
.Lx275_1:
 call rt_faildescr@PLT
.Lx275_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$49_ω
 jmp xchain273_n1_α
 xchain273_n0_β:
 jmp proc_EXPR$49_ω
.Lx275_0:
 .quad .Lx275_0_s
.Lx275_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain273_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053424], rax
 mov qword ptr [1879053432], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$49_γ
 xchain273_n1_β:
 jmp proc_EXPR$49_ω
proc_EXPR$49_res:
add rsp, 8
pop rbp
proc_EXPR$49_β:
jmp proc_EXPR$49_ω
proc_EXPR$49_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$49_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$50_α
proc_EXPR$50_α:
#=======================================================================================================================
    .global proc_EXPR$50_α
    .global proc_EXPR$50_β
    .global proc_EXPR$50_γ
    .global proc_EXPR$50_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$50_α_body:
# IR_VAR
 xchain277_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain277_n1_α
 xchain277_n0_β:
 jmp proc_EXPR$50_ω
# IR_ASSIGN gva
 xchain277_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053440], rax
 mov qword ptr [1879053448], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$50_γ
 xchain277_n1_β:
 jmp proc_EXPR$50_ω
proc_EXPR$50_res:
add rsp, 8
pop rbp
proc_EXPR$50_β:
jmp proc_EXPR$50_ω
proc_EXPR$50_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$50_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$51_α
proc_EXPR$51_α:
#=======================================================================================================================
    .global proc_EXPR$51_α
    .global proc_EXPR$51_β
    .global proc_EXPR$51_γ
    .global proc_EXPR$51_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$51_α_body:
# IR_VAR
 xchain280_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain280_n1_α
 xchain280_n0_β:
 jmp proc_EXPR$51_ω
# IR_ASSIGN gva
 xchain280_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053456], rax
 mov qword ptr [1879053464], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$51_γ
 xchain280_n1_β:
 jmp proc_EXPR$51_ω
proc_EXPR$51_res:
add rsp, 8
pop rbp
proc_EXPR$51_β:
jmp proc_EXPR$51_ω
proc_EXPR$51_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$51_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$52_α
proc_EXPR$52_α:
#=======================================================================================================================
    .global proc_EXPR$52_α
    .global proc_EXPR$52_β
    .global proc_EXPR$52_γ
    .global proc_EXPR$52_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$52_α_body:
# IR_VAR
 xchain283_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain283_n1_α
 xchain283_n0_β:
 jmp proc_EXPR$52_ω
# IR_ASSIGN gva
 xchain283_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053472], rax
 mov qword ptr [1879053480], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$52_γ
 xchain283_n1_β:
 jmp proc_EXPR$52_ω
proc_EXPR$52_res:
add rsp, 8
pop rbp
proc_EXPR$52_β:
jmp proc_EXPR$52_ω
proc_EXPR$52_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$52_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$53_α
proc_EXPR$53_α:
#=======================================================================================================================
    .global proc_EXPR$53_α
    .global proc_EXPR$53_β
    .global proc_EXPR$53_γ
    .global proc_EXPR$53_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$53_α_body:
 xchain286_n0_α:
 mov rdi, qword ptr [rip + .Lx288_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx288_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx288_3]
 lea rdx, [rip + .Lx288_4]
 jmp rax
.Lx288_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx288_2
.Lx288_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx288_2
.Lx288_1:
 call rt_faildescr@PLT
.Lx288_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$53_ω
 jmp xchain286_n1_α
 xchain286_n0_β:
 jmp proc_EXPR$53_ω
.Lx288_0:
 .quad .Lx288_0_s
.Lx288_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain286_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053488], rax
 mov qword ptr [1879053496], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$53_γ
 xchain286_n1_β:
 jmp proc_EXPR$53_ω
proc_EXPR$53_res:
add rsp, 8
pop rbp
proc_EXPR$53_β:
jmp proc_EXPR$53_ω
proc_EXPR$53_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$53_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$54_α
proc_EXPR$54_α:
#=======================================================================================================================
    .global proc_EXPR$54_α
    .global proc_EXPR$54_β
    .global proc_EXPR$54_γ
    .global proc_EXPR$54_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$54_α_body:
# IR_VAR
 xchain290_n0_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain290_n1_α
 xchain290_n0_β:
 jmp proc_EXPR$54_ω
# IR_ASSIGN gva
 xchain290_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053504], rax
 mov qword ptr [1879053512], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$54_γ
 xchain290_n1_β:
 jmp proc_EXPR$54_ω
proc_EXPR$54_res:
add rsp, 8
pop rbp
proc_EXPR$54_β:
jmp proc_EXPR$54_ω
proc_EXPR$54_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$54_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$55_α
proc_EXPR$55_α:
#=======================================================================================================================
    .global proc_EXPR$55_α
    .global proc_EXPR$55_β
    .global proc_EXPR$55_γ
    .global proc_EXPR$55_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$55_α_body:
# IR_VAR
 xchain293_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain293_n1_α
 xchain293_n0_β:
 jmp proc_EXPR$55_ω
# IR_ASSIGN gva
 xchain293_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053520], rax
 mov qword ptr [1879053528], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$55_γ
 xchain293_n1_β:
 jmp proc_EXPR$55_ω
proc_EXPR$55_res:
add rsp, 8
pop rbp
proc_EXPR$55_β:
jmp proc_EXPR$55_ω
proc_EXPR$55_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$55_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$56_α
proc_EXPR$56_α:
#=======================================================================================================================
    .global proc_EXPR$56_α
    .global proc_EXPR$56_β
    .global proc_EXPR$56_γ
    .global proc_EXPR$56_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$56_α_body:
 xchain296_n0_α:
 mov rdi, qword ptr [rip + .Lx298_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx298_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx298_3]
 lea rdx, [rip + .Lx298_4]
 jmp rax
.Lx298_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx298_2
.Lx298_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx298_2
.Lx298_1:
 call rt_faildescr@PLT
.Lx298_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$56_ω
 jmp xchain296_n1_α
 xchain296_n0_β:
 jmp proc_EXPR$56_ω
.Lx298_0:
 .quad .Lx298_0_s
.Lx298_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain296_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053536], rax
 mov qword ptr [1879053544], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$56_γ
 xchain296_n1_β:
 jmp proc_EXPR$56_ω
proc_EXPR$56_res:
add rsp, 8
pop rbp
proc_EXPR$56_β:
jmp proc_EXPR$56_ω
proc_EXPR$56_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$56_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$57_α
proc_EXPR$57_α:
#=======================================================================================================================
    .global proc_EXPR$57_α
    .global proc_EXPR$57_β
    .global proc_EXPR$57_γ
    .global proc_EXPR$57_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$57_α_body:
 xchain300_n0_α:
 mov rdi, qword ptr [rip + .Lx302_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx302_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx302_3]
 lea rdx, [rip + .Lx302_4]
 jmp rax
.Lx302_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx302_2
.Lx302_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx302_2
.Lx302_1:
 call rt_faildescr@PLT
.Lx302_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$57_ω
 jmp xchain300_n1_α
 xchain300_n0_β:
 jmp proc_EXPR$57_ω
.Lx302_0:
 .quad .Lx302_0_s
.Lx302_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain300_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053552], rax
 mov qword ptr [1879053560], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$57_γ
 xchain300_n1_β:
 jmp proc_EXPR$57_ω
proc_EXPR$57_res:
add rsp, 8
pop rbp
proc_EXPR$57_β:
jmp proc_EXPR$57_ω
proc_EXPR$57_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$57_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$58_α
proc_EXPR$58_α:
#=======================================================================================================================
    .global proc_EXPR$58_α
    .global proc_EXPR$58_β
    .global proc_EXPR$58_γ
    .global proc_EXPR$58_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$58_α_body:
 xchain304_n0_α:
 mov rdi, qword ptr [rip + .Lx306_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx306_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx306_3]
 lea rdx, [rip + .Lx306_4]
 jmp rax
.Lx306_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx306_2
.Lx306_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx306_2
.Lx306_1:
 call rt_faildescr@PLT
.Lx306_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$58_ω
 jmp xchain304_n1_α
 xchain304_n0_β:
 jmp proc_EXPR$58_ω
.Lx306_0:
 .quad .Lx306_0_s
.Lx306_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain304_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053568], rax
 mov qword ptr [1879053576], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$58_γ
 xchain304_n1_β:
 jmp proc_EXPR$58_ω
proc_EXPR$58_res:
add rsp, 8
pop rbp
proc_EXPR$58_β:
jmp proc_EXPR$58_ω
proc_EXPR$58_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$58_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$59_α
proc_EXPR$59_α:
#=======================================================================================================================
    .global proc_EXPR$59_α
    .global proc_EXPR$59_β
    .global proc_EXPR$59_γ
    .global proc_EXPR$59_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$59_α_body:
 xchain308_n0_α:
 mov rdi, qword ptr [rip + .Lx310_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx310_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx310_3]
 lea rdx, [rip + .Lx310_4]
 jmp rax
.Lx310_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx310_2
.Lx310_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx310_2
.Lx310_1:
 call rt_faildescr@PLT
.Lx310_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$59_ω
 jmp xchain308_n1_α
 xchain308_n0_β:
 jmp proc_EXPR$59_ω
.Lx310_0:
 .quad .Lx310_0_s
.Lx310_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain308_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053584], rax
 mov qword ptr [1879053592], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$59_γ
 xchain308_n1_β:
 jmp proc_EXPR$59_ω
proc_EXPR$59_res:
add rsp, 8
pop rbp
proc_EXPR$59_β:
jmp proc_EXPR$59_ω
proc_EXPR$59_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$59_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$60_α
proc_EXPR$60_α:
#=======================================================================================================================
    .global proc_EXPR$60_α
    .global proc_EXPR$60_β
    .global proc_EXPR$60_γ
    .global proc_EXPR$60_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$60_α_body:
# IR_VAR
 xchain312_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain312_n1_α
 xchain312_n0_β:
 jmp proc_EXPR$60_ω
# IR_ASSIGN gva
 xchain312_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053600], rax
 mov qword ptr [1879053608], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$60_γ
 xchain312_n1_β:
 jmp proc_EXPR$60_ω
proc_EXPR$60_res:
add rsp, 8
pop rbp
proc_EXPR$60_β:
jmp proc_EXPR$60_ω
proc_EXPR$60_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$60_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$61_α
proc_EXPR$61_α:
#=======================================================================================================================
    .global proc_EXPR$61_α
    .global proc_EXPR$61_β
    .global proc_EXPR$61_γ
    .global proc_EXPR$61_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$61_α_body:
# IR_VAR
 xchain315_n0_α:
 mov rax, qword ptr [1879052560]
 mov rdx, qword ptr [1879052568]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain315_n1_α
 xchain315_n0_β:
 jmp proc_EXPR$61_ω
# IR_ASSIGN gva
 xchain315_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053616], rax
 mov qword ptr [1879053624], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$61_γ
 xchain315_n1_β:
 jmp proc_EXPR$61_ω
proc_EXPR$61_res:
add rsp, 8
pop rbp
proc_EXPR$61_β:
jmp proc_EXPR$61_ω
proc_EXPR$61_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$61_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$62_α
proc_EXPR$62_α:
#=======================================================================================================================
    .global proc_EXPR$62_α
    .global proc_EXPR$62_β
    .global proc_EXPR$62_γ
    .global proc_EXPR$62_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$62_α_body:
# IR_VAR
 xchain318_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain318_n1_α
 xchain318_n0_β:
 jmp proc_EXPR$62_ω
# IR_ASSIGN gva
 xchain318_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053632], rax
 mov qword ptr [1879053640], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$62_γ
 xchain318_n1_β:
 jmp proc_EXPR$62_ω
proc_EXPR$62_res:
add rsp, 8
pop rbp
proc_EXPR$62_β:
jmp proc_EXPR$62_ω
proc_EXPR$62_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$62_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$63_α
proc_EXPR$63_α:
#=======================================================================================================================
    .global proc_EXPR$63_α
    .global proc_EXPR$63_β
    .global proc_EXPR$63_γ
    .global proc_EXPR$63_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$63_α_body:
 xchain321_n0_α:
 mov rdi, qword ptr [rip + .Lx323_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx323_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx323_3]
 lea rdx, [rip + .Lx323_4]
 jmp rax
.Lx323_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx323_2
.Lx323_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx323_2
.Lx323_1:
 call rt_faildescr@PLT
.Lx323_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$63_ω
 jmp xchain321_n1_α
 xchain321_n0_β:
 jmp proc_EXPR$63_ω
.Lx323_0:
 .quad .Lx323_0_s
.Lx323_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain321_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053648], rax
 mov qword ptr [1879053656], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$63_γ
 xchain321_n1_β:
 jmp proc_EXPR$63_ω
proc_EXPR$63_res:
add rsp, 8
pop rbp
proc_EXPR$63_β:
jmp proc_EXPR$63_ω
proc_EXPR$63_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$63_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$64_α
proc_EXPR$64_α:
#=======================================================================================================================
    .global proc_EXPR$64_α
    .global proc_EXPR$64_β
    .global proc_EXPR$64_γ
    .global proc_EXPR$64_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$64_α_body:
# IR_VAR
 xchain325_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain325_n1_α
 xchain325_n0_β:
 jmp proc_EXPR$64_ω
# IR_ASSIGN gva
 xchain325_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053664], rax
 mov qword ptr [1879053672], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$64_γ
 xchain325_n1_β:
 jmp proc_EXPR$64_ω
proc_EXPR$64_res:
add rsp, 8
pop rbp
proc_EXPR$64_β:
jmp proc_EXPR$64_ω
proc_EXPR$64_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$64_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$65_α
proc_EXPR$65_α:
#=======================================================================================================================
    .global proc_EXPR$65_α
    .global proc_EXPR$65_β
    .global proc_EXPR$65_γ
    .global proc_EXPR$65_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$65_α_body:
# IR_VAR
 xchain328_n0_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain328_n1_α
 xchain328_n0_β:
 jmp proc_EXPR$65_ω
# IR_ASSIGN gva
 xchain328_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053680], rax
 mov qword ptr [1879053688], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$65_γ
 xchain328_n1_β:
 jmp proc_EXPR$65_ω
proc_EXPR$65_res:
add rsp, 8
pop rbp
proc_EXPR$65_β:
jmp proc_EXPR$65_ω
proc_EXPR$65_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$65_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$66_α
proc_EXPR$66_α:
#=======================================================================================================================
    .global proc_EXPR$66_α
    .global proc_EXPR$66_β
    .global proc_EXPR$66_γ
    .global proc_EXPR$66_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$66_α_body:
# IR_VAR
 xchain331_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain331_n1_α
 xchain331_n0_β:
 jmp proc_EXPR$66_ω
# IR_ASSIGN gva
 xchain331_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053696], rax
 mov qword ptr [1879053704], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$66_γ
 xchain331_n1_β:
 jmp proc_EXPR$66_ω
proc_EXPR$66_res:
add rsp, 8
pop rbp
proc_EXPR$66_β:
jmp proc_EXPR$66_ω
proc_EXPR$66_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$66_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$67_α
proc_EXPR$67_α:
#=======================================================================================================================
    .global proc_EXPR$67_α
    .global proc_EXPR$67_β
    .global proc_EXPR$67_γ
    .global proc_EXPR$67_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$67_α_body:
 xchain334_n0_α:
 mov rdi, qword ptr [rip + .Lx336_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx336_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx336_3]
 lea rdx, [rip + .Lx336_4]
 jmp rax
.Lx336_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx336_2
.Lx336_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx336_2
.Lx336_1:
 call rt_faildescr@PLT
.Lx336_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$67_ω
 jmp xchain334_n1_α
 xchain334_n0_β:
 jmp proc_EXPR$67_ω
.Lx336_0:
 .quad .Lx336_0_s
.Lx336_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain334_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053712], rax
 mov qword ptr [1879053720], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$67_γ
 xchain334_n1_β:
 jmp proc_EXPR$67_ω
proc_EXPR$67_res:
add rsp, 8
pop rbp
proc_EXPR$67_β:
jmp proc_EXPR$67_ω
proc_EXPR$67_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$67_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$68_α
proc_EXPR$68_α:
#=======================================================================================================================
    .global proc_EXPR$68_α
    .global proc_EXPR$68_β
    .global proc_EXPR$68_γ
    .global proc_EXPR$68_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$68_α_body:
# IR_VAR
 xchain338_n0_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain338_n1_α
 xchain338_n0_β:
 jmp proc_EXPR$68_ω
# IR_ASSIGN gva
 xchain338_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053728], rax
 mov qword ptr [1879053736], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$68_γ
 xchain338_n1_β:
 jmp proc_EXPR$68_ω
proc_EXPR$68_res:
add rsp, 8
pop rbp
proc_EXPR$68_β:
jmp proc_EXPR$68_ω
proc_EXPR$68_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$68_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$69_α
proc_EXPR$69_α:
#=======================================================================================================================
    .global proc_EXPR$69_α
    .global proc_EXPR$69_β
    .global proc_EXPR$69_γ
    .global proc_EXPR$69_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$69_α_body:
# IR_VAR
 xchain341_n0_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 jmp xchain341_n1_α
 xchain341_n0_β:
 jmp proc_EXPR$69_ω
# IR_ASSIGN gva
 xchain341_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053744], rax
 mov qword ptr [1879053752], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$69_γ
 xchain341_n1_β:
 jmp proc_EXPR$69_ω
proc_EXPR$69_res:
add rsp, 8
pop rbp
proc_EXPR$69_β:
jmp proc_EXPR$69_ω
proc_EXPR$69_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$69_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_PAT$0_α_body:
lea rax, [rip + xchain344_n0_β]
mov qword ptr [rbp + 48], rax
# IR_MATCH_SPAN
 xchain344_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
.Lx346_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx346_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 call strchr@PLT
 test rax, rax
 je .Lx346_1
 add dword ptr [rsp + 0], 1
 jmp .Lx346_0
.Lx346_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx346_240
 add rsp, 16
 jmp proc_PAT$0_ω
.Lx346_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$0_γ
 xchain344_n0_β:
 xchain344_n0_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp proc_PAT$0_ω
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 48]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp + 88]
mov rbp, [rbp + 104]
jmp rax
proc_PAT$0_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$1_α
proc_PAT$1_α:
#=======================================================================================================================
    .global proc_PAT$1_α
    .global proc_PAT$1_β
    .global proc_PAT$1_γ
    .global proc_PAT$1_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
proc_PAT$1_α_body:
lea rax, [rip + xchain347_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_SEQ_NARY
 xchain347_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain347_n1_α
xchain347_n0_as:
 mov eax, dword ptr [rbp + 36]
 add eax, 1
 mov dword ptr [rbp + 36], eax
 cmp eax, 1
 je xchain347_n2_α
 cmp eax, 2
 je xchain347_n3_α
 jmp proc_PAT$1_γ
 xchain347_n0_β:
 mov dword ptr [rbp + 36], 3
xchain347_n0_af:
 mov eax, dword ptr [rbp + 36]
 sub eax, 1
 mov dword ptr [rbp + 36], eax
 cmp eax, 0
 je xchain347_n1_β
 cmp eax, 1
 je xchain347_n2_β
 cmp eax, 2
 je xchain347_n3_β
 jmp proc_PAT$1_ω
# IR_MATCH_ANY
 xchain347_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain347_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 call strchr@PLT
 test rax, rax
 je xchain347_n0_af
 add r14d, 1
 jmp xchain347_n0_as
 xchain347_n1_β:
 sub r14d, 1
 jmp xchain347_n0_af
# IR_MATCH_ALT_NARY
 xchain347_n2_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain347_n4_α
xchain347_n2_as:
 jmp xchain347_n0_as
 xchain347_n2_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain347_n4_β
 jmp xchain347_n5_β
xchain347_n2_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain347_n5_α
 jmp xchain347_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain347_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx353_0
 lea rcx, [rip + .Lx353_4]
 lea rdx, [rip + .Lx353_5]
 jmp rax
.Lx353_4:
 jmp xchain347_n0_as
.Lx353_5:
 jmp xchain347_n0_af
.Lx353_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx353_2:
 test rax, rax
 je .Lx353_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx353_7]
 lea rdx, [rip + .Lx353_8]
 jmp rax
.Lx353_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx353_2
.Lx353_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx353_2
.Lx353_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain347_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx353_6]
 sub rsp, 8
 push rax
 jmp xchain347_n0_as
.Lx353_6:
 add rsp, 16
 jmp xchain347_n0_af
 xchain347_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_ANY
 xchain347_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain347_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 call strchr@PLT
 test rax, rax
 je xchain347_n2_af
 add r14d, 1
 jmp xchain347_n2_as
 xchain347_n4_β:
 sub r14d, 1
 jmp xchain347_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain347_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx355_0
 lea rcx, [rip + .Lx355_4]
 lea rdx, [rip + .Lx355_5]
 jmp rax
.Lx355_4:
 jmp xchain347_n2_as
.Lx355_5:
 jmp xchain347_n2_af
.Lx355_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx355_2:
 test rax, rax
 je .Lx355_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx355_7]
 lea rdx, [rip + .Lx355_8]
 jmp rax
.Lx355_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx355_2
.Lx355_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx355_2
.Lx355_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain347_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx355_6]
 sub rsp, 8
 push rax
 jmp xchain347_n2_as
.Lx355_6:
 add rsp, 16
 jmp xchain347_n2_af
 xchain347_n5_β:
 jmp qword ptr [rsp]
proc_PAT$1_res:
add rsp, 8
pop rbp
proc_PAT$1_β:
jmp qword ptr [rbp + 176]
proc_PAT$1_γ:
push rbp
lea rax, [rip + proc_PAT$1_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$1_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$2_α
proc_PAT$2_α:
#=======================================================================================================================
    .global proc_PAT$2_α
    .global proc_PAT$2_β
    .global proc_PAT$2_γ
    .global proc_PAT$2_ω
  sub rsp, 480
  mov [rsp + 456], rcx
  mov [rsp + 464], rdx
  mov [rsp + 472], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 448
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 440], rsp
proc_PAT$2_α_body:
lea rax, [rip + xchain356_n0_β]
mov qword ptr [rbp + 416], rax
# IR_MATCH_ALT_NARY
 xchain356_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain356_n1_α
xchain356_n0_as:
 jmp proc_PAT$2_γ
 xchain356_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain356_n1_β
 jmp xchain356_n2_β
xchain356_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain356_n2_α
 jmp proc_PAT$2_ω
# IR_MATCH_SEQ_NARY
 xchain356_n1_α:
 mov dword ptr [rbp + 64], r14d
 mov dword ptr [rbp + 68], 0
 jmp xchain356_n3_α
xchain356_n1_as:
 mov eax, dword ptr [rbp + 68]
 add eax, 1
 mov dword ptr [rbp + 68], eax
 cmp eax, 1
 je xchain356_n4_α
 cmp eax, 2
 je xchain356_n5_α
 cmp eax, 3
 je xchain356_n6_α
 jmp xchain356_n0_as
 xchain356_n1_β:
 mov dword ptr [rbp + 68], 4
xchain356_n1_af:
 mov eax, dword ptr [rbp + 68]
 sub eax, 1
 mov dword ptr [rbp + 68], eax
 cmp eax, 0
 je xchain356_n3_β
 cmp eax, 1
 je xchain356_n4_β
 cmp eax, 2
 je xchain356_n5_β
 cmp eax, 3
 je xchain356_n6_β
 jmp xchain356_n0_af
# IR_MATCH_SEQ_NARY
 xchain356_n2_α:
 mov dword ptr [rbp + 336], r14d
 mov dword ptr [rbp + 340], 0
 jmp xchain356_n7_α
xchain356_n2_as:
 mov eax, dword ptr [rbp + 340]
 add eax, 1
 mov dword ptr [rbp + 340], eax
 cmp eax, 1
 je xchain356_n8_α
 jmp xchain356_n0_as
 xchain356_n2_β:
 mov dword ptr [rbp + 340], 2
xchain356_n2_af:
 mov eax, dword ptr [rbp + 340]
 sub eax, 1
 mov dword ptr [rbp + 340], eax
 cmp eax, 0
 je xchain356_n7_β
 cmp eax, 1
 je xchain356_n8_β
 jmp xchain356_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain356_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx363_0
 lea rcx, [rip + .Lx363_4]
 lea rdx, [rip + .Lx363_5]
 jmp rax
.Lx363_4:
 jmp xchain356_n1_as
.Lx363_5:
 jmp xchain356_n1_af
.Lx363_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx363_2:
 test rax, rax
 je .Lx363_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx363_7]
 lea rdx, [rip + .Lx363_8]
 jmp rax
.Lx363_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx363_2
.Lx363_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx363_2
.Lx363_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain356_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx363_6]
 sub rsp, 8
 push rax
 jmp xchain356_n1_as
.Lx363_6:
 add rsp, 16
 jmp xchain356_n1_af
 xchain356_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain356_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain356_n1_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain356_n1_af
 add r14d, 1
 jmp xchain356_n1_as
 xchain356_n4_β:
 sub r14d, 1
 jmp xchain356_n1_af
# IR_MATCH_ALT_NARY
 xchain356_n5_α:
 mov dword ptr [rbp + 144], r14d
 mov dword ptr [rbp + 148], 0
 jmp xchain356_n9_α
xchain356_n5_as:
 jmp xchain356_n1_as
 xchain356_n5_β:
 mov eax, dword ptr [rbp + 148]
 cmp eax, 0
 je xchain356_n9_β
 jmp xchain356_n10_β
xchain356_n5_af:
 add dword ptr [rbp + 148], 1
 mov r14d, dword ptr [rbp + 144]
 mov eax, dword ptr [rbp + 148]
 cmp eax, 1
 je xchain356_n10_α
 jmp xchain356_n1_af
# IR_MATCH_ALT_NARY
 xchain356_n6_α:
 mov dword ptr [rbp + 240], r14d
 mov dword ptr [rbp + 244], 0
 jmp xchain356_n11_α
xchain356_n6_as:
 jmp xchain356_n1_as
 xchain356_n6_β:
 mov eax, dword ptr [rbp + 244]
 cmp eax, 0
 je xchain356_n11_β
 jmp xchain356_n12_β
xchain356_n6_af:
 add dword ptr [rbp + 244], 1
 mov r14d, dword ptr [rbp + 240]
 mov eax, dword ptr [rbp + 244]
 cmp eax, 1
 je xchain356_n12_α
 jmp xchain356_n1_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain356_n7_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx370_0
 lea rcx, [rip + .Lx370_4]
 lea rdx, [rip + .Lx370_5]
 jmp rax
.Lx370_4:
 jmp xchain356_n2_as
.Lx370_5:
 jmp xchain356_n2_af
.Lx370_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx370_2:
 test rax, rax
 je .Lx370_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx370_7]
 lea rdx, [rip + .Lx370_8]
 jmp rax
.Lx370_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx370_2
.Lx370_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx370_2
.Lx370_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain356_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx370_6]
 sub rsp, 8
 push rax
 jmp xchain356_n2_as
.Lx370_6:
 add rsp, 16
 jmp xchain356_n2_af
 xchain356_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain356_n8_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx371_0
 lea rcx, [rip + .Lx371_4]
 lea rdx, [rip + .Lx371_5]
 jmp rax
.Lx371_4:
 jmp xchain356_n2_as
.Lx371_5:
 jmp xchain356_n2_af
.Lx371_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx371_2:
 test rax, rax
 je .Lx371_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx371_7]
 lea rdx, [rip + .Lx371_8]
 jmp rax
.Lx371_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx371_2
.Lx371_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx371_2
.Lx371_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain356_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx371_6]
 sub rsp, 8
 push rax
 jmp xchain356_n2_as
.Lx371_6:
 add rsp, 16
 jmp xchain356_n2_af
 xchain356_n8_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain356_n9_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx372_0
 lea rcx, [rip + .Lx372_4]
 lea rdx, [rip + .Lx372_5]
 jmp rax
.Lx372_4:
 jmp xchain356_n5_as
.Lx372_5:
 jmp xchain356_n5_af
.Lx372_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx372_2:
 test rax, rax
 je .Lx372_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx372_7]
 lea rdx, [rip + .Lx372_8]
 jmp rax
.Lx372_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx372_2
.Lx372_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx372_2
.Lx372_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain356_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx372_6]
 sub rsp, 8
 push rax
 jmp xchain356_n5_as
.Lx372_6:
 add rsp, 16
 jmp xchain356_n5_af
 xchain356_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain356_n10_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx373_0
 lea rcx, [rip + .Lx373_4]
 lea rdx, [rip + .Lx373_5]
 jmp rax
.Lx373_4:
 jmp xchain356_n5_as
.Lx373_5:
 jmp xchain356_n5_af
.Lx373_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx373_2:
 test rax, rax
 je .Lx373_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx373_7]
 lea rdx, [rip + .Lx373_8]
 jmp rax
.Lx373_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx373_2
.Lx373_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx373_2
.Lx373_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain356_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx373_6]
 sub rsp, 8
 push rax
 jmp xchain356_n5_as
.Lx373_6:
 add rsp, 16
 jmp xchain356_n5_af
 xchain356_n10_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain356_n11_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx374_0
 lea rcx, [rip + .Lx374_4]
 lea rdx, [rip + .Lx374_5]
 jmp rax
.Lx374_4:
 jmp xchain356_n6_as
.Lx374_5:
 jmp xchain356_n6_af
.Lx374_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx374_2:
 test rax, rax
 je .Lx374_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx374_7]
 lea rdx, [rip + .Lx374_8]
 jmp rax
.Lx374_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx374_2
.Lx374_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx374_2
.Lx374_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain356_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx374_6]
 sub rsp, 8
 push rax
 jmp xchain356_n6_as
.Lx374_6:
 add rsp, 16
 jmp xchain356_n6_af
 xchain356_n11_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain356_n12_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx375_0
 lea rcx, [rip + .Lx375_4]
 lea rdx, [rip + .Lx375_5]
 jmp rax
.Lx375_4:
 jmp xchain356_n6_as
.Lx375_5:
 jmp xchain356_n6_af
.Lx375_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx375_2:
 test rax, rax
 je .Lx375_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx375_7]
 lea rdx, [rip + .Lx375_8]
 jmp rax
.Lx375_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx375_2
.Lx375_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx375_2
.Lx375_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain356_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx375_6]
 sub rsp, 8
 push rax
 jmp xchain356_n6_as
.Lx375_6:
 add rsp, 16
 jmp xchain356_n6_af
 xchain356_n12_β:
 jmp qword ptr [rsp]
proc_PAT$2_res:
add rsp, 8
pop rbp
proc_PAT$2_β:
jmp qword ptr [rbp + 416]
proc_PAT$2_γ:
push rbp
lea rax, [rip + proc_PAT$2_res]
push rax
mov rax, [rbp + 456]
mov rbp, [rbp + 472]
jmp rax
proc_PAT$2_ω:
mov rax, [rbp + 464]
lea rsp, [rbp + 480]
mov rbp, [rbp + 472]
jmp rax
  .globl proc_PAT$3_α
proc_PAT$3_α:
#=======================================================================================================================
    .global proc_PAT$3_α
    .global proc_PAT$3_β
    .global proc_PAT$3_γ
    .global proc_PAT$3_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
proc_PAT$3_α_body:
lea rax, [rip + xchain376_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain376_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain376_n1_α
xchain376_n0_as:
 jmp proc_PAT$3_γ
 xchain376_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain376_n1_β
 jmp xchain376_n2_β
xchain376_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain376_n2_α
 jmp proc_PAT$3_ω
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain376_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx379_0
 lea rcx, [rip + .Lx379_4]
 lea rdx, [rip + .Lx379_5]
 jmp rax
.Lx379_4:
 jmp xchain376_n0_as
.Lx379_5:
 jmp xchain376_n0_af
.Lx379_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_open@PLT
.Lx379_2:
 test rax, rax
 je .Lx379_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx379_7]
 lea rdx, [rip + .Lx379_8]
 jmp rax
.Lx379_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx379_2
.Lx379_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx379_2
.Lx379_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain376_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx379_6]
 sub rsp, 8
 push rax
 jmp xchain376_n0_as
.Lx379_6:
 add rsp, 16
 jmp xchain376_n0_af
 xchain376_n1_β:
 jmp qword ptr [rsp]
# IR_MATCH_SEQ_NARY
 xchain376_n2_α:
 mov dword ptr [rbp + 96], r14d
 mov dword ptr [rbp + 100], 0
 jmp xchain376_n3_α
xchain376_n2_as:
 mov eax, dword ptr [rbp + 100]
 add eax, 1
 mov dword ptr [rbp + 100], eax
 cmp eax, 1
 je xchain376_n4_α
 cmp eax, 2
 je xchain376_n5_α
 jmp xchain376_n0_as
 xchain376_n2_β:
 mov dword ptr [rbp + 100], 3
xchain376_n2_af:
 mov eax, dword ptr [rbp + 100]
 sub eax, 1
 mov dword ptr [rbp + 100], eax
 cmp eax, 0
 je xchain376_n3_β
 cmp eax, 1
 je xchain376_n4_β
 cmp eax, 2
 je xchain376_n5_β
 jmp xchain376_n0_af
# IR_MATCH_LIT
 xchain376_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain376_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain376_n2_af
 add r14d, 1
 jmp xchain376_n2_as
 xchain376_n3_β:
 sub r14d, 1
 jmp xchain376_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain376_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx384_0
 lea rcx, [rip + .Lx384_4]
 lea rdx, [rip + .Lx384_5]
 jmp rax
.Lx384_4:
 jmp xchain376_n2_as
.Lx384_5:
 jmp xchain376_n2_af
.Lx384_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_open@PLT
.Lx384_2:
 test rax, rax
 je .Lx384_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx384_7]
 lea rdx, [rip + .Lx384_8]
 jmp rax
.Lx384_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx384_2
.Lx384_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx384_2
.Lx384_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain376_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx384_6]
 sub rsp, 8
 push rax
 jmp xchain376_n2_as
.Lx384_6:
 add rsp, 16
 jmp xchain376_n2_af
 xchain376_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain376_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain376_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain376_n2_af
 add r14d, 1
 jmp xchain376_n2_as
 xchain376_n5_β:
 sub r14d, 1
 jmp xchain376_n2_af
proc_PAT$3_res:
add rsp, 8
pop rbp
proc_PAT$3_β:
jmp qword ptr [rbp + 176]
proc_PAT$3_γ:
push rbp
lea rax, [rip + proc_PAT$3_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$3_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$4_α
proc_PAT$4_α:
#=======================================================================================================================
    .global proc_PAT$4_α
    .global proc_PAT$4_β
    .global proc_PAT$4_γ
    .global proc_PAT$4_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_PAT$4_α_body:
lea rax, [rip + xchain387_n0_β]
mov qword ptr [rbp + 48], rax
# IR_MATCH_SPAN
 xchain387_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
.Lx389_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx389_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 call strchr@PLT
 test rax, rax
 je .Lx389_1
 add dword ptr [rsp + 0], 1
 jmp .Lx389_0
.Lx389_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx389_240
 add rsp, 16
 jmp proc_PAT$4_ω
.Lx389_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$4_γ
 xchain387_n0_β:
 xchain387_n0_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp proc_PAT$4_ω
proc_PAT$4_res:
add rsp, 8
pop rbp
proc_PAT$4_β:
jmp qword ptr [rbp + 48]
proc_PAT$4_γ:
push rbp
lea rax, [rip + proc_PAT$4_res]
push rax
mov rax, [rbp + 88]
mov rbp, [rbp + 104]
jmp rax
proc_PAT$4_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$5_α
proc_PAT$5_α:
#=======================================================================================================================
    .global proc_PAT$5_α
    .global proc_PAT$5_β
    .global proc_PAT$5_γ
    .global proc_PAT$5_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
proc_PAT$5_α_body:
lea rax, [rip + xchain390_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_SEQ_NARY
 xchain390_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain390_n1_α
xchain390_n0_as:
 mov eax, dword ptr [rbp + 36]
 add eax, 1
 mov dword ptr [rbp + 36], eax
 cmp eax, 1
 je xchain390_n2_α
 cmp eax, 2
 je xchain390_n3_α
 jmp proc_PAT$5_γ
 xchain390_n0_β:
 mov dword ptr [rbp + 36], 3
xchain390_n0_af:
 mov eax, dword ptr [rbp + 36]
 sub eax, 1
 mov dword ptr [rbp + 36], eax
 cmp eax, 0
 je xchain390_n1_β
 cmp eax, 1
 je xchain390_n2_β
 cmp eax, 2
 je xchain390_n3_β
 jmp proc_PAT$5_ω
# IR_MATCH_ANY
 xchain390_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain390_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 call strchr@PLT
 test rax, rax
 je xchain390_n0_af
 add r14d, 1
 jmp xchain390_n0_as
 xchain390_n1_β:
 sub r14d, 1
 jmp xchain390_n0_af
# IR_MATCH_ALT_NARY
 xchain390_n2_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain390_n4_α
xchain390_n2_as:
 jmp xchain390_n0_as
 xchain390_n2_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain390_n4_β
 jmp xchain390_n5_β
xchain390_n2_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain390_n5_α
 jmp xchain390_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain390_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx396_0
 lea rcx, [rip + .Lx396_4]
 lea rdx, [rip + .Lx396_5]
 jmp rax
.Lx396_4:
 jmp xchain390_n0_as
.Lx396_5:
 jmp xchain390_n0_af
.Lx396_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx396_2:
 test rax, rax
 je .Lx396_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx396_7]
 lea rdx, [rip + .Lx396_8]
 jmp rax
.Lx396_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx396_2
.Lx396_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx396_2
.Lx396_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain390_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx396_6]
 sub rsp, 8
 push rax
 jmp xchain390_n0_as
.Lx396_6:
 add rsp, 16
 jmp xchain390_n0_af
 xchain390_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_ANY
 xchain390_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain390_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 call strchr@PLT
 test rax, rax
 je xchain390_n2_af
 add r14d, 1
 jmp xchain390_n2_as
 xchain390_n4_β:
 sub r14d, 1
 jmp xchain390_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain390_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx398_0
 lea rcx, [rip + .Lx398_4]
 lea rdx, [rip + .Lx398_5]
 jmp rax
.Lx398_4:
 jmp xchain390_n2_as
.Lx398_5:
 jmp xchain390_n2_af
.Lx398_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx398_2:
 test rax, rax
 je .Lx398_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx398_7]
 lea rdx, [rip + .Lx398_8]
 jmp rax
.Lx398_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx398_2
.Lx398_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx398_2
.Lx398_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain390_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx398_6]
 sub rsp, 8
 push rax
 jmp xchain390_n2_as
.Lx398_6:
 add rsp, 16
 jmp xchain390_n2_af
 xchain390_n5_β:
 jmp qword ptr [rsp]
proc_PAT$5_res:
add rsp, 8
pop rbp
proc_PAT$5_β:
jmp qword ptr [rbp + 176]
proc_PAT$5_γ:
push rbp
lea rax, [rip + proc_PAT$5_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$5_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$6_α
proc_PAT$6_α:
#=======================================================================================================================
    .global proc_PAT$6_α
    .global proc_PAT$6_β
    .global proc_PAT$6_γ
    .global proc_PAT$6_ω
  sub rsp, 480
  mov [rsp + 456], rcx
  mov [rsp + 464], rdx
  mov [rsp + 472], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 448
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 440], rsp
proc_PAT$6_α_body:
lea rax, [rip + xchain399_n0_β]
mov qword ptr [rbp + 416], rax
# IR_MATCH_ALT_NARY
 xchain399_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain399_n1_α
xchain399_n0_as:
 jmp proc_PAT$6_γ
 xchain399_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain399_n1_β
 jmp xchain399_n2_β
xchain399_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain399_n2_α
 jmp proc_PAT$6_ω
# IR_MATCH_SEQ_NARY
 xchain399_n1_α:
 mov dword ptr [rbp + 64], r14d
 mov dword ptr [rbp + 68], 0
 jmp xchain399_n3_α
xchain399_n1_as:
 mov eax, dword ptr [rbp + 68]
 add eax, 1
 mov dword ptr [rbp + 68], eax
 cmp eax, 1
 je xchain399_n4_α
 cmp eax, 2
 je xchain399_n5_α
 cmp eax, 3
 je xchain399_n6_α
 jmp xchain399_n0_as
 xchain399_n1_β:
 mov dword ptr [rbp + 68], 4
xchain399_n1_af:
 mov eax, dword ptr [rbp + 68]
 sub eax, 1
 mov dword ptr [rbp + 68], eax
 cmp eax, 0
 je xchain399_n3_β
 cmp eax, 1
 je xchain399_n4_β
 cmp eax, 2
 je xchain399_n5_β
 cmp eax, 3
 je xchain399_n6_β
 jmp xchain399_n0_af
# IR_MATCH_SEQ_NARY
 xchain399_n2_α:
 mov dword ptr [rbp + 336], r14d
 mov dword ptr [rbp + 340], 0
 jmp xchain399_n7_α
xchain399_n2_as:
 mov eax, dword ptr [rbp + 340]
 add eax, 1
 mov dword ptr [rbp + 340], eax
 cmp eax, 1
 je xchain399_n8_α
 jmp xchain399_n0_as
 xchain399_n2_β:
 mov dword ptr [rbp + 340], 2
xchain399_n2_af:
 mov eax, dword ptr [rbp + 340]
 sub eax, 1
 mov dword ptr [rbp + 340], eax
 cmp eax, 0
 je xchain399_n7_β
 cmp eax, 1
 je xchain399_n8_β
 jmp xchain399_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain399_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx406_0
 lea rcx, [rip + .Lx406_4]
 lea rdx, [rip + .Lx406_5]
 jmp rax
.Lx406_4:
 jmp xchain399_n1_as
.Lx406_5:
 jmp xchain399_n1_af
.Lx406_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx406_2:
 test rax, rax
 je .Lx406_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx406_7]
 lea rdx, [rip + .Lx406_8]
 jmp rax
.Lx406_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx406_2
.Lx406_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx406_2
.Lx406_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain399_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx406_6]
 sub rsp, 8
 push rax
 jmp xchain399_n1_as
.Lx406_6:
 add rsp, 16
 jmp xchain399_n1_af
 xchain399_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain399_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain399_n1_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain399_n1_af
 add r14d, 1
 jmp xchain399_n1_as
 xchain399_n4_β:
 sub r14d, 1
 jmp xchain399_n1_af
# IR_MATCH_ALT_NARY
 xchain399_n5_α:
 mov dword ptr [rbp + 144], r14d
 mov dword ptr [rbp + 148], 0
 jmp xchain399_n9_α
xchain399_n5_as:
 jmp xchain399_n1_as
 xchain399_n5_β:
 mov eax, dword ptr [rbp + 148]
 cmp eax, 0
 je xchain399_n9_β
 jmp xchain399_n10_β
xchain399_n5_af:
 add dword ptr [rbp + 148], 1
 mov r14d, dword ptr [rbp + 144]
 mov eax, dword ptr [rbp + 148]
 cmp eax, 1
 je xchain399_n10_α
 jmp xchain399_n1_af
# IR_MATCH_ALT_NARY
 xchain399_n6_α:
 mov dword ptr [rbp + 240], r14d
 mov dword ptr [rbp + 244], 0
 jmp xchain399_n11_α
xchain399_n6_as:
 jmp xchain399_n1_as
 xchain399_n6_β:
 mov eax, dword ptr [rbp + 244]
 cmp eax, 0
 je xchain399_n11_β
 jmp xchain399_n12_β
xchain399_n6_af:
 add dword ptr [rbp + 244], 1
 mov r14d, dword ptr [rbp + 240]
 mov eax, dword ptr [rbp + 244]
 cmp eax, 1
 je xchain399_n12_α
 jmp xchain399_n1_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain399_n7_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx413_0
 lea rcx, [rip + .Lx413_4]
 lea rdx, [rip + .Lx413_5]
 jmp rax
.Lx413_4:
 jmp xchain399_n2_as
.Lx413_5:
 jmp xchain399_n2_af
.Lx413_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx413_2:
 test rax, rax
 je .Lx413_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx413_7]
 lea rdx, [rip + .Lx413_8]
 jmp rax
.Lx413_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx413_2
.Lx413_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx413_2
.Lx413_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain399_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx413_6]
 sub rsp, 8
 push rax
 jmp xchain399_n2_as
.Lx413_6:
 add rsp, 16
 jmp xchain399_n2_af
 xchain399_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain399_n8_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx414_0
 lea rcx, [rip + .Lx414_4]
 lea rdx, [rip + .Lx414_5]
 jmp rax
.Lx414_4:
 jmp xchain399_n2_as
.Lx414_5:
 jmp xchain399_n2_af
.Lx414_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx414_2:
 test rax, rax
 je .Lx414_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx414_7]
 lea rdx, [rip + .Lx414_8]
 jmp rax
.Lx414_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx414_2
.Lx414_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx414_2
.Lx414_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain399_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx414_6]
 sub rsp, 8
 push rax
 jmp xchain399_n2_as
.Lx414_6:
 add rsp, 16
 jmp xchain399_n2_af
 xchain399_n8_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain399_n9_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx415_0
 lea rcx, [rip + .Lx415_4]
 lea rdx, [rip + .Lx415_5]
 jmp rax
.Lx415_4:
 jmp xchain399_n5_as
.Lx415_5:
 jmp xchain399_n5_af
.Lx415_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx415_2:
 test rax, rax
 je .Lx415_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx415_7]
 lea rdx, [rip + .Lx415_8]
 jmp rax
.Lx415_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx415_2
.Lx415_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx415_2
.Lx415_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain399_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx415_6]
 sub rsp, 8
 push rax
 jmp xchain399_n5_as
.Lx415_6:
 add rsp, 16
 jmp xchain399_n5_af
 xchain399_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain399_n10_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx416_0
 lea rcx, [rip + .Lx416_4]
 lea rdx, [rip + .Lx416_5]
 jmp rax
.Lx416_4:
 jmp xchain399_n5_as
.Lx416_5:
 jmp xchain399_n5_af
.Lx416_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx416_2:
 test rax, rax
 je .Lx416_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx416_7]
 lea rdx, [rip + .Lx416_8]
 jmp rax
.Lx416_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx416_2
.Lx416_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx416_2
.Lx416_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain399_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx416_6]
 sub rsp, 8
 push rax
 jmp xchain399_n5_as
.Lx416_6:
 add rsp, 16
 jmp xchain399_n5_af
 xchain399_n10_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain399_n11_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx417_0
 lea rcx, [rip + .Lx417_4]
 lea rdx, [rip + .Lx417_5]
 jmp rax
.Lx417_4:
 jmp xchain399_n6_as
.Lx417_5:
 jmp xchain399_n6_af
.Lx417_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx417_2:
 test rax, rax
 je .Lx417_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx417_7]
 lea rdx, [rip + .Lx417_8]
 jmp rax
.Lx417_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx417_2
.Lx417_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx417_2
.Lx417_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain399_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx417_6]
 sub rsp, 8
 push rax
 jmp xchain399_n6_as
.Lx417_6:
 add rsp, 16
 jmp xchain399_n6_af
 xchain399_n11_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain399_n12_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx418_0
 lea rcx, [rip + .Lx418_4]
 lea rdx, [rip + .Lx418_5]
 jmp rax
.Lx418_4:
 jmp xchain399_n6_as
.Lx418_5:
 jmp xchain399_n6_af
.Lx418_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx418_2:
 test rax, rax
 je .Lx418_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx418_7]
 lea rdx, [rip + .Lx418_8]
 jmp rax
.Lx418_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx418_2
.Lx418_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx418_2
.Lx418_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain399_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx418_6]
 sub rsp, 8
 push rax
 jmp xchain399_n6_as
.Lx418_6:
 add rsp, 16
 jmp xchain399_n6_af
 xchain399_n12_β:
 jmp qword ptr [rsp]
proc_PAT$6_res:
add rsp, 8
pop rbp
proc_PAT$6_β:
jmp qword ptr [rbp + 416]
proc_PAT$6_γ:
push rbp
lea rax, [rip + proc_PAT$6_res]
push rax
mov rax, [rbp + 456]
mov rbp, [rbp + 472]
jmp rax
proc_PAT$6_ω:
mov rax, [rbp + 464]
lea rsp, [rbp + 480]
mov rbp, [rbp + 472]
jmp rax
  .globl proc_PAT$7_α
proc_PAT$7_α:
#=======================================================================================================================
    .global proc_PAT$7_α
    .global proc_PAT$7_β
    .global proc_PAT$7_γ
    .global proc_PAT$7_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
proc_PAT$7_α_body:
lea rax, [rip + xchain419_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain419_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain419_n1_α
xchain419_n0_as:
 jmp proc_PAT$7_γ
 xchain419_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain419_n1_β
 jmp xchain419_n2_β
xchain419_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain419_n2_α
 jmp proc_PAT$7_ω
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain419_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx422_0
 lea rcx, [rip + .Lx422_4]
 lea rdx, [rip + .Lx422_5]
 jmp rax
.Lx422_4:
 jmp xchain419_n0_as
.Lx422_5:
 jmp xchain419_n0_af
.Lx422_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_open@PLT
.Lx422_2:
 test rax, rax
 je .Lx422_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx422_7]
 lea rdx, [rip + .Lx422_8]
 jmp rax
.Lx422_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx422_2
.Lx422_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx422_2
.Lx422_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain419_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx422_6]
 sub rsp, 8
 push rax
 jmp xchain419_n0_as
.Lx422_6:
 add rsp, 16
 jmp xchain419_n0_af
 xchain419_n1_β:
 jmp qword ptr [rsp]
# IR_MATCH_SEQ_NARY
 xchain419_n2_α:
 mov dword ptr [rbp + 96], r14d
 mov dword ptr [rbp + 100], 0
 jmp xchain419_n3_α
xchain419_n2_as:
 mov eax, dword ptr [rbp + 100]
 add eax, 1
 mov dword ptr [rbp + 100], eax
 cmp eax, 1
 je xchain419_n4_α
 cmp eax, 2
 je xchain419_n5_α
 jmp xchain419_n0_as
 xchain419_n2_β:
 mov dword ptr [rbp + 100], 3
xchain419_n2_af:
 mov eax, dword ptr [rbp + 100]
 sub eax, 1
 mov dword ptr [rbp + 100], eax
 cmp eax, 0
 je xchain419_n3_β
 cmp eax, 1
 je xchain419_n4_β
 cmp eax, 2
 je xchain419_n5_β
 jmp xchain419_n0_af
# IR_MATCH_LIT
 xchain419_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain419_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain419_n2_af
 add r14d, 1
 jmp xchain419_n2_as
 xchain419_n3_β:
 sub r14d, 1
 jmp xchain419_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain419_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx427_0
 lea rcx, [rip + .Lx427_4]
 lea rdx, [rip + .Lx427_5]
 jmp rax
.Lx427_4:
 jmp xchain419_n2_as
.Lx427_5:
 jmp xchain419_n2_af
.Lx427_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_open@PLT
.Lx427_2:
 test rax, rax
 je .Lx427_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx427_7]
 lea rdx, [rip + .Lx427_8]
 jmp rax
.Lx427_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx427_2
.Lx427_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx427_2
.Lx427_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain419_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx427_6]
 sub rsp, 8
 push rax
 jmp xchain419_n2_as
.Lx427_6:
 add rsp, 16
 jmp xchain419_n2_af
 xchain419_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain419_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain419_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain419_n2_af
 add r14d, 1
 jmp xchain419_n2_as
 xchain419_n5_β:
 sub r14d, 1
 jmp xchain419_n2_af
proc_PAT$7_res:
add rsp, 8
pop rbp
proc_PAT$7_β:
jmp qword ptr [rbp + 176]
proc_PAT$7_γ:
push rbp
lea rax, [rip + proc_PAT$7_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$7_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$8_α
proc_PAT$8_α:
#=======================================================================================================================
    .global proc_PAT$8_α
    .global proc_PAT$8_β
    .global proc_PAT$8_γ
    .global proc_PAT$8_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_PAT$8_α_body:
lea rax, [rip + xchain430_n0_β]
mov qword ptr [rbp + 48], rax
# IR_MATCH_SPAN
 xchain430_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
.Lx432_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx432_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 call strchr@PLT
 test rax, rax
 je .Lx432_1
 add dword ptr [rsp + 0], 1
 jmp .Lx432_0
.Lx432_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx432_240
 add rsp, 16
 jmp proc_PAT$8_ω
.Lx432_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$8_γ
 xchain430_n0_β:
 xchain430_n0_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp proc_PAT$8_ω
proc_PAT$8_res:
add rsp, 8
pop rbp
proc_PAT$8_β:
jmp qword ptr [rbp + 48]
proc_PAT$8_γ:
push rbp
lea rax, [rip + proc_PAT$8_res]
push rax
mov rax, [rbp + 88]
mov rbp, [rbp + 104]
jmp rax
proc_PAT$8_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$9_α
proc_PAT$9_α:
#=======================================================================================================================
    .global proc_PAT$9_α
    .global proc_PAT$9_β
    .global proc_PAT$9_γ
    .global proc_PAT$9_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
proc_PAT$9_α_body:
lea rax, [rip + xchain433_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_SEQ_NARY
 xchain433_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain433_n1_α
xchain433_n0_as:
 mov eax, dword ptr [rbp + 36]
 add eax, 1
 mov dword ptr [rbp + 36], eax
 cmp eax, 1
 je xchain433_n2_α
 cmp eax, 2
 je xchain433_n3_α
 jmp proc_PAT$9_γ
 xchain433_n0_β:
 mov dword ptr [rbp + 36], 3
xchain433_n0_af:
 mov eax, dword ptr [rbp + 36]
 sub eax, 1
 mov dword ptr [rbp + 36], eax
 cmp eax, 0
 je xchain433_n1_β
 cmp eax, 1
 je xchain433_n2_β
 cmp eax, 2
 je xchain433_n3_β
 jmp proc_PAT$9_ω
# IR_MATCH_ANY
 xchain433_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain433_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 call strchr@PLT
 test rax, rax
 je xchain433_n0_af
 add r14d, 1
 jmp xchain433_n0_as
 xchain433_n1_β:
 sub r14d, 1
 jmp xchain433_n0_af
# IR_MATCH_ALT_NARY
 xchain433_n2_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain433_n4_α
xchain433_n2_as:
 jmp xchain433_n0_as
 xchain433_n2_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain433_n4_β
 jmp xchain433_n5_β
xchain433_n2_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain433_n5_α
 jmp xchain433_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain433_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx439_0
 lea rcx, [rip + .Lx439_4]
 lea rdx, [rip + .Lx439_5]
 jmp rax
.Lx439_4:
 jmp xchain433_n0_as
.Lx439_5:
 jmp xchain433_n0_af
.Lx439_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx439_2:
 test rax, rax
 je .Lx439_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx439_7]
 lea rdx, [rip + .Lx439_8]
 jmp rax
.Lx439_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx439_2
.Lx439_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx439_2
.Lx439_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain433_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx439_6]
 sub rsp, 8
 push rax
 jmp xchain433_n0_as
.Lx439_6:
 add rsp, 16
 jmp xchain433_n0_af
 xchain433_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_ANY
 xchain433_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain433_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 call strchr@PLT
 test rax, rax
 je xchain433_n2_af
 add r14d, 1
 jmp xchain433_n2_as
 xchain433_n4_β:
 sub r14d, 1
 jmp xchain433_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain433_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx441_0
 lea rcx, [rip + .Lx441_4]
 lea rdx, [rip + .Lx441_5]
 jmp rax
.Lx441_4:
 jmp xchain433_n2_as
.Lx441_5:
 jmp xchain433_n2_af
.Lx441_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx441_2:
 test rax, rax
 je .Lx441_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx441_7]
 lea rdx, [rip + .Lx441_8]
 jmp rax
.Lx441_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx441_2
.Lx441_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx441_2
.Lx441_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain433_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx441_6]
 sub rsp, 8
 push rax
 jmp xchain433_n2_as
.Lx441_6:
 add rsp, 16
 jmp xchain433_n2_af
 xchain433_n5_β:
 jmp qword ptr [rsp]
proc_PAT$9_res:
add rsp, 8
pop rbp
proc_PAT$9_β:
jmp qword ptr [rbp + 176]
proc_PAT$9_γ:
push rbp
lea rax, [rip + proc_PAT$9_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$9_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$10_α
proc_PAT$10_α:
#=======================================================================================================================
    .global proc_PAT$10_α
    .global proc_PAT$10_β
    .global proc_PAT$10_γ
    .global proc_PAT$10_ω
  sub rsp, 480
  mov [rsp + 456], rcx
  mov [rsp + 464], rdx
  mov [rsp + 472], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 448
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 440], rsp
proc_PAT$10_α_body:
lea rax, [rip + xchain442_n0_β]
mov qword ptr [rbp + 416], rax
# IR_MATCH_ALT_NARY
 xchain442_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain442_n1_α
xchain442_n0_as:
 jmp proc_PAT$10_γ
 xchain442_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain442_n1_β
 jmp xchain442_n2_β
xchain442_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain442_n2_α
 jmp proc_PAT$10_ω
# IR_MATCH_SEQ_NARY
 xchain442_n1_α:
 mov dword ptr [rbp + 64], r14d
 mov dword ptr [rbp + 68], 0
 jmp xchain442_n3_α
xchain442_n1_as:
 mov eax, dword ptr [rbp + 68]
 add eax, 1
 mov dword ptr [rbp + 68], eax
 cmp eax, 1
 je xchain442_n4_α
 cmp eax, 2
 je xchain442_n5_α
 cmp eax, 3
 je xchain442_n6_α
 jmp xchain442_n0_as
 xchain442_n1_β:
 mov dword ptr [rbp + 68], 4
xchain442_n1_af:
 mov eax, dword ptr [rbp + 68]
 sub eax, 1
 mov dword ptr [rbp + 68], eax
 cmp eax, 0
 je xchain442_n3_β
 cmp eax, 1
 je xchain442_n4_β
 cmp eax, 2
 je xchain442_n5_β
 cmp eax, 3
 je xchain442_n6_β
 jmp xchain442_n0_af
# IR_MATCH_SEQ_NARY
 xchain442_n2_α:
 mov dword ptr [rbp + 336], r14d
 mov dword ptr [rbp + 340], 0
 jmp xchain442_n7_α
xchain442_n2_as:
 mov eax, dword ptr [rbp + 340]
 add eax, 1
 mov dword ptr [rbp + 340], eax
 cmp eax, 1
 je xchain442_n8_α
 jmp xchain442_n0_as
 xchain442_n2_β:
 mov dword ptr [rbp + 340], 2
xchain442_n2_af:
 mov eax, dword ptr [rbp + 340]
 sub eax, 1
 mov dword ptr [rbp + 340], eax
 cmp eax, 0
 je xchain442_n7_β
 cmp eax, 1
 je xchain442_n8_β
 jmp xchain442_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain442_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx449_0
 lea rcx, [rip + .Lx449_4]
 lea rdx, [rip + .Lx449_5]
 jmp rax
.Lx449_4:
 jmp xchain442_n1_as
.Lx449_5:
 jmp xchain442_n1_af
.Lx449_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx449_2:
 test rax, rax
 je .Lx449_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx449_7]
 lea rdx, [rip + .Lx449_8]
 jmp rax
.Lx449_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx449_2
.Lx449_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx449_2
.Lx449_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain442_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx449_6]
 sub rsp, 8
 push rax
 jmp xchain442_n1_as
.Lx449_6:
 add rsp, 16
 jmp xchain442_n1_af
 xchain442_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain442_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain442_n1_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain442_n1_af
 add r14d, 1
 jmp xchain442_n1_as
 xchain442_n4_β:
 sub r14d, 1
 jmp xchain442_n1_af
# IR_MATCH_ALT_NARY
 xchain442_n5_α:
 mov dword ptr [rbp + 144], r14d
 mov dword ptr [rbp + 148], 0
 jmp xchain442_n9_α
xchain442_n5_as:
 jmp xchain442_n1_as
 xchain442_n5_β:
 mov eax, dword ptr [rbp + 148]
 cmp eax, 0
 je xchain442_n9_β
 jmp xchain442_n10_β
xchain442_n5_af:
 add dword ptr [rbp + 148], 1
 mov r14d, dword ptr [rbp + 144]
 mov eax, dword ptr [rbp + 148]
 cmp eax, 1
 je xchain442_n10_α
 jmp xchain442_n1_af
# IR_MATCH_ALT_NARY
 xchain442_n6_α:
 mov dword ptr [rbp + 240], r14d
 mov dword ptr [rbp + 244], 0
 jmp xchain442_n11_α
xchain442_n6_as:
 jmp xchain442_n1_as
 xchain442_n6_β:
 mov eax, dword ptr [rbp + 244]
 cmp eax, 0
 je xchain442_n11_β
 jmp xchain442_n12_β
xchain442_n6_af:
 add dword ptr [rbp + 244], 1
 mov r14d, dword ptr [rbp + 240]
 mov eax, dword ptr [rbp + 244]
 cmp eax, 1
 je xchain442_n12_α
 jmp xchain442_n1_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain442_n7_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx456_0
 lea rcx, [rip + .Lx456_4]
 lea rdx, [rip + .Lx456_5]
 jmp rax
.Lx456_4:
 jmp xchain442_n2_as
.Lx456_5:
 jmp xchain442_n2_af
.Lx456_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx456_2:
 test rax, rax
 je .Lx456_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx456_7]
 lea rdx, [rip + .Lx456_8]
 jmp rax
.Lx456_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx456_2
.Lx456_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx456_2
.Lx456_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain442_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx456_6]
 sub rsp, 8
 push rax
 jmp xchain442_n2_as
.Lx456_6:
 add rsp, 16
 jmp xchain442_n2_af
 xchain442_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain442_n8_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx457_0
 lea rcx, [rip + .Lx457_4]
 lea rdx, [rip + .Lx457_5]
 jmp rax
.Lx457_4:
 jmp xchain442_n2_as
.Lx457_5:
 jmp xchain442_n2_af
.Lx457_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx457_2:
 test rax, rax
 je .Lx457_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx457_7]
 lea rdx, [rip + .Lx457_8]
 jmp rax
.Lx457_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx457_2
.Lx457_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx457_2
.Lx457_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain442_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx457_6]
 sub rsp, 8
 push rax
 jmp xchain442_n2_as
.Lx457_6:
 add rsp, 16
 jmp xchain442_n2_af
 xchain442_n8_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain442_n9_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx458_0
 lea rcx, [rip + .Lx458_4]
 lea rdx, [rip + .Lx458_5]
 jmp rax
.Lx458_4:
 jmp xchain442_n5_as
.Lx458_5:
 jmp xchain442_n5_af
.Lx458_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx458_2:
 test rax, rax
 je .Lx458_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx458_7]
 lea rdx, [rip + .Lx458_8]
 jmp rax
.Lx458_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx458_2
.Lx458_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx458_2
.Lx458_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain442_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx458_6]
 sub rsp, 8
 push rax
 jmp xchain442_n5_as
.Lx458_6:
 add rsp, 16
 jmp xchain442_n5_af
 xchain442_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain442_n10_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx459_0
 lea rcx, [rip + .Lx459_4]
 lea rdx, [rip + .Lx459_5]
 jmp rax
.Lx459_4:
 jmp xchain442_n5_as
.Lx459_5:
 jmp xchain442_n5_af
.Lx459_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx459_2:
 test rax, rax
 je .Lx459_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx459_7]
 lea rdx, [rip + .Lx459_8]
 jmp rax
.Lx459_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx459_2
.Lx459_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx459_2
.Lx459_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain442_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx459_6]
 sub rsp, 8
 push rax
 jmp xchain442_n5_as
.Lx459_6:
 add rsp, 16
 jmp xchain442_n5_af
 xchain442_n10_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain442_n11_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx460_0
 lea rcx, [rip + .Lx460_4]
 lea rdx, [rip + .Lx460_5]
 jmp rax
.Lx460_4:
 jmp xchain442_n6_as
.Lx460_5:
 jmp xchain442_n6_af
.Lx460_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx460_2:
 test rax, rax
 je .Lx460_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx460_7]
 lea rdx, [rip + .Lx460_8]
 jmp rax
.Lx460_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx460_2
.Lx460_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx460_2
.Lx460_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain442_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx460_6]
 sub rsp, 8
 push rax
 jmp xchain442_n6_as
.Lx460_6:
 add rsp, 16
 jmp xchain442_n6_af
 xchain442_n11_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain442_n12_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx461_0
 lea rcx, [rip + .Lx461_4]
 lea rdx, [rip + .Lx461_5]
 jmp rax
.Lx461_4:
 jmp xchain442_n6_as
.Lx461_5:
 jmp xchain442_n6_af
.Lx461_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx461_2:
 test rax, rax
 je .Lx461_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx461_7]
 lea rdx, [rip + .Lx461_8]
 jmp rax
.Lx461_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx461_2
.Lx461_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx461_2
.Lx461_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain442_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx461_6]
 sub rsp, 8
 push rax
 jmp xchain442_n6_as
.Lx461_6:
 add rsp, 16
 jmp xchain442_n6_af
 xchain442_n12_β:
 jmp qword ptr [rsp]
proc_PAT$10_res:
add rsp, 8
pop rbp
proc_PAT$10_β:
jmp qword ptr [rbp + 416]
proc_PAT$10_γ:
push rbp
lea rax, [rip + proc_PAT$10_res]
push rax
mov rax, [rbp + 456]
mov rbp, [rbp + 472]
jmp rax
proc_PAT$10_ω:
mov rax, [rbp + 464]
lea rsp, [rbp + 480]
mov rbp, [rbp + 472]
jmp rax
  .globl proc_PAT$11_α
proc_PAT$11_α:
#=======================================================================================================================
    .global proc_PAT$11_α
    .global proc_PAT$11_β
    .global proc_PAT$11_γ
    .global proc_PAT$11_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
proc_PAT$11_α_body:
lea rax, [rip + xchain462_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain462_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain462_n1_α
xchain462_n0_as:
 jmp proc_PAT$11_γ
 xchain462_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain462_n1_β
 jmp xchain462_n2_β
xchain462_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain462_n2_α
 jmp proc_PAT$11_ω
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain462_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx465_0
 lea rcx, [rip + .Lx465_4]
 lea rdx, [rip + .Lx465_5]
 jmp rax
.Lx465_4:
 jmp xchain462_n0_as
.Lx465_5:
 jmp xchain462_n0_af
.Lx465_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_open@PLT
.Lx465_2:
 test rax, rax
 je .Lx465_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx465_7]
 lea rdx, [rip + .Lx465_8]
 jmp rax
.Lx465_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx465_2
.Lx465_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx465_2
.Lx465_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain462_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx465_6]
 sub rsp, 8
 push rax
 jmp xchain462_n0_as
.Lx465_6:
 add rsp, 16
 jmp xchain462_n0_af
 xchain462_n1_β:
 jmp qword ptr [rsp]
# IR_MATCH_SEQ_NARY
 xchain462_n2_α:
 mov dword ptr [rbp + 96], r14d
 mov dword ptr [rbp + 100], 0
 jmp xchain462_n3_α
xchain462_n2_as:
 mov eax, dword ptr [rbp + 100]
 add eax, 1
 mov dword ptr [rbp + 100], eax
 cmp eax, 1
 je xchain462_n4_α
 cmp eax, 2
 je xchain462_n5_α
 jmp xchain462_n0_as
 xchain462_n2_β:
 mov dword ptr [rbp + 100], 3
xchain462_n2_af:
 mov eax, dword ptr [rbp + 100]
 sub eax, 1
 mov dword ptr [rbp + 100], eax
 cmp eax, 0
 je xchain462_n3_β
 cmp eax, 1
 je xchain462_n4_β
 cmp eax, 2
 je xchain462_n5_β
 jmp xchain462_n0_af
# IR_MATCH_LIT
 xchain462_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain462_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain462_n2_af
 add r14d, 1
 jmp xchain462_n2_as
 xchain462_n3_β:
 sub r14d, 1
 jmp xchain462_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain462_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx470_0
 lea rcx, [rip + .Lx470_4]
 lea rdx, [rip + .Lx470_5]
 jmp rax
.Lx470_4:
 jmp xchain462_n2_as
.Lx470_5:
 jmp xchain462_n2_af
.Lx470_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_open@PLT
.Lx470_2:
 test rax, rax
 je .Lx470_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx470_7]
 lea rdx, [rip + .Lx470_8]
 jmp rax
.Lx470_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx470_2
.Lx470_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx470_2
.Lx470_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain462_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx470_6]
 sub rsp, 8
 push rax
 jmp xchain462_n2_as
.Lx470_6:
 add rsp, 16
 jmp xchain462_n2_af
 xchain462_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain462_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain462_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain462_n2_af
 add r14d, 1
 jmp xchain462_n2_as
 xchain462_n5_β:
 sub r14d, 1
 jmp xchain462_n2_af
proc_PAT$11_res:
add rsp, 8
pop rbp
proc_PAT$11_β:
jmp qword ptr [rbp + 176]
proc_PAT$11_γ:
push rbp
lea rax, [rip + proc_PAT$11_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$11_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$12_α
proc_PAT$12_α:
#=======================================================================================================================
    .global proc_PAT$12_α
    .global proc_PAT$12_β
    .global proc_PAT$12_γ
    .global proc_PAT$12_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_PAT$12_α_body:
lea rax, [rip + xchain473_n0_β]
mov qword ptr [rbp + 48], rax
# IR_MATCH_SPAN
 xchain473_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
.Lx475_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx475_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 call strchr@PLT
 test rax, rax
 je .Lx475_1
 add dword ptr [rsp + 0], 1
 jmp .Lx475_0
.Lx475_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx475_240
 add rsp, 16
 jmp proc_PAT$12_ω
.Lx475_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$12_γ
 xchain473_n0_β:
 xchain473_n0_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp proc_PAT$12_ω
proc_PAT$12_res:
add rsp, 8
pop rbp
proc_PAT$12_β:
jmp qword ptr [rbp + 48]
proc_PAT$12_γ:
push rbp
lea rax, [rip + proc_PAT$12_res]
push rax
mov rax, [rbp + 88]
mov rbp, [rbp + 104]
jmp rax
proc_PAT$12_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$13_α
proc_PAT$13_α:
#=======================================================================================================================
    .global proc_PAT$13_α
    .global proc_PAT$13_β
    .global proc_PAT$13_γ
    .global proc_PAT$13_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
proc_PAT$13_α_body:
lea rax, [rip + xchain476_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_SEQ_NARY
 xchain476_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain476_n1_α
xchain476_n0_as:
 mov eax, dword ptr [rbp + 36]
 add eax, 1
 mov dword ptr [rbp + 36], eax
 cmp eax, 1
 je xchain476_n2_α
 cmp eax, 2
 je xchain476_n3_α
 jmp proc_PAT$13_γ
 xchain476_n0_β:
 mov dword ptr [rbp + 36], 3
xchain476_n0_af:
 mov eax, dword ptr [rbp + 36]
 sub eax, 1
 mov dword ptr [rbp + 36], eax
 cmp eax, 0
 je xchain476_n1_β
 cmp eax, 1
 je xchain476_n2_β
 cmp eax, 2
 je xchain476_n3_β
 jmp proc_PAT$13_ω
# IR_MATCH_ANY
 xchain476_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain476_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 call strchr@PLT
 test rax, rax
 je xchain476_n0_af
 add r14d, 1
 jmp xchain476_n0_as
 xchain476_n1_β:
 sub r14d, 1
 jmp xchain476_n0_af
# IR_MATCH_ALT_NARY
 xchain476_n2_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain476_n4_α
xchain476_n2_as:
 jmp xchain476_n0_as
 xchain476_n2_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain476_n4_β
 jmp xchain476_n5_β
xchain476_n2_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain476_n5_α
 jmp xchain476_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain476_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx482_0
 lea rcx, [rip + .Lx482_4]
 lea rdx, [rip + .Lx482_5]
 jmp rax
.Lx482_4:
 jmp xchain476_n0_as
.Lx482_5:
 jmp xchain476_n0_af
.Lx482_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx482_2:
 test rax, rax
 je .Lx482_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx482_7]
 lea rdx, [rip + .Lx482_8]
 jmp rax
.Lx482_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx482_2
.Lx482_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx482_2
.Lx482_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain476_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx482_6]
 sub rsp, 8
 push rax
 jmp xchain476_n0_as
.Lx482_6:
 add rsp, 16
 jmp xchain476_n0_af
 xchain476_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_ANY
 xchain476_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain476_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 call strchr@PLT
 test rax, rax
 je xchain476_n2_af
 add r14d, 1
 jmp xchain476_n2_as
 xchain476_n4_β:
 sub r14d, 1
 jmp xchain476_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain476_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx484_0
 lea rcx, [rip + .Lx484_4]
 lea rdx, [rip + .Lx484_5]
 jmp rax
.Lx484_4:
 jmp xchain476_n2_as
.Lx484_5:
 jmp xchain476_n2_af
.Lx484_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx484_2:
 test rax, rax
 je .Lx484_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx484_7]
 lea rdx, [rip + .Lx484_8]
 jmp rax
.Lx484_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx484_2
.Lx484_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx484_2
.Lx484_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain476_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx484_6]
 sub rsp, 8
 push rax
 jmp xchain476_n2_as
.Lx484_6:
 add rsp, 16
 jmp xchain476_n2_af
 xchain476_n5_β:
 jmp qword ptr [rsp]
proc_PAT$13_res:
add rsp, 8
pop rbp
proc_PAT$13_β:
jmp qword ptr [rbp + 176]
proc_PAT$13_γ:
push rbp
lea rax, [rip + proc_PAT$13_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$13_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$14_α
proc_PAT$14_α:
#=======================================================================================================================
    .global proc_PAT$14_α
    .global proc_PAT$14_β
    .global proc_PAT$14_γ
    .global proc_PAT$14_ω
  sub rsp, 480
  mov [rsp + 456], rcx
  mov [rsp + 464], rdx
  mov [rsp + 472], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 448
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 440], rsp
proc_PAT$14_α_body:
lea rax, [rip + xchain485_n0_β]
mov qword ptr [rbp + 416], rax
# IR_MATCH_ALT_NARY
 xchain485_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain485_n1_α
xchain485_n0_as:
 jmp proc_PAT$14_γ
 xchain485_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain485_n1_β
 jmp xchain485_n2_β
xchain485_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain485_n2_α
 jmp proc_PAT$14_ω
# IR_MATCH_SEQ_NARY
 xchain485_n1_α:
 mov dword ptr [rbp + 64], r14d
 mov dword ptr [rbp + 68], 0
 jmp xchain485_n3_α
xchain485_n1_as:
 mov eax, dword ptr [rbp + 68]
 add eax, 1
 mov dword ptr [rbp + 68], eax
 cmp eax, 1
 je xchain485_n4_α
 cmp eax, 2
 je xchain485_n5_α
 cmp eax, 3
 je xchain485_n6_α
 jmp xchain485_n0_as
 xchain485_n1_β:
 mov dword ptr [rbp + 68], 4
xchain485_n1_af:
 mov eax, dword ptr [rbp + 68]
 sub eax, 1
 mov dword ptr [rbp + 68], eax
 cmp eax, 0
 je xchain485_n3_β
 cmp eax, 1
 je xchain485_n4_β
 cmp eax, 2
 je xchain485_n5_β
 cmp eax, 3
 je xchain485_n6_β
 jmp xchain485_n0_af
# IR_MATCH_SEQ_NARY
 xchain485_n2_α:
 mov dword ptr [rbp + 336], r14d
 mov dword ptr [rbp + 340], 0
 jmp xchain485_n7_α
xchain485_n2_as:
 mov eax, dword ptr [rbp + 340]
 add eax, 1
 mov dword ptr [rbp + 340], eax
 cmp eax, 1
 je xchain485_n8_α
 jmp xchain485_n0_as
 xchain485_n2_β:
 mov dword ptr [rbp + 340], 2
xchain485_n2_af:
 mov eax, dword ptr [rbp + 340]
 sub eax, 1
 mov dword ptr [rbp + 340], eax
 cmp eax, 0
 je xchain485_n7_β
 cmp eax, 1
 je xchain485_n8_β
 jmp xchain485_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain485_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx492_0
 lea rcx, [rip + .Lx492_4]
 lea rdx, [rip + .Lx492_5]
 jmp rax
.Lx492_4:
 jmp xchain485_n1_as
.Lx492_5:
 jmp xchain485_n1_af
.Lx492_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx492_2:
 test rax, rax
 je .Lx492_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx492_7]
 lea rdx, [rip + .Lx492_8]
 jmp rax
.Lx492_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx492_2
.Lx492_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx492_2
.Lx492_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain485_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx492_6]
 sub rsp, 8
 push rax
 jmp xchain485_n1_as
.Lx492_6:
 add rsp, 16
 jmp xchain485_n1_af
 xchain485_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain485_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain485_n1_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain485_n1_af
 add r14d, 1
 jmp xchain485_n1_as
 xchain485_n4_β:
 sub r14d, 1
 jmp xchain485_n1_af
# IR_MATCH_ALT_NARY
 xchain485_n5_α:
 mov dword ptr [rbp + 144], r14d
 mov dword ptr [rbp + 148], 0
 jmp xchain485_n9_α
xchain485_n5_as:
 jmp xchain485_n1_as
 xchain485_n5_β:
 mov eax, dword ptr [rbp + 148]
 cmp eax, 0
 je xchain485_n9_β
 jmp xchain485_n10_β
xchain485_n5_af:
 add dword ptr [rbp + 148], 1
 mov r14d, dword ptr [rbp + 144]
 mov eax, dword ptr [rbp + 148]
 cmp eax, 1
 je xchain485_n10_α
 jmp xchain485_n1_af
# IR_MATCH_ALT_NARY
 xchain485_n6_α:
 mov dword ptr [rbp + 240], r14d
 mov dword ptr [rbp + 244], 0
 jmp xchain485_n11_α
xchain485_n6_as:
 jmp xchain485_n1_as
 xchain485_n6_β:
 mov eax, dword ptr [rbp + 244]
 cmp eax, 0
 je xchain485_n11_β
 jmp xchain485_n12_β
xchain485_n6_af:
 add dword ptr [rbp + 244], 1
 mov r14d, dword ptr [rbp + 240]
 mov eax, dword ptr [rbp + 244]
 cmp eax, 1
 je xchain485_n12_α
 jmp xchain485_n1_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain485_n7_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx499_0
 lea rcx, [rip + .Lx499_4]
 lea rdx, [rip + .Lx499_5]
 jmp rax
.Lx499_4:
 jmp xchain485_n2_as
.Lx499_5:
 jmp xchain485_n2_af
.Lx499_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx499_2:
 test rax, rax
 je .Lx499_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx499_7]
 lea rdx, [rip + .Lx499_8]
 jmp rax
.Lx499_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx499_2
.Lx499_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx499_2
.Lx499_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain485_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx499_6]
 sub rsp, 8
 push rax
 jmp xchain485_n2_as
.Lx499_6:
 add rsp, 16
 jmp xchain485_n2_af
 xchain485_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain485_n8_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx500_0
 lea rcx, [rip + .Lx500_4]
 lea rdx, [rip + .Lx500_5]
 jmp rax
.Lx500_4:
 jmp xchain485_n2_as
.Lx500_5:
 jmp xchain485_n2_af
.Lx500_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx500_2:
 test rax, rax
 je .Lx500_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx500_7]
 lea rdx, [rip + .Lx500_8]
 jmp rax
.Lx500_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx500_2
.Lx500_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx500_2
.Lx500_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain485_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx500_6]
 sub rsp, 8
 push rax
 jmp xchain485_n2_as
.Lx500_6:
 add rsp, 16
 jmp xchain485_n2_af
 xchain485_n8_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain485_n9_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx501_0
 lea rcx, [rip + .Lx501_4]
 lea rdx, [rip + .Lx501_5]
 jmp rax
.Lx501_4:
 jmp xchain485_n5_as
.Lx501_5:
 jmp xchain485_n5_af
.Lx501_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx501_2:
 test rax, rax
 je .Lx501_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx501_7]
 lea rdx, [rip + .Lx501_8]
 jmp rax
.Lx501_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx501_2
.Lx501_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx501_2
.Lx501_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain485_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx501_6]
 sub rsp, 8
 push rax
 jmp xchain485_n5_as
.Lx501_6:
 add rsp, 16
 jmp xchain485_n5_af
 xchain485_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain485_n10_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx502_0
 lea rcx, [rip + .Lx502_4]
 lea rdx, [rip + .Lx502_5]
 jmp rax
.Lx502_4:
 jmp xchain485_n5_as
.Lx502_5:
 jmp xchain485_n5_af
.Lx502_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx502_2:
 test rax, rax
 je .Lx502_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx502_7]
 lea rdx, [rip + .Lx502_8]
 jmp rax
.Lx502_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx502_2
.Lx502_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx502_2
.Lx502_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain485_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx502_6]
 sub rsp, 8
 push rax
 jmp xchain485_n5_as
.Lx502_6:
 add rsp, 16
 jmp xchain485_n5_af
 xchain485_n10_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain485_n11_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx503_0
 lea rcx, [rip + .Lx503_4]
 lea rdx, [rip + .Lx503_5]
 jmp rax
.Lx503_4:
 jmp xchain485_n6_as
.Lx503_5:
 jmp xchain485_n6_af
.Lx503_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx503_2:
 test rax, rax
 je .Lx503_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx503_7]
 lea rdx, [rip + .Lx503_8]
 jmp rax
.Lx503_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx503_2
.Lx503_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx503_2
.Lx503_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain485_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx503_6]
 sub rsp, 8
 push rax
 jmp xchain485_n6_as
.Lx503_6:
 add rsp, 16
 jmp xchain485_n6_af
 xchain485_n11_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain485_n12_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx504_0
 lea rcx, [rip + .Lx504_4]
 lea rdx, [rip + .Lx504_5]
 jmp rax
.Lx504_4:
 jmp xchain485_n6_as
.Lx504_5:
 jmp xchain485_n6_af
.Lx504_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx504_2:
 test rax, rax
 je .Lx504_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx504_7]
 lea rdx, [rip + .Lx504_8]
 jmp rax
.Lx504_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx504_2
.Lx504_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx504_2
.Lx504_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain485_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx504_6]
 sub rsp, 8
 push rax
 jmp xchain485_n6_as
.Lx504_6:
 add rsp, 16
 jmp xchain485_n6_af
 xchain485_n12_β:
 jmp qword ptr [rsp]
proc_PAT$14_res:
add rsp, 8
pop rbp
proc_PAT$14_β:
jmp qword ptr [rbp + 416]
proc_PAT$14_γ:
push rbp
lea rax, [rip + proc_PAT$14_res]
push rax
mov rax, [rbp + 456]
mov rbp, [rbp + 472]
jmp rax
proc_PAT$14_ω:
mov rax, [rbp + 464]
lea rsp, [rbp + 480]
mov rbp, [rbp + 472]
jmp rax
  .globl proc_PAT$15_α
proc_PAT$15_α:
#=======================================================================================================================
    .global proc_PAT$15_α
    .global proc_PAT$15_β
    .global proc_PAT$15_γ
    .global proc_PAT$15_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
proc_PAT$15_α_body:
lea rax, [rip + xchain505_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain505_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain505_n1_α
xchain505_n0_as:
 jmp proc_PAT$15_γ
 xchain505_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain505_n1_β
 jmp xchain505_n2_β
xchain505_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain505_n2_α
 jmp proc_PAT$15_ω
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain505_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx508_0
 lea rcx, [rip + .Lx508_4]
 lea rdx, [rip + .Lx508_5]
 jmp rax
.Lx508_4:
 jmp xchain505_n0_as
.Lx508_5:
 jmp xchain505_n0_af
.Lx508_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_open@PLT
.Lx508_2:
 test rax, rax
 je .Lx508_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx508_7]
 lea rdx, [rip + .Lx508_8]
 jmp rax
.Lx508_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx508_2
.Lx508_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx508_2
.Lx508_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain505_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx508_6]
 sub rsp, 8
 push rax
 jmp xchain505_n0_as
.Lx508_6:
 add rsp, 16
 jmp xchain505_n0_af
 xchain505_n1_β:
 jmp qword ptr [rsp]
# IR_MATCH_SEQ_NARY
 xchain505_n2_α:
 mov dword ptr [rbp + 96], r14d
 mov dword ptr [rbp + 100], 0
 jmp xchain505_n3_α
xchain505_n2_as:
 mov eax, dword ptr [rbp + 100]
 add eax, 1
 mov dword ptr [rbp + 100], eax
 cmp eax, 1
 je xchain505_n4_α
 cmp eax, 2
 je xchain505_n5_α
 jmp xchain505_n0_as
 xchain505_n2_β:
 mov dword ptr [rbp + 100], 3
xchain505_n2_af:
 mov eax, dword ptr [rbp + 100]
 sub eax, 1
 mov dword ptr [rbp + 100], eax
 cmp eax, 0
 je xchain505_n3_β
 cmp eax, 1
 je xchain505_n4_β
 cmp eax, 2
 je xchain505_n5_β
 jmp xchain505_n0_af
# IR_MATCH_LIT
 xchain505_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain505_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain505_n2_af
 add r14d, 1
 jmp xchain505_n2_as
 xchain505_n3_β:
 sub r14d, 1
 jmp xchain505_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain505_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx513_0
 lea rcx, [rip + .Lx513_4]
 lea rdx, [rip + .Lx513_5]
 jmp rax
.Lx513_4:
 jmp xchain505_n2_as
.Lx513_5:
 jmp xchain505_n2_af
.Lx513_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_open@PLT
.Lx513_2:
 test rax, rax
 je .Lx513_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx513_7]
 lea rdx, [rip + .Lx513_8]
 jmp rax
.Lx513_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx513_2
.Lx513_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx513_2
.Lx513_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain505_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx513_6]
 sub rsp, 8
 push rax
 jmp xchain505_n2_as
.Lx513_6:
 add rsp, 16
 jmp xchain505_n2_af
 xchain505_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain505_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain505_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain505_n2_af
 add r14d, 1
 jmp xchain505_n2_as
 xchain505_n5_β:
 sub r14d, 1
 jmp xchain505_n2_af
proc_PAT$15_res:
add rsp, 8
pop rbp
proc_PAT$15_β:
jmp qword ptr [rbp + 176]
proc_PAT$15_γ:
push rbp
lea rax, [rip + proc_PAT$15_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$15_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$16_α
proc_PAT$16_α:
#=======================================================================================================================
    .global proc_PAT$16_α
    .global proc_PAT$16_β
    .global proc_PAT$16_γ
    .global proc_PAT$16_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_PAT$16_α_body:
lea rax, [rip + xchain516_n0_β]
mov qword ptr [rbp + 48], rax
# IR_MATCH_SPAN
 xchain516_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
.Lx518_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx518_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 call strchr@PLT
 test rax, rax
 je .Lx518_1
 add dword ptr [rsp + 0], 1
 jmp .Lx518_0
.Lx518_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx518_240
 add rsp, 16
 jmp proc_PAT$16_ω
.Lx518_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$16_γ
 xchain516_n0_β:
 xchain516_n0_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp proc_PAT$16_ω
proc_PAT$16_res:
add rsp, 8
pop rbp
proc_PAT$16_β:
jmp qword ptr [rbp + 48]
proc_PAT$16_γ:
push rbp
lea rax, [rip + proc_PAT$16_res]
push rax
mov rax, [rbp + 88]
mov rbp, [rbp + 104]
jmp rax
proc_PAT$16_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$17_α
proc_PAT$17_α:
#=======================================================================================================================
    .global proc_PAT$17_α
    .global proc_PAT$17_β
    .global proc_PAT$17_γ
    .global proc_PAT$17_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
proc_PAT$17_α_body:
lea rax, [rip + xchain519_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_SEQ_NARY
 xchain519_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain519_n1_α
xchain519_n0_as:
 mov eax, dword ptr [rbp + 36]
 add eax, 1
 mov dword ptr [rbp + 36], eax
 cmp eax, 1
 je xchain519_n2_α
 cmp eax, 2
 je xchain519_n3_α
 jmp proc_PAT$17_γ
 xchain519_n0_β:
 mov dword ptr [rbp + 36], 3
xchain519_n0_af:
 mov eax, dword ptr [rbp + 36]
 sub eax, 1
 mov dword ptr [rbp + 36], eax
 cmp eax, 0
 je xchain519_n1_β
 cmp eax, 1
 je xchain519_n2_β
 cmp eax, 2
 je xchain519_n3_β
 jmp proc_PAT$17_ω
# IR_MATCH_ANY
 xchain519_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain519_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 call strchr@PLT
 test rax, rax
 je xchain519_n0_af
 add r14d, 1
 jmp xchain519_n0_as
 xchain519_n1_β:
 sub r14d, 1
 jmp xchain519_n0_af
# IR_MATCH_ALT_NARY
 xchain519_n2_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain519_n4_α
xchain519_n2_as:
 jmp xchain519_n0_as
 xchain519_n2_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain519_n4_β
 jmp xchain519_n5_β
xchain519_n2_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain519_n5_α
 jmp xchain519_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain519_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx525_0
 lea rcx, [rip + .Lx525_4]
 lea rdx, [rip + .Lx525_5]
 jmp rax
.Lx525_4:
 jmp xchain519_n0_as
.Lx525_5:
 jmp xchain519_n0_af
.Lx525_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx525_2:
 test rax, rax
 je .Lx525_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx525_7]
 lea rdx, [rip + .Lx525_8]
 jmp rax
.Lx525_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx525_2
.Lx525_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx525_2
.Lx525_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain519_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx525_6]
 sub rsp, 8
 push rax
 jmp xchain519_n0_as
.Lx525_6:
 add rsp, 16
 jmp xchain519_n0_af
 xchain519_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_ANY
 xchain519_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain519_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 call strchr@PLT
 test rax, rax
 je xchain519_n2_af
 add r14d, 1
 jmp xchain519_n2_as
 xchain519_n4_β:
 sub r14d, 1
 jmp xchain519_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain519_n5_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx527_0
 lea rcx, [rip + .Lx527_4]
 lea rdx, [rip + .Lx527_5]
 jmp rax
.Lx527_4:
 jmp xchain519_n2_as
.Lx527_5:
 jmp xchain519_n2_af
.Lx527_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx527_2:
 test rax, rax
 je .Lx527_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx527_7]
 lea rdx, [rip + .Lx527_8]
 jmp rax
.Lx527_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx527_2
.Lx527_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx527_2
.Lx527_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain519_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx527_6]
 sub rsp, 8
 push rax
 jmp xchain519_n2_as
.Lx527_6:
 add rsp, 16
 jmp xchain519_n2_af
 xchain519_n5_β:
 jmp qword ptr [rsp]
proc_PAT$17_res:
add rsp, 8
pop rbp
proc_PAT$17_β:
jmp qword ptr [rbp + 176]
proc_PAT$17_γ:
push rbp
lea rax, [rip + proc_PAT$17_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$17_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$18_α
proc_PAT$18_α:
#=======================================================================================================================
    .global proc_PAT$18_α
    .global proc_PAT$18_β
    .global proc_PAT$18_γ
    .global proc_PAT$18_ω
  sub rsp, 480
  mov [rsp + 456], rcx
  mov [rsp + 464], rdx
  mov [rsp + 472], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 448
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 440], rsp
proc_PAT$18_α_body:
lea rax, [rip + xchain528_n0_β]
mov qword ptr [rbp + 416], rax
# IR_MATCH_ALT_NARY
 xchain528_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain528_n1_α
xchain528_n0_as:
 jmp proc_PAT$18_γ
 xchain528_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain528_n1_β
 jmp xchain528_n2_β
xchain528_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain528_n2_α
 jmp proc_PAT$18_ω
# IR_MATCH_SEQ_NARY
 xchain528_n1_α:
 mov dword ptr [rbp + 64], r14d
 mov dword ptr [rbp + 68], 0
 jmp xchain528_n3_α
xchain528_n1_as:
 mov eax, dword ptr [rbp + 68]
 add eax, 1
 mov dword ptr [rbp + 68], eax
 cmp eax, 1
 je xchain528_n4_α
 cmp eax, 2
 je xchain528_n5_α
 cmp eax, 3
 je xchain528_n6_α
 jmp xchain528_n0_as
 xchain528_n1_β:
 mov dword ptr [rbp + 68], 4
xchain528_n1_af:
 mov eax, dword ptr [rbp + 68]
 sub eax, 1
 mov dword ptr [rbp + 68], eax
 cmp eax, 0
 je xchain528_n3_β
 cmp eax, 1
 je xchain528_n4_β
 cmp eax, 2
 je xchain528_n5_β
 cmp eax, 3
 je xchain528_n6_β
 jmp xchain528_n0_af
# IR_MATCH_SEQ_NARY
 xchain528_n2_α:
 mov dword ptr [rbp + 336], r14d
 mov dword ptr [rbp + 340], 0
 jmp xchain528_n7_α
xchain528_n2_as:
 mov eax, dword ptr [rbp + 340]
 add eax, 1
 mov dword ptr [rbp + 340], eax
 cmp eax, 1
 je xchain528_n8_α
 jmp xchain528_n0_as
 xchain528_n2_β:
 mov dword ptr [rbp + 340], 2
xchain528_n2_af:
 mov eax, dword ptr [rbp + 340]
 sub eax, 1
 mov dword ptr [rbp + 340], eax
 cmp eax, 0
 je xchain528_n7_β
 cmp eax, 1
 je xchain528_n8_β
 jmp xchain528_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain528_n3_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx535_0
 lea rcx, [rip + .Lx535_4]
 lea rdx, [rip + .Lx535_5]
 jmp rax
.Lx535_4:
 jmp xchain528_n1_as
.Lx535_5:
 jmp xchain528_n1_af
.Lx535_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx535_2:
 test rax, rax
 je .Lx535_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx535_7]
 lea rdx, [rip + .Lx535_8]
 jmp rax
.Lx535_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx535_2
.Lx535_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx535_2
.Lx535_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain528_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx535_6]
 sub rsp, 8
 push rax
 jmp xchain528_n1_as
.Lx535_6:
 add rsp, 16
 jmp xchain528_n1_af
 xchain528_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain528_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain528_n1_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S5]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain528_n1_af
 add r14d, 1
 jmp xchain528_n1_as
 xchain528_n4_β:
 sub r14d, 1
 jmp xchain528_n1_af
# IR_MATCH_ALT_NARY
 xchain528_n5_α:
 mov dword ptr [rbp + 144], r14d
 mov dword ptr [rbp + 148], 0
 jmp xchain528_n9_α
xchain528_n5_as:
 jmp xchain528_n1_as
 xchain528_n5_β:
 mov eax, dword ptr [rbp + 148]
 cmp eax, 0
 je xchain528_n9_β
 jmp xchain528_n10_β
xchain528_n5_af:
 add dword ptr [rbp + 148], 1
 mov r14d, dword ptr [rbp + 144]
 mov eax, dword ptr [rbp + 148]
 cmp eax, 1
 je xchain528_n10_α
 jmp xchain528_n1_af
# IR_MATCH_ALT_NARY
 xchain528_n6_α:
 mov dword ptr [rbp + 240], r14d
 mov dword ptr [rbp + 244], 0
 jmp xchain528_n11_α
xchain528_n6_as:
 jmp xchain528_n1_as
 xchain528_n6_β:
 mov eax, dword ptr [rbp + 244]
 cmp eax, 0
 je xchain528_n11_β
 jmp xchain528_n12_β
xchain528_n6_af:
 add dword ptr [rbp + 244], 1
 mov r14d, dword ptr [rbp + 240]
 mov eax, dword ptr [rbp + 244]
 cmp eax, 1
 je xchain528_n12_α
 jmp xchain528_n1_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain528_n7_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx542_0
 lea rcx, [rip + .Lx542_4]
 lea rdx, [rip + .Lx542_5]
 jmp rax
.Lx542_4:
 jmp xchain528_n2_as
.Lx542_5:
 jmp xchain528_n2_af
.Lx542_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx542_2:
 test rax, rax
 je .Lx542_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx542_7]
 lea rdx, [rip + .Lx542_8]
 jmp rax
.Lx542_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx542_2
.Lx542_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx542_2
.Lx542_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain528_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx542_6]
 sub rsp, 8
 push rax
 jmp xchain528_n2_as
.Lx542_6:
 add rsp, 16
 jmp xchain528_n2_af
 xchain528_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain528_n8_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx543_0
 lea rcx, [rip + .Lx543_4]
 lea rdx, [rip + .Lx543_5]
 jmp rax
.Lx543_4:
 jmp xchain528_n2_as
.Lx543_5:
 jmp xchain528_n2_af
.Lx543_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx543_2:
 test rax, rax
 je .Lx543_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx543_7]
 lea rdx, [rip + .Lx543_8]
 jmp rax
.Lx543_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx543_2
.Lx543_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx543_2
.Lx543_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain528_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx543_6]
 sub rsp, 8
 push rax
 jmp xchain528_n2_as
.Lx543_6:
 add rsp, 16
 jmp xchain528_n2_af
 xchain528_n8_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain528_n9_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx544_0
 lea rcx, [rip + .Lx544_4]
 lea rdx, [rip + .Lx544_5]
 jmp rax
.Lx544_4:
 jmp xchain528_n5_as
.Lx544_5:
 jmp xchain528_n5_af
.Lx544_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx544_2:
 test rax, rax
 je .Lx544_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx544_7]
 lea rdx, [rip + .Lx544_8]
 jmp rax
.Lx544_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx544_2
.Lx544_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx544_2
.Lx544_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain528_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx544_6]
 sub rsp, 8
 push rax
 jmp xchain528_n5_as
.Lx544_6:
 add rsp, 16
 jmp xchain528_n5_af
 xchain528_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain528_n10_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx545_0
 lea rcx, [rip + .Lx545_4]
 lea rdx, [rip + .Lx545_5]
 jmp rax
.Lx545_4:
 jmp xchain528_n5_as
.Lx545_5:
 jmp xchain528_n5_af
.Lx545_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx545_2:
 test rax, rax
 je .Lx545_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx545_7]
 lea rdx, [rip + .Lx545_8]
 jmp rax
.Lx545_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx545_2
.Lx545_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx545_2
.Lx545_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain528_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx545_6]
 sub rsp, 8
 push rax
 jmp xchain528_n5_as
.Lx545_6:
 add rsp, 16
 jmp xchain528_n5_af
 xchain528_n10_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain528_n11_α:
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx546_0
 lea rcx, [rip + .Lx546_4]
 lea rdx, [rip + .Lx546_5]
 jmp rax
.Lx546_4:
 jmp xchain528_n6_as
.Lx546_5:
 jmp xchain528_n6_af
.Lx546_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx546_2:
 test rax, rax
 je .Lx546_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx546_7]
 lea rdx, [rip + .Lx546_8]
 jmp rax
.Lx546_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx546_2
.Lx546_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx546_2
.Lx546_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain528_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx546_6]
 sub rsp, 8
 push rax
 jmp xchain528_n6_as
.Lx546_6:
 add rsp, 16
 jmp xchain528_n6_af
 xchain528_n11_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain528_n12_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx547_0
 lea rcx, [rip + .Lx547_4]
 lea rdx, [rip + .Lx547_5]
 jmp rax
.Lx547_4:
 jmp xchain528_n6_as
.Lx547_5:
 jmp xchain528_n6_af
.Lx547_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx547_2:
 test rax, rax
 je .Lx547_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx547_7]
 lea rdx, [rip + .Lx547_8]
 jmp rax
.Lx547_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx547_2
.Lx547_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx547_2
.Lx547_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain528_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx547_6]
 sub rsp, 8
 push rax
 jmp xchain528_n6_as
.Lx547_6:
 add rsp, 16
 jmp xchain528_n6_af
 xchain528_n12_β:
 jmp qword ptr [rsp]
proc_PAT$18_res:
add rsp, 8
pop rbp
proc_PAT$18_β:
jmp qword ptr [rbp + 416]
proc_PAT$18_γ:
push rbp
lea rax, [rip + proc_PAT$18_res]
push rax
mov rax, [rbp + 456]
mov rbp, [rbp + 472]
jmp rax
proc_PAT$18_ω:
mov rax, [rbp + 464]
lea rsp, [rbp + 480]
mov rbp, [rbp + 472]
jmp rax
  .globl proc_PAT$19_α
proc_PAT$19_α:
#=======================================================================================================================
    .global proc_PAT$19_α
    .global proc_PAT$19_β
    .global proc_PAT$19_γ
    .global proc_PAT$19_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
proc_PAT$19_α_body:
lea rax, [rip + xchain548_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain548_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain548_n1_α
xchain548_n0_as:
 jmp proc_PAT$19_γ
 xchain548_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain548_n1_β
 jmp xchain548_n2_β
xchain548_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain548_n2_α
 jmp proc_PAT$19_ω
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain548_n1_α:
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx551_0
 lea rcx, [rip + .Lx551_4]
 lea rdx, [rip + .Lx551_5]
 jmp rax
.Lx551_4:
 jmp xchain548_n0_as
.Lx551_5:
 jmp xchain548_n0_af
.Lx551_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_open@PLT
.Lx551_2:
 test rax, rax
 je .Lx551_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx551_7]
 lea rdx, [rip + .Lx551_8]
 jmp rax
.Lx551_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx551_2
.Lx551_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx551_2
.Lx551_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain548_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx551_6]
 sub rsp, 8
 push rax
 jmp xchain548_n0_as
.Lx551_6:
 add rsp, 16
 jmp xchain548_n0_af
 xchain548_n1_β:
 jmp qword ptr [rsp]
# IR_MATCH_SEQ_NARY
 xchain548_n2_α:
 mov dword ptr [rbp + 96], r14d
 mov dword ptr [rbp + 100], 0
 jmp xchain548_n3_α
xchain548_n2_as:
 mov eax, dword ptr [rbp + 100]
 add eax, 1
 mov dword ptr [rbp + 100], eax
 cmp eax, 1
 je xchain548_n4_α
 cmp eax, 2
 je xchain548_n5_α
 jmp xchain548_n0_as
 xchain548_n2_β:
 mov dword ptr [rbp + 100], 3
xchain548_n2_af:
 mov eax, dword ptr [rbp + 100]
 sub eax, 1
 mov dword ptr [rbp + 100], eax
 cmp eax, 0
 je xchain548_n3_β
 cmp eax, 1
 je xchain548_n4_β
 cmp eax, 2
 je xchain548_n5_β
 jmp xchain548_n0_af
# IR_MATCH_LIT
 xchain548_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain548_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain548_n2_af
 add r14d, 1
 jmp xchain548_n2_as
 xchain548_n3_β:
 sub r14d, 1
 jmp xchain548_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain548_n4_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx556_0
 lea rcx, [rip + .Lx556_4]
 lea rdx, [rip + .Lx556_5]
 jmp rax
.Lx556_4:
 jmp xchain548_n2_as
.Lx556_5:
 jmp xchain548_n2_af
.Lx556_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_open@PLT
.Lx556_2:
 test rax, rax
 je .Lx556_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx556_7]
 lea rdx, [rip + .Lx556_8]
 jmp rax
.Lx556_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx556_2
.Lx556_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx556_2
.Lx556_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain548_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx556_6]
 sub rsp, 8
 push rax
 jmp xchain548_n2_as
.Lx556_6:
 add rsp, 16
 jmp xchain548_n2_af
 xchain548_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain548_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain548_n2_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S10]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain548_n2_af
 add r14d, 1
 jmp xchain548_n2_as
 xchain548_n5_β:
 sub r14d, 1
 jmp xchain548_n2_af
proc_PAT$19_res:
add rsp, 8
pop rbp
proc_PAT$19_β:
jmp qword ptr [rbp + 176]
proc_PAT$19_γ:
push rbp
lea rax, [rip + proc_PAT$19_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$19_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "Push"
  .Lstartup_pp0_0: .string "x"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 1
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_Push_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 5776
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname1: .string "Pop"
  .align 8
  .Lstartup_pnames1:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname1]
  lea rsi, [rip + .Lstartup_pnames1]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname1]
  lea rsi, [rip + proc_Pop_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 5776
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname2: .string "Unary"
  .Lstartup_pp2_0: .string "arg"
  .Lstartup_pp2_1: .string "op"
  .align 8
  .Lstartup_pnames2:
  .quad .Lstartup_pp2_0
  .quad .Lstartup_pp2_1
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname2]
  lea rsi, [rip + .Lstartup_pnames2]
  mov edx, 2
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname2]
  lea rsi, [rip + proc_Unary_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 2
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 5776
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname3: .string "Binary"
  .Lstartup_pp3_0: .string "op"
  .Lstartup_pp3_1: .string "left"
  .Lstartup_pp3_2: .string "right"
  .align 8
  .Lstartup_pnames3:
  .quad .Lstartup_pp3_0
  .quad .Lstartup_pp3_1
  .quad .Lstartup_pp3_2
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname3]
  lea rsi, [rip + .Lstartup_pnames3]
  mov edx, 3
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname3]
  lea rsi, [rip + proc_Binary_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 3
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 5776
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname4: .string "EXPR$0"
  .align 8
  .Lstartup_pnames4:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname4]
  lea rsi, [rip + .Lstartup_pnames4]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname4]
  lea rsi, [rip + proc_EXPR$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname5: .string "EXPR$1"
  .align 8
  .Lstartup_pnames5:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname5]
  lea rsi, [rip + .Lstartup_pnames5]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname5]
  lea rsi, [rip + proc_EXPR$1_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname6: .string "EXPR$2"
  .align 8
  .Lstartup_pnames6:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname6]
  lea rsi, [rip + .Lstartup_pnames6]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname6]
  lea rsi, [rip + proc_EXPR$2_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname7: .string "EXPR$3"
  .align 8
  .Lstartup_pnames7:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname7]
  lea rsi, [rip + .Lstartup_pnames7]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname7]
  lea rsi, [rip + proc_EXPR$3_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname8: .string "EXPR$4"
  .align 8
  .Lstartup_pnames8:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname8]
  lea rsi, [rip + .Lstartup_pnames8]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname8]
  lea rsi, [rip + proc_EXPR$4_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname9: .string "EXPR$5"
  .align 8
  .Lstartup_pnames9:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname9]
  lea rsi, [rip + .Lstartup_pnames9]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname9]
  lea rsi, [rip + proc_EXPR$5_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname10: .string "EXPR$6"
  .align 8
  .Lstartup_pnames10:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname10]
  lea rsi, [rip + .Lstartup_pnames10]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname10]
  lea rsi, [rip + proc_EXPR$6_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname11: .string "EXPR$7"
  .align 8
  .Lstartup_pnames11:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname11]
  lea rsi, [rip + .Lstartup_pnames11]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname11]
  lea rsi, [rip + proc_EXPR$7_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname12: .string "EXPR$8"
  .align 8
  .Lstartup_pnames12:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname12]
  lea rsi, [rip + .Lstartup_pnames12]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname12]
  lea rsi, [rip + proc_EXPR$8_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname13: .string "EXPR$9"
  .align 8
  .Lstartup_pnames13:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname13]
  lea rsi, [rip + .Lstartup_pnames13]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname13]
  lea rsi, [rip + proc_EXPR$9_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname14: .string "EXPR$10"
  .align 8
  .Lstartup_pnames14:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname14]
  lea rsi, [rip + .Lstartup_pnames14]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname14]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname14]
  lea rsi, [rip + proc_EXPR$10_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname14]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname14]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname14]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname15: .string "EXPR$11"
  .align 8
  .Lstartup_pnames15:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname15]
  lea rsi, [rip + .Lstartup_pnames15]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname15]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname15]
  lea rsi, [rip + proc_EXPR$11_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname15]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname15]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname15]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname16: .string "EXPR$12"
  .align 8
  .Lstartup_pnames16:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname16]
  lea rsi, [rip + .Lstartup_pnames16]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname16]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname16]
  lea rsi, [rip + proc_EXPR$12_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname16]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname16]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname16]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname17: .string "EXPR$13"
  .align 8
  .Lstartup_pnames17:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname17]
  lea rsi, [rip + .Lstartup_pnames17]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname17]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname17]
  lea rsi, [rip + proc_EXPR$13_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname17]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname17]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname17]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname18: .string "EXPR$14"
  .align 8
  .Lstartup_pnames18:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname18]
  lea rsi, [rip + .Lstartup_pnames18]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname18]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname18]
  lea rsi, [rip + proc_EXPR$14_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname18]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname18]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname18]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname19: .string "EXPR$15"
  .align 8
  .Lstartup_pnames19:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname19]
  lea rsi, [rip + .Lstartup_pnames19]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname19]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname19]
  lea rsi, [rip + proc_EXPR$15_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname19]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname19]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname19]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname20: .string "EXPR$16"
  .align 8
  .Lstartup_pnames20:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname20]
  lea rsi, [rip + .Lstartup_pnames20]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname20]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname20]
  lea rsi, [rip + proc_EXPR$16_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname20]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname20]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname20]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname21: .string "EXPR$17"
  .align 8
  .Lstartup_pnames21:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname21]
  lea rsi, [rip + .Lstartup_pnames21]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname21]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname21]
  lea rsi, [rip + proc_EXPR$17_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname21]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname21]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname21]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname22: .string "EXPR$18"
  .align 8
  .Lstartup_pnames22:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname22]
  lea rsi, [rip + .Lstartup_pnames22]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname22]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname22]
  lea rsi, [rip + proc_EXPR$18_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname22]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname22]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname22]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname23: .string "EXPR$19"
  .align 8
  .Lstartup_pnames23:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname23]
  lea rsi, [rip + .Lstartup_pnames23]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname23]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname23]
  lea rsi, [rip + proc_EXPR$19_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname23]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname23]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname23]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname24: .string "EXPR$20"
  .align 8
  .Lstartup_pnames24:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname24]
  lea rsi, [rip + .Lstartup_pnames24]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname24]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname24]
  lea rsi, [rip + proc_EXPR$20_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname24]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname24]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname24]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname25: .string "EXPR$21"
  .align 8
  .Lstartup_pnames25:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname25]
  lea rsi, [rip + .Lstartup_pnames25]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname25]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname25]
  lea rsi, [rip + proc_EXPR$21_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname25]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname25]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname25]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname26: .string "EXPR$22"
  .align 8
  .Lstartup_pnames26:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname26]
  lea rsi, [rip + .Lstartup_pnames26]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname26]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname26]
  lea rsi, [rip + proc_EXPR$22_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname26]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname26]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname26]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname27: .string "EXPR$23"
  .align 8
  .Lstartup_pnames27:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname27]
  lea rsi, [rip + .Lstartup_pnames27]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname27]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname27]
  lea rsi, [rip + proc_EXPR$23_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname27]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname27]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname27]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname28: .string "EXPR$24"
  .align 8
  .Lstartup_pnames28:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname28]
  lea rsi, [rip + .Lstartup_pnames28]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname28]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname28]
  lea rsi, [rip + proc_EXPR$24_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname28]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname28]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname28]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname29: .string "EXPR$25"
  .align 8
  .Lstartup_pnames29:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname29]
  lea rsi, [rip + .Lstartup_pnames29]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname29]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname29]
  lea rsi, [rip + proc_EXPR$25_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname29]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname29]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname29]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname30: .string "EXPR$26"
  .align 8
  .Lstartup_pnames30:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname30]
  lea rsi, [rip + .Lstartup_pnames30]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname30]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname30]
  lea rsi, [rip + proc_EXPR$26_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname30]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname30]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname30]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname31: .string "EXPR$27"
  .align 8
  .Lstartup_pnames31:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname31]
  lea rsi, [rip + .Lstartup_pnames31]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname31]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname31]
  lea rsi, [rip + proc_EXPR$27_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname31]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname31]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname31]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname32: .string "EXPR$28"
  .align 8
  .Lstartup_pnames32:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname32]
  lea rsi, [rip + .Lstartup_pnames32]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname32]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname32]
  lea rsi, [rip + proc_EXPR$28_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname32]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname32]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname32]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname33: .string "EXPR$29"
  .align 8
  .Lstartup_pnames33:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname33]
  lea rsi, [rip + .Lstartup_pnames33]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname33]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname33]
  lea rsi, [rip + proc_EXPR$29_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname33]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname33]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname33]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname34: .string "EXPR$30"
  .align 8
  .Lstartup_pnames34:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname34]
  lea rsi, [rip + .Lstartup_pnames34]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname34]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname34]
  lea rsi, [rip + proc_EXPR$30_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname34]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname34]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname34]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname35: .string "EXPR$31"
  .align 8
  .Lstartup_pnames35:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname35]
  lea rsi, [rip + .Lstartup_pnames35]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname35]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname35]
  lea rsi, [rip + proc_EXPR$31_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname35]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname35]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname35]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname36: .string "EXPR$32"
  .align 8
  .Lstartup_pnames36:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname36]
  lea rsi, [rip + .Lstartup_pnames36]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname36]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname36]
  lea rsi, [rip + proc_EXPR$32_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname36]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname36]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname36]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname37: .string "EXPR$33"
  .align 8
  .Lstartup_pnames37:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname37]
  lea rsi, [rip + .Lstartup_pnames37]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname37]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname37]
  lea rsi, [rip + proc_EXPR$33_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname37]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname37]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname37]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname38: .string "EXPR$34"
  .align 8
  .Lstartup_pnames38:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname38]
  lea rsi, [rip + .Lstartup_pnames38]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname38]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname38]
  lea rsi, [rip + proc_EXPR$34_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname38]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname38]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname38]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname39: .string "EXPR$35"
  .align 8
  .Lstartup_pnames39:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname39]
  lea rsi, [rip + .Lstartup_pnames39]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname39]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname39]
  lea rsi, [rip + proc_EXPR$35_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname39]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname39]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname39]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname40: .string "EXPR$36"
  .align 8
  .Lstartup_pnames40:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname40]
  lea rsi, [rip + .Lstartup_pnames40]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname40]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname40]
  lea rsi, [rip + proc_EXPR$36_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname40]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname40]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname40]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname41: .string "EXPR$37"
  .align 8
  .Lstartup_pnames41:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname41]
  lea rsi, [rip + .Lstartup_pnames41]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname41]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname41]
  lea rsi, [rip + proc_EXPR$37_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname41]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname41]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname41]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname42: .string "EXPR$38"
  .align 8
  .Lstartup_pnames42:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname42]
  lea rsi, [rip + .Lstartup_pnames42]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname42]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname42]
  lea rsi, [rip + proc_EXPR$38_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname42]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname42]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname42]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname43: .string "EXPR$39"
  .align 8
  .Lstartup_pnames43:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname43]
  lea rsi, [rip + .Lstartup_pnames43]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname43]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname43]
  lea rsi, [rip + proc_EXPR$39_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname43]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname43]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname43]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname44: .string "EXPR$40"
  .align 8
  .Lstartup_pnames44:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname44]
  lea rsi, [rip + .Lstartup_pnames44]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname44]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname44]
  lea rsi, [rip + proc_EXPR$40_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname44]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname44]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname44]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname45: .string "EXPR$41"
  .align 8
  .Lstartup_pnames45:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname45]
  lea rsi, [rip + .Lstartup_pnames45]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname45]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname45]
  lea rsi, [rip + proc_EXPR$41_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname45]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname45]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname45]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname46: .string "EXPR$42"
  .align 8
  .Lstartup_pnames46:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname46]
  lea rsi, [rip + .Lstartup_pnames46]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname46]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname46]
  lea rsi, [rip + proc_EXPR$42_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname46]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname46]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname46]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname47: .string "EXPR$43"
  .align 8
  .Lstartup_pnames47:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname47]
  lea rsi, [rip + .Lstartup_pnames47]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname47]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname47]
  lea rsi, [rip + proc_EXPR$43_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname47]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname47]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname47]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname48: .string "EXPR$44"
  .align 8
  .Lstartup_pnames48:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname48]
  lea rsi, [rip + .Lstartup_pnames48]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname48]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname48]
  lea rsi, [rip + proc_EXPR$44_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname48]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname48]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname48]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname49: .string "EXPR$45"
  .align 8
  .Lstartup_pnames49:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname49]
  lea rsi, [rip + .Lstartup_pnames49]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname49]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname49]
  lea rsi, [rip + proc_EXPR$45_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname49]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname49]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname49]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname50: .string "EXPR$46"
  .align 8
  .Lstartup_pnames50:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname50]
  lea rsi, [rip + .Lstartup_pnames50]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname50]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname50]
  lea rsi, [rip + proc_EXPR$46_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname50]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname50]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname50]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname51: .string "EXPR$47"
  .align 8
  .Lstartup_pnames51:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname51]
  lea rsi, [rip + .Lstartup_pnames51]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname51]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname51]
  lea rsi, [rip + proc_EXPR$47_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname51]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname51]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname51]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname52: .string "EXPR$48"
  .align 8
  .Lstartup_pnames52:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname52]
  lea rsi, [rip + .Lstartup_pnames52]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname52]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname52]
  lea rsi, [rip + proc_EXPR$48_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname52]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname52]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname52]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname53: .string "EXPR$49"
  .align 8
  .Lstartup_pnames53:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname53]
  lea rsi, [rip + .Lstartup_pnames53]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname53]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname53]
  lea rsi, [rip + proc_EXPR$49_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname53]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname53]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname53]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname54: .string "EXPR$50"
  .align 8
  .Lstartup_pnames54:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname54]
  lea rsi, [rip + .Lstartup_pnames54]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname54]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname54]
  lea rsi, [rip + proc_EXPR$50_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname54]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname54]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname54]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname55: .string "EXPR$51"
  .align 8
  .Lstartup_pnames55:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname55]
  lea rsi, [rip + .Lstartup_pnames55]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname55]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname55]
  lea rsi, [rip + proc_EXPR$51_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname55]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname55]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname55]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname56: .string "EXPR$52"
  .align 8
  .Lstartup_pnames56:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname56]
  lea rsi, [rip + .Lstartup_pnames56]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname56]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname56]
  lea rsi, [rip + proc_EXPR$52_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname56]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname56]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname56]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname57: .string "EXPR$53"
  .align 8
  .Lstartup_pnames57:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname57]
  lea rsi, [rip + .Lstartup_pnames57]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname57]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname57]
  lea rsi, [rip + proc_EXPR$53_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname57]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname57]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname57]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname58: .string "EXPR$54"
  .align 8
  .Lstartup_pnames58:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname58]
  lea rsi, [rip + .Lstartup_pnames58]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname58]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname58]
  lea rsi, [rip + proc_EXPR$54_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname58]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname58]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname58]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname59: .string "EXPR$55"
  .align 8
  .Lstartup_pnames59:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname59]
  lea rsi, [rip + .Lstartup_pnames59]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname59]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname59]
  lea rsi, [rip + proc_EXPR$55_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname59]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname59]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname59]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname60: .string "EXPR$56"
  .align 8
  .Lstartup_pnames60:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname60]
  lea rsi, [rip + .Lstartup_pnames60]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname60]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname60]
  lea rsi, [rip + proc_EXPR$56_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname60]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname60]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname60]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname61: .string "EXPR$57"
  .align 8
  .Lstartup_pnames61:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname61]
  lea rsi, [rip + .Lstartup_pnames61]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname61]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname61]
  lea rsi, [rip + proc_EXPR$57_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname61]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname61]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname61]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname62: .string "EXPR$58"
  .align 8
  .Lstartup_pnames62:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname62]
  lea rsi, [rip + .Lstartup_pnames62]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname62]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname62]
  lea rsi, [rip + proc_EXPR$58_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname62]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname62]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname62]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname63: .string "EXPR$59"
  .align 8
  .Lstartup_pnames63:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname63]
  lea rsi, [rip + .Lstartup_pnames63]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname63]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname63]
  lea rsi, [rip + proc_EXPR$59_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname63]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname63]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname63]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname64: .string "EXPR$60"
  .align 8
  .Lstartup_pnames64:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname64]
  lea rsi, [rip + .Lstartup_pnames64]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname64]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname64]
  lea rsi, [rip + proc_EXPR$60_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname64]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname64]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname64]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname65: .string "EXPR$61"
  .align 8
  .Lstartup_pnames65:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname65]
  lea rsi, [rip + .Lstartup_pnames65]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname65]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname65]
  lea rsi, [rip + proc_EXPR$61_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname65]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname65]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname65]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname66: .string "EXPR$62"
  .align 8
  .Lstartup_pnames66:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname66]
  lea rsi, [rip + .Lstartup_pnames66]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname66]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname66]
  lea rsi, [rip + proc_EXPR$62_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname66]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname66]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname66]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname67: .string "EXPR$63"
  .align 8
  .Lstartup_pnames67:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname67]
  lea rsi, [rip + .Lstartup_pnames67]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname67]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname67]
  lea rsi, [rip + proc_EXPR$63_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname67]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname67]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname67]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname68: .string "EXPR$64"
  .align 8
  .Lstartup_pnames68:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname68]
  lea rsi, [rip + .Lstartup_pnames68]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname68]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname68]
  lea rsi, [rip + proc_EXPR$64_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname68]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname68]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname68]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname69: .string "EXPR$65"
  .align 8
  .Lstartup_pnames69:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname69]
  lea rsi, [rip + .Lstartup_pnames69]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname69]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname69]
  lea rsi, [rip + proc_EXPR$65_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname69]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname69]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname69]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname70: .string "EXPR$66"
  .align 8
  .Lstartup_pnames70:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname70]
  lea rsi, [rip + .Lstartup_pnames70]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname70]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname70]
  lea rsi, [rip + proc_EXPR$66_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname70]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname70]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname70]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname71: .string "EXPR$67"
  .align 8
  .Lstartup_pnames71:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname71]
  lea rsi, [rip + .Lstartup_pnames71]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname71]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname71]
  lea rsi, [rip + proc_EXPR$67_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname71]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname71]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname71]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname72: .string "EXPR$68"
  .align 8
  .Lstartup_pnames72:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname72]
  lea rsi, [rip + .Lstartup_pnames72]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname72]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname72]
  lea rsi, [rip + proc_EXPR$68_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname72]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname72]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname72]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname73: .string "EXPR$69"
  .align 8
  .Lstartup_pnames73:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname73]
  lea rsi, [rip + .Lstartup_pnames73]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname73]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname73]
  lea rsi, [rip + proc_EXPR$69_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname73]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname73]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname73]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname74: .string "PAT$0"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname74]
  lea rsi, [rip + proc_PAT$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname74]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname74]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname74]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname75: .string "PAT$1"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname75]
  lea rsi, [rip + proc_PAT$1_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname75]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname75]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname75]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname76: .string "PAT$2"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname76]
  lea rsi, [rip + proc_PAT$2_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname76]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname76]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname76]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname77: .string "PAT$3"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname77]
  lea rsi, [rip + proc_PAT$3_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname77]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname77]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname77]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname78: .string "PAT$4"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname78]
  lea rsi, [rip + proc_PAT$4_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname78]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname78]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname78]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname79: .string "PAT$5"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname79]
  lea rsi, [rip + proc_PAT$5_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname79]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname79]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname79]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname80: .string "PAT$6"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname80]
  lea rsi, [rip + proc_PAT$6_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname80]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname80]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname80]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname81: .string "PAT$7"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname81]
  lea rsi, [rip + proc_PAT$7_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname81]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname81]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname81]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname82: .string "PAT$8"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname82]
  lea rsi, [rip + proc_PAT$8_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname82]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname82]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname82]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname83: .string "PAT$9"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname83]
  lea rsi, [rip + proc_PAT$9_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname83]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname83]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname83]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname84: .string "PAT$10"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname84]
  lea rsi, [rip + proc_PAT$10_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname84]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname84]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname84]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname85: .string "PAT$11"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname85]
  lea rsi, [rip + proc_PAT$11_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname85]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname85]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname85]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname86: .string "PAT$12"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname86]
  lea rsi, [rip + proc_PAT$12_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname86]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname86]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname86]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname87: .string "PAT$13"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname87]
  lea rsi, [rip + proc_PAT$13_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname87]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname87]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname87]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname88: .string "PAT$14"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname88]
  lea rsi, [rip + proc_PAT$14_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname88]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname88]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname88]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname89: .string "PAT$15"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname89]
  lea rsi, [rip + proc_PAT$15_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname89]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname89]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname89]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname90: .string "PAT$16"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname90]
  lea rsi, [rip + proc_PAT$16_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname90]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname90]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname90]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname91: .string "PAT$17"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname91]
  lea rsi, [rip + proc_PAT$17_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname91]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname91]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname91]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname92: .string "PAT$18"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname92]
  lea rsi, [rip + proc_PAT$18_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname92]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname92]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname92]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname93: .string "PAT$19"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname93]
  lea rsi, [rip + proc_PAT$19_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname93]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname93]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname93]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "Push"
  .Lgvan1: .string "x"
  .Lgvan2: .string "Pop"
  .Lgvan3: .string "Unary"
  .Lgvan4: .string "arg"
  .Lgvan5: .string "op"
  .Lgvan6: .string "Binary"
  .Lgvan7: .string "left"
  .Lgvan8: .string "right"
  .Lgvan9: .string "stk"
  .Lgvan10: .string "dummy"
  .Lgvan11: .string "integer"
  .Lgvan12: .string "exponent"
  .Lgvan13: .string "real"
  .Lgvan14: .string "addop"
  .Lgvan15: .string "mulop"
  .Lgvan16: .string "constant"
  .Lgvan17: .string "primary"
  .Lgvan18: .string "factor"
  .Lgvan19: .string "term"
  .Lgvan20: .string "expr"
  .Lgvan21: .string "line"
  .Lgvan22: .string "EXPR$0"
  .Lgvan23: .string "EXPR$1"
  .Lgvan24: .string "EXPR$2"
  .Lgvan25: .string "EXPR$3"
  .Lgvan26: .string "EXPR$4"
  .Lgvan27: .string "EXPR$5"
  .Lgvan28: .string "EXPR$6"
  .Lgvan29: .string "EXPR$7"
  .Lgvan30: .string "EXPR$8"
  .Lgvan31: .string "EXPR$9"
  .Lgvan32: .string "EXPR$10"
  .Lgvan33: .string "EXPR$11"
  .Lgvan34: .string "EXPR$12"
  .Lgvan35: .string "EXPR$13"
  .Lgvan36: .string "EXPR$14"
  .Lgvan37: .string "EXPR$15"
  .Lgvan38: .string "EXPR$16"
  .Lgvan39: .string "EXPR$17"
  .Lgvan40: .string "EXPR$18"
  .Lgvan41: .string "EXPR$19"
  .Lgvan42: .string "EXPR$20"
  .Lgvan43: .string "EXPR$21"
  .Lgvan44: .string "EXPR$22"
  .Lgvan45: .string "EXPR$23"
  .Lgvan46: .string "EXPR$24"
  .Lgvan47: .string "EXPR$25"
  .Lgvan48: .string "EXPR$26"
  .Lgvan49: .string "EXPR$27"
  .Lgvan50: .string "EXPR$28"
  .Lgvan51: .string "EXPR$29"
  .Lgvan52: .string "EXPR$30"
  .Lgvan53: .string "EXPR$31"
  .Lgvan54: .string "EXPR$32"
  .Lgvan55: .string "EXPR$33"
  .Lgvan56: .string "EXPR$34"
  .Lgvan57: .string "EXPR$35"
  .Lgvan58: .string "EXPR$36"
  .Lgvan59: .string "EXPR$37"
  .Lgvan60: .string "EXPR$38"
  .Lgvan61: .string "EXPR$39"
  .Lgvan62: .string "EXPR$40"
  .Lgvan63: .string "EXPR$41"
  .Lgvan64: .string "EXPR$42"
  .Lgvan65: .string "EXPR$43"
  .Lgvan66: .string "EXPR$44"
  .Lgvan67: .string "EXPR$45"
  .Lgvan68: .string "EXPR$46"
  .Lgvan69: .string "EXPR$47"
  .Lgvan70: .string "EXPR$48"
  .Lgvan71: .string "EXPR$49"
  .Lgvan72: .string "EXPR$50"
  .Lgvan73: .string "EXPR$51"
  .Lgvan74: .string "EXPR$52"
  .Lgvan75: .string "EXPR$53"
  .Lgvan76: .string "EXPR$54"
  .Lgvan77: .string "EXPR$55"
  .Lgvan78: .string "EXPR$56"
  .Lgvan79: .string "EXPR$57"
  .Lgvan80: .string "EXPR$58"
  .Lgvan81: .string "EXPR$59"
  .Lgvan82: .string "EXPR$60"
  .Lgvan83: .string "EXPR$61"
  .Lgvan84: .string "EXPR$62"
  .Lgvan85: .string "EXPR$63"
  .Lgvan86: .string "EXPR$64"
  .Lgvan87: .string "EXPR$65"
  .Lgvan88: .string "EXPR$66"
  .Lgvan89: .string "EXPR$67"
  .Lgvan90: .string "EXPR$68"
  .Lgvan91: .string "EXPR$69"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .quad .Lgvan6
  .quad .Lgvan7
  .quad .Lgvan8
  .quad .Lgvan9
  .quad .Lgvan10
  .quad .Lgvan11
  .quad .Lgvan12
  .quad .Lgvan13
  .quad .Lgvan14
  .quad .Lgvan15
  .quad .Lgvan16
  .quad .Lgvan17
  .quad .Lgvan18
  .quad .Lgvan19
  .quad .Lgvan20
  .quad .Lgvan21
  .quad .Lgvan22
  .quad .Lgvan23
  .quad .Lgvan24
  .quad .Lgvan25
  .quad .Lgvan26
  .quad .Lgvan27
  .quad .Lgvan28
  .quad .Lgvan29
  .quad .Lgvan30
  .quad .Lgvan31
  .quad .Lgvan32
  .quad .Lgvan33
  .quad .Lgvan34
  .quad .Lgvan35
  .quad .Lgvan36
  .quad .Lgvan37
  .quad .Lgvan38
  .quad .Lgvan39
  .quad .Lgvan40
  .quad .Lgvan41
  .quad .Lgvan42
  .quad .Lgvan43
  .quad .Lgvan44
  .quad .Lgvan45
  .quad .Lgvan46
  .quad .Lgvan47
  .quad .Lgvan48
  .quad .Lgvan49
  .quad .Lgvan50
  .quad .Lgvan51
  .quad .Lgvan52
  .quad .Lgvan53
  .quad .Lgvan54
  .quad .Lgvan55
  .quad .Lgvan56
  .quad .Lgvan57
  .quad .Lgvan58
  .quad .Lgvan59
  .quad .Lgvan60
  .quad .Lgvan61
  .quad .Lgvan62
  .quad .Lgvan63
  .quad .Lgvan64
  .quad .Lgvan65
  .quad .Lgvan66
  .quad .Lgvan67
  .quad .Lgvan68
  .quad .Lgvan69
  .quad .Lgvan70
  .quad .Lgvan71
  .quad .Lgvan72
  .quad .Lgvan73
  .quad .Lgvan74
  .quad .Lgvan75
  .quad .Lgvan76
  .quad .Lgvan77
  .quad .Lgvan78
  .quad .Lgvan79
  .quad .Lgvan80
  .quad .Lgvan81
  .quad .Lgvan82
  .quad .Lgvan83
  .quad .Lgvan84
  .quad .Lgvan85
  .quad .Lgvan86
  .quad .Lgvan87
  .quad .Lgvan88
  .quad .Lgvan89
  .quad .Lgvan90
  .quad .Lgvan91
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 92
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 92
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
  mov qword ptr [rsp + 5768], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
 xchain559_n0_α:
# BOX IR_CALL TABLE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
  .section .rodata
  .Lrkfn561: .string "TABLE"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn561]
 lea rsi, [rbp + 128]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 cmp eax, 99
 je xchain559_n2_α
 jmp xchain559_n1_α
 xchain559_n0_β:
 jmp xchain559_n2_α
# IR_ASSIGN gva
 xchain559_n1_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052432], rax
 mov qword ptr [1879052440], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain559_n2_α
 xchain559_n1_β:
 jmp xchain559_n2_α
# IR_LIT_STRING
 xchain559_n2_α:
 mov qword ptr [rbp + 2480], 1
 mov rax, qword ptr [rip + .Lx563_0]
 mov qword ptr [rbp + 2488], rax
 jmp xchain559_n3_α
 xchain559_n2_β:
 jmp xchain559_n5_α
.Lx563_0:
 .quad .Lx563_0_s
.Lx563_0_s:
 .string "PAT$0"
 xchain559_n3_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2480] -> [zr+2448]
 mov rax, qword ptr [rbp + 2480]
 mov qword ptr [rbp + 2448], rax
 mov rax, qword ptr [rbp + 2488]
 mov qword ptr [rbp + 2456], rax
  .section .rodata
  .Lrkfn565: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn565]
 lea rsi, [rbp + 2448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2432], rax
 mov qword ptr [rbp + 2440], rdx
 cmp eax, 99
 je xchain559_n5_α
 jmp xchain559_n4_α
 xchain559_n3_β:
 jmp xchain559_n5_α
# IR_ASSIGN gva
 xchain559_n4_α:
 mov rax, qword ptr [rbp + 2432]
 mov rdx, qword ptr [rbp + 2440]
 mov qword ptr [1879052464], rax
 mov qword ptr [1879052472], rdx
 mov qword ptr [rbp + 2416], rax
 mov qword ptr [rbp + 2424], rdx
 jmp xchain559_n5_α
 xchain559_n4_β:
 jmp xchain559_n5_α
# IR_LIT_STRING
 xchain559_n5_α:
 mov qword ptr [rbp + 2576], 1
 mov rax, qword ptr [rip + .Lx567_0]
 mov qword ptr [rbp + 2584], rax
 jmp xchain559_n6_α
 xchain559_n5_β:
 jmp xchain559_n8_α
.Lx567_0:
 .quad .Lx567_0_s
.Lx567_0_s:
 .string "PAT$1"
 xchain559_n6_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2576] -> [zr+2544]
 mov rax, qword ptr [rbp + 2576]
 mov qword ptr [rbp + 2544], rax
 mov rax, qword ptr [rbp + 2584]
 mov qword ptr [rbp + 2552], rax
  .section .rodata
  .Lrkfn569: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn569]
 lea rsi, [rbp + 2544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2528], rax
 mov qword ptr [rbp + 2536], rdx
 cmp eax, 99
 je xchain559_n8_α
 jmp xchain559_n7_α
 xchain559_n6_β:
 jmp xchain559_n8_α
# IR_ASSIGN gva
 xchain559_n7_α:
 mov rax, qword ptr [rbp + 2528]
 mov rdx, qword ptr [rbp + 2536]
 mov qword ptr [1879052480], rax
 mov qword ptr [1879052488], rdx
 mov qword ptr [rbp + 2512], rax
 mov qword ptr [rbp + 2520], rdx
 jmp xchain559_n8_α
 xchain559_n7_β:
 jmp xchain559_n8_α
# IR_LIT_STRING
 xchain559_n8_α:
 mov qword ptr [rbp + 2672], 1
 mov rax, qword ptr [rip + .Lx571_0]
 mov qword ptr [rbp + 2680], rax
 jmp xchain559_n9_α
 xchain559_n8_β:
 jmp xchain559_n11_α
.Lx571_0:
 .quad .Lx571_0_s
.Lx571_0_s:
 .string "PAT$2"
 xchain559_n9_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2672] -> [zr+2640]
 mov rax, qword ptr [rbp + 2672]
 mov qword ptr [rbp + 2640], rax
 mov rax, qword ptr [rbp + 2680]
 mov qword ptr [rbp + 2648], rax
  .section .rodata
  .Lrkfn573: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn573]
 lea rsi, [rbp + 2640]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2624], rax
 mov qword ptr [rbp + 2632], rdx
 cmp eax, 99
 je xchain559_n11_α
 jmp xchain559_n10_α
 xchain559_n9_β:
 jmp xchain559_n11_α
# IR_ASSIGN gva
 xchain559_n10_α:
 mov rax, qword ptr [rbp + 2624]
 mov rdx, qword ptr [rbp + 2632]
 mov qword ptr [1879052496], rax
 mov qword ptr [1879052504], rdx
 mov qword ptr [rbp + 2608], rax
 mov qword ptr [rbp + 2616], rdx
 jmp xchain559_n11_α
 xchain559_n10_β:
 jmp xchain559_n11_α
# IR_LIT_INTEGER
 xchain559_n11_α:
 mov qword ptr [rbp + 2800], 6
 mov rax, qword ptr [rip + .Lx575_0]
 mov qword ptr [rbp + 2808], rax
 jmp xchain559_n12_α
 xchain559_n11_β:
 jmp xchain559_n17_α
.Lx575_0:
 .quad 42
# IR_LIT_STRING
 xchain559_n12_α:
 mov qword ptr [rbp + 2832], 1
 mov rax, qword ptr [rip + .Lx576_0]
 mov qword ptr [rbp + 2840], rax
 jmp xchain559_n13_α
 xchain559_n12_β:
 jmp xchain559_n17_α
.Lx576_0:
 .quad .Lx576_0_s
.Lx576_0_s:
 .string "*EXPR$0"
# IR_LIT_INTEGER
 xchain559_n13_α:
 mov qword ptr [rbp + 2928], 6
 mov rax, qword ptr [rip + .Lx577_0]
 mov qword ptr [rbp + 2936], rax
 jmp xchain559_n14_α
 xchain559_n13_β:
 jmp xchain559_n17_α
.Lx577_0:
 .quad 28
# IR_LIT_STRING
 xchain559_n14_α:
 mov qword ptr [rbp + 2960], 1
 mov rax, qword ptr [rip + .Lx578_0]
 mov qword ptr [rbp + 2968], rax
 jmp xchain559_n15_α
 xchain559_n14_β:
 jmp xchain559_n17_α
.Lx578_0:
 .quad .Lx578_0_s
.Lx578_0_s:
 .string "+-"
 xchain559_n15_α:
# BOX CALL SNO$PBK(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2928] -> [zr+2880]
 mov rax, qword ptr [rbp + 2928]
 mov qword ptr [rbp + 2880], rax
 mov rax, qword ptr [rbp + 2936]
 mov qword ptr [rbp + 2888], rax
# marshal arg1 = producer-box slot [zr+2960] -> [zr+2896]
 mov rax, qword ptr [rbp + 2960]
 mov qword ptr [rbp + 2896], rax
 mov rax, qword ptr [rbp + 2968]
 mov qword ptr [rbp + 2904], rax
  .section .rodata
  .Lbynamefn369: .string "SNO$PBK"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn369]
 lea rsi, [rbp + 2880]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2864], rax
 mov qword ptr [rbp + 2872], rdx
 cmp eax, 99
 je xchain559_n17_α
 jmp xchain559_n16_α
 xchain559_n15_β:
 jmp xchain559_n17_α
 xchain559_n16_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2800] -> [zr+2736]
 mov rax, qword ptr [rbp + 2800]
 mov qword ptr [rbp + 2736], rax
 mov rax, qword ptr [rbp + 2808]
 mov qword ptr [rbp + 2744], rax
# marshal arg1 = producer-box slot [zr+2832] -> [zr+2752]
 mov rax, qword ptr [rbp + 2832]
 mov qword ptr [rbp + 2752], rax
 mov rax, qword ptr [rbp + 2840]
 mov qword ptr [rbp + 2760], rax
# marshal arg2 = producer-box slot [zr+2864] -> [zr+2768]
 mov rax, qword ptr [rbp + 2864]
 mov qword ptr [rbp + 2768], rax
 mov rax, qword ptr [rbp + 2872]
 mov qword ptr [rbp + 2776], rax
  .section .rodata
  .Lbynamefn370: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn370]
 lea rsi, [rbp + 2736]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2720], rax
 mov qword ptr [rbp + 2728], rdx
 cmp eax, 99
 je xchain559_n17_α
 jmp xchain559_n18_α
 xchain559_n16_β:
 jmp xchain559_n17_α
# IR_LIT_INTEGER
 xchain559_n17_α:
 mov qword ptr [rbp + 3088], 6
 mov rax, qword ptr [rip + .Lx581_0]
 mov qword ptr [rbp + 3096], rax
 jmp xchain559_n19_α
 xchain559_n17_β:
 jmp xchain559_n24_α
.Lx581_0:
 .quad 42
# IR_ASSIGN gva
 xchain559_n18_α:
 mov rax, qword ptr [rbp + 2720]
 mov rdx, qword ptr [rbp + 2728]
 mov qword ptr [1879052512], rax
 mov qword ptr [1879052520], rdx
 mov qword ptr [rbp + 2704], rax
 mov qword ptr [rbp + 2712], rdx
 jmp xchain559_n17_α
 xchain559_n18_β:
 jmp xchain559_n17_α
# IR_LIT_STRING
 xchain559_n19_α:
 mov qword ptr [rbp + 3120], 1
 mov rax, qword ptr [rip + .Lx583_0]
 mov qword ptr [rbp + 3128], rax
 jmp xchain559_n20_α
 xchain559_n19_β:
 jmp xchain559_n24_α
.Lx583_0:
 .quad .Lx583_0_s
.Lx583_0_s:
 .string "*EXPR$1"
# IR_LIT_INTEGER
 xchain559_n20_α:
 mov qword ptr [rbp + 3216], 6
 mov rax, qword ptr [rip + .Lx584_0]
 mov qword ptr [rbp + 3224], rax
 jmp xchain559_n21_α
 xchain559_n20_β:
 jmp xchain559_n24_α
.Lx584_0:
 .quad 28
# IR_LIT_STRING
 xchain559_n21_α:
 mov qword ptr [rbp + 3248], 1
 mov rax, qword ptr [rip + .Lx585_0]
 mov qword ptr [rbp + 3256], rax
 jmp xchain559_n22_α
 xchain559_n21_β:
 jmp xchain559_n24_α
.Lx585_0:
 .quad .Lx585_0_s
.Lx585_0_s:
 .string "*/"
 xchain559_n22_α:
# BOX CALL SNO$PBK(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3216] -> [zr+3168]
 mov rax, qword ptr [rbp + 3216]
 mov qword ptr [rbp + 3168], rax
 mov rax, qword ptr [rbp + 3224]
 mov qword ptr [rbp + 3176], rax
# marshal arg1 = producer-box slot [zr+3248] -> [zr+3184]
 mov rax, qword ptr [rbp + 3248]
 mov qword ptr [rbp + 3184], rax
 mov rax, qword ptr [rbp + 3256]
 mov qword ptr [rbp + 3192], rax
  .section .rodata
  .Lbynamefn376: .string "SNO$PBK"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn376]
 lea rsi, [rbp + 3168]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3152], rax
 mov qword ptr [rbp + 3160], rdx
 cmp eax, 99
 je xchain559_n24_α
 jmp xchain559_n23_α
 xchain559_n22_β:
 jmp xchain559_n24_α
 xchain559_n23_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3088] -> [zr+3024]
 mov rax, qword ptr [rbp + 3088]
 mov qword ptr [rbp + 3024], rax
 mov rax, qword ptr [rbp + 3096]
 mov qword ptr [rbp + 3032], rax
# marshal arg1 = producer-box slot [zr+3120] -> [zr+3040]
 mov rax, qword ptr [rbp + 3120]
 mov qword ptr [rbp + 3040], rax
 mov rax, qword ptr [rbp + 3128]
 mov qword ptr [rbp + 3048], rax
# marshal arg2 = producer-box slot [zr+3152] -> [zr+3056]
 mov rax, qword ptr [rbp + 3152]
 mov qword ptr [rbp + 3056], rax
 mov rax, qword ptr [rbp + 3160]
 mov qword ptr [rbp + 3064], rax
  .section .rodata
  .Lbynamefn377: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn377]
 lea rsi, [rbp + 3024]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3008], rax
 mov qword ptr [rbp + 3016], rdx
 cmp eax, 99
 je xchain559_n24_α
 jmp xchain559_n25_α
 xchain559_n23_β:
 jmp xchain559_n24_α
# IR_LIT_INTEGER
 xchain559_n24_α:
 mov qword ptr [rbp + 3376], 6
 mov rax, qword ptr [rip + .Lx588_0]
 mov qword ptr [rbp + 3384], rax
 jmp xchain559_n26_α
 xchain559_n24_β:
 jmp xchain559_n29_α
.Lx588_0:
 .quad 42
# IR_ASSIGN gva
 xchain559_n25_α:
 mov rax, qword ptr [rbp + 3008]
 mov rdx, qword ptr [rbp + 3016]
 mov qword ptr [1879052528], rax
 mov qword ptr [1879052536], rdx
 mov qword ptr [rbp + 2992], rax
 mov qword ptr [rbp + 3000], rdx
 jmp xchain559_n24_α
 xchain559_n25_β:
 jmp xchain559_n24_α
# IR_LIT_STRING
 xchain559_n26_α:
 mov qword ptr [rbp + 3408], 1
 mov rax, qword ptr [rip + .Lx590_0]
 mov qword ptr [rbp + 3416], rax
 jmp xchain559_n27_α
 xchain559_n26_β:
 jmp xchain559_n29_α
.Lx590_0:
 .quad .Lx590_0_s
.Lx590_0_s:
 .string "*EXPR$2"
# IR_VAR
 xchain559_n27_α:
 mov rax, qword ptr [1879052496]
 mov rdx, qword ptr [1879052504]
 mov qword ptr [rbp + 3504], rax
 mov qword ptr [rbp + 3512], rdx
 jmp xchain559_n28_α
 xchain559_n27_β:
 jmp xchain559_n29_α
# IR_VAR
 xchain559_n28_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 mov qword ptr [rbp + 3536], rax
 mov qword ptr [rbp + 3544], rdx
 jmp xchain559_n30_α
 xchain559_n28_β:
 jmp xchain559_n29_α
# IR_LIT_STRING
 xchain559_n29_α:
 mov qword ptr [rbp + 3632], 1
 mov rax, qword ptr [rip + .Lx593_0]
 mov qword ptr [rbp + 3640], rax
 jmp xchain559_n31_α
 xchain559_n29_β:
 jmp xchain559_n34_α
.Lx593_0:
 .quad .Lx593_0_s
.Lx593_0_s:
 .string "PAT$3"
 xchain559_n30_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3504] -> [zr+3456]
 mov rax, qword ptr [rbp + 3504]
 mov qword ptr [rbp + 3456], rax
 mov rax, qword ptr [rbp + 3512]
 mov qword ptr [rbp + 3464], rax
# marshal arg1 = producer-box slot [zr+3536] -> [zr+3472]
 mov rax, qword ptr [rbp + 3536]
 mov qword ptr [rbp + 3472], rax
 mov rax, qword ptr [rbp + 3544]
 mov qword ptr [rbp + 3480], rax
  .section .rodata
  .Lbynamefn384: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn384]
 lea rsi, [rbp + 3456]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3440], rax
 mov qword ptr [rbp + 3448], rdx
 cmp eax, 99
 je xchain559_n29_α
 jmp xchain559_n32_α
 xchain559_n30_β:
 jmp xchain559_n29_α
 xchain559_n31_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3632] -> [zr+3600]
 mov rax, qword ptr [rbp + 3632]
 mov qword ptr [rbp + 3600], rax
 mov rax, qword ptr [rbp + 3640]
 mov qword ptr [rbp + 3608], rax
  .section .rodata
  .Lrkfn596: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn596]
 lea rsi, [rbp + 3600]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3584], rax
 mov qword ptr [rbp + 3592], rdx
 cmp eax, 99
 je xchain559_n34_α
 jmp xchain559_n33_α
 xchain559_n31_β:
 jmp xchain559_n34_α
 xchain559_n32_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3376] -> [zr+3312]
 mov rax, qword ptr [rbp + 3376]
 mov qword ptr [rbp + 3312], rax
 mov rax, qword ptr [rbp + 3384]
 mov qword ptr [rbp + 3320], rax
# marshal arg1 = producer-box slot [zr+3408] -> [zr+3328]
 mov rax, qword ptr [rbp + 3408]
 mov qword ptr [rbp + 3328], rax
 mov rax, qword ptr [rbp + 3416]
 mov qword ptr [rbp + 3336], rax
# marshal arg2 = producer-box slot [zr+3440] -> [zr+3344]
 mov rax, qword ptr [rbp + 3440]
 mov qword ptr [rbp + 3344], rax
 mov rax, qword ptr [rbp + 3448]
 mov qword ptr [rbp + 3352], rax
  .section .rodata
  .Lbynamefn386: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn386]
 lea rsi, [rbp + 3312]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3296], rax
 mov qword ptr [rbp + 3304], rdx
 cmp eax, 99
 je xchain559_n29_α
 jmp xchain559_n35_α
 xchain559_n32_β:
 jmp xchain559_n29_α
# IR_ASSIGN gva
 xchain559_n33_α:
 mov rax, qword ptr [rbp + 3584]
 mov rdx, qword ptr [rbp + 3592]
 mov qword ptr [1879052560], rax
 mov qword ptr [1879052568], rdx
 mov qword ptr [rbp + 3568], rax
 mov qword ptr [rbp + 3576], rdx
 jmp xchain559_n34_α
 xchain559_n33_β:
 jmp xchain559_n34_α
# IR_VAR
 xchain559_n34_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 mov qword ptr [rbp + 3776], rax
 mov qword ptr [rbp + 3784], rdx
 jmp xchain559_n36_α
 xchain559_n34_β:
 jmp xchain559_n37_α
# IR_ASSIGN gva
 xchain559_n35_α:
 mov rax, qword ptr [rbp + 3296]
 mov rdx, qword ptr [rbp + 3304]
 mov qword ptr [1879052544], rax
 mov qword ptr [1879052552], rdx
 mov qword ptr [rbp + 3280], rax
 mov qword ptr [rbp + 3288], rdx
 jmp xchain559_n29_α
 xchain559_n35_β:
 jmp xchain559_n29_α
# IR_LIT_INTEGER
 xchain559_n36_α:
 mov qword ptr [rbp + 3888], 6
 mov rax, qword ptr [rip + .Lx601_0]
 mov qword ptr [rbp + 3896], rax
 jmp xchain559_n38_α
 xchain559_n36_β:
 jmp xchain559_n37_α
.Lx601_0:
 .quad 42
# IR_LIT_STRING
 xchain559_n37_α:
 mov qword ptr [rbp + 4304], 1
 mov rax, qword ptr [rip + .Lx602_0]
 mov qword ptr [rbp + 4312], rax
 jmp xchain559_n39_α
 xchain559_n37_β:
 jmp xchain559_n42_α
.Lx602_0:
 .quad .Lx602_0_s
.Lx602_0_s:
 .string "EXPR$6"
# IR_LIT_STRING
 xchain559_n38_α:
 mov qword ptr [rbp + 3920], 1
 mov rax, qword ptr [rip + .Lx603_0]
 mov qword ptr [rbp + 3928], rax
 jmp xchain559_n40_α
 xchain559_n38_β:
 jmp xchain559_n37_α
.Lx603_0:
 .quad .Lx603_0_s
.Lx603_0_s:
 .string "*EXPR$3"
 xchain559_n39_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4304] -> [zr+4272]
 mov rax, qword ptr [rbp + 4304]
 mov qword ptr [rbp + 4272], rax
 mov rax, qword ptr [rbp + 4312]
 mov qword ptr [rbp + 4280], rax
  .section .rodata
  .Lrkfn605: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn605]
 lea rsi, [rbp + 4272]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 4256], rax
 mov qword ptr [rbp + 4264], rdx
 cmp eax, 99
 je xchain559_n42_α
 jmp xchain559_n41_α
 xchain559_n39_β:
 jmp xchain559_n42_α
# IR_LIT_STRING
 xchain559_n40_α:
 mov qword ptr [rbp + 4000], 1
 mov rax, qword ptr [rip + .Lx606_0]
 mov qword ptr [rbp + 4008], rax
 jmp xchain559_n43_α
 xchain559_n40_β:
 jmp xchain559_n37_α
.Lx606_0:
 .quad .Lx606_0_s
.Lx606_0_s:
 .string "EXPR$4"
# IR_VAR
 xchain559_n41_α:
 mov rax, qword ptr [1879052528]
 mov rdx, qword ptr [1879052536]
 mov qword ptr [rbp + 4336], rax
 mov qword ptr [rbp + 4344], rdx
 jmp xchain559_n44_α
 xchain559_n41_β:
 jmp xchain559_n42_α
# IR_LIT_STRING
 xchain559_n42_α:
 mov qword ptr [rbp + 4864], 1
 mov rax, qword ptr [rip + .Lx608_0]
 mov qword ptr [rbp + 4872], rax
 jmp xchain559_n45_α
 xchain559_n42_β:
 jmp xchain559_n49_α
.Lx608_0:
 .quad .Lx608_0_s
.Lx608_0_s:
 .string "EXPR$10"
 xchain559_n43_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4000] -> [zr+3968]
 mov rax, qword ptr [rbp + 4000]
 mov qword ptr [rbp + 3968], rax
 mov rax, qword ptr [rbp + 4008]
 mov qword ptr [rbp + 3976], rax
  .section .rodata
  .Lrkfn610: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn610]
 lea rsi, [rbp + 3968]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3952], rax
 mov qword ptr [rbp + 3960], rdx
 cmp eax, 99
 je xchain559_n37_α
 jmp xchain559_n46_α
 xchain559_n43_β:
 jmp xchain559_n37_α
 xchain559_n44_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 4256]
 mov rsi, qword ptr [rbp + 4264]
 mov rdx, qword ptr [rbp + 4336]
 mov rcx, qword ptr [rbp + 4344]
 call str_concat_d@PLT
 mov qword ptr [rbp + 4224], rax
 mov qword ptr [rbp + 4232], rdx
 jmp xchain559_n47_α
 xchain559_n44_β:
 jmp xchain559_n42_α
 xchain559_n45_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4864] -> [zr+4832]
 mov rax, qword ptr [rbp + 4864]
 mov qword ptr [rbp + 4832], rax
 mov rax, qword ptr [rbp + 4872]
 mov qword ptr [rbp + 4840], rax
  .section .rodata
  .Lrkfn613: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn613]
 lea rsi, [rbp + 4832]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 4816], rax
 mov qword ptr [rbp + 4824], rdx
 cmp eax, 99
 je xchain559_n49_α
 jmp xchain559_n48_α
 xchain559_n45_β:
 jmp xchain559_n49_α
 xchain559_n46_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3888] -> [zr+3824]
 mov rax, qword ptr [rbp + 3888]
 mov qword ptr [rbp + 3824], rax
 mov rax, qword ptr [rbp + 3896]
 mov qword ptr [rbp + 3832], rax
# marshal arg1 = producer-box slot [zr+3920] -> [zr+3840]
 mov rax, qword ptr [rbp + 3920]
 mov qword ptr [rbp + 3840], rax
 mov rax, qword ptr [rbp + 3928]
 mov qword ptr [rbp + 3848], rax
# marshal arg2 = producer-box slot [zr+3952] -> [zr+3856]
 mov rax, qword ptr [rbp + 3952]
 mov qword ptr [rbp + 3856], rax
 mov rax, qword ptr [rbp + 3960]
 mov qword ptr [rbp + 3864], rax
  .section .rodata
  .Lbynamefn400: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn400]
 lea rsi, [rbp + 3824]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3808], rax
 mov qword ptr [rbp + 3816], rdx
 cmp eax, 99
 je xchain559_n37_α
 jmp xchain559_n50_α
 xchain559_n46_β:
 jmp xchain559_n37_α
# IR_LIT_INTEGER
 xchain559_n47_α:
 mov qword ptr [rbp + 4448], 6
 mov rax, qword ptr [rip + .Lx615_0]
 mov qword ptr [rbp + 4456], rax
 jmp xchain559_n51_α
 xchain559_n47_β:
 jmp xchain559_n42_α
.Lx615_0:
 .quad 42
# IR_VAR
 xchain559_n48_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 mov qword ptr [rbp + 4896], rax
 mov qword ptr [rbp + 4904], rdx
 jmp xchain559_n52_α
 xchain559_n48_β:
 jmp xchain559_n49_α
# IR_LIT_STRING
 xchain559_n49_α:
 mov qword ptr [rbp + 5296], 1
 mov rax, qword ptr [rip + .Lx617_0]
 mov qword ptr [rbp + 5304], rax
 jmp xchain559_n53_α
 xchain559_n49_β:
 jmp xchain559_n61_α
.Lx617_0:
 .quad .Lx617_0_s
.Lx617_0_s:
 .string "TRIM"
 xchain559_n50_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 3776]
 mov rsi, qword ptr [rbp + 3784]
 mov rdx, qword ptr [rbp + 3808]
 mov rcx, qword ptr [rbp + 3816]
 call str_concat_d@PLT
 mov qword ptr [rbp + 3744], rax
 mov qword ptr [rbp + 3752], rdx
 jmp xchain559_n54_α
 xchain559_n50_β:
 jmp xchain559_n37_α
# IR_LIT_STRING
 xchain559_n51_α:
 mov qword ptr [rbp + 4480], 1
 mov rax, qword ptr [rip + .Lx619_0]
 mov qword ptr [rbp + 4488], rax
 jmp xchain559_n55_α
 xchain559_n51_β:
 jmp xchain559_n42_α
.Lx619_0:
 .quad .Lx619_0_s
.Lx619_0_s:
 .string "*EXPR$7"
 xchain559_n52_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 4816]
 mov rsi, qword ptr [rbp + 4824]
 mov rdx, qword ptr [rbp + 4896]
 mov rcx, qword ptr [rbp + 4904]
 call str_concat_d@PLT
 mov qword ptr [rbp + 4784], rax
 mov qword ptr [rbp + 4792], rdx
 jmp xchain559_n56_α
 xchain559_n52_β:
 jmp xchain559_n49_α
# IR_LIT_INTEGER
 xchain559_n53_α:
 mov qword ptr [rbp + 5328], 6
 mov rax, qword ptr [rip + .Lx621_0]
 mov qword ptr [rbp + 5336], rax
 jmp xchain559_n57_α
 xchain559_n53_β:
 jmp xchain559_n61_α
.Lx621_0:
 .quad 1
# IR_LIT_STRING
 xchain559_n54_α:
 mov qword ptr [rbp + 4080], 1
 mov rax, qword ptr [rip + .Lx622_0]
 mov qword ptr [rbp + 4088], rax
 jmp xchain559_n58_α
 xchain559_n54_β:
 jmp xchain559_n37_α
.Lx622_0:
 .quad .Lx622_0_s
.Lx622_0_s:
 .string "EXPR$5"
# IR_LIT_STRING
 xchain559_n55_α:
 mov qword ptr [rbp + 4560], 1
 mov rax, qword ptr [rip + .Lx623_0]
 mov qword ptr [rbp + 4568], rax
 jmp xchain559_n59_α
 xchain559_n55_β:
 jmp xchain559_n42_α
.Lx623_0:
 .quad .Lx623_0_s
.Lx623_0_s:
 .string "EXPR$8"
# IR_LIT_INTEGER
 xchain559_n56_α:
 mov qword ptr [rbp + 5008], 6
 mov rax, qword ptr [rip + .Lx624_0]
 mov qword ptr [rbp + 5016], rax
 jmp xchain559_n60_α
 xchain559_n56_β:
 jmp xchain559_n49_α
.Lx624_0:
 .quad 42
 xchain559_n57_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+5296] -> [zr+5248]
 mov rax, qword ptr [rbp + 5296]
 mov qword ptr [rbp + 5248], rax
 mov rax, qword ptr [rbp + 5304]
 mov qword ptr [rbp + 5256], rax
# marshal arg1 = producer-box slot [zr+5328] -> [zr+5264]
 mov rax, qword ptr [rbp + 5328]
 mov qword ptr [rbp + 5264], rax
 mov rax, qword ptr [rbp + 5336]
 mov qword ptr [rbp + 5272], rax
  .section .rodata
  .Lrkfn626: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn626]
 lea rsi, [rbp + 5248]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 5232], rax
 mov qword ptr [rbp + 5240], rdx
 cmp eax, 99
 je xchain559_n61_α
 jmp xchain559_n61_α
 xchain559_n57_β:
 jmp xchain559_n61_α
 xchain559_n58_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4080] -> [zr+4048]
 mov rax, qword ptr [rbp + 4080]
 mov qword ptr [rbp + 4048], rax
 mov rax, qword ptr [rbp + 4088]
 mov qword ptr [rbp + 4056], rax
  .section .rodata
  .Lrkfn628: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn628]
 lea rsi, [rbp + 4048]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 4032], rax
 mov qword ptr [rbp + 4040], rdx
 cmp eax, 99
 je xchain559_n37_α
 jmp xchain559_n62_α
 xchain559_n58_β:
 jmp xchain559_n37_α
 xchain559_n59_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4560] -> [zr+4528]
 mov rax, qword ptr [rbp + 4560]
 mov qword ptr [rbp + 4528], rax
 mov rax, qword ptr [rbp + 4568]
 mov qword ptr [rbp + 4536], rax
  .section .rodata
  .Lrkfn630: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn630]
 lea rsi, [rbp + 4528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 4512], rax
 mov qword ptr [rbp + 4520], rdx
 cmp eax, 99
 je xchain559_n42_α
 jmp xchain559_n63_α
 xchain559_n59_β:
 jmp xchain559_n42_α
# IR_LIT_STRING
 xchain559_n60_α:
 mov qword ptr [rbp + 5040], 1
 mov rax, qword ptr [rip + .Lx631_0]
 mov qword ptr [rbp + 5048], rax
 jmp xchain559_n64_α
 xchain559_n60_β:
 jmp xchain559_n49_α
.Lx631_0:
 .quad .Lx631_0_s
.Lx631_0_s:
 .string "*EXPR$11"
# IR_VAR
 xchain559_n61_α:
 mov rdi, qword ptr [rip + .Lx632_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rbp + 5376], rax
 mov qword ptr [rbp + 5384], rdx
 jmp xchain559_n65_α
 xchain559_n61_β:
 jmp main_γ
.Lx632_0:
 .quad .Lx632_0_s
.Lx632_0_s:
 .string "INPUT"
 xchain559_n62_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3744] -> [zr+3696]
 mov rax, qword ptr [rbp + 3744]
 mov qword ptr [rbp + 3696], rax
 mov rax, qword ptr [rbp + 3752]
 mov qword ptr [rbp + 3704], rax
# marshal arg1 = producer-box slot [zr+4032] -> [zr+3712]
 mov rax, qword ptr [rbp + 4032]
 mov qword ptr [rbp + 3712], rax
 mov rax, qword ptr [rbp + 4040]
 mov qword ptr [rbp + 3720], rax
  .section .rodata
  .Lbynamefn416: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn416]
 lea rsi, [rbp + 3696]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3680], rax
 mov qword ptr [rbp + 3688], rdx
 cmp eax, 99
 je xchain559_n37_α
 jmp xchain559_n66_α
 xchain559_n62_β:
 jmp xchain559_n37_α
 xchain559_n63_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+4448] -> [zr+4384]
 mov rax, qword ptr [rbp + 4448]
 mov qword ptr [rbp + 4384], rax
 mov rax, qword ptr [rbp + 4456]
 mov qword ptr [rbp + 4392], rax
# marshal arg1 = producer-box slot [zr+4480] -> [zr+4400]
 mov rax, qword ptr [rbp + 4480]
 mov qword ptr [rbp + 4400], rax
 mov rax, qword ptr [rbp + 4488]
 mov qword ptr [rbp + 4408], rax
# marshal arg2 = producer-box slot [zr+4512] -> [zr+4416]
 mov rax, qword ptr [rbp + 4512]
 mov qword ptr [rbp + 4416], rax
 mov rax, qword ptr [rbp + 4520]
 mov qword ptr [rbp + 4424], rax
  .section .rodata
  .Lbynamefn417: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn417]
 lea rsi, [rbp + 4384]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rbp + 4368], rax
 mov qword ptr [rbp + 4376], rdx
 cmp eax, 99
 je xchain559_n42_α
 jmp xchain559_n67_α
 xchain559_n63_β:
 jmp xchain559_n42_α
# IR_LIT_STRING
 xchain559_n64_α:
 mov qword ptr [rbp + 5120], 1
 mov rax, qword ptr [rip + .Lx635_0]
 mov qword ptr [rbp + 5128], rax
 jmp xchain559_n68_α
 xchain559_n64_β:
 jmp xchain559_n49_α
.Lx635_0:
 .quad .Lx635_0_s
.Lx635_0_s:
 .string "EXPR$12"
# IR_ASSIGN gva
 xchain559_n65_α:
 mov rax, qword ptr [rbp + 5376]
 mov rdx, qword ptr [rbp + 5384]
 mov qword ptr [1879052624], rax
 mov qword ptr [1879052632], rdx
 mov qword ptr [rbp + 5360], rax
 mov qword ptr [rbp + 5368], rdx
 jmp xchain559_n69_α
 xchain559_n65_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain559_n66_α:
 mov rax, qword ptr [rbp + 3680]
 mov rdx, qword ptr [rbp + 3688]
 mov qword ptr [1879052576], rax
 mov qword ptr [1879052584], rdx
 mov qword ptr [rbp + 3664], rax
 mov qword ptr [rbp + 3672], rdx
 jmp xchain559_n37_α
 xchain559_n66_β:
 jmp xchain559_n37_α
 xchain559_n67_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 4224]
 mov rsi, qword ptr [rbp + 4232]
 mov rdx, qword ptr [rbp + 4368]
 mov rcx, qword ptr [rbp + 4376]
 call str_concat_d@PLT
 mov qword ptr [rbp + 4192], rax
 mov qword ptr [rbp + 4200], rdx
 jmp xchain559_n70_α
 xchain559_n67_β:
 jmp xchain559_n42_α
 xchain559_n68_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+5120] -> [zr+5088]
 mov rax, qword ptr [rbp + 5120]
 mov qword ptr [rbp + 5088], rax
 mov rax, qword ptr [rbp + 5128]
 mov qword ptr [rbp + 5096], rax
  .section .rodata
  .Lrkfn640: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn640]
 lea rsi, [rbp + 5088]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 5072], rax
 mov qword ptr [rbp + 5080], rdx
 cmp eax, 99
 je xchain559_n49_α
 jmp xchain559_n71_α
 xchain559_n68_β:
 jmp xchain559_n49_α
# IR_VAR
 xchain559_n69_α:
 mov rax, qword ptr [1879052624]
 mov rdx, qword ptr [1879052632]
 mov qword ptr [rbp + 5632], rax
 mov qword ptr [rbp + 5640], rdx
 jmp xchain559_n72_α
 xchain559_n69_β:
 jmp xchain559_n73_α
# IR_LIT_STRING
 xchain559_n70_α:
 mov qword ptr [rbp + 4640], 1
 mov rax, qword ptr [rip + .Lx642_0]
 mov qword ptr [rbp + 4648], rax
 jmp xchain559_n74_α
 xchain559_n70_β:
 jmp xchain559_n42_α
.Lx642_0:
 .quad .Lx642_0_s
.Lx642_0_s:
 .string "EXPR$9"
 xchain559_n71_α:
# BOX CALL SNO$PBC(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+5008] -> [zr+4944]
 mov rax, qword ptr [rbp + 5008]
 mov qword ptr [rbp + 4944], rax
 mov rax, qword ptr [rbp + 5016]
 mov qword ptr [rbp + 4952], rax
# marshal arg1 = producer-box slot [zr+5040] -> [zr+4960]
 mov rax, qword ptr [rbp + 5040]
 mov qword ptr [rbp + 4960], rax
 mov rax, qword ptr [rbp + 5048]
 mov qword ptr [rbp + 4968], rax
# marshal arg2 = producer-box slot [zr+5072] -> [zr+4976]
 mov rax, qword ptr [rbp + 5072]
 mov qword ptr [rbp + 4976], rax
 mov rax, qword ptr [rbp + 5080]
 mov qword ptr [rbp + 4984], rax
  .section .rodata
  .Lbynamefn425: .string "SNO$PBC"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn425]
 lea rsi, [rbp + 4944]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rbp + 4928], rax
 mov qword ptr [rbp + 4936], rdx
 cmp eax, 99
 je xchain559_n49_α
 jmp xchain559_n75_α
 xchain559_n71_β:
 jmp xchain559_n49_α
# IR_MATCH_HEAD
 xchain559_n72_α:
 mov qword ptr [rbp + 5464], rbp
 mov rdi, qword ptr [rbp + 5632]
 mov rsi, qword ptr [rbp + 5640]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 5456], r12
 mov qword ptr [rbp + 5440], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 5432], rax
 mov dword ptr [rbp + 5424], 0
.Lx645_0:
 mov r14d, dword ptr [rbp + 5424]
 jmp xchain559_n76_α
 xchain559_n72_β:
 add dword ptr [rbp + 5424], 1
 mov eax, dword ptr [rbp + 5424]
 cmp eax, r15d
 jg .Lx645_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx645_1
 jmp .Lx645_0
.Lx645_1:
 mov rax, qword ptr [rbp + 5432]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 5440]
 mov r12, qword ptr [rbp + 5456]
 mov rbp, qword ptr [rbp + 5464]
 jmp xchain559_n73_α
# IR_LIT_STRING
 xchain559_n73_α:
 mov qword ptr [rbp + 5728], 1
 mov rax, qword ptr [rip + .Lx646_0]
 mov qword ptr [rbp + 5736], rax
 jmp xchain559_n77_α
 xchain559_n73_β:
 jmp xchain559_n61_α
.Lx646_0:
 .quad .Lx646_0_s
.Lx646_0_s:
 .string "Bad input, try again"
 xchain559_n74_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+4640] -> [zr+4608]
 mov rax, qword ptr [rbp + 4640]
 mov qword ptr [rbp + 4608], rax
 mov rax, qword ptr [rbp + 4648]
 mov qword ptr [rbp + 4616], rax
  .section .rodata
  .Lrkfn648: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn648]
 lea rsi, [rbp + 4608]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 4592], rax
 mov qword ptr [rbp + 4600], rdx
 cmp eax, 99
 je xchain559_n42_α
 jmp xchain559_n78_α
 xchain559_n74_β:
 jmp xchain559_n42_α
 xchain559_n75_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 4784]
 mov rsi, qword ptr [rbp + 4792]
 mov rdx, qword ptr [rbp + 4928]
 mov rcx, qword ptr [rbp + 4936]
 call str_concat_d@PLT
 mov qword ptr [rbp + 4752], rax
 mov qword ptr [rbp + 4760], rdx
 jmp xchain559_n79_α
 xchain559_n75_β:
 jmp xchain559_n49_α
# IR_MATCH_SEQ_NARY
 xchain559_n76_α:
 mov dword ptr [rbp + 5488], r14d
 mov dword ptr [rbp + 5492], 0
 jmp xchain559_n81_α
xchain559_n76_as:
 mov eax, dword ptr [rbp + 5492]
 add eax, 1
 mov dword ptr [rbp + 5492], eax
 cmp eax, 1
 je xchain559_n83_α
 cmp eax, 2
 je xchain559_n84_α
 jmp xchain559_n80_α
 xchain559_n76_β:
 mov dword ptr [rbp + 5492], 3
xchain559_n76_af:
 mov eax, dword ptr [rbp + 5492]
 sub eax, 1
 mov dword ptr [rbp + 5492], eax
 cmp eax, 0
 je xchain559_n82_β
 cmp eax, 1
 je xchain559_n83_β
 cmp eax, 2
 je xchain559_n85_β
 jmp xchain559_n72_β
# IR_ASSIGN global
 xchain559_n77_α:
 mov rsi, qword ptr [rbp + 5728]
 mov rdx, qword ptr [rbp + 5736]
 mov rdi, qword ptr [rip + .Lx652_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 5712], rax
 mov qword ptr [rbp + 5720], rdx
 jmp xchain559_n61_α
 xchain559_n77_β:
 jmp xchain559_n61_α
.Lx652_0:
 .quad .Lx652_0_s
.Lx652_0_s:
 .string "OUTPUT"
 xchain559_n78_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+4192] -> [zr+4144]
 mov rax, qword ptr [rbp + 4192]
 mov qword ptr [rbp + 4144], rax
 mov rax, qword ptr [rbp + 4200]
 mov qword ptr [rbp + 4152], rax
# marshal arg1 = producer-box slot [zr+4592] -> [zr+4160]
 mov rax, qword ptr [rbp + 4592]
 mov qword ptr [rbp + 4160], rax
 mov rax, qword ptr [rbp + 4600]
 mov qword ptr [rbp + 4168], rax
  .section .rodata
  .Lbynamefn432: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn432]
 lea rsi, [rbp + 4144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 4128], rax
 mov qword ptr [rbp + 4136], rdx
 cmp eax, 99
 je xchain559_n42_α
 jmp xchain559_n86_α
 xchain559_n78_β:
 jmp xchain559_n42_α
# IR_LIT_STRING
 xchain559_n79_α:
 mov qword ptr [rbp + 5200], 1
 mov rax, qword ptr [rip + .Lx654_0]
 mov qword ptr [rbp + 5208], rax
 jmp xchain559_n87_α
 xchain559_n79_β:
 jmp xchain559_n49_α
.Lx654_0:
 .quad .Lx654_0_s
.Lx654_0_s:
 .string "EXPR$13"
# IR_MATCH_RELEASE
 xchain559_n80_α:
 mov rax, qword ptr [rbp + 5432]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 5440]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 5488]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx656_1:
 test rax, rax
 je .Lx656_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx656_3]
 lea rdx, [rip + .Lx656_4]
 jmp rax
.Lx656_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx656_1
.Lx656_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx656_1
.Lx656_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 5456]
 mov rbp, qword ptr [rbp + 5464]
 jmp xchain559_n88_α
# IR_LIT_INTEGER
 xchain559_n81_α:
 mov qword ptr [rbp + 5520], 6
 mov rax, qword ptr [rip + .Lx657_0]
 mov qword ptr [rbp + 5528], rax
 jmp xchain559_n82_α
 xchain559_n81_β:
 jmp xchain559_n76_af
.Lx657_0:
 .quad 0
# IR_MATCH_POS
 xchain559_n82_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain559_n76_af
 jmp xchain559_n76_as
 xchain559_n82_β:
 jmp xchain559_n76_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain559_n83_α:
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx659_0
 lea rcx, [rip + .Lx659_4]
 lea rdx, [rip + .Lx659_5]
 jmp rax
.Lx659_4:
 jmp xchain559_n76_as
.Lx659_5:
 jmp xchain559_n76_af
.Lx659_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_open@PLT
.Lx659_2:
 test rax, rax
 je .Lx659_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx659_7]
 lea rdx, [rip + .Lx659_8]
 jmp rax
.Lx659_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx659_2
.Lx659_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx659_2
.Lx659_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain559_n76_af
 mov r14d, eax
 lea rax, [rip + .Lx659_6]
 sub rsp, 8
 push rax
 jmp xchain559_n76_as
.Lx659_6:
 add rsp, 16
 jmp xchain559_n76_af
 xchain559_n83_β:
 jmp qword ptr [rsp]
# IR_LIT_INTEGER
 xchain559_n84_α:
 mov qword ptr [rbp + 5600], 6
 mov rax, qword ptr [rip + .Lx660_0]
 mov qword ptr [rbp + 5608], rax
 jmp xchain559_n85_α
 xchain559_n84_β:
 jmp xchain559_n76_af
.Lx660_0:
 .quad 0
# IR_MATCH_RPOS
 xchain559_n85_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain559_n76_af
 jmp xchain559_n76_as
 xchain559_n85_β:
 jmp xchain559_n76_af
# IR_ASSIGN gva
 xchain559_n86_α:
 mov rax, qword ptr [rbp + 4128]
 mov rdx, qword ptr [rbp + 4136]
 mov qword ptr [1879052592], rax
 mov qword ptr [1879052600], rdx
 mov qword ptr [rbp + 4112], rax
 mov qword ptr [rbp + 4120], rdx
 jmp xchain559_n42_α
 xchain559_n86_β:
 jmp xchain559_n42_α
 xchain559_n87_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+5200] -> [zr+5168]
 mov rax, qword ptr [rbp + 5200]
 mov qword ptr [rbp + 5168], rax
 mov rax, qword ptr [rbp + 5208]
 mov qword ptr [rbp + 5176], rax
  .section .rodata
  .Lrkfn664: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn664]
 lea rsi, [rbp + 5168]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 5152], rax
 mov qword ptr [rbp + 5160], rdx
 cmp eax, 99
 je xchain559_n49_α
 jmp xchain559_n89_α
 xchain559_n87_β:
 jmp xchain559_n49_α
 xchain559_n88_α:
 mov rdi, qword ptr [rip + .Lx666_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx666_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx666_3]
 lea rdx, [rip + .Lx666_4]
 jmp rax
.Lx666_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx666_2
.Lx666_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx666_2
.Lx666_1:
 call rt_faildescr@PLT
.Lx666_2:
 mov qword ptr [rbp + 5680], rax
 mov qword ptr [rbp + 5688], rdx
 cmp eax, 99
 je xchain559_n61_α
 jmp xchain559_n90_α
 xchain559_n88_β:
 jmp xchain559_n61_α
.Lx666_0:
 .quad .Lx666_0_s
.Lx666_0_s:
 .string "Pop"
 xchain559_n89_α:
# BOX CALL SNO$PBALT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+4752] -> [zr+4704]
 mov rax, qword ptr [rbp + 4752]
 mov qword ptr [rbp + 4704], rax
 mov rax, qword ptr [rbp + 4760]
 mov qword ptr [rbp + 4712], rax
# marshal arg1 = producer-box slot [zr+5152] -> [zr+4720]
 mov rax, qword ptr [rbp + 5152]
 mov qword ptr [rbp + 4720], rax
 mov rax, qword ptr [rbp + 5160]
 mov qword ptr [rbp + 4728], rax
  .section .rodata
  .Lbynamefn443: .string "SNO$PBALT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn443]
 lea rsi, [rbp + 4704]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 4688], rax
 mov qword ptr [rbp + 4696], rdx
 cmp eax, 99
 je xchain559_n49_α
 jmp xchain559_n91_α
 xchain559_n89_β:
 jmp xchain559_n49_α
# IR_ASSIGN global
 xchain559_n90_α:
 mov rsi, qword ptr [rbp + 5680]
 mov rdx, qword ptr [rbp + 5688]
 mov rdi, qword ptr [rip + .Lx668_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 5664], rax
 mov qword ptr [rbp + 5672], rdx
 jmp xchain559_n61_α
 xchain559_n90_β:
 jmp xchain559_n61_α
.Lx668_0:
 .quad .Lx668_0_s
.Lx668_0_s:
 .string "OUTPUT"
# IR_ASSIGN gva
 xchain559_n91_α:
 mov rax, qword ptr [rbp + 4688]
 mov rdx, qword ptr [rbp + 4696]
 mov qword ptr [1879052608], rax
 mov qword ptr [1879052616], rdx
 mov qword ptr [rbp + 4672], rax
 mov qword ptr [rbp + 4680], rdx
 jmp xchain559_n49_α
 xchain559_n91_β:
 jmp xchain559_n49_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 5768]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 5768]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
.section .rodata
.S0: .string "0123456789"
.S1: .string "eEdD"
.S2: .string "integer"
.S3: .string "+-"
.S4: .string "epsilon"
.S5: .string "."
.S6: .string "exponent"
.S7: .string "constant"
.S8: .string "("
.S9: .string "expr"
.S10: .string ")"
.text
