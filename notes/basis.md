## 程序设计基础

### 表达式/语句
- 引言
```
这2个概念，所有程序设计语言都会讲。但是具体的概念及其区分，一直没有搞得很透彻。
```

- 为什么要有expression?
```
An expression is an combination of values, variables, oerators and calls to functions.
Expression needs to be evaluated.
```
显然，定义精准的描述了expession的作用：表达式就是用来做计算的。

```
那显而易见，计算机的最基本功能是什么？不就是做计算。计算的是什么，不就是计算表达式。
只不过，这里的计算，由于oeprator分为算数，关系，逻辑这3中，因此可以计算3中表达式。算数表达式（这个易懂，就是计算机的初衷），关系表达式和逻辑表达式，不能在计算的场景直接理解。需要结合更复杂的程序逻辑。eg: if(a > 3) print("a > 3") 即结合控制结构，进行使用。
```
- 为什么要有statements?
```
A statement is an instruction that the Python intepreter can execute.(从Python解释器的角度出发)
```
- 既然statement对应到指令，那expression又算什么呢?
```
考虑最简单的statement,就是打印算数表达式。
print(1+2+3+4+5) //计算机的本质是做计算没错，但是计算结果你要怎么处理呢？是打印，还是做其它用途。这些就要由指令来驱动
expression，是做数值，逻辑，关系的计算。对于这些计算，有对应的计算指令。
但是，计算机并不是只有计算的功能，比如基本的输入输出，这些就不是expression的范围。
严格意义上来说，计算机语言的statements对应到机器层面的instructions.
而，expression是构成诸多statements当中，最主要的一种。
```
参考<br>
[Statements and Expressions](http://interactivepython.org/runestone/static/CS152f17/SimplePythonData/StatementsandExpressions.html)<br>

### 类型

参考<br>
[Magic lies here - Statically vs Dynamically Typed Languages](https://android.jlelse.eu/magic-lies-here-statically-typed-vs-dynamically-typed-languages-d151c7f95e2b)<br>

### 回调

参考<br>
[Callback](https://en.wikipedia.org/wiki/Callback_%28computer_programming%29)<br>
[回调函数（callback）是什么？](https://www.zhihu.com/question/19801131)<br>
[如何浅显地解释回调函数](https://bot-man-jl.github.io/articles/?post=2017/Callback-Explained)<br>

### 热更新
>你可以在不中断进程运行的情况下，把修改过的代码加载到进程中，让随后的过程运行新版本的代码。by cloud wu
相信代码参考[lua热更新](https://github.com/MaxEntroy/notes/blob/master/notes/language.md)

参考<br>
[如何让lua做尽量正确的更新](https://blog.codingnow.com/2016/11/lua_update.html)
[Lua热更新](http://asqbtcupid.github.io/hotupdte-implement/)

### 流(stream)
这个概念所涉及的领域比较多，我只说程序设计语言当中流的概念。我们先看wikipedia给的定义
>In computer science, a stream is a sequence of data elements made available over time. A stream can be thought of as items on a conveyor belt being processed one at a time rather than in large batches.

我觉得这段定义强调2个东西：
- 序列
- 个体

序列是想说，这段data item的逻辑组成关系，线性关系，一个挨着一个。所以，是有序的。这点重要。其次，它不是一个整体，是一个一个有序的个体。强调每次只处理一个个体。

下面我们再来看程序设计语言当中的一段定义
>On Unix and related systems based on the C language, a stream is a source or sink of data, usually individual bytes or characters. Streams are an abstraction used when reading or writing files, or communicating over network sockets. The standard streams are three streams made available to all programs.

上面这段话说了2件事，一是流的定义，二是流这么定义的抽象来源是什么。先说定义，本质上还是数据序列，但这里强调了方向性，进行流入或者流出的数据，把它称作流。或者直接这么说，**c程序当中进行IO的数据就是流**。
其实后面的抽象，印证了我的总结，程序当中进行文件或者socket的读写数据，就是流。一个程序进行IO除了文件，就是网络。

代码中，一般会关联一个fd或者socket来进行流的抽象表示(即通过fd或者socket来操作的数据就是stream)，比如下面代码
```c
int fflush ( FILE * stream );
```

有了上面对于个体的理解，bit stream和byte stream就很容易理解了。前者的处理个体是bit，后者是byte.

参考<br>
[Stream (computing)](https://en.wikipedia.org/wiki/Stream_(computing))<br>


### 幂等(idempotent)
我们先来看在程序设计当中的定义
>在编程中一个幂等操作的特点是其任意多次执行所产生的影响均与一次执行的影响相同.幂等函数，或幂等方法，是指可以使用相同参数重复执行，并能获得相同结果的函数
核心点在于，这些函数不会影响系统状态，也不用担心重复执行会对系统造成改变。比如http get接口就是幂等的。

## 基础算法


### 启发式算法
- 定义
```
Cambridge Dictionary给出的解释
heuristic: 
(of a method of teaching) allowing students to learn by discovering things themselves and learning from their own experiences rather than by telling them things

做一件很有意思的事情，我们替换students为machine
(of a method of teaching) allowing machine to learn by discovering things themselves and learning from their own experiences rather than by telling them things

再给出朗文高阶(6th)给出的解释
heuristics: the study of how people use their experience to find answers to questions or to improve performance
```
从上面的定义当中，我们先对启发式有了一个基本的认知，关键有2点：
- learning by themselvs
- learning form experiences rather than by telling them things.

对应到程序设计算法，我们不难理解，是不是有如下的策略：
- 不给出解决问题的具体思路，通过经验或者其他途径
- 通过自身的迭代来解决问题

我们再看来看wikipedia对启发式算法给出的定义
```
In computer science, artificial intelligence, and mathematical optimization, a heuristic (from Greek εὑρίσκω "I find, discover") is a
technique designed for solving a problem more quickly when classic methods are too slow, or for finding an approximate solution 
when classic methods fail to find any exact solution. 
This is achieved by trading optimality, completeness, accuracy, or precision for speed. In a way, it can be considered a shortcut.
```
从以上的定义中，我们可以看出该定义对启发式这个东西本身是什么，没有好的解释。而是更多的强调了，在经典方法失效的情况下(np-hard问题，算力达不到)，我们采用启发式方法，在效果上做tradeoff，但是可以换来更快的一个近似解，当然前提是这个近似解可以接受。

综上，启发式算法我们可以给出一个自己的解释：
- 应用场景通常是算力不可解的场景，比如常见的优化类问题，全局解往往是NP-hard问题
- 具体策略不是直观的算法(算力不可解)，而是通过给出自学习的策略(目标+经验)进行解决
- 一般是一个近似解

和AI/ML的关系
wikipedia多给出了一条启发式算法和AI的关系
```
Heuristics underlie the whole field of Artificial Intelligence and the computer simulation of thinking, 
as they may be used in situations where there are no known algorithms
```
显然，机器学习算法均属于启发式算法。因为，机器学习本质就是自学习，给出的目标函数对应的就是自学习的策略。而最优化问题，一般是算力不可解，所以，采用启发式算法来解决。常建的比如有sgd。具体来说，KMeasn算法，N个点给出K个类别，我们可以枚举所有可行解，但是算力不可解。所以，提出目标函数，采用sgd的方法得到了每个质心的坐标

## 网络/通信/协议

### webservice
先来看一看阮一峰对这个概念的阐述：
- 什么是service
>通俗地说，"服务"就是计算机可以提供的某一种功能。
- 本地服务 or 网络服务
>根据来源不同，服务又分为2种。一种是"本地服务"（使用同一台机器提供的服务，不需要网络），另一种是"网络服务"（使用另一台计算机提供的服务，必须通过网络才能完成）。
**"网络服务"（Web Service）的本质，就是通过网络调用其他网站的资源。** by ruanyifeng

- why webserivce?
>Web Service架构的基本思想，就是尽量把非核心功能交给其他人去做，自己全力开发核心功能。比如，如果你要开发一个相册软件，完全可以使用Flickr的网络服务，把相片都储存到它上面，你只要全力做好相册本身就可以了。总体上看，凡是不属于你核心竞争力的功能，都应该把它"外包"出去。

- 个人理解
本人觉得，上面的解释有一点点问题。我觉得网络服务应该是包含web service，因为网络服务有很多种。ssh server, ftp server，都是网络服务。不是只有web server.
也可能是阮一峰说的web有泛指Internet的作用。

- 网络服务之间的区别？
像上文所说，web service不代表所有网络服务。但是，web service的使用是最多的，因为人类使用最多的就是浏览器去浏览各种页面来获得信息。这也是Internet最大的贡献。
其实各种网络行为，也就关联了各种网络服务。比如，浏览行为关联了web service，下载行为关联了ftp service。

具体来说，web service具有如下特性
- web service特性
    - 应用层使用http协议
    - 采用http文档格式
    - 使用url
    - 客户端使用浏览器(这个是显而易见的，浏览器就是为浏览而生，没人愿意在terminal当中浏览)

尹圣雨给的解释，我觉得非常赞,后者对web server的定义
- 基于http协议
- 将url对应的文件发送给client端
- 在服务端找到client端请求的url对应的文件

所以，这个解释非常清楚的说明白了几个东西
1.应用层采用什么协议 2.client端请求的是什么 3.server端发送什么结果回去

他进一步给出了作为一个初学者，应该如何理解web server
- 理解hypertext
- 理解http
- web服务器端是以http协议为基础传输hyptertext的服务器端

参考<br>
[Web service是什么](http://www.ruanyifeng.com/blog/2009/08/what_is_web_service.html)<br>
[web服务器与ftp服务器的区别](https://blog.csdn.net/w252064/article/details/77581691)<br>
[Web service](https://en.wikipedia.org/wiki/Web_service)<br>

### protobuffer

参考<br>
[Pb语法](https://developers.google.com/protocol-buffers/docs/proto#scalar)<br>
[Pb-cpp相关](https://developers.google.com/protocol-buffers/docs/cpptutorial )<br>
[Pb-generated code](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated#repeatedmessage)<br>
[Pb-反射](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.descriptor)<br>
[Pb-反射-demo](https://yrczone.com/blog/article/45)<br>


- pb字段没对对齐
```
问题背景：约定好协议，进行联调
问题表现：接收端看到可以收到数据，字段a和字段b的值明显是有的。但是，在debug pb的时候，发现没有Key.比如a,b,c,d,e5个field，c,d,e
都能打印出相应的key和value，但是a和b只能打印出value，没有key，只有数字5和6
问题解决：上游同学没有以来pb 的debug string，而是手动把key和value进行解析，打印。返现确实a,b没有key.没有置位。但是，value也是有。
后来发现，这个数字5和6是index.至于为什么是5和6，对照了Pb的field，暂时觉得不会是这2个值。
总之：如果出现类似情形。就是字段没有对其。
```

### 同步/异步
这个概念是一对比较容易混淆的概念，主要在于这一对概念应用的场景比较多。区别不同的场景，就容易理解。
- 原始定义
参照longman 6th给出的解释
```
syn-chro-nize: 
1. to happen at exactly the same time, or arrange for two or more actions to happen at exactly the same time.
eg: The show was designed so that the lights synchronized with the music.
基本概念还是强调一个同时，其实内在的逻辑没有说明白。
同步，本质还是保持内在一个“一样”的状态，即一致性的状态
```

还是同步本身，我们来看wikipedia的定义
```
Synchronization is the coordination of events to operate a system in unison. The conductor of an orchestra keeps the orchestra synchronized or in time. Systems that operate with all parts in synchrony are said to be synchronous or in sync—and those that are not are asynchronous.
这个定义和上面基本一直，同步就是对不同事件的协调，使得他们在某一个指标上(声音是否整齐)保持一致。
```
- 通信当中的同步
下面讲到比较熟需的一个场景，通信当中的一致。和时序电路一个意思。
```
In electrical engineering terms, for digital logic and data transfer, a synchronous circuit requires a clock signal. However, the use of the word "clock" in this sense is different from the typical sense of a clock as a device that keeps track of time-of-day; the clock signal simply signals the start and/or end of some time period, often very minute (measured in microseconds or nanoseconds), that has an arbitrary relationship to sidereal, solar, or lunar time, or to any other system of measurement of the passage of minutes, hours, and days.
```

- synchronization of processes(computer science)
```
Process synchronization refers to the idea that multiple processes are to join up or handshake at a certain point, in order to reach an agreement or commit to a certain sequence of action.
这个定义的内在也是强调了某种程度的一致性，比如哲学家进餐问题，如果不按照同步序列进行操作，大家都吃不了饭。如果按照同步序列，大家处于“都能吃到饭”的一致性状态
```

- synchronization of data(computer science)
```
keeping multiple copies of a dataset in coherence with one another, or to maintain data integrity.
这个概念也是非常清晰的，就是保持多份副本数据的一致性
```
**说下异步**，上面讲了几个常用的同步场景。基本定义，通信当中的同步，计算机科学当中的同步。显然，异步的定义也简单。不是同步的就是异步。
- 本质定义
```
同步需要维护不同事件之间，内在的某种状态的一致性。比如，交响乐团，每个人的声音需要保持一致，才能齐奏。
异步，不需要维护不同事件之前，内在状态的一致性。即，不同事件之间，内在的状态是不一致的。
```

在wiki上，异步的定义很多。我们直接说重点，其实我们一般些程序讨论的同步/异步，主要是指同步io/异步io
在这个背景下，我们再继续讨论，会牵扯阻塞和非阻塞进来。

- 同步调用/异步调用
>
同步：在发出一个同步调用时，在没有得到结果之前，该调用就不返回。
异步：在发出一个异步调用后，调用者不会立刻得到结果，该调用就返回了。
同步调用和异步调用，强调的是一个调用是否立即返回。和进程和线程的状态没有关系。

- 阻塞调用/非阻塞调用
>一个线程/进程经历的5个状态， 创建，就绪，运行，阻塞，终止
阻塞调用是指调用结果返回之前，调用者会进入阻塞状态等待。只有在得到结果之后才会返回。
非阻塞调用是指在不能立刻得到结果之前，该函数不会阻塞当前线程，而会立刻返回。
所以，阻塞调用和非阻塞调用是指进程/线程的状态，和调用是否立即返回没有关系

上面两组概念，就有4种组合。

>
同步阻塞调用：得不到结果不返回，线程进入阻塞态等待。
同步非阻塞调用：得不到结果不返回，线程不阻塞一直在CPU运行。
异步阻塞调用：去到别的线程，让别的线程阻塞起来等待结果，自己不阻塞。
异步非阻塞调用：去到别的线程，别的线程一直在运行，直到得出结果。


参考<br>
[Asynchronous I/O](https://en.wikipedia.org/wiki/Asynchronous_I/O)<br>
[Synchronous and Asynchronous I/O](https://docs.microsoft.com/en-us/windows/win32/fileio/synchronous-and-asynchronous-i-o)<br>
[同步，异步，阻塞，非阻塞等关系轻松理解 ](https://github.com/calidion/calidion.github.io/issues/40)<br>

### uri/url/urn
这是3个非常容易混淆的概念，但是理解的层次其实非常容易。
uri是抽象类，告诉我们，网络上的资源同样需要唯一的标识。这件事是重要的，但是怎么实现，没有说。
url和urn分别是两种实现。
url靠位置来表明一个资源,urn靠名称来表明一个资源。

参考<br>
[HTTP 协议中 URI 和 URL 有什么区别](https://www.zhihu.com/question/21950864)

### HTTP

- http怎么来的

- http现在都怎么用
这个感觉需要特别说下，和我们平时使用息息相关。先说直观体验，我理解http为浏览器而生，但是它又大量应用在我们的服务交互当中，所以我不是很理解这是怎么回事。
直到看到知乎上给了一个好的解释

>HTTP最早被用来做浏览器与服务器之间交互HTML和表单的通讯协议；后来又被被广泛的扩充到接口格式的定义上;
所以在讨论GET和POST区别的时候，需要现确定下到底是浏览器使用的GET/POST还是用HTTP作为接口传输协议的场景。

从上面这个定义当中，很容易得到http的2种用法。目前工作当中是用作第二种，即用http作为接口定义。
这又会带来另一个问题，为什么要有接口定义。回想我以前写的echo server，socket连接之后，就直接在socket上进行read/write。说下我自己的理解啊，如果服务简单，
这么写到没事。如果服务复杂，比如一个Server可能提供多种子服务，如果还是之前的方式，不清晰。我们自己看brpc搭建http/h2服务，通过proto来暴露对外接口，客户端通过
http_client或者curl进行请求，语义上非常清晰。

参考<br>
[GET 和 POST 到底有什么区别？](https://www.zhihu.com/question/28586791/answer/767316172?hb_wx_block=1&utm_source=wechat_session&utm_medium=social&utm_oi=658626377876639744)

- http的过程
    - 数据请求(client)
        - 建立TCP连接(TCP,TCP,TCP)
        - 发送http请求(HTTP)
    - 数据响应(server)
        - 发送http响应(TCP,HTTP)
    - 断开连接(server)
        - server主动断开(TCP,TCP)

特别注意，
1.其实数据请求和数据响应，都是要维护TCP的，因为TCP和HTTP不在同一个层次。
发送HTTP肯定得有TCP状态才可以。数据请求这里只不过单独拿出来说下。
2.server发送响应之后主动断开连接，所以没有四次挥手。
- http协议
协议主要就是说些彼此之间约定的数据发送格式
    - http_request
        - 请求行
        - 消息头
        - 空行
        - 消息体
    - http_response
        - 状态行
        - 消息头
        - 空行
        - 消息体

- GET/POST

### RPC

说一下我的这个简单理解，socket和http都比较好理解，是很具体的东西。
socket就是os提供的进行网络交互的handle，至于应用层什么协议使用，和socket本身没有关系。http底层用socket，ftp底层也使用socket，所以这点很清晰。
http也好理解，应用层协议，应用程序之间约定好**业务数据**的协议以及一些细节。http主要是web service这类业务进行的应用层协议，当然http我在上面也提到了，也可以被当做接口传输协议场景。
rpc我理解，不是一个非常具体的概念。是在基础网络基础上，抽象出来的高级逻辑。不仅有网络通信，还有服务发现，服务治理等这些东西。

参考<br>
[RPC、HTTP、Socket区别](https://zhuanlan.zhihu.com/p/48760074)

## 操作系统(Linux)

### 高级语言的compile/link/load
这一小节主要写一篇读书笔记，总结一些linux下的基本概念

- 引言

q:既然java/python等高级语言，运行效率低，为什么还可以接受？
>当前，服务器的计算能力已经不是瓶颈，cpu/内存/磁盘的性能都教以前有了很大提升，即目前机器的性能提升非常大。可能以前运行效率低跑不了的代码，在现在的机器上可以跑了，并且目前机器的价格比30-40年前便宜了太多，所以性能不够，机器来凑也成为可能。所以，大家可以不用那么关心代码的运行效率，而把重心放到开发效率上。

q:vm或者interpeter在client和os的层次上，处于一种什么样的关系
>vm或者interpreter处于client和os的中间层次，避免了client代码和os直接进行交互，这样也可以让程序员把工作尽可能的多放在client代码上，而不是放到和os交互的一些细节上。提升了开发效率。


- cpu体系结构

q:当我们说cpu体系结构的时候，我们在说什么？
>有一个确定的方面，就是指令集。指令集有cisc和risc区分，x86_64属于cisc指令集，ibm的power pc系列则是risc.指令集显然会涉及到指令的做成，即computing set由哪些instruction组成，毕竟任何代码最终
还是转换到computing set当中的一条条instruction，然后cpu进行执行，显然，cpu的设计也是基于指令集的，因为不同指令集的指令长度以及基本组成都可能不同。

q:我们在一种cpu体系结构的编译产物可以在另一种cpu体系结构下运行吗？
>显然不行，假设os一样，虽然源代码一样，但是编译产物最终是该cpu体系结构下的computing set当中的instruction，上文已经提到，不同cpu体系接口个，computing set不同，instruction不同。
某一个平台的instruction显然无法在另一个平台执行。

- os

不同os的编译产物，可以在彼此系统上运行吗？
>不行。编译产物，是二进制可执行程序。不同os对于binary的文件格式不同。比如，linux下是elf，win下是exe.那么，这个**binary文件格式**又是什么呢？
说一个我的简单理解，elf里面规定了程序的入口地址，以及text,bss段的地址，即一个binary装入内存之后，我们怎么能知道入口在哪里？程序需要的变量和函数又去哪里找？elf就是告诉我们这些内容。
当然，Linux进程空间的布局，我理解应该就是elf规定的。所以，linux生成的binary，在windows下肯定找不到，因为入口地址，大家规定的不一行，linux binary当中，elf指定一个入口地址。
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

参考<br>
[高级语言的编译：链接及装载过程介绍](https://tech.meituan.com/2015/01/22/linker.html?from=timeline&isappinstalled=0)<br>
[Linux elf格式](http://www.iloveandroid.net/2015/11/17/studyElf/)<br>
[ELF(Executable and Linkable Format)](https://www.cnblogs.com/littlehann/p/3871092.html)<br>
[https://www.zhihu.com/question/20484931](https://www.zhihu.com/question/20484931)


## 数据库

## 其它

### 模型(model)

- 定义
```
朗文高阶给出的解释
a simple discription of a system or structure that is used to help people understand similar systems or structures.
eg: Cival society is a classical economist's model of the free market.
```
对于这一版的解释，其实是更通用的。但是，整体的思路是清晰的，对于系统/结构的一种描述，目的是为了更容易的来理解不易理解的系统/结构。

```
A mathematical model is a description of a system using mathematical concepts and language.
```
比如，我们用数学语言来描述，因为数学语言本身是非常精确地，简洁的，所以数学模型是非常直观的。

所以说，到底怎么理解模型，其实就是对复杂系统，或者结构更加**本质的展现**。
这其中，又因为数学语言的精确性，与简洁性，数学模型对于本质的展现是相对比较清晰的。

- 怎么理解数学模型
```
集合论是一种非常有效的方式来帮我们理解数学模型，因为对于复杂系统/复杂结构，我们可以借助集合去描述它。
那么，我们必然要抽象集合的构造表达式，或者说是“集合的特征”，这个也就是我们所说的“本质的展现”。
```
比如，线性模型，我们一般有如下的表示：
$$\{(X,Y)|Y=XB+U\}$$

参考<br>
[Mathematical model](https://en.wikipedia.org/wiki/Mathematical_model)<br>
[General linear model](https://en.wikipedia.org/wiki/General_linear_model)<br>
[Set](https://en.wikipedia.org/wiki/Set_(mathematics))<br>


## 工具

### git
- git修改上次提交
主要说下，怎么修改username和email
```c
git commit --amend --author="userName <userEmail>"
```

参考<br>
[git中 修改已提交的commit的用户名邮箱](https://www.jianshu.com/p/7def4f387e9f)

- merge/rebase
先说下merge，merge比较简单，就是分叉的分支，进行一次三路合并。第3路是最近common ancestor.
需要注意的一点是，commit message会写在同一列。但是commit符号(*)并不在一起。我以前总看错

### wireshark

### Tex

对几个概念先进行简单描述：
>
TEX: 一种排版语言
Latex: 基于TEX实现的一个宏集，自然也是一种排版语言
>
Tex Live: TEX/TEX based 语言编译器的发行版，如g++, visual c++。目前使用的CTEX = Tex live + 中文支持
>
Tex Studio: IDE