[TOC]

## 操作系统(Linux)

### CPU

#### 进程/线程/协程

### Memory

本小节主要记录linux性能调整io篇的一些学习心得

#### 常用命令

q:buffer/cache的演进
>linux设计缓存系统的原因是，大部分内存，内核用的很少，主要是给应用。对于一些剩余的内存，也得用起来，所以，设计了这样一套缓存系统。
最初的时候：
cache:读缓存(app read)
buf:写缓冲(app write)
>
>但是这么设计有一个问题，比如一个应用刚读取数据，又需要写入到另一个file，那么此时还需要写一次buf，没有办法利用cache当中的数据.所以2.4(这个不是很确定)之后的版本，做了新的变更
page cache: file content
buffer cache: file meta(格式化文件系统，需要写文件系统结构数据，这个数据会缓存在buffer里面啊，所以buffer里面一般是file的meta)

q:top命令的一些细节
```
             total       used       free     shared    buffers     cached
Mem:          7.8G       5.1G       2.7G        31M       306M       2.7G
-/+ buffers/cache:       2.1G       5.7G(这一行很重要)
Swap:         8.0G         0B       8.0G
```

>之前提到过，对于buf/cache是利用操作系统当中剩下的数据，进行一个再利用。那么本质上来说，这一部分数据可以被free掉
所以-+buffer/cache是考虑这层因素之后的结果。
>
>但是，即使考虑了这一部分结果，这里的数据也不能完全可信
原因在于,cache当中有些数据，内核是不能释放的。只能强制手动释放。
那既然这里的数据也不准，哪的数据准呢？

```
cat /proc/meminfo (使用这条命令)
MemTotal:        8157844 kB
MemFree:         2806844 kB
MemAvailable:    6065884 kB(关注这个属性)
Buffers:          314296 kB
Cached:          2854064 kB
SwapCached:            0 kB
Active:          3620924 kB
Inactive:         740924 kB
Active(anon):    1208984 kB
Inactive(anon):    16984 kB
Active(file):    2411940 kB
Inactive(file):   723940 kB
Unevictable:          84 kB
Mlocked:              84 kB
SwapTotal:       8386556 kB
SwapFree:        8386556 kB
Dirty:                 4 kB
Writeback:             0 kB
AnonPages:       1193568 kB
Mapped:           371352 kB
Shmem:             32484 kB
Slab:             487112 kB
SReclaimable:     429072 kB
SUnreclaim:        58040 kB
KernelStack:       11152 kB
PageTables:        35128 kB
NFS_Unstable:          0 kB
Bounce:                0 kB
WritebackTmp:          0 kB
CommitLimit:    12465476 kB
Committed_AS:    4674748 kB
VmallocTotal:   34359738367 kB
VmallocUsed:           0 kB
VmallocChunk:          0 kB
HardwareCorrupted:     0 kB
AnonHugePages:    606208 kB
CmaTotal:              0 kB
CmaFree:               0 kB
HugePages_Total:       0
HugePages_Free:        0
HugePages_Rsvd:        0
HugePages_Surp:        0
Hugepagesize:       2048 kB
DirectMap4k:      198464 kB
DirectMap2M:     8189952 kB
DirectMap1G:     2097152 kB
```

q:cache当中的哪些数据是不能释放的呢?
>跟ipc相关的不能释放
echo 3 > /proc/sys/vm/drop_caches(这条命令可以强制丢弃cache 1和２分别代码buffer和cache)
```cpp
ipcs
------------ 共享内存段 --------------
键        shmid      拥有者  权限     字节     连接数  状态      
0x00000000 131072     kang       600        16777216   2                       
0x00000000 425985     kang       600        134217728  2          目标       
0x00000000 393218     kang       600        524288     2          目标       
0x00000000 688131     kang       600        524288     2          目标       
0x00000000 917508     kang       600        393216     2          目标       
0x00000000 1015813    kang       600        1048576    2          目标       
0x00000000 1048582    kang       600        393216     2          目标       
0x00000000 1146887    kang       700        16472      2          目标       
0x00000000 2195464    kang       700        648576     2          目标       
0x00000000 1736714    kang       600        393216     2          目标       

--------- 信号量数组 -----------
键        semid      拥有者  权限     nsems     

--------- 消息队列 -----------
键        msqid      拥有者  权限     已用字节数 消息
```

q:Swap的作用?
>内存里面不活跃的空间，交换到磁盘中，空出来的空间给别的程序用

q:top 命令当中需要关注的指标有哪些?
>RES是物理内存总量
VIRT虚拟内存占用量(VIRT不一定映射到RES)

q:还有哪些常用命令?
>sudo sar -r 1

#### os是怎么管理memory(宏观)

