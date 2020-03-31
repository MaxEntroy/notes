[TOC]

## 操作系统(Linux)

### compile and run time

#### 构建(build)

q:什么是构建？
>我们先来看阮一峰的解释：代码变成可执行文件，叫做编译（compile）；先编译这个，还是先编译那个（即编译的安排），叫做构建（build）；本质来说，就是关联到把源文件编译成可执行文件的过程，我们叫做构建。
我们再来看wiki给的解释：Historically, build has often referred either to the process of converting source code files into standalone software artifact(s) that can be run on a computer, or the result of doing so. 简单来说，就是把源文件转换成机器可执行文件的过程，我们叫做构建。

q:编译和构建什么关系？
>构建是把源文件转化成机器可执行程序的过程，编译是构建当中最重要的一步。对于gnu gcc/g++来说，整个构建的过程由proprocessing, compilation, assembly, and linking.


参考<br>
[Make命令教程](http://www.ruanyifeng.com/blog/2015/02/make.html)

- make

q: makefile的基本规则是什么？
>makefile的逻辑特别简单，指定输入，输出，以及输入>输出的过程(规则)
```makefile
<target> : <prerequisites> 
[tab]  <commands>
```

q:.PHONY的作用是什么？
>目标除了可以是文件名，还可以是一个操作。当一个目标是一个操作时，我们称这个目标是一个伪目标。但是，如果一个Makefile存在两个同名目标，前者是文件，后者是操作。此时，操作就不会生效，因为存在一个文件。此时，如果用.PHONY对这个目标进行声明，make就不会检查是否存在这个名称的文件，把这个目标就当做一个操作。简言之，对于伪目标对象，不进行构建，只执行操作。

q:如果前置条件的某个文件进行了更新，make是否能感知到，进行重新构建？
>可以。make会做如下检查：如果前置文件的last-modification时间戳比目标的时间戳新，目标就需要重新构建

参考<br>
[Make 命令教程](http://www.ruanyifeng.com/blog/2015/02/make.html)

#### 高级语言的compile/link/load
这一小节主要写一篇读书笔记，总结一些linux下的基本概念

- 引言

q:既然java/python等高级语言，运行效率低，为什么还可以接受？
>当前，服务器的计算能力已经不是瓶颈，cpu/内存/磁盘的性能都教以前有了很大提升，即目前机器的性能提升非常大。可能以前运行效率低跑不了的代码，在现在的机器上可以跑了，并且目前机器的价格比30-40年前便宜了太多，所以性能不够，机器来凑也成为可能。所以，大家可以不用那么关心代码的运行效率，而把重心放到开发效率上。

q:vm或者interpeter在client和os的层次上，处于一种什么样的关系
>vm或者interpreter处于client和os的中间层次，避免了client代码和os直接进行交互，这样也可以让程序员把工作尽可能的多放在client代码上，而不是放到和os交互的一些细节上。提升了开发效率。


- cpu体系结构

q:当我们说cpu体系结构的时候，我们在说什么？
>有一个确定的方面，就是指令集。指令集有cisc和risc区分，x86_64属于cisc指令集，ibm的power pc系列则是risc.指令集显然会涉及到指令的组成，即computing set由哪些instruction组成，毕竟任何代码最终
还是转换到computing set当中的一条条instruction，然后cpu进行执行，显然，cpu的设计也是基于指令集的，因为不同指令集的指令长度以及基本组成都可能不同。

q:我们在一种cpu体系结构的编译产物可以在另一种cpu体系结构下运行吗？
>显然不行，假设os一样，虽然源代码一样，但是编译产物最终是该cpu体系结构下的computing set当中的instruction，上文已经提到，不同cpu体系computing set不同，instruction不同。
某一个平台的instruction显然无法在另一个平台执行。

- os

q:不同os的编译产物，可以在彼此系统上运行吗？
>不行。编译产物，是二进制可执行程序。不同os对于binary的文件格式不同。比如，linux下是elf，win下是exe.那么，这个**binary文件格式**又是什么呢？
说一个我的简单理解，elf里面规定了程序的入口地址，以及text,bss段的地址，即一个binary装入内存之后，我们怎么能知道入口在哪里？程序需要的变量和函数又去哪里找？elf就是告诉我们这些内容。
当然，Linux进程空间的布局，我理解应该就是elf规定的。所以，linux生成的binary，在windows下肯定找不到，因为入口地址，大家规定的不一样，linux binary当中，elf会指定一个入口地址。
类似的，win下相关格式也会指定一个入口地址，既然大家不是一种格式，这个入口地址没有协议保证一样，肯定无法执行正确。

- c源文件

q:c当中的声明和定义有什么区别？
>声明一个变量/函数：1.这个变量不在当前文件中定义 2.这个变量当前文件会用到 3.基于1和2，请compiler不要报错
定义一个变量：1.生成的目标文件中，请为该变量分配空间 2.如果该变量有初值，请compiler保存
定义一个函数：1.生成的目标文件中，生成该函数的代码

- 目标文件生成/编译

q:编译器需要做哪些工作？
>1.source code都是字符串，显然无法被机器执行。显然，把这些代码编译成binary是必须的。
2.支持指令跳转(函数调用)为函数定义标签，每个标签关联函数入口地址。
3.支持变量访问(读取变量数据)为变量定义标签，每个变量关联变量所在内存地址。

```c
int g_a = 1;
int g_b;

static int g_c = 1;
static int g_d;

extern int g_e; // 如果声明，必须这么操作 默认全局变量也是extern属性，但是在另一个文件当中进行声明，则需要extern 声明

extern int sub();
int sub1();

int sum(int a, int b) {return a + b;}

int main() {
    int a;
    int b = 1;

    static int c;
    static int d = 1;

    sum(3,4);
    return 0;
}
/*
0000000000000004 b c.1736       bss
0000000000000008 d d.1737       data
0000000000000000 D g_a          initialized data section
0000000000000004 C g_b          common
0000000000000004 d g_c          initialized data section
0000000000000000 b g_d          uninitialized data section, bss
0000000000000014 T main
0000000000000000 T sum

a和b没有，是因为局部变量。放在stack里，不存在于这些段。
g_e不在本文件中声明，不存在symbol的值和类型

为什么stack变量就不显示，也值得考虑，因为程序毕竟没有开始执行，这个是我的理解。但是，其余数据，应该是程序开始前，就要确定的。因为他们的生存期是整个程序运行期间。
*/
```
这里补充到一点，变量的作用域和生存期。
>局部变量：
1.作用域：定义该变量的块
2.生存期：执行到块中该变量处，分配内存单元。块结束，变量所占内存单元被回收

>全局变量：
1.作用域：本文件从开始定义处到结束。外部文件，需要用extern声明，则是声明处到结束。
2.生存期：整个程序运行期间
逻辑上是这么理解，但是实现上，一定要结合linux进程内存布局来看。

- 目标文件链接

我们来看如下代码
```c
// main.c
int g_a = 1;
int g_b;

static int g_c = 1;
static int g_d;

extern int g_e; // 如果声明，必须这么操作 默认全局变量也是extern属性，但是在另一个文件当中进行声明，则需要extern 声明

extern int sub();
int sub1();

int sum(int a, int b) {return a + b;}

int main() {
    int a;
    int b = 1;

    static int c;
    static int d = 1;

    g_c = g_d + g_e;

    sum(3,4);
    return 0;
}
/*
0000000000000004 b c.1736
0000000000000008 d d.1737
0000000000000000 D g_a
0000000000000004 C g_b
0000000000000004 d g_c
0000000000000000 b g_d
                 U g_e
0000000000000014 T main
0000000000000000 T sum
*/
```
执行如下编译命令
```c
kang@ubuntu:~/tmp/c-test/compile-link-load-test$ gcc main.o -o main
main.o：在函数‘main’中：
main.c:(.text+0x2b)：对‘g_e’未定义的引用
collect2: error: ld returned 1 exit status
```
我们定义其他源文件，增加对该变量的定义。
```c
// test.c
int g_e = 1;
/*
0000000000000000 D g_e
*/
```
我们观察到一个很有意思的现象：g_a与g_e的段内地址都是0x0000000000000000，这当然也好理解，不同的目标文件，有各自的相对地址，对于各个段都是一样。
我们来进行link操作：gcc test.o main.o -o main，并用nm观察symbol list in object files
```c
/*
0000000000601048 B __bss_start
0000000000601050 b c.1736
0000000000601048 b completed.6982
0000000000601044 d d.1737
0000000000601028 D __data_start
0000000000601028 W data_start
0000000000400430 t deregister_tm_clones
00000000004004a0 t __do_global_dtors_aux
0000000000600e18 t __do_global_dtors_aux_fini_array_entry
0000000000601030 D __dso_handle
0000000000600e28 d _DYNAMIC
0000000000601048 D _edata
0000000000601058 B _end
00000000004005b4 T _fini
00000000004004c0 t frame_dummy
0000000000600e10 t __frame_dummy_init_array_entry
0000000000400710 r __FRAME_END__
000000000060103c D g_a
0000000000601054 B g_b
0000000000601040 d g_c
000000000060104c b g_d
0000000000601038 D g_e
0000000000601000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
00000000004003a8 T _init
0000000000600e18 t __init_array_end
0000000000600e10 t __init_array_start
00000000004005c0 R _IO_stdin_used
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
0000000000600e20 d __JCR_END__
0000000000600e20 d __JCR_LIST__
                 w _Jv_RegisterClasses
00000000004005b0 T __libc_csu_fini
0000000000400540 T __libc_csu_init
                 U __libc_start_main@@GLIBC_2.2.5
0000000000400501 T main
0000000000400460 t register_tm_clones
0000000000400400 T _start
00000000004004ed T sum
0000000000601048 D __TMC_END__
*/
```

q:link的作用是什么？
>1.对各个目标文件中没有定义的变量(symbol)，在其他文件中寻找相关定义保证编译通过，生成binary
2.把不同目标文件当中的变量，根据其所在段，进行合并
3.经过2的合并，把不同文件当中的变量，进行重定位，生成绝对地址

- binary load and run

q:shared object/archive 有什么区别
>在对库的概念无异议的前提下，这二者的区别主要来自于linking阶段对于库的两种处理方式差异，所带来的区别。
.a: 以static linking进行链接的库称为.a. static linking是指，将assembler生成的.a与引用到的库，在linking阶段，一起打包到binary当中。
.so: 以dynamic linking进行链接的库称为.so. dynamic linking是指，compile time并不会把库打包到binary当中，而是在run time时，才会加载到进程空间。

q:有了.a为什么还要.so
>通过上面的介绍，我们知道.a使用方便，达到了库的复用。但是有一个问题，如果有多个client使用到了同一个.a，那么显然，内存当中必然会有多份.a的代码。因为每一个client binary都包含一份.a的副本。
显然，空间的浪费是.a的一个缺点。
.a的另一个问题是，.a的更新会影响到client binary。试想如果.a进行了更新，所有使用他的client binary都需要重新进行编译，从而会带来重新发布。
以上总结了.a的两个缺点。
>
>显然，.so就是为了解决.a的这2个缺点诞生的，
1..so是在run time才进行load，所以，client binary当中并没有.so副本，这样，不同的client binary可以共享同一个.so，从而减轻了.a重复占用空间带来的空间浪费的问题，即.so在内存中只有一份副本。
2..so由于不在client binary当中，相当于是和client binary解耦。.so的更新，不会影响到client binary，因此不用重新编译，也不用重新上线。

补充：
1..o和.a在link阶段可以链接到一个binary当中，证明他们有类似的elf格式，即.a和.o都属于可重定位文件。2..out自然是可执行文件。3..so就是共享目标文件

参考<br>
[高级语言的编译：链接及装载过程介绍](https://tech.meituan.com/2015/01/22/linker.html?from=timeline&isappinstalled=0)<br>
[Linux elf格式](http://www.iloveandroid.net/2015/11/17/studyElf/)<br>
[ELF(Executable and Linkable Format)](https://www.cnblogs.com/littlehann/p/3871092.html)<br>
[Linux 中的动态链接库和静态链接库是干什么的？](https://www.zhihu.com/question/20484931)

#### 编译问题集锦

- compile time符号找不到

文件结构如下
```c
├── log.c
├── log.h
├── main.c
├── Makefile
└── test_hello.conf
```

Makefile文件如下
```makefile
CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

DYNAMIC_ROOT= /usr/local/thirdparty/zlog-1.2
STATIC_ROOT= .

MYCFLAGS= -I$(DYNAMIC_ROOT)/include
MYLDFLAGS= -L$(DYNAMIC_ROOT)/lib -Wl,-rpath=$(DYNAMIC_ROOT)/lib
MYLIBS= -lzlog -lpthread

AR= ar rcu
RANLIB= ranlib
RM= rm -f

MAIN_A= libmain.a
BASE_O= log.o

MAIN_T= main
MAIN_O= main.o

# Targets start here
a: $(MAIN_A)

t: $(MAIN_T)

$(MAIN_A): $(BASE_O)
	$(AR) $@ $(BASE_O)
	$(RANLIB) $@

$(MAIN_T): $(MAIN_O) $(MAIN_A)
	$(CC) -o $@ $(LDFLAGS) $(MAIN_O) $(MAIN_A) $(LIBS)

echo:
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "AR= $(AR)"
	@echo "RANLIB= $(RANLIB)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(MAIN_A) $(BASE_O) $(MAIN_T) $(MAIN_O)

.PHONY: a t echo clean

main.o: main.c log.h
log.o: log.c log.h
```

q:代码是如何组织的？
>这种写法是按照lua-5.3.0的Makefile写法。对于所有文件，除了main.c之外，其余文件编译时打成一个.a
最后，main.o main.a一起编译。

q:本次的特点是什么?
>本次的main.a用到了一个.so，所以，
正常.a的写法，应该是想办法把.so->.a，装入到main.a当中。对于client代码来说，第三方库的依赖对我应该是透明的，不应该关心。这是正常的写法。
但是，这里特殊的在于，我只是用了把其他.c/.cc打成.a的形式，提供给main.o，即这个第三方库肯定是client作者打成的，他能知道.a的依赖，所以，可以直接链.so

总结，compile time符号一般找不到的原因就是，.h和.c当中的实现没对上。所以，.h提供的符号，在.c->.o(.a/.so)当中，找不到。


- 动态库链接静态库

```
问题描述：
g++ -fPIC -shared -o other.so other_lib.cc
编译没问题，但是run time找不到定义.
排查：run time找不到定义，即没有objs，所以.so当中没有包括相应的内容。

g++ -fPIC -shared -o other.so other_lib.cc /usr/local/thirdparty/lua5.3/lib/liblua.a
但是这么编译又过不去：
kang@ubuntu:~/workspace/myspace/git-personal/lua-best-practise/chapter21/demo-08/thirdparty/Other/src(master)$ g++ -fPIC -shared -o other.so other_lib.cc /usr/local/thirdparty/lua5.3/lib/liblua.a
/usr/bin/ld: /usr/local/thirdparty/lua5.3/lib/liblua.a(lapi.o): relocation R_X86_64_32 against `luaO_nilobject_' can not be used when making a shared object; recompile with -fPIC
/usr/local/thirdparty/lua5.3/lib/liblua.a: error adding symbols: 错误的值
collect2: error: ld returned 1 exit status
这个问题，进一步追查，直接根据错误信息进行google，得知，.so链接的.a，后者并不是fPIC代码，所以，需要对.a进行重新编译
第2篇参考文献，告诉我们如何判断一个.a是不是fPIC代码。

在对liblua.a的Makefile进行学习当中，又碰到问题，不理解Makefile底层没有commands，此时又进行排查，得知有一些Makefile的隐含规则。在对Makefile的学习过程中，参考了阮一峰写的Makefile入门概述，感觉特别好。
```

参考<br>
[动态库(.so)链接静态库(.a)的情况总结](https://www.cnblogs.com/nobugtodebug/archive/2012/11/07/e6cd72c67b3dd843f40d7ce919f7336a.html)
[linux编译问题集锦（持续更新中）](https://www.cnblogs.com/octave/p/4824584.html)
[how-to-write-makefile/implicit_rules](https://seisman.github.io/how-to-write-makefile/implicit_rules.html)


- 动态库run time找不到

q:现象是什么?
>./main: error while loading shared libraries: libzlog.so.1.2: cannot open shared object file: No such file or directory
即，编译通过，但是运行时告诉我libzlog找不到。问题就是，编译时能找到，但是运行时找不到

q:背景信息？
>1.zlog安装在/usr/local/thirdparty下面,lib,include,bin常见目录
2.没有更新ld.so.conf，没有执行ldconfig，也没有配置zlog的环境变量
3.Makefile写法如下

```
CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

RM= rm -f

ROOT= /usr/local/thirdparty/zlog-1.2

MYCFLAGS= -I$(ROOT)/include
MYLDFLAGS= -L$(ROOT)/lib
MYLIBS= -lzlog -lpthread

MAIN_T= main
MAIN_O= main.o

# Targets start here
t: $(MAIN_T)

$(MAIN_T): $(MAIN_O)
	$(CC) -o $@ $(LDFLAGS) $(MAIN_O) $(LIBS)


echo:
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(MAIN_T) $(MAIN_O)

.PHONY: t echo clean

main.o: main.c
```

q:原因？
>run time期间，libzlog.so的路径没有指定

q:解决？
>1.Makefile当中指定run time时，libzlog的路径(-Wl， -rpath指定动态搜索的路径)
2.LD_LIBRARY_PAH指定

q:更一般的解决办法是什么?
>dynamic loader负责在程序运行期间，加载.so。它依赖如下两个文件和一个命令：
1.ld.so.conf(动态库配置文件，给出动态库的目录)
2.ls.so.cache(动态库缓存文件，给出动态链接库名字列表)
3.ldconfing根据ld.so.conf指定的.so目录以及/lib /usr/lib，创建动态库链接库缓存文件，本质是动态链接库名字。
4.ld.so.cache的作用，也就是为了高速的查找，根据ld.so.conf指定的目录查找相对较慢

参考<br>
["error while loading shared libraries: xxx.so.x" 错误的原因和解决办法](https://blog.csdn.net/sahusoft/article/details/7388617)<br>
[LIBRARY_PATH和LD_LIBRARY_PATH环境变量的区别](https://www.cnblogs.com/panfeng412/archive/2011/10/20/library_path-and-ld_library_path.html)<br>
[Relationship between ldconfig and ld.so.cache](https://unix.stackexchange.com/questions/256893/relationship-between-ldconfig-and-ld-so-cache/317526)

#### linux elf格式

- 引言

- 分类

### 进程/线程/协程

#### 进程

#### 线程

#### 协程

### 文件

### 一些细节

#### 回车换行问题(CRLF)

q:这个问题的背景是什么？
>在早期的打印机时代，开始新的一行要占用两个字符的时间。如果到了一行的结尾处，你要快速回到新的一行的开头，需要打印针头在纸面上飞快地掠过，常常会在纸面上留下污点。解决这个问题的办法就是，用两个字符：一个字符<Return>来移到第一列，另一个字符<Line feed>来新增一行。我们可以看到，这个问题的背景就是如何产生新的一行，并换到行首。

q:这个问题为什么又会有多个方案？
>计算机产生以后，**存储较为昂贵**，在如何解决回车换行这个老问题上，人们产生了不同的意见。

q:各种os的方案是什么？
>1.unix:UNIX人认为在到达一行的结尾时新增一行<Line feed> (LF)
2.mac:Mac人则认同<Return> (CR)的解决办法
3.win:MS则坚持古老的<Return><Line feed> (CRLF)的方法

q:这个问题会导致实践的什么问题？
>由于不同os对于回车换行不同的处理，这就意味着如果你将一个文件从一个系统转移到另一个系统，就面临着回车换行的问题

```lua
kang@ubuntu:/mnt/hgfs/f-ubuntu-shared-folder$ cat -A test-crlf-copy.txt 
this is a testfile.^M$
let's see crlf.^M$
-- 上面是一个win下的文件，放到linux下面，换行是\r\n(crlf)(^M$)


kang@ubuntu:/mnt/hgfs/f-ubuntu-shared-folder$ vim test-crlf-copy.txt 
kang@ubuntu:/mnt/hgfs/f-ubuntu-shared-folder$ cat -A  test-crlf-copy.txt 
this is a testfile.$
let's see crlf.$
-- 通过vim 进行set ff = unix设置，将文件格式从dos转到unix，我们再看换行，变成了\n($)
```

注意，对于不同os下的文件，可以先用```cat -A```来查看源文件的格式

参考<br>
[vi下显示回车换行符等特殊符号](https://blog.csdn.net/chenxy02/article/details/53407677)