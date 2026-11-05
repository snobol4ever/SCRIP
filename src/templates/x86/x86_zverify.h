#ifndef X86_ZVERIFY_H
#define X86_ZVERIFY_H
#include <string>
extern "C++" {
std::string x86_zdp_rbp_omega_at(int port);
std::string x86_zdp_rbp_gamma_at(int port);
std::string x86_zdp_probe_at(int port);
std::string x86_zdp_rbp_at(int port);
std::string x86_zdp_rbp_pre_alpha();
std::string x86_alpha_zchk();
std::string x86_beta_zchk();
std::string x86_step_zchk();
int x86_key_unbacked(int off);
int x86_zop_regime(int off);
const char * x86_zref(int off, int q);
const char * x86_zop(int off, int q, int bump);
std::string x86_zop_base(int off, int q, int bump, const char * base);
int x86_zdp_rbp_on();
std::string x86_zsm_ev(int kind);
std::string x86_zsm_disc(long restore);
}
#endif