q:这个问题可以这么考虑，加入现在要支持并发1024个进程，目前有4G内存，我们如果设计操作系统来管理内存，应该怎么管理？
>朴素的想法自然是，4G / 1024 = 4M，那就每个进程分配4M空间。但是这带来的问题是，如果目前只有2个进程，也只能用这么大空间。显然不合理。
此时引入虚拟空间的概念，即每个进程都可以使用4G的空间。先说风险，如果有一个进程真的用了4G空间，那多余一个进程都不能分配资源。
显然，之所有能这么考虑的原因在于，对于进程使用空间的预判，知道进程使用的空间远小于这个数，所以这么设计，因为programmer一般都会按照自己的需求来申请，是克制的。
我之所以这么说，是想举银行的例子，有1000个人，每个人都向银行存4k，银行总共有400w，每个人贷款的时候，可以贷远多余1000的个数，这里是想类比虚拟内存的概念。
但实际上，由于人借了钱可能不还钱，银行为了降低自己的分享给每个人做了限制。
>
>内核的实际做法就是引入虚拟内存的概念来解决这个问题。
内核给app分配的是虚拟内存。(malloc返回给app的地址，是一个虚拟地址，只有具体写这段空间的时候，内核才会做映射)
内核只需做的是把虚拟内存和物理内存对应起来。

#### 内核到底是怎么管理memory(微观)

q:rt?
>buddy是底层分配器
slab帮助用户代码，来进行底层物理内存的管理。slab也是同过buddy进行管理
>
>下面的知识没有求证，因为我听课听一半被叫走了：
实际物理page块: 1 1 1 1 1 1 1 1(2^0)
page表         1 1 1 1 1 1 1 1(2^1，此时每一个page块，有一个page表跟它对应)
再一层管理       1   1   1   1  (2^2，每2个page表，被一个二级page表管理)
再一层管理       1       1      (2^3，第三级page表管理)
基本思路我是知道的，但是至于是否是这样管理，后续再验证下

### 编译系统

#### linux elf格式

- 引言

- 分类

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

##### compile time符号找不到

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


##### 动态库链接静态库
- case1
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

- case2

