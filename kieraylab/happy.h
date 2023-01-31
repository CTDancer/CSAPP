#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <string.h>

typedef unsigned long long u64;

struct ctx {
    u64 regs[8];    //reg[8]: [ [rsp](i.e. return address), rsp, rbx, rbp, r12, r13, r14, r15 ]
    u64* nxt_ctx;
};

typedef struct ctx __ctx_type;

struct gen;

#define ERR_THROW0 -1
#define ERR_GENEND -2
#define ERR_GENNIL -3


int __ctx_record(__ctx_type*);
void __ctx_recover(__ctx_type*, int);

void __eh_push(void*);
void* __eh_pop();
void throw(int);
static inline void __eh_check_cleanup(int* error) {
    if(*error == 0) __eh_pop();
}

#define try int error __attribute__((cleanup(__eh_check_cleanup))) = 0; \
            __ctx_type a;\
            __eh_push(&a);\
            error = __ctx_record(&a);\
            if(error == 0)

#define catch else


struct gen* generator(void (*)(int), int);
void genfree(struct gen**);
int yield(int);
int send(struct gen*, int);
#define next(gen) send(gen, 0)
