#ifndef X86_ZETA_H
#define X86_ZETA_H
#include <string>
extern "C++" {
std::string x86_zdp_rbp_omega_at(int port);
std::string x86_zdp_rbp_gamma_at(int port);
std::string x86_zdp_probe_at(int port);
std::string x86_zdp_rbp_at(int port);
std::string x86_zdp_rbp_pre_alpha();
std::string x86_alpha_zchk();
std::string x86_beta_zchk();
int x86_key_unbacked(int off);
int x86_zop_regime(int off);
const char * x86_zref(int off, int q);
const char * ZCELL(int w);
const char * ZCELLD(int w);
const char * ZRES_SPINE(int w);
const char * x86_zop(int off, int q, int bump);
const char * ZRES(int w);
const char * ZRESD(int w);
const char * ZOPQ(int k, int w);
const char * ZOPQS(int k, int sp, int w);
const char * ZOPD(int k, int w);
const char * ZRESN();
const char * ZOPN(int k);
const char * HKN(int k);
const char * XSAQ(int d);
const char * XSAD(int d);
int x86_zdp_rbp_on();
std::string x86_zsm_ev(int kind);
std::string x86_zsm_disc(long restore);
}
#endif
