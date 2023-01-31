# Make Lab 实验报告

21307130002 陈通

## Task 0

1. 运行 `make` ，现象为输出:
> echo hello world
  hello world

2. 运行 `make clean && make`，现象为输出：
> rm -rf /mnt/c/Users/Life_Dancer/Desktop/makelab/build
mkdir /mnt/c/Users/Life_Dancer/Desktop/makelab/build
echo hello world
hello world

3. 将 `Makefile:4 .PHONY: clean all` 改为 `.PHONY: clean all $(OUTPUT_DIR)` ，再次运行 `make`，现象为输出：
> mkdir /mnt/c/Users/Life_Dancer/Desktop/makelab/build
mkdir: cannot create directory ‘/mnt/c/Users/Life_Dancer/Desktop/makelab/build’: File exists
make: *** [Makefile:14: /mnt/c/Users/Life_Dancer/Desktop/makelab/build] Error 1

4. .PHONY的效果和make的工作原理
> `make`首先会寻找该目录中的Makefile文件，然后阅读整个Makefile，看哪些文件是需要更新或建立的目标文件（targets）。如果targets的依赖文件（dependencies）比targets新，那么要先执行相关命令来建立或者更新targets。在 Makefile 中，.PHONY 目标用于指定一个目标列表，这些目标并不是实际的文件或目录，而是用于某种其他用途的“假”目标。默认情况下，`make`假定所有目标都是需要创建或更新的文件或目录。但是，如果一个目标被列为 .PHONY，则 make 不会检查它是否实际存在，而总是运行与该目标关联的命令。

5. 将 all 和 clean 标记为 .PHONY 是否必要？
> 不是必要的。我将.PHONY这一行删掉，运行`make clean && make`,其得到的结果相同。这是因为在目录中没有名为all和clean的文件，用make的时候不会产生歧义，所以并不是必要的。


7. 只输出一个hello world的方法
> 将第八行的`echo hello world`改成`@echo hello world`

## Task1
1. 将 `Makefile:14 mkdir $(OUTPUT_DIR)` 改为 `-mkdir (OUTPUT_DIR)`，然后运行 `make`，结果为
> mkdir /mnt/c/Users/Life_Dancer/Desktop/makelab/build
mkdir: cannot create directory ‘/mnt/c/Users/Life_Dancer/Desktop/makelab/build’: File exists
make: [Makefile:14: /mnt/c/Users/Life_Dancer/Desktop/makelab/build] Error 1 (ignored)
hello world

2. 将 `mkdir` 那一行改为 `mkdir$(OUTPUT_DIR) || true` ，再运行一次 `make` ，结果为：
> mkdir: cannot create directory ‘/mnt/c/Users/Life_Dancer/Desktop/makelab/build’: File exists
hello world

3. 两种方法的差异
> 第二种方法相比第一种方法少了一行告诉你有1个Error但是被ignored。第一种方法能够显式地告诉你有几个错误且是因为被ignored才继续执行的。提供的信息更多，所以觉得第一种方法更好

4. 在忽略错误之后，将 $(OUTPUT_DIR) 标记为 .PHONY 并不影响Makefile 的执行，与原先的 Makefile 相比，你认为哪种实现更优呢？
> 现在的实现更优。如果标记为.PHONY那么无论\$(OUTPUT_DIR)是否存在都会执行相应的命令。现在的实现和原来的实现的差异在于，原来的实现无法告诉我是否\$(OUTPUT_DIR)存在，是执行了相应命令后进行了mkdir，还是原来就存在。使用忽略错误能为我提供更多的信息。

## Task2
1. 运行 `make PART=1` 结果为：
> mkdir /mnt/c/Users/Life_Dancer/Desktop/makelab/build || true
mkdir: cannot create directory ‘/mnt/c/Users/Life_Dancer/Desktop/makelab/build’: 
File exists
make -j -C /mnt/c/Users/Life_Dancer/Desktop/makelab/build -f /mnt/c/Users/Life_Dancer/Desktop/makelab/mk/part1.mk
make[1]: Entering directory '/mnt/c/Users/Life_Dancer/Desktop/makelab/build'     
make[1]: 'main' is up to date.
make[1]: Leaving directory '/mnt/c/Users/Life_Dancer/Desktop/makelab/build' 