q:问题描述？
>背景是，我之前安装好了gflags，给出了.a。我也不知道这个.a是不是fPIC
>安装glog时，有如下问题
/usr/bin/ld: /usr/local/thirdparty/gflags/lib/../lib/libgflags.a(gflags.cc.o): relocation R_X86_64_32S against `.rodata' can not be used when making a shared object; recompile with -fPIC
>
>根据错误信息，我们得知：
1.编译glog的过程中，用到了libgflags.a，但是后者不是fPIC代码
2.对于问题的解决，我们有如下思考
2.1.如果glog只是生成一个.a，那么用到了.a直接打包即可。不需要fPIC代码，那么这代表默认的glog库生成方式，不是紧紧需要.a without fpic的形式。我们可以从这个角度尝试修改
2.2.重新编译libgflags.a，参考case1地第二篇参考文献可解决
>
网上的参考：提示说需要-fPIC编译，然后在链接动态库的地方加上-fPIC的参数编译结果还是报错，需要把共享库所用到的所有静态库都采用-fPIC编译一边才可以成功的在64位环境下编译出动态库。

q:解决？
>方案一：glog编译成.a，那么对于.a and .o没有fPIC的要求
方案二：gflags重新编译成.so或者.a with fPIC
>
>为了不影响现有代码，我采用了方案一。但是一直都不行，主要卡在进行了congifure配置，但是从提示上来看，生成的还是.so，无法生成.a。配置没有生效。
衡量下，暂时找不到解决办法，不想一致卡在这。换到方法二解决

参考<br>
[一些与编译，链接相关的问题(-fPIC)](https://blog.csdn.net/laoyi_grace/article/details/9404269)

##### 动态库run time找不到

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

### 虚拟化

这一小节主要说下容器相关技术

#### 基本概念

q:什么是LXC?
>LXC (Linux Containers) is an operating-system-level virtualization method for running multiple isolated Linux systems (containers) on a control host using a single Linux kernel.
>
>翻译过来就是linux容器。具体来说，容器是一种os级别的虚拟化方法，这种方法主要用来在单一os kernel上面提供多个独立的linux系统(也即是linux容器)

q:LXC主要使用了哪些技术？
1. cgroups functionality(that allows limitation and prioritization of resources (CPU, memory, block I/O, network, etc.) without the need for starting any virtual machines,)
2. namespace isolation functionality( that allows complete isolation of an application's view of the operating environment, including process trees, networking, user IDs and mounted file systems)

简单来说，分别从物理层面，应用层面进行了隔离。

**LXC combines the kernel's cgroups and support for isolated namespaces to provide an isolated environment for applications. Early versions of Docker used LXC as the container execution driver**

q:什么是docker?
>Docker is a set of platform as a service (PaaS) products that uses OS-level virtualization to deliver software in packages called containers
>
>docker我的理解，应该是平台。在这个平台上面，软件可以在容器当中运行。整体来看，container的概念更像是virtual machine，而docker作为平台(本质是服务)，把容器作为服务对象，提供给大家。
或者简单理解为，提供容器服务的平台。

q:docker的组成？
>The Docker software as a service offering consists of three components:
1. Software: The Docker daemon, called dockerd, is a persistent process that manages Docker containers and handles container objects. The daemon listens for requests sent via the Docker Engine API.[38][39] The Docker client program, called docker, provides a command-line interface that allows users to interact with Docker daemons
2. Objects: Docker objects are various entities used to assemble an application in Docker. The main classes of Docker objects are images, containers, and services
2.1. A Docker container is a standardized, encapsulated environment that runs applications.[41] A container is managed using the Docker API or CL
2.2. A Docker image is a read-only template used to build containers. Images are used to store and ship applications.
2.3. A Docker service allows containers to be scaled across multiple Docker daemons. The result is known as a swarm, a set of cooperating daemons that communicate through the Docker API
3. A Docker registry is a repository for Docker images. Docker clients connect to registries to download ("pull") images for use or upload ("push") images that they have built

q:docker vs virtual machine?
>最本质的区别： OS-level virtualization software
docker支持是在os层面进行支持，而vm不是，比如你在win上启动一个ubuntu，提供的是一个独立的os。但是docker则是在同一个os提供不同的container.
1. 性能：docker一般接近原生,vm则逊于docker
2. 硬盘：docker逊于vm
3. 系统支持量：单机支持上千个container，vm则只能支持一般几十个

q:docker的优点
1. 更高效的利用资源。由于docker不需要硬件虚拟以及完整的os支持，跟传统虚拟化技术相比，显得轻量级。
2. 一致的运行环境。开发过程中一个常见的问题是环境一致性问题。由于开发环境、测试环境、生产环境不一致，导致有些 bug 并未在开发过程中被发现。而 Docker 的镜像提供了除内核外完整的运行时环境，确保了应用运行环境一致性
3. 更轻松的迁移。由于 Docker 确保了执行环境的一致性，使得应用的迁移更加容易。Docker 可以在很多平台上运行，无论是物理机、虚拟机、公有云、私有云，甚至是笔记本，其运行结果是一致的
4. 更轻松的扩展和维护。Docker 使用的分层存储以及镜像的技术，使得应用重复部分的复用更为容易，也使得应用的维护更新更加简单，基于基础镜像进一步扩展镜像也变得非常简单。

q:再议docker image?
>A Docker image is a read-only template that contains a set of instructions for creating a container that can run on the Docker platform. It provides a convenient way to package up applications and preconfigured server environments, which you can use for your own private use or share publicly with other Docker users.
>
>功能上来说，是用来创建容器的。具体来说，定制运行环境的离线存储。

q:docker image vs os image?
>因为镜像包含操作系统完整的 root 文件系统，其体积往往是庞大的，因此在 Docker 设计时，就充分利用 Union FS 的技术，将其设计为分层存储的架构。所以严格来说，镜像并非是像一个 ISO 那样的打包文件，镜像只是一个虚拟的概念，其实际体现并非由一个文件组成，而是由一组文件系统组成，或者说，由多层文件系统联合组成
>
>镜像构建时，会一层层构建，前一层是后一层的基础。每一层构建完就不会再发生改变，后一层上的任何改变只发生在自己这一层。比如，删除前一层文件的操作，实际不是真的删除前一层的文件，而是仅在当前层标记为该文件已删除。在最终容器运行的时候，虽然不会看到这个文件，但是实际上该文件会一直跟随镜像。因此，在构建镜像的时候，需要额外小心，每一层尽量只包含该层需要添加的东西，任何额外的东西应该在该层构建结束前清理掉
>
>分层存储的特征还使得镜像的复用、定制变的更为容易。甚至可以用之前构建好的镜像作为基础层，然后进一步添加新的层，以定制自己所需的内容，构建新的镜像。

q:容器有什么需要特别注意的地方？
>镜像使用的是分层存储，容器也是如此。每一个容器运行时，是以镜像为基础层，在其上创建一个当前容器的存储层，我们可以称这个为容器运行时读写而准备的存储层为 容器存储层
>
>可以这么理解，我们可以使用同一个docker image来构造不同的container，那么不同container的基础镜像是一致的。但是container也需要自己的容器存储层。
>
>容器存储层的生存周期和容器一样，容器消亡时，容器存储层也随之消亡。因此，任何保存于容器存储层的信息都会随容器删除而丢失。
按照 Docker 最佳实践的要求，容器不应该向其存储层内写入任何数据，容器存储层要保持无状态化。所有的文件写入操作，都应该使用 数据卷（Volume）、或者绑定宿主目录，在这些位置的读写会跳过容器存储层，直接对宿主（或网络存储）发生读写，其性能和稳定性更高

#### 容器实现的一些技术细节

参考<br>
[LXC](https://en.wikipedia.org/wiki/LXC)
[Docker (software)](https://en.wikipedia.org/wiki/Docker_(software))<br>
[什么是 Docker](https://yeasy.gitbook.io/docker_practice/introduction/what)<br>
[C++ 实现简易 Docker 容器](https://www.shiyanlou.com/courses/608)<br>


### 其他

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
