/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ZMQ_ABI_H
#define ZMQ_ABI_H
#include <stddef.h>
typedef struct { unsigned char opaque[64]; } zmq_msg_t;
typedef struct { void *socket; int fd; short events; short revents; } zmq_pollitem_t;
typedef struct {
    void *(*ctx_new)(void);
    int (*ctx_term)(void *ctx);
    void *(*socket)(void *ctx, int type);
    int (*close)(void *s);
    int (*bind)(void *s, const char *addr);
    int (*setsockopt)(void *s, int option, const void *optval, size_t optvallen);
    int (*msg_init)(zmq_msg_t *m);
    int (*msg_init_size)(zmq_msg_t *m, size_t size);
    int (*msg_close)(zmq_msg_t *m);
    void *(*msg_data)(zmq_msg_t *m);
    size_t (*msg_size)(zmq_msg_t *m);
    int (*msg_more)(zmq_msg_t *m);
    int (*msg_recv)(zmq_msg_t *m, void *s, int flags);
    int (*msg_send)(zmq_msg_t *m, void *s, int flags);
    int (*poll)(zmq_pollitem_t *items, int nitems, long timeout);
} zmq_abi_t;
#define ZMQ_PUB 1
#define ZMQ_REP 4
#define ZMQ_ROUTER 6
#define ZMQ_XPUB 9
#define ZMQ_XPUB_VERBOSE 40
#define ZMQ_LINGER 17
#define ZMQ_SNDMORE 2
#define ZMQ_POLLIN 1
#endif
