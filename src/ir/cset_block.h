#ifndef CSET_BLOCK_H
#define CSET_BLOCK_H
#include <stdint.h>
#include <string.h>
#define CSET_BLK_HDR 40
#define CSET_BLK_LEN_OFF (-40)
#define CSET_BLK_MAGIC_OFF (-36)
#define CSET_BLK_BITS_OFF (-32)
#define CSET_BLK_MAGIC 0xC5E7B175u
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void cset_block_header(unsigned char * base, const char * chars, int len) {
    int32_t n = (int32_t) len;
    uint32_t m = CSET_BLK_MAGIC;
    memcpy(base, &n, 4);
    memcpy(base + 4, &m, 4);
    memset(base + 8, 0, 32);
    for (int i = 0; i < len; i++) base[8 + ((unsigned char) chars[i] >> 3)] |= (unsigned char) (1u << ((unsigned char) chars[i] & 7));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline char * cset_block_init(char * base, const char * chars, int len) {
    cset_block_header((unsigned char *) base, chars, len);
    if (len > 0) memcpy(base + CSET_BLK_HDR, chars, (size_t) len);
    base[CSET_BLK_HDR + (len > 0 ? len : 0)] = '\0';
    return base + CSET_BLK_HDR;
}
#endif
