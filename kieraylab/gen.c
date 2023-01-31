#include "happy.h"

static struct gen
{
    int error;
    int data;
    void* stack;
    __ctx_type ctx;
    struct gen* caller;
    void* eh_list;
    void (*f)(int);
    int flag;   // -1: 结束     0：未开始   1：正在进行
    int arg;    
} __gen_0;
struct gen* __this_gen = &__gen_0;

int yield(int value)
{
    if (__this_gen->caller == NULL)
        throw(ERR_GENNIL);
    __this_gen->data = value;

    // TODO
    if(__ctx_record(&__this_gen->ctx) == 0) {
        __ctx_recover(&__this_gen->caller->ctx, 1);
    }

    return __this_gen->data;
}

void __f() {
    __ctx_recover(&(__this_gen->caller->ctx), -1);
}

int send(struct gen* gen, int value)
{
    if (gen == NULL)
        throw(ERR_GENNIL);
    if (gen->error)
        throw(gen->error);
    if(gen->flag == -1) //生成结束
        throw(ERR_GENEND);
    gen->data = value;

    // TODO
    int status = __ctx_record(&__this_gen->ctx);
    if(status == 0) {
        gen->caller = __this_gen;
        __this_gen = gen;
        if(gen->flag == 1) {
            __ctx_recover(&(gen->ctx), 1);  //已经开始了，就不用调函数f了
        }
        else if(gen->flag == 0) {
            gen->ctx.regs[1] = ((((u64)(gen->stack) + 8016) >> 4) << 4);    //16位对齐
            gen->ctx.regs[1] -= 8;
            *((u64*)gen->ctx.regs[1]) = (u64)__f;   //设置栈帧
            gen->ctx.regs[1] -= 8;  
            gen->ctx.regs[0] = (u64)(gen->f);   //设置返回地址
            gen->flag = 1;
            __ctx_recover(&(gen->ctx), gen->arg);
        }
    }
    else if(status == -1) {     //若已经结束
        gen->error = ERR_GENEND;    
        gen->flag = -1;
    }
    __this_gen = gen->caller;

    if (gen->error) 
        throw(gen->error);

    return gen->data;
}

struct gen* generator(void (*f)(int), int arg)
{
    struct gen* g = malloc(sizeof(*g));
    memset(g, 0, sizeof(*g));
    g->f = f;
    g->stack = malloc(8192);

    // TODO
    g->arg = arg;
    g->flag = 0;    //未开始

    return g;
}

void genfree(struct gen** g)
{
    if (*g == NULL)
        throw(ERR_GENNIL);
    free((*g)->stack);
    free(*g);
    *g = NULL;
}

void __eh_push(void* ctx)
{
    assert(ctx != NULL);

    ((__ctx_type*)ctx)->nxt_ctx = __this_gen->eh_list;
    __this_gen->eh_list = ((__ctx_type*)ctx)->regs;
}

void* __eh_pop()
{
    assert(__this_gen->eh_list != NULL);

    void* ctx = __this_gen->eh_list;
    __this_gen->eh_list = ((__ctx_type*)ctx)->nxt_ctx;
    return ctx;
}

void* __eh_head()
{
    return __this_gen->eh_list;
}

void throw(int error)
{
    if (error == 0)
        throw(ERR_THROW0);
    
    // TODO
    if (__this_gen->eh_list == NULL)
    {
        __this_gen->error = error;
        __ctx_recover(&(__this_gen->caller->ctx), error);
    }
   __ctx_recover(__eh_pop(), error);
    assert(0); // shouldn't run here
}