2. 修改一下 src/main.cpp ，再次运行 make PART=1，结果为：
> mkdir /mnt/c/Users/Life_Dancer/Desktop/makelab/build || true
mkdir: cannot create directory ‘/mnt/c/Users/Life_Dancer/Desktop/makelab/build’: File exists
make -j -C /mnt/c/Users/Life_Dancer/Desktop/makelab/build -f /mnt/c/Users/Life_Dancer/Desktop/makelab/mk/part1.mk
make[1]: Entering directory '/mnt/c/Users/Life_Dancer/Desktop/makelab/build'
cp /mnt/c/Users/Life_Dancer/Desktop/makelab/src/main.cpp main.cpp
g++  -I/mnt/c/Users/Life_Dancer/Desktop/makelab/include  -c -o main.o main.cpp
main.cpp:4:16: warning: comma-separated list in using-declaration only available with ‘-std=c++17’ or ‘-std=gnu++17’
    4 | using std::cout, std::endl;
      |                ^
g++ -o main A.a.o some.a.o B.b.o main.o
rm main.cpp
make[1]: Leaving directory '/mnt/c/Users/Life_Dancer/Desktop/makelab/build'

3. 改一下 include/shared.h ，再次运行 make PART=1 , 结果为：
> mkdir /mnt/c/Users/Life_Dancer/Desktop/makelab/build || true
mkdir: cannot create directory ‘/mnt/c/Users/Life_Dancer/Desktop/makelab/build’: File exists
make -j -C /mnt/c/Users/Life_Dancer/Desktop/makelab/build -f /mnt/c/Users/Life_Dancer/Desktop/makelab/mk/part1.mk
make[1]: Entering directory '/mnt/c/Users/Life_Dancer/Desktop/makelab/build'
make[1]: 'main' is up to date.
make[1]: Leaving directory '/mnt/c/Users/Life_Dancer/Desktop/makelab/build'

4. 结合 mk/part1.mk 的代码和 make 的工作原理，分析我们的增量编译是如何实现的，并探讨如何处理涉及头文件的增量编译
> mk/part1.mk中首先include根目录下mk文件夹中的config.mk文件，然后在config.mk中定义了变量OUTPUT = main，因此在park1.mk中得以继承，如果main的依赖：A.a.o some.a.o B.b.o main.o有不存在的或者最近被更新的话，启用命令$(CXX) -o $@ $^ 来对所有依赖进行编译。在对其中每个.o文件进行更新的时候看到config.mk中的规则，如果它对应的依赖存在，且最近没有被修改过，那么就直接获得，不再进行编译。这样就实现了只对更改的.c/.cpp文件进行编译，其他的编译。对于头文件，应该在config.mk中后面再添加.c/.cpp与其对应.h的依赖关系，与对应编译命令即可。

## Task 3
1. （1）注释掉 include/shared.h:1 #pragma once ，运行 make clean && make PART=1, 结果为：（只保留了最后error部分）

> make[1]: *** [<builtin>: B.b.o] Error 1
make[1]: *** Waiting for unfinished jobs....
make[1]: *** [<builtin>: main.o] Error 1
rm some.a.cpp main.cpp A.a.cpp B.b.cpp
make[1]: Leaving directory '/mnt/c/Users/Life_Dancer/Desktop/makelab/build'
make: *** [Makefile:10: all] Error 2

（2）说明#pragma once的效果

> 避免一个头文件被include多次，造成重复定义等问题

