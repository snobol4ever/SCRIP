/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ZMQ_ABI_H
#define ZMQ_ABI_H
#include <stddef.h>
typedef struct { unsigned char opaque[64]; } zmq_msg_t;
typedef struct { void *socket; int fd; short events; short revents; } zmq_pollitem_t;
void *zmq_ctx_new(void);
int zmq_ctx_term(void *ctx);
void *zmq_socket(void *ctx, int type);
int zmq_close(void *s);
int zmq_bind(void *s, const char *addr);
int zmq_setsockopt(void *s, int option, const void *optval, size_t optvallen);
int zmq_msg_init(zmq_msg_t *m);
int zmq_msg_init_size(zmq_msg_t *m, size_t size);
int zmq_msg_close(zmq_msg_t *m);
void *zmq_msg_data(zmq_msg_t *m);
size_t zmq_msg_size(zmq_msg_t *m);
int zmq_msg_more(zmq_msg_t *m);
int zmq_msg_recv(zmq_msg_t *m, void *s, int flags);
int zmq_msg_send(zmq_msg_t *m, void *s, int flags);
int zmq_poll(zmq_pollitem_t *items, int nitems, long timeout);
#define ZMQ_PUB 1
#define ZMQ_REP 4
#define ZMQ_ROUTER 6
#define ZMQ_LINGER 17
#define ZMQ_SNDMORE 2
#define ZMQ_POLLIN 1
#endif
