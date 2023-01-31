.text

.globl __ctx_record
__ctx_record:
endbr64
movq (%rsp),%rax
movq %rax,(%rdi)
movq %rsp,8(%rdi)
movq %rbx,16(%rdi)
movq %rbp,24(%rdi)
movq %r12,32(%rdi)
movq %r13,40(%rdi)
movq %r14,48(%rdi)
movq %r15,56(%rdi)
xor %rax,%rax
ret


.globl __ctx_recover
__ctx_recover:
endbr64
movq 8(%rdi),%rsp
movq (%rdi),%rax
movq %rax,(%rsp)
movq 16(%rdi),%rbx
movq 24(%rdi),%rbp
movq 32(%rdi),%r12
movq 40(%rdi),%r13
movq 48(%rdi),%r14
movq 56(%rdi),%r15
xor %rax,%rax
movq %rsi,%rdi
movq %rsi,%rax
ret




