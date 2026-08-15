/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef BB_POOL_H
#define BB_POOL_H
#include <stddef.h>
#include <stdint.h>
typedef uint8_t * bb_buf_t;
#define BB_POOL_SIZE   (64 * 1024 * 1024)   /* s91: reserve only (MAP_ANON, pages commit on touch); FLAT_BUF_MAX grew 1MB->4MB for beauty-class mains */
void     bb_pool_init    (void);
int      bb_in_pool      (const void * p);
bb_buf_t bb_alloc        (size_t size);
void     bb_seal         (bb_buf_t buf, size_t size);
void     bb_pool_trim_last(bb_buf_t buf, size_t reserved, size_t used);
void     bb_free         (bb_buf_t buf, size_t size);
void     bb_pool_destroy (void);
void     bb_pool_reset   (void);
size_t   bb_pool_used    (void);
size_t   bb_pool_mark    (void);
void     bb_pool_release (size_t mark);
#endif
