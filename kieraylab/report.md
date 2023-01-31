# Report
## 21307130002 陈通

## 理论部分
### Problem 1
    %rbx, %rbp, %r12, %r13, %r14, %r15

### Problem 2
```
endbr64
movq (%rsp) %rax
movq %rax (%rdi)
xor  %rax %rax
ret
```

### Problem 3
    因为该函数没有任何局部变量，不会改变%rbp的值
    因此不用通过把%rbp压入栈，将其值进行保存

### Problem 4
    除了保存返回地址，只用保存返回值,栈指针以及所有callee-saved的寄存器即可。

### Problem 5
    C++中的try-catch:
    try 块中的代码标识将被激活的特定异常。它后面通常跟着一个或多个 catch 块。
    catch: 在想要处理问题的地方，通过异常处理程序捕获异常。catch 关键字用于捕获异常。
    throw: 当问题出现时，程序会抛出一个异常。这是通过使用 throw 关键字来完成的。

### Problem 6
    在执行try语句之前保存当前的返回值、返回地址以及所有callee-saved的寄存器
    然后执行try代码块中的语句，如果出现异常，进入到catch代码块中
    此时将返回值、返回地址以及所有callee-saved的寄存器恢复成刚刚保存的值
    接着再执行catch代码块中的内容

### Problem 7
    输出：
    0
    1
    2
    3
    4
    5
    6
    7
    8
    9


### Problem 8
    定义一个函数，它会生成一个列表，将其保存，然后调用next函数，最后返回列表中的一个数。
    用寄存器保存初识指向第一个数的指针（或者下标）
    然后每次调用next函数之后，先将返回值设置为这个下标对应的列表中的值，然后下标+1。
    在next函数调用的时候，要判断下标是否超过列表的长度，
    如果超了，可以用try-catch来捕捉异常，输出StopIteration，然后返回到最开始保存的上下文状态。

### Problem 9
    可以设计成类似if-else。
    下面是伪代码
``` C
int error;
// record()函数保存当前上下文，默认返回值为0
if((error = record()) == 0) {
    try ...
    throw(1);
    catch(int x)   recover();  //如果有异常，回到record(),根据异常类型更改error的值
}
else if(error == 1) {
    cout << "StopIteration!";
}
else if(error == 2) {
    cout << "KeyboardInterrupt!";
}
else { 
    // 未处理的异常
    cout << "Unhandled Exception!";
}
```
---
## 实验部分 
### test1 & test2
> 要保存一下返回地址、栈指针以及所有callee-saved寄存器
利用movq将各个值保存起来即可

### test3 & test4
> 每个ctx不仅要保存上述8个8位的内容，还要一个指针，将每个ctx串起来
所以为每个ctx的类型重新定义成一个结构体，分别包含一个数组和一个指针。
根据头插法，对各个ctx连接进链表中，容易实现的
既然try-catch可以根据if-else实现，那么根据观察，catch的表现非常像else
而try不仅有if的操作，还要将上下文进行保存
对于error的clean-up function，根据题意，只要当error值为0的时候进行pop就行

### test5 & test6
> 参考了一下python中generator的源码，可以定义一个flag变量用于send函数中，如果还没开始就执行f。
函数之间的跳转主要是要设置栈指针和返回地址，这个需要改变对应的ctx
一个非常隐蔽的容易出现的bug：当我send函数里利用recover调用函数f的时候，f的参数是recover函数的第二个参数
所以在recover的函数体里面，这个参数是放在%rsi中，要将其移动到%rdi才能正常进行
generator()的初始化函数，已经定义好了自己的栈空间，只要额外定义当前的状态，保存参数即可。
yield函数就是普通的record+recover
send函数要根据不同的函数状态执行不同的操作，其中各步功能已经在源代码中简单注释了
最后根据题目要求，考虑一下当异常处理栈为空的情况即可。
其实感觉各个函数只要根据其含义和功能就能写出函数实现了orz。
---
###结果截图
![image-20221103003051199](C:\Users\Life_Dancer\AppData\Roaming\Typora\typora-user-images\image-20221103003051199.png)
