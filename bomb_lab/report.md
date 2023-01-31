# BombLab Report
## 21307130002 陈通

    利用objdump -d -s -C --no-show-raw-insn ./bomb > bomb.s 得到反汇编代码

## P1 字符串比较1
    解决思路：
    （一） phase_1(char*)
    1. 函数名为phase_1(char*)，可知输入为字符串
    2. 后续调用 string_not_equal(char*, char*) 函数后面看%al是否为0，如果是0就炸。
    3.这些条件可以知道本题是看输入的字符串是否与答案相等，因此只要找到答案存放的位置即可。
    （二）string_not_equal(char*, char*)
    1. 要比较两个字符串，应该一个一个字符比较，那么要找到比较语句：cmp。
    2. 可以看到是-0x1(%rdi,%rax,1) 和 %dl 比较，而%dl存的是(%rsi,%rax,1)。因为%rdi存的是我们的字符串，所以可以猜测%rsi存的就是答案。
    3. 在gdb中输入%rsi存的字符串即为答案
---
## P2 等比数列
    解决思路：
    1. 一上来调用read_six_numbers(char*, int*)，可知要输入6个整数(组成一个数组)
    2. 在gdb中可以看到，一开始(rax)存的是输入的第一个数，先把它赋给%esi，然后将%eci的值和它相乘。
    3. 然后比较这个乘积是否和0x4(rax)，也就是第二个数相等，如果不相等，就炸了。
    4. 如果相等，那么把第二个数字存到%rax里，重复3.的操作，也就是一个循环。
    5. 所以本题的意思就是让我们输入的6个数，前一个数乘上%ecx存的常数，等于后一个数。在gdb中找到%ecx存的数是-3。所以只要输入公比是-3的等比数列即可。
---
## P3 switch
    解决思路：
    1. gdb中可以看到%rsi存的字符串是"%d %c %d",说明是输入一个整数，一个字符，一个整数
    2. 看到jmpq *%rax以及后面有很多跳转语句，可以知道这是一个跳转表
    3. 跳转的每种情况几乎一样，无论选哪种情况，只要输入的字符和数字和那种情况里给的数相同，就能过。
---
## P4 递归
    解决思路：
    （一）phase_4(long)
    1. 由函数名称phase_4(long)可知，输入一个long
    2. 首先取得这个数的高32位，如果它减1大于13，那么就炸了。所以这个数算术右移32位只能小于等于14
    3. 这个数的低32位存在了%eax中。%eax中的数先-1，然后它不能和0xd相等否则会炸。然后调用hope(int)函数。
    4. hope函数出来后的%eax必须等于0x1000000，否则就炸
    （二）hope(int)
    1. hope函数C++代码如下：
    2. 经过尝试一些数，分析一下即可得到答案

``` C++
int hope(int& rdi, int& rax) {
    int r8d = 1;
    int h = rdi; // %edi中存的高32位
    int l = rax; // %eax中存的低32位
    if(h == 0) {
        l = 1;
        return l;
    }
    int ebx = h;
    h /= 2;
    l = hope(h, l);
    r8d = l;
    l *= r8d;
    if(ebx != 0) {
        r8d <<= 2;
    }
    l = r8d;
    return l;
}
```
---
## P5 虚函数
    解决思路：（个人感觉最难的一题）
    （一）phase_5(long, long, long)
    1. 由函数名可知输入三个long
    2. 从一开始到后面的跳转之前，中间这一段（由旁边的注释）可知是定义了三个虚函数的虚表，并为其中的变量(2个整数)和指针分配了空间
    3. 栈空间的分配如下图：

![vtable_stack](C:\Users\Life_Dancer\Desktop\bomb_lab2\vtable_stack.png)

    4. %rdi存的是第一个整数，接下来就是根据这个数进行跳转，调用函数run_lock_test(baselock*, int, int)，跳转代码如下：
```C++
if(x > 1) {
    if(x > 3) {
        if(x == 8) {
            0x234f(%rip) = 0x79;
        }
        run_lock_test(pointer_to_baselock, y, z);
    }
    else {
        run_lock_test(pointer_to_lock1, y, z);
    }
}
else {
    run_lock_test(pointer_to_lock2, y, z);
}
```
    5. 可以猜测，如果%rdi不等于8的话，那行赋值就不会进行，会导致错误，所以第一个数知道了。
    6. 接下来调用的函数都用下面的C++伪代码表示了。可以知道，当第三个数等于4080的时候能过。第二个数任意。

```C++
void run_lock_test(pointer_to_baselock, int& b, int& c) {
    baselock::acquire(b);
    if(eax != 0) {
        baselock::release(int& b, int& c);
    }
    if(eax == 0) {
        explode();
    }
}

int baselock::acquire(int& b) {
    int ebp = b;
    rax = baselock::isholding();
    int r8d = eax;
    eax = 0;
    if(r8d == 0) {
        while(eax != 0) {
            c = 1;
            eax = c;
            swap(eax, b);
        }
        baselock::mem_sync();
        c = b;
        eax = 4080;
    }
    return eax;
}

void baselock::isholding(int& c) {
    int edx = c;
    edx = ~edx;
    int eax = (b==c);
    edx = (edx<0); 
    eax = (edx!=0);//if c>0, then eax=1;
}

int baselock::release(int& b, int& c) {
    int ebp = c;
    baselock::isholding(c);
    if(ebp != 4080 || --al != 0) {
        eax = 0;
        return eax;
    }
    eax = 1;
    rax = ffffffff00000000;
    return rax;
}
```
## P6 字符串比较2
    解决思路：
    1. 由函数名称可知输入字符串
    2. 一开始调用string_len(char*)，先算输入的字符串长度，然后存在%r8d里。如果长度不为6就炸了。
    3. 接下来就是存储我输入的串，把它拼接到原来给定的字符串上。在build_candidate_expression_tree(char*, int)前，%rdi里存的是"(1+2)*(9-0)abcdef"，其中"abcdef"是我输入的字符串。
    4. build_candidate_expression_tree(char*, int)构建我的树
    5. 在compare_answer_and_candidate(tree_node*, tree_node*, int)中，我们不断比较答案字符串和输入字符串，只要不断gdb中输出找到答案字符串即可。

## P7 浮点数
    解决思路：
    1. 先把输入的长整数转换为单精度浮点数，然后翻倍，并加上一个常数10
    2. 然后转换为双精度，它要小于0x471(%rip)存的值3820.0000009999999，还要大于0x46f(%rip)存的值3819.9999990000001
    3. 所以这个数只能是3820，减去一开始加的10，再除以二即可得到输入1905

## 拆弹成功截图

![image-20221013214959842](C:\Users\Life_Dancer\AppData\Roaming\Typora\typora-user-images\image-20221013214959842.png)
