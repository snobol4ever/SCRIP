/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { unsigned short callee; unsigned short role[6]; } x86_argrole_t;
extern const char x86_argrole_blob[];
extern const x86_argrole_t x86_argroles[123];
static inline const char * x86_argrole_str(unsigned short off) { return x86_argrole_blob + off; }
