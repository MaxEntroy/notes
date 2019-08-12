## 程序设计基础

#### 表达式/语句
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

#### 类型

参考<br>
[Magic lies here - Statically vs Dynamically Typed Languages](https://android.jlelse.eu/magic-lies-here-statically-typed-vs-dynamically-typed-languages-d151c7f95e2b)<br>

#### 回调

参考<br>
[Callback](https://en.wikipedia.org/wiki/Callback_%28computer_programming%29)<br>
[回调函数（callback）是什么？](https://www.zhihu.com/question/19801131)<br>
[如何浅显地解释回调函数](https://bot-man-jl.github.io/articles/?post=2017/Callback-Explained)<br>

#### 热更新
>你可以在不中断进程运行的情况下，把修改过的代码加载到进程中，让随后的过程运行新版本的代码。by cloud wu
相信代码参考[lua热更新](https://github.com/MaxEntroy/notes/blob/master/notes/language.md)

参考<br>
[如何让lua做尽量正确的更新](https://blog.codingnow.com/2016/11/lua_update.html)
[Lua热更新](http://asqbtcupid.github.io/hotupdte-implement/)


## 基础算法


#### 启发式算法
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

#### webservice
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

尹圣雨给的解释，我觉得非常赞,后者对web server的定义
- 基于http协议
- 在服务端找到client端请求的url对应的文件
- 将url对应的文件发送给client端
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

#### protobuffer

参考<br>
[Pb语法](https://developers.google.com/protocol-buffers/docs/proto#scalar)<br>
[Pb-cpp相关](https://developers.google.com/protocol-buffers/docs/cpptutorial )<br>
[Pb-generated code](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated#repeatedmessage)<br>
[Pb-反射](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.descriptor)<br>
[Pb-反射-demo](https://yrczone.com/blog/article/45)<br>

#### 同步/异步
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

#### uri/url/urn
这是3个非常容易混淆的概念，但是理解的层次其实非常容易。
uri是抽象类，告诉我们，网络上的资源同样需要唯一的标识。这件事是重要的，但是怎么实现，没有说。
url和urn分别是两种实现。
url靠位置来表明一个资源,urn靠名称来表明一个资源。

参考<br>
[HTTP 协议中 URI 和 URL 有什么区别](https://www.zhihu.com/question/21950864)

#### HTTP

- http怎么来的

- http现在都怎么用

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


## 操作系统

## 数据库

## 其它

#### 模型(model)

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

#### git
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

#### wireshark