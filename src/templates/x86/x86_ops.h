#ifndef X86_OPS_H
#define X86_OPS_H
#include <cstdint>
#include <cstring>
#include <string>
typedef enum {
    XA_MACRO_LIBRARY, XA_MACRO_LIBRARY_OPEN, XA_MACRO_LIBRARY_CLOSE, XA_BB_MACRO_LIBRARY, XA_EXEC_STMT_BLOB, XA_FILE_HEADER, XA_FILE_FOOTER, XA_RODATA_STRTAB, XA_PREDICATE_REGISTRY, XA_BB_PTR_SLOT,
        XA_ENTRY_DISPATCH, XA_FLAT_DATA_SECTION, XA_PATTERN_BLOBS, XA_PROLOGUE, XA_EPILOGUE, XA_EXPRESSION_REGISTRY, XA_KIDS_RODATA, XA_SUB_BUILDER, XA_BUILDER, XA_REGISTRY_TABLE, XA_STRTAB_RODATA,
        XA_CSETTAB_RODATA, XA_CAP_FIXUP, XA_OP_COUNT
} XA_op_t;
extern "C++" {
std::string x86_rec(const char * op);
std::string x86_recn(const char * op);
std::string x86_reclbl(const std::string & nm);
enum { X86P_ALPHA = 0, X86P_BETA = 1, X86P_GAMMA = 2, X86P_OMEGA = 3 };
#define PORT_ALPHA "\xCE\xB1"
#define PORT_BETA "\xCE\xB2"
#define PORT_GAMMA "\xCE\xB3"
#define PORT_OMEGA "\xCF\x89"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int x86_rnum(const char * r);
std::string x86_Lrec(const std::string & b);
std::string x86_Jrec(int port);
std::string x86_Drec(int port);
std::string x86_b1(uint8_t a);
std::string x86_b2(uint8_t a, uint8_t b);
std::string x86_b3(uint8_t a, uint8_t b, uint8_t c);
std::string x86_b4(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_movabs_r64(const char * dst, uint64_t imm);
std::string x86_add(const char * reg, long imm);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_sub(const char * reg, long imm);
std::string x86_alpha();
std::string x86_beta();
std::string x86_beta_trampoline();
std::string x86_gamma();
std::string x86_gamma(const char * mnem);
std::string x86_omega();
std::string x86_omega(const char * mnem);
const char * x86_boxkind(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void x86_begin();
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_or_imm(const char * reg, long imm);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_shift_imm(const char * mnem, int ext, const char * reg, long imm);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * RDQ(const char * base, int off);
const char * RDD(const char * base, int off);
const char * x86_strkeep(const std::string & v);
const char * ROQ(int n);
const char * ABSQ(unsigned long va);
const char * GVARQ(int k, int w);
const char * FR(int off);
const char * FRQ(int off);
const char * FRQB(int off, int bump);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * LFC(int d);
const char * LFDQ(int d, int sd);
const char * LFD(int d, int sd);
const char * AFCQ(int d);
const char * CFC(int d);
const char * FFC(int d);
const char * FFCQ(int d);
const char * LIDX(long k);
long LITQ(long k);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_reg_disp32_load64(const char * dst, const char * base, int disp);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_reg_disp32_store64(const char * base, int disp, const char * src);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_abs_disp32_store64(long va, const char * src);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_reg_disp32_load32(const char * dst, const char * base, int disp);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_reg_disp32_lea64(const char * dst, const char * base, int disp);
struct xop {
    const char * s; uint64_t u; int tag;
    xop() : s(0), u(0), tag(0) {}
    xop(const char * p) : s(p), u(0), tag(p ? 1 : 0) {}
    xop(char * p) : s(p), u(0), tag(p ? 1 : 0) {}
    xop(const std::string & v) : s(x86_strkeep(v)), u(0), tag(1) {}
    xop(int v) : s(0), u((uint64_t)(int64_t)v), tag(2) {}
    xop(long v) : s(0), u((uint64_t)v), tag(2) {}
    xop(long long v) : s(0), u((uint64_t)v), tag(2) {}
    xop(unsigned v) : s(0), u(v), tag(2) {}
    xop(unsigned long v) : s(0), u(v), tag(2) {}
    xop(unsigned long long v): s(0), u(v), tag(2) {}
};
enum { XK_NONE = 0, XK_REG, XK_IMM, XK_PORT, XK_ILBL, XK_FR32, XK_FR64, XK_RSP64, XK_RSP32, XK_MEMIND, XK_MEMIDX8, XK_R13RCX, XK_RIPSEAL, XK_REGDISP, XK_REGDISP32, XK_SYM, XK_ROSLOT, XK_EXTLBL,
    XK_PAIR, XK_ABS64, XK_MEMBI, XK_RIPGOT, XK_RIPCELL };
struct opnd {
    int kind; const char * txt;
    int reg; long imm; int port; int lbl; int off;
    char base[8]; char idx[8]; const char * sym;
};
std::string x86_bomb(const char * msg);
struct x86_rec_t { const char * lb; size_t ll; const char * op; size_t ol; const char * ar; size_t al; int marg; };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_4col(const std::string & s);
#define X86_MEQ(m, lit) ((m)[0] == (lit)[0] && !strcmp((m), (lit)))
std::string x86_core_(const char * mnem, xop xa, xop xb, xop xc, xop xd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86(const char * mnem, xop xa = xop(), xop xb = xop(), xop xc = xop(), xop xd = xop());
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_core_inner_(const char * mnem, xop xa, xop xb, xop xc, xop xd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define X86_IS_PORTMARK(s_, i_, n_) ((s_)[i_] == '\x01' && (i_) + 1 < (n_) && ((i_) == 0 || (s_)[(i_) - 1] == '\n') && (s_)[(i_) + 1] >= '0' && (s_)[(i_) + 1] <= '3')
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int x86_ir_digit(char c);
long x86_ir_num(const std::string & s, size_t & i);
void bb_emit_x86(const std::string & s);
long x86_rec_bytes(const std::string & s);
}
#endif
