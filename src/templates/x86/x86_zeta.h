#ifndef X86_ZETA_H
#define X86_ZETA_H
#include <string>
extern "C++" {
const char * ZCELL(int w);
const char * ZCELLD(int w);
const char * ZRES_SPINE(int w);
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
}
#endif