2. （1）恢复所做的更改，然后删去 `include/shared.h:5 static std::string MassSTR` 中的 `static` ，再运行 `make clean && make PART=1`，结果为：
>  g++ -o main A.a.o some.a.o B.b.o main.o
/usr/bin/ld: some.a.o:(.bss+0x0): multiple definition of `MassSTR[abi:cxx11]'; A.a.o:(.bss+0x0): first defined here
/usr/bin/ld: B.b.o:(.bss+0x0): multiple definition of `MassSTR[abi:cxx11]'; A.a.o:(.bss+0x0): first defined here
/usr/bin/ld: main.o:(.bss+0x0): multiple definition of `MassSTR[abi:cxx11]'; A.a.o:(.bss+0x0): first defined here
collect2: error: ld returned 1 exit status
make[1]: *** [/mnt/c/Users/Life_Dancer/Desktop/makelab/mk/part1.mk:4: main] Error 1
rm some.a.cpp main.cpp A.a.cpp B.b.cpp
make[1]: Leaving directory '/mnt/c/Users/Life_Dancer/Desktop/makelab/build'make: *** [Makefile:10: all] Error 2

（2）借助 `objdump -Ct build/*` 输出的符号表信息static的效果是什么？

> 在有static存在时，用`objdump -Ct build/*` 输出的符号表信息中，有关MassSTR的有这样一行：`0000000000000020 l     O .bss   0000000000000020 MassSTR` 其中 `l` 代表local，说明它只能在该文件内部使用。
而当static不存在时，输出的符号表信息对应的一行是`0000000000000000 g     O .bss   0000000000000020 MassSTR[abi:cxx11]` 其中的`g`代表global，所以在所有文件中都可见，是全局的。这样会导致在多次include的时候，该变量被多次定义。
static保证在头文件中定义的该变量仅会被定义一次，无论这个头文件被包含多少次

3. 为什么四对同名全局变量a b c d 不会发生链接冲突？
> 我使用了`readelf -s build/*` 来输出符号表。在build/A.a.o这个section中，有如下两行：
`67: 0000000000000004     4 OBJECT  GLOBAL DEFAULT  COM a`
`68: 0000000000000000     4 OBJECT  WEAK   DEFAULT    5 b`
1.其中变量a被标记为COM，代表common，说明它在多个文件内被定义，但没有被分配内存，不会发生冲突。
2.其中变量b它的绑定类型是weak，说明它可以被另一个文件中同名的强符号所覆盖或替换。如果没有定义强符号，将使用弱符号。所以也不会冲突
3.在这个section中没有对c的标记。extern int 代表这个变量c是在别的文件里定义的，所以不会链接冲突
在build/main.o这个section中，对于a,b,c,d四个变量有如下四行：
`66: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    5 a`
`67: 0000000000000004     4 OBJECT  GLOBAL DEFAULT    5 b`
`68: 0000000000000008     4 OBJECT  GLOBAL DEFAULT    5 c`
`69: 000000000000000c     4 OBJECT  GLOBAL DEFAULT    5 d`
都是global的类型，外部文件都可访问，所以上述c用的是这里的c
4.在这个section中是有对d的标记的：
`54: 00000000000000a8     4 OBJECT  LOCAL  DEFAULT    6 _ZL1d`
这里可以看到它的Ndx=6，和其它regex_constants的位置一样。因为它被定义成const而且已经被初始化，所以它被视为静态变量，不会造成冲突。


### Task 4
1. 将 `include/shared.h:7 static int LenOfMassSTR()` 中的 `static` 删去，运行 `make clean&& make PART=1`，结果为：（只保留了Error部分）
>  g++ -o main A.a.o some.a.o B.b.o main.o
/usr/bin/ld: some.a.o: in function `LenOfMassSTR()':
some.a.cpp:(.text+0x0): multiple definition of `LenOfMassSTR()'; A.a.o:A.a.cpp:(.text+0x0): first defined here
/usr/bin/ld: B.b.o: in function `LenOfMassSTR()':
B.b.cpp:(.text+0x0): multiple definition of `LenOfMassSTR()'; A.a.o:A.a.cpp:(.text+0x0): first defined here
/usr/bin/ld: main.o: in function `LenOfMassSTR()':
main.cpp:(.text+0x0): multiple definition of `LenOfMassSTR()'; A.a.o:A.a.cpp:(.text+0x0): first defined here
collect2: error: ld returned 1 exit status
make[1]: *** [/mnt/c/Users/Life_Dancer/Desktop/makelab/mk/part1.mk:4: main] Error 1
rm some.a.cpp main.cpp A.a.cpp B.b.cpp
make[1]: Leaving directory '/mnt/c/Users/Life_Dancer/Desktop/makelab/build'
make: *** [Makefile:10: all] Error 2

2. 借助 objdump -Ct build/* 输出的符号表信息,说明用inline能够避免链接冲突的原因。这样做好吗？如果一个函数同时被标记为 static inline 会怎么样？
>1. 在符号表中，有关LenOfMassSTR()的行是：
>   `0000000000000000 w     F .text  0000000000000016 LenOfMassSTR()` 此处标记为weak，这样就能允许多次定义，避免了冲突。
>2.  这样做挺好，内联函数不是在调用时发生控制转移，而是在编译时直接将函数体嵌入在每一个调用处，能够减少冗余的函数声明，减少调用函数时的开销。
>3.  如果一个函数同时被标记为 static inline，通过符号表可以看出它被标记为local，它的作用于就在这个文件当中，不会被其它文件访问，生命周期为整个程序的运行时间，且是内联函数，所以同样能够避免链接冲突的发生。

3. 对于一些小型的工具函数，如我们的 LenOfMassSTR ，一般可以直接定义在头文件中。你认为以上提及的定义方式中哪种最优？
> 用static inline更优，既能将其设定为local避免冲突，又能将其直接嵌入调用处，减少冗余的声明，减少调用函数时的开销。由于它是小型的工具函数，所以对代码量的增加可以不计。static能够维护代码的鲁棒性。

### Task 5
1.  libA.a 中的 A.cpp 和 notA.cpp 间存在链接冲突（同名函数 void A() ），但是他们可以正常编译出静态链接库和可执行程序的原因。
> 在符号表中我们可以看到，在notA.a.o和A.a.o中，A()的内存地址是一样的：0000000000000016。程序当要调用A()函数的时候，首先会搜索与它链接的库，这个搜索顺序是按链接顺序顺序搜索的，所以先链接到哪个文件有A()函数，就将其链接。程序在执行静态链接的时候，维护3个集合：E D U。当发现了可重定位目标文件，就放到E里。将引用了但未定义的符号放在集合U里，将已经定义的符号放进D里。当扫描到静态链接库的时候会看U集合里哪些是在自己里面的，就将其删除，并将对应的可执行文件放入E中。所以先扫描到的先链接。

### Task 6
1. 更改 `mk/part2.mk:3 $(OUTPUT): main.o libB.a libA.a` 中各链接对象的顺序为 `libA.a libB.a main.o`，然后运行`make clean && make PART=2`，结果为：
> 会出现报错：
/usr/bin/ld: main.o: in function `main':
main.cpp:(.text+0x24): undefined reference to `A()'
/usr/bin/ld: main.cpp:(.text+0x29): undefined reference to `B()'
collect2: error: ld returned 1 exit status

2. 更改 `mk/part1.mk:3 $(OUTPUT): A.a.o some.a.o B.b.o main.o` 中各链接对象的顺序为`main.o A.a.o some.a.o B.b.o`，然后运行 `make clean && make PART=1`，结果：
> 没有任何报错，尝试了其他顺序也没有报错

3. 链接对象对链接的影响及原因
> 在链接的过程中，编译器会根据链接对象的顺序一次将每个链接对象的代码添加到应用程序中。如果在添加某个链接对象时，它引用了之前尚未链接的另一个链接对象中的函数或变量，那么就会导致链接错误。

### Task 7
1. 直接在项目根目录下执行 build/main 会发生什么,并结合 objdump -p build/main 输出的动态链接库信息分析原因
> 会发生：build/main: error while loading shared libraries: ./libB.so: cannot open shared object file: No such file or directory
我没用objdump，而是用了`readelf -d build/main`命令，得到的前两行是：
 `0x0000000000000001 (NEEDED)             Shared library: [./libB.so]
 0x0000000000000001 (NEEDED)             Shared library: [./libA.so]` 
可见，运行时需要共享库./libB.so 和 ./libA.so，如果直接在根目录下执行，就没有这两个文件了。

2. 现在应该如何运行这个程序？请查阅一些资料，说明系统是如何查找动态链接库的，并设法在不重新编译的情况下让我们的程序运行起来。
> 我是通过`sudo cp libA.so /usr/lib` 和 `sudo cp libB.so /usr/lib`这两条命令将这两个共享库复制到/usr/lib文件夹中，然后再build文件夹中运行./main命令就可正常运行了。动过动态链接库的信息，我们依旧可以看到main.exe的运行时需要libA.so libB.so这两个动态链接库。程序通常是通过如下顺序查找动态链接库的：1）看是否用到动态链接库的函数和变量，如果用到了且已链接，则不用找了 2）没链接的话看是否提供了指定路径 3）如果没指定路径，系统会查找环境变量指定的位置，比如 Linux 系统中的 LD_LIBRARY_PATH 和 Windows 系统中的 PATH 环境变量。 4）系统会查找系统默认的位置，比如 Linux 系统中的 /usr/lib 和 /usr/local/lib 目录，Windows 系统中的 %WINDIR%\system32 目录。 若还是没找到，就报错了。

### Task8
1. 程序是如何决定执行哪个版本的void A() 呢？
> 根据objdump -Ct build/main输出的符号表信息，可以找到A()的那一行：`0000000000000000       F *UND*  0000000000000000              A()`
可以看出，A()函数是属于未定义的类型，需要在动态链接库中寻找，而寻找的顺序通常由操作系统或编译器决定。某些操作系统或编译器可能会优先搜索程序中最先链接的动态链接库中的函数，而其他则可能会优先搜索程序中最后链接的动态链接库中的函数。

2. （1）改变不同的链接顺序，如改为libA.so libB.so main.o，结果为：
> g++ -shared -fPIC -o libB.so B.b.o notA.a.o
g++ -shared -fPIC -o libA.so A.a.o some.a.o
g++ -o main ./libA.so ./libB.so ./main.o
/usr/bin/ld: ./main.o: in function `main':
main.cpp:(.text+0x24): undefined reference to `A()'
/usr/bin/ld: main.cpp:(.text+0x29): undefined reference to `B()'
collect2: error: ld returned 1 exit status
make[1]: *** [/mnt/c/Users/Life_Dancer/Desktop/makelab/mk/part3.mk:6: main] Error 1
rm some.a.cpp main.cpp notA.a.cpp A.a.cpp B.b.cpp
make[1]: Leaving directory '/mnt/c/Users/Life_Dancer/Desktop/makelab/build'
make: *** [Makefile:10: all] Error 2

（2）Task 6中讨论的规律是否对动态链接库也适用？
> 由于无法调用仍未定义的函数，所以这对动态链接库仍然适用

3. （1）注释掉 `mk/part3.mk:3 CPPFLAGS += -fPIC` 运行 `make clean && make PART=3` ，记录你观察到的现象
> g++ -shared -fPIC -o libB.so B.b.o notA.a.o
g++ -shared -fPIC -o libA.so A.a.o some.a.o
/usr/bin/ld: B.b.o: relocation R_X86_64_PC32 against symbol `_ZSt4cout@@GLIBCXX_3.4' can not be used when making a shared object; recompile with -fPIC
/usr/bin/ld: final link failed: bad value
collect2: error: ld returned 1 exit status
make[1]: *** [/mnt/c/Users/Life_Dancer/Desktop/makelab/mk/part3.mk:12: libB.so] Error 1
make[1]: *** Waiting for unfinished jobs....
/usr/bin/ld: A.a.o: relocation R_X86_64_PC32 against symbol `_ZSt4cout@@GLIBCXX_3.4' can not be used when making a shared object; recompile with -fPIC
/usr/bin/ld: final link failed: bad value
collect2: error: ld returned 1 exit status
make[1]: *** [/mnt/c/Users/Life_Dancer/Desktop/makelab/mk/part3.mk:9: libA.so] Error 1
rm some.a.cpp main.cpp notA.a.cpp A.a.cpp B.b.cpp
make[1]: Leaving directory '/mnt/c/Users/Life_Dancer/Desktop/makelab/build'
make: *** [Makefile:10: all] Error 2

（2）说明-fPIC的作用
> 它的作用是生成位置独立的代码（Position-Independent Code）。这意味着在使用了这个选项编译的代码中，每个函数和全局变量的位置都是可以在运行时确定的。当我们链接库文件的时候，如果库文件中的函数和变量的位置不确定，那么程序在运行时就找不到它们，无法正常工作。