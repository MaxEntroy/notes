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

- 数据类型与垃圾回收

q:一个错误问题(为什么int,double这种值类型不需要垃圾回收而对象类型则需要)?
>这个问题是我使用c/cpp时，切换到lua时问的一个问题。其中值类型就是指int,double,这些基础类型。对象类型则是用户自定义的类类型。这是个错误的问题，因为可以有int* pint = new int();的操作，pint一样需要回收。
最初想问这个问的根源在于我错误的认为，为什么对象类型需要垃圾回收而值类型不需要？其实是否需要垃圾回收和值类型，动态类型有关系，但却不是从这个角度切入理解的，需要结合stack和heap去说。

q:什么是值类型(value type)和引用类型(reference type/object type)?
>我们通过java语言来解释这个问题：
> - 值类型，保存的就是值本身
> - 引用类型，保存的并非值本身，而是对象的引用。对象本身保存在对象引用所指的地址处

>多说一点，对于java对象来说，好理解。但是对于c/cpp来说，不存在这样的概念(不存在定义一个变量，这个变量自动保存对象的地址，只能通过指针间接的保存)。c中只有值类型，对于对象这种变量，通常是通过指针变量来保存其地址。cpp支持了引用类型，但是也需要间接的绑定对象才行。

q:数据类型与stack and heap的关系
>参看下文stack and heap，我们知道stack对应执行逻辑，heap对应数据存储。由于stack存储的都是一些辅助运行逻辑的数据，所以这部分数据不应该较大，且不具备动态增长能力。而堆就是用来存储数据，所以大块数据以及具有动态增长能力的数据都存储在堆中。
具体到语言中，java值类型都是存储在栈中。对象的引用也存储在栈中。原因就是，值类型本身非常基础(number, string)，占用空间不大。且不会出现动态增长的情形，所以放在栈里。对于引用类型，通常关联的是一个对象(java中arr，或者像cpp当中的vector, list, map这类容器)，后者数据较大且具备动态增长的能力,存储在堆中。

q:存储在stack and heap当中的数据在操作上有什么区别？
>上文总结到，value type(basic type)通常存储在栈中。reference type(object type)通常存储在堆中。我们以Lua语言为例，lua当中有8种数据类型，nil, boolean, string, number为type value, table, function, thread, userdata位reference type(object type)。他们在操作上的区别是：
Tables, functions, threads, and (full) userdata values are objects: variables do not actually contain these values, only references to them. Assignment, parameter passing, and function returns always manipulate references to such values; these operations do not imply any kind of copy.
管中窥豹，可见一斑：
> - 栈中数据操作统一
> - 堆中数据操作不统一，其差异来自于不同语言的实现

>上面提到的操作，主要是指赋值，参数传递，函数返回值时，对于数据拷贝操作的区别。值类型都一样，拷贝。但是引用类型，则取决于语言的实现。像lua这种，统一成浅拷贝。但是cpp这种，可是更精细的实现对象拷贝，则依赖于不同对象对于这个操作的实现，默认都是浅拷贝。

参考<br>
[数据类型与垃圾回收](https://blog.csdn.net/zhuhuiby/article/details/6832091)<br>

- 数据交换类型

这一小结来说说json

q:什么是json?
>JSON(JavaScript Object Notation) 是一种轻量级的数据交换格式


q:json给我们什么启发？
>我给出阮一峰的解释：从结构上看，所有的数据（data）最终都可以分解成三种类型：
第一种类型是标量（scalar），也就是一个单独的字符串（string）或数字（numbers），比如"北京"这个单独的词。(对于scalar，我有更进一步的理解，从变量的尺度来说，它只有一个维度，就是值。我们常说的一维数组从尺度上来说，则是二维的，arr[i]这个变量即有值，也有它在数组中的偏移量)
>
第二种类型是序列（sequence），也就是若干个相关的数据按照一定顺序并列在一起，又叫做数组（array）或列表（List），比如"北京，上海"。
>
第三种类型是映射（mapping），也就是一个名/值对（Name/value），即数据有一个名称，还有一个与之相对应的值，这又称作散列（hash）或字典（dictionary），比如"首都：北京"。

q:阮一峰对于数据的理解和数据结构的描述有什么区别？
>我理解，阮一峰是从数据描述性这个角度，来对数据做了划分。比如标量，只能描述一个属性的值。序列，可以描述并列的属性。映射，则是描述一个object的多个属性(属性不并列，就如同人脸由五官组成，五官并不是并列关系。不同人的五官是并列关系)
而我们说的数据结构(主要说逻辑结构)，则指的是从数据元素与数据元素之间的逻辑关系。更关注数据元素本身，而不是与现实的描述性有关系
拿阮一峰举得例子来说，对于一个城市，有很多个属性，我们通过一个mapping来表达city的概念。但是从数据结构来说，我们看中的是元素之间的关系，所以只有一个city，我们无法谈论任何结构。比如，我们可以从城市的连通性角度，用图的结构，来表达城市之间连通性的逻辑关系。当然，高维mapping也可以表达这个关系，这么说来，阮一峰说的结构，则更像是实现逻辑结构的物理结构。

q:json的规则
>
1） 并列数据的集合（数组）用方括号("[]")表示
2） 并列的数据之间用逗号（", "）分隔
3） 映射的集合（对象）用大括号（"{}"）表示
4） 映射用冒号（": "）表示

参考<br>
[JSON](https://en.wikipedia.org/wiki/JSON)<br>


### 回调

q:当我们说回调时，我们说的是什么？
>说的就是回调函数(callback)

q:什么是回调函数？
>根据wikipedia的定义：
1.作为参数传递给其他代码的可执行代码
2.在其他代码中会对这段可行代码就行调用
In computer programming, a callback, also known as a "call-after"[1] function, is any executable code that is passed as an argument to other code that is expected to call back (execute) the argument at a given time. 
我自己的理解：
1.回调函数是一个函数
2.作为参数fn传递给高阶函数，高阶函数在某个时刻调用参数fn

q:如何理解回调函数？
>从函数层面来说，回调函数是一个函数，和别的普通函数没有什么区别。从调用逻辑关系上来说，回调函数和其他函数的调用逻辑并不一样。回调函数作为一个参数传递给逻辑低于本层的函数，再该函数中再对回调函数进行调用。调用的逻辑是从高层向底层，再由底层调用高层，有这么一个来回的逻辑，我们称之为回调(callback)

q:为什么要有回调函数？
>因为有回调场景的存在

q:什么是回调场景？
>在软件工程当中，为了解决模块之间的耦合，有学者提出了控制反转这样一种设计原则，这种原则的具体实现，回调的场景。即回调是对控制反转的一种实现。

q:什么是控制反转
>控制反转（Inversion of Control）是一种是面向对象编程中的一种设计原则，用来减低计算机代码之间的耦合度。其基本思想是：借助于“第三方”实现具有依赖关系的对象之间的解耦。

参考<br>
[Callback](https://en.wikipedia.org/wiki/Callback_%28computer_programming%29)<br>
[回调函数（callback）是什么？](https://www.zhihu.com/question/19801131)<br>
[如何浅显地解释回调函数](https://bot-man-jl.github.io/articles/?post=2017/Callback-Explained)<br>
[深入理解回调函数](https://flat2010.github.io/2017/01/10/%E6%B7%B1%E5%85%A5%E7%90%86%E8%A7%A3%E5%9B%9E%E8%B0%83%E5%87%BD%E6%95%B0/#undefined)<br>
[控制反转与依赖注入](https://www.jianshu.com/p/07af9dbbbc4b)<br>

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

### 尾递归(tail recursion)
为了说清楚尾递归，我们需要先引入尾调用。
q:什么是尾调用(tail call)?
>尾调用，当一个函数的最后一个动作是调用另一个函数并且没有其它工作时，就形成了尾调用

q:尾调用有什么特点？
>尾调用结束之后，原函数没有其余工作要做。这是尾调用最大的特点。基于这个特点，尾调用结束之后，可以无需返回原函数。这也使得，尾调用结束之后，程序栈中无需保存原函数任何调用信息。尾调用结束之后，可以直接返回原函数调用之处。

q:尾递归有什么特点？
>递归最大的问题是容易造成栈空间溢出，而尾递归则不会，因为每次尾调用不在保存调用函数的stack frame.

### stack and heap

q:stack and heap的语义是什么？编译器设计者基于怎样的考虑，设计了这两个部分？
>栈是运行时的单位，堆是存储的单位。 栈解决程序的运行问题，即程序如何执行，或者说如何处理数据；堆解决的是数据存储的问题，即数据怎么放、放在哪儿。
以java语言为例，Java中一个线程就会相应有一个线程栈与之对应，这点很容易理解，因为不同的线程在语义上代表不同的执行逻辑，因此需要一个独立的线程栈。这点c/cpp也是一致的。而堆则是所有线程共享的，每个线程不独有。
我们再关联一下，线程是处理机调度的单位，而进程是资源调度的单位。那么同理，栈和线程一起，随处理机调度而分配，堆则是和进程一起，随资源调度而分配。

q:实际中stack and heap都存储数据？这又有什么区别？
>上文提到，stack语义对应到执行逻辑。那么在一次执行当中，所需要的和本次执行相关的数据，就由栈来进行存储。即栈存储的是和处理逻辑相关的数据，比如程序运行状态，函数调用的返回值等。而堆中存储的数据，则相对来说不一定和执行逻辑强相关。

q:stack and heap区分的考虑除了上文语义上的，还有那些考虑？
> - 从软件设计的角度看，栈代表了处理逻辑 ，而堆代表了数据 。这样分开，使得处理逻辑更为清晰
> - 堆与栈的分离，使得堆中的内容可以被多个栈共享 
> - 栈因为运行时的需要，比如保存系统运行的上下文，需要进行地址段的划分。栈只能单向增长，会限制栈存储数据的能力。而堆不同，每个堆都是散落的，可以根据需要动态增长。栈中只需记录堆中的一个地址即可。
> - 从面向对象的角度考虑，对象的属性就是数据，存放在堆中。而对象的行为，就是运行逻辑，其数据放在栈中。

参考<br>
[数据类型与垃圾回收](https://blog.csdn.net/zhuhuiby/article/details/6832091)<br>
[堆、栈的地址高低？ 栈的增长方向？](https://www.zhihu.com/question/36103513)<br>

### 重构

- if/else嵌套优化

参考<br>
[6个实例详解如何把if-else代码重构成高质量代码](https://blog.csdn.net/qq_35440678/article/details/77939999)


### 个人实践

q:异常处理写到什么地步?
>最近在给lua写单测，发现代码需要处理的异常逻辑很多。但是，我看lua源码的时候，发现它对于底层一些辅助函数的设计，根本不考虑异常处理。后来从君哥那得知，这种设计需要上层保证调用的正确。否则，底层的逻辑太臃肿。
那么需要我们异常处理的时候，尽量克制。我有一些原则：
1.编译期期间能发现的，不做处理。比如保证参数的个数
2.预发布期间能发现的，不做处理。

看下面这段代码，首先就没有异常处理。这是一个问题。那么怎么设计异常处理。对于policy，group_reason_list这样的变量，如果nil。显然会异常。
但是不进行判断，因为如果异常，预发布就过不去。因此不需要处理。
```lua
for reason, conf in pairs(self.policy) do
    if self.group[reason] then
        local group_reason_list = self.group[reason]
        for _, subreason in ipairs(group_reason_list) do
            if conf[subreason] and conf[subreason].order then
                TableSetValue(self.order_mapping, subreason, conf[subreason].order)
            else
                TableSetValue(self.order_mapping, subreason, conf.order)
            end
        end
    else
        if conf.order then
            TableSetValue(self.order_mapping, reason, conf.order)
        end
    end
end
```

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

## 编程范式
### 基础
q:什么是编程范式?
>A style of building the structure and elements of computer programs

q:有那些编程范式？
>Common programming paradigms include:
imperative in which the programmer instructs the machine how to change its state,(imperative programming)
declarative in which the programmer merely declares properties of the desired result, but not how to compute it(declarative programming)

### 函数式编程

q:如何理解函数式编程
>显然，根据上面的划分，函数式编程属于declarative programming的一种。具体来说，
1.treats computation as the evaluation of mathematical functions 
2.avoids changing-state and mutable data. 
用我自己的话来说，以lamda calculus作为思想的语言范式，就是函数式编程。具体来说，支持function abstraction and function application.

参考<br>
[Functional_programming](https://en.wikipedia.org/wiki/Functional_programming)<br>
[Programming_paradigm](https://en.wikipedia.org/wiki/Programming_paradigm)<br>

## 设计模式

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

### proto buffer

- pb初识

q:什么是pb?
>Protocol buffers are Google's language-neutral, platform-neutral, extensible mechanism for serializing structured data.
按照英文的描述，pb只是一种机制，这种机制可以用来序列化结构化数据，并且这种机制是跨语言，跨平台，可扩展的。
跨语言，跨平台，可扩展；三大特性把握好.

q:pb对比json,xml的定义上，有什么区别?
>从上文看，pb是一种机制，但是这种描述，我以为是强调它在序列化时的能力较好，对于更好的把握这个概念，并不好。
我们来看看json的定义：
JavaScript Object Notation is an open-standard file format or data interchange format that uses human-readable text to transmit data objects consisting of attribute–value pairs and array data types
json的这个定义就好多了，显然pb在使用上，也是好json是一样的，作为一种跨语言，跨平台，可扩展的数据交换格式。只不过,json显然没有提供序列化的能力，但是pb提供了。
>
>xml的定义如下：
Extensible Markup Language (XML) is a markup language that defines a set of rules for encoding documents in a format that is both human-readable and machine-readable. 

q:pb自己的理解?
>1.跨语言，跨平台，可扩展的数据交换格式(当然，叫protocal也没毛病)
2.支持序列化(json就不支持)

参考<br>
[Pb语法](https://developers.google.com/protocol-buffers/docs/proto#scalar)<br>
[Pb-cpp相关](https://developers.google.com/protocol-buffers/docs/cpptutorial )<br>
[Pb-generated code](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated#repeatedmessage)<br>
[Pb-序列化](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.message_lite#MessageLite.ByteSize)
[Pb-反射](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.descriptor)<br>
[Pb-反射-demo](https://yrczone.com/blog/article/45)<br>

- pb 语法(proto2)

q:how to defining a message?
>1.Specifying Field Types
2.Assigning Field Numbers
3.Specifying Field Rules
>
>需要注意的点:
1.required字段慎用，pb2里面，必须有定义。但是，如果这个字段在语义上废弃，并不能像optional那样不填即可，还是要写。

q:What's Generated From Your .proto?
>the compiler generates the code, mainly include:
1.getting and setting field values
2.serializing your messages to an output stream, and parsing your messages from an input stream.


- pb字段没对对齐
```
问题背景：约定好协议，进行联调
问题表现：接收端看到可以收到数据，字段a和字段b的值明显是有的。但是，在debug pb的时候，发现没有Key.比如a,b,c,d,e5个field，c,d,e
都能打印出相应的key和value，但是a和b只能打印出value，没有key，只有数字5和6
问题解决：上游同学没有以来pb 的debug string，而是手动把key和value进行解析，打印。返现确实a,b没有key.没有置位。但是，value也是有。
后来发现，这个数字5和6是index.至于为什么是5和6，对照了Pb的field，暂时觉得不会是这2个值。
总之：如果出现类似情形。就是字段没有对其。
```

- pb深入理解

q:pb的优点有哪些？
> A lot of the time we have no control over the network that the application is on and therefore our only control over this performance factor is the amount of data we are sending across it.
Binary message formats allow us to compress data much more efficiently than other formats and therefore are a good match to c++.

ps:第一篇参考文献有一点特别棒，对比了xml和json，需要翻作者的blog


q:序列化接口的选择?
```ParseFromArray/SerializeToArray```

```cpp
void do_io_event(int clnt_sfd) {
  // recv from client
  char buf[BUF_SZ];
  ssize_t nread = read(clnt_sfd, buf, BUF_SZ);

  cal::CalRequest req;
  req.ParseFromArray(buf, nread);

  if(!req.has_seqno() || !req.has_left() || !req.has_right() || !req.has_optr()) {
    fprintf(stderr, "%s\n", "Invalid CalRequest.");
    return;
  }

  // do caculation
  cal::CalResponse res;
  do_cal(req, &res);

  // send to client
  int sz = res.ByteSize();
  res.SerializeToArray(buf, sz);

  write(clnt_sfd, buf, sz);
}
```

```cpp
bool	
ParseFromArray(const void * data, int size)
//Parse a protocol buffer contained in an array of bytes.

bool	
SerializeToArray(void * data, int size) const
//Serialize the message and store it in the given byte array.
```

通过这个demo，我看可以看出。```SerializeToArray```这个接口的使用，buf和sz都没有什么问题。```ParseFromArray```这个接口的问题在于，sz如何确定，还是需要知道

```ParseFromString/SerializeToString```

```cpp
bool SerializeToString(string* output) const 
// serializes the message and stores the bytes in the given string. Note that the bytes are binary, not text; we only use the string class as a convenient container.
// 这点特别要注意，不是unp当中提到的text strings，就是binary bytes，不是character bytes.即前者不具有可解释性
bool ParseFromString(const string& data);
// parses a message from the given string.
```

```cpp
void do_io_event(int clnt_sfd) {
  // recv from client
  char buf[BUF_SZ];
  ssize_t nread = read(clnt_sfd, buf, BUF_SZ);

  cal::CalRequest req;
  std::string recv_buf(buf, nread);
  req.ParseFromString(recv_buf);

  if(!req.has_seqno() || !req.has_left() || !req.has_right() || !req.has_optr()) {
    fprintf(stderr, "%s\n", "Invalid CalRequest.");
    return;
  }

  // do caculation
  cal::CalResponse res;
  do_cal(req, &res);

  // send to client
  std::string send_buf;
  res.SerializeToString(&send_buf);

  write(clnt_sfd, send_buf.data(), send_buf.size());
}
```

对于这两个接口，注意如下：
1.SerializeToString没什么说的，write时，注意是send_buf.data()，之前一直写的是send_buf.c_str(). 通过c_str()我们知道，这个方法，返回的是包括null-terminated sequence of characters.
通过上面的接口，我们知道，string做的只是一个convenient container，是binary bytes.
data：returns a pointer to the first character of a string 
c_str：returns a non-modifiable standard C character array version of the string 

2.ParseFromString这个接口，虽然是对string进行反序列化，但是string的构造要注意，不能直接通过buf，否则会截断

参考<br>
[Binary Message Format C++ Examples](https://www.thomaswhitton.com/blog/2013/07/03/binary-message-format-c-plus-plus-examples/)

### 序列化(Serialization)

q:序列化的概念是什么?
>Serialization is the process of converting an object into a stream of bytes to store the object or transmit it to memory, a database, or a file.
简单来说，就是把对象专户为字节流进行传输的过程.

q:序列化为什么一定要把对象转化为字节流？直接发送binary structures不行吗?
>考虑unp 5.18 data format的例子，如果直接发送结构体会导致如下问题：
1.字节序的问题，会导致对于binary numbers(binary structures)有不同的解释,大端小端，导致接受到的数据出错。
2.不同平台导致的类型解释不一致。比如有些平台，整型4 bytes，有些是2 bytes，导致接受到的数据出错。
3.结构体对齐不一致，导致对于binary numbers(binary structures)有不同的解释，导致接受到的数据出错
>
>下面给出英文的解释：
1.Different implementations store binary numbers in different formats.
2.Different implementations can store the same C datatype differently.
3.Different implementations pack structures differently
>导致这一问题的根本原因是，发送的是binary structures，接收端会直接解释binary structures.
>
>比如，int a = 3;write(sfd, &a, sizoef(a)); 直接直接这么发送，发送了4个字节。但是对端是16 bit int，那么肯定会接受错误。包括字节序的问题。
>所以，正是因为不能直接发送binary structures，才是我们需要序列化的原因

q:是不是只有序列化这一种方法才能解决上述问题呢?
>unp 给了一些建议：
1.passing all numering data as text strings.
2.Explicitly define the binary formats of the supported datatypes (number of bits, bigor
little-endian) and pass all data between the client and server in this format. RPC
packages normally use this technique.
>
>先来解释第一种，第一种方法可行的原因在于，利用char进行发送，相当于用ascii对于binary structure的numeric data进行编码，当然，也只能是singular data，结构体还是不行。
因为，char类型就占一个字节，不存在不同平台解释不一致的原因，也没有字节序的问题。
第二种，我不知道是不是现在pb的做法。

q:序列化的目的是什么?
>通过上面的讨论，我们知道如果直接发送对象(binary structures)，其实会破坏对象的状态，因为接收端无法得到正确的对象。
所以，序列化这个过程是希望在进行对象传输的时候，可以把对象的状态进行正确保存，然后再进行发送。在接收端再恢复对象的状态。
Its main purpose is to save the state of an object in order to be able to recreate it when needed. The reverse process is called deserialization.
>
>整个过程有点类似于快递的过程，比如快递要给你发一张桌子，他不可能直接把拼好的桌子发送给你，这样运输的流程中，这个对象肯定会遭到破坏。发送方会先把桌子拆了，然后给出安装图纸。把桌子和图纸，一起通过快递
发送给客户。客户拿到图纸和被拆了的桌子，根据图纸进行重新拼接，然后恢复。

q:序列化的过程都有哪些操作?
>1.拆桌子，肯定需要把对象转化为字节流
2.画图纸，给出字节序，大小端，可能也有结构对其，即从字节流重新构造这个对象的信息。
The object is serialized to a stream that carries the data. 
The stream may also have information about the object's type, such as its version, culture, and assembly name. From that stream, the object can be stored in a database, a file, or memory.

参考<br>
[Serialization ](https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/serialization/)


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
Synchronization is the coordination of events to operate a system in unison. 
The conductor of an orchestra keeps the orchestra synchronized or in time. 
Systems that operate with all parts in synchrony are said to be synchronous or in sync and those that are not are asynchronous.
这个定义和上面基本一直，同步就是对不同事件的协调，使得他们在某一个指标上(声音是否整齐)保持一致。
```
- 通信当中的同步
下面讲到比较熟需的一个场景，通信当中的一致。和时序电路一个意思。
```
In electrical engineering terms, for digital logic and data transfer, a synchronous circuit requires a clock signal. 
However, the use of the word "clock" in this sense is different from the typical sense of a clock as a device that keeps track of time-of-day; 
the clock signal simply signals the start and/or end of some time period, often very minute (measured in microseconds or nanoseconds), that has an arbitrary relationship to sidereal, solar, or lunar time, or to any other system of measurement of the passage of minutes, hours, and days.
举个例子，打一下拍子，开始；再打一下拍子，停止。
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

在wiki上，异步的定义很多。我们直接说重点，我们一般在讨论同步/异步时，主要是指同步io/异步io
在这个背景下，我们再继续讨论，会牵扯阻塞和非阻塞进来。

- 同步调用/异步调用
>
同步：在发出一个同步调用时，在没有得到结果之前，该调用就不返回。(这里的理解和os当中进程同步一致，属于直接限制)根据日常经验，大部分调用都属于同步调用。比如，int a = add(a,b)，显然通常情况下这是一个同步调用，该函数一经调用，当前函数stack frame进行相关信息保存，新的函数stack frame入栈。只有调用函数执行完成，才会恢复调用函数。
异步：在发出一个异步调用后，调用者不会立刻得到结果，该调用就返回了。
同步调用和异步调用，强调的是一个调用是否立即返回。和进程和线程的状态没有关系。

- 阻塞调用/非阻塞调用
>一个线程/进程经历的5个状态， 创建，就绪，运行，阻塞，终止
阻塞调用是指调用结果返回之前，调用者会进入阻塞状态等待。只有在得到结果之后才会返回。
非阻塞调用是指在不能立刻得到结果之前，该函数不会阻塞当前线程，而会立刻返回。
所以，阻塞调用和非阻塞调用是指进程/线程的状态，和调用是否立即返回没有关系

上面两组概念，就有4种组合。

这里需要特别强调一点的是，只有当一个调用涉及到io的时候，我们才考虑同步调用/异步调用，阻塞调用/非阻塞调用，这类事情才有意义。试想主函数调用一些四则运算函数，考虑同步\异步，阻塞\非阻塞没什么意义。
都是cpu计算，很快。通常情况下，caller发出一个调用，必然是对结果有依赖，所以同步是很正常的理解。因为只有依赖你的结果，才需要发生调用。同步很正常。前面也说道，cpu计算很块，没有必要还切换上下文，同步非阻塞就够了。

但是，如果一个调用(函数)，牵扯到io，会非常慢。比如，调用标准输入，标准输出这样的函数。与其等用户缓慢的数据，不如先把处理机释放掉。

>
同步阻塞调用：得不到结果不返回，线程进入阻塞态等待。大家一般都认为同步调用是阻塞，那是因为同步io通常很慢，比如同步调用一个函数，但是这个函数会请求一个io，那么当前进程/线程可以进入阻塞状态。
同步非阻塞调用：得不到结果不返回，线程不阻塞一直在CPU运行。简单例子，大部分不耗时的调用都是同步非阻塞，所以本质上来说，大部分同步调用都是非阻塞的，只有在执行包含io的同步调用时，才有可能是同步阻塞。
异步阻塞调用：异步调用，当异步调用包含io是，因io很慢，所以函数直接返回。但是，此时会把自己(进程/线程)阻塞起来，显然直观理解没必要。
异步非阻塞调用：直接讨论异步io调用，io很慢，没有必要耗在io上，所以直接异步返回。但是此时没有必要阻塞自己，去做其他事就好。剩余依赖异步io调用结果的逻辑，放在回调里就好。


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

补充一下自己的理解，这两天对于RPC有了一定新的认识。首先RPC肯定不是socket也不是http，socket是内核提供的网络服务fd，http是一种应用层协议，这两个东西都是实现网络服务的基础，socket的层次比http更低。对于RPC而言，则是在网络服务的基础上，又增加了一部分东西(这部分东西暂时我还不能清晰描述)，提供的一种远程服务调用。作为对比，考虑这样一个例子：
>本地服务，可以启一个echo server，当然，我这个echo server支持并发的功能，然后添加一个client，执行这个过程socket.send(clt_fd, message)，然后server调用echo(msg)这个函数去处理，这个函数内部会把msg丢给一个线程池，然后fd也丢给任务线程，由任务线程最后把结果发回给client。这是个非常清晰的过程。
我们考虑，如果是rpc，是不是也是一样的过程。本地服务没有太多变化，但是client端，不是显示的的发送数据，而是调用远程的echo函数，这个过程，对于下层屏蔽了网络这一部分。所以，rpc调用，肯定有网络服务作为基础来进行支撑的。之所以说这个过程，其实想表达的是，对于服务端的写法，逻辑上应该没有太多变化，但是需要考虑暴露接口，这个是肯定的，我们从brpc中已经可以看到，这一部分内容也是通过pb进行暴露的。

参考<br>
[RPC、HTTP、Socket区别](https://zhuanlan.zhihu.com/p/48760074)

### brpc

参考
[brpc学习 ParallelChannel的使用](https://www.twblogs.net/a/5c00fc12bd9eee7aed339fe4/zh-cn)

### Json

参考阮一峰对于json的理解，对于数据格式和json都有更进一步的理解。
阮一峰谈到，从结构上看，所有的数据(data)都可以分解成三种类型
>第一种类型是标量（scalar），也就是一个单独的字符串（string）或数字（numbers），比如"北京"这个单独的词。
>
>第二种类型是序列（sequence），也就是若干个相关的数据按照一定顺序并列在一起，又叫做数组（array）或列表（List），比如"北京，上海"。
>
>第三种类型是映射（mapping），也就是一个名/值对（Name/value），即数据有一个名称，还有一个与之相对应的值，这又称作散列（hash）或字典（dictionary），比如"首都：北京"。

q:对于他的这个划分，我们怎么理解？
>从已有的知识来看，当我们谈数据的结构的时候，我们就是在说数据结构，常见的数据结构有，集合，线性表，树，图，哈希表(散列)这5种，这个毋庸置疑。上文中提到的结构，标量，序列和映射，是物理层面进行分解后最终不能再进行划分的数据结构。其实也对。你想想，集合，线性表，树，图都是可以用序列来表示的。而哈希表就是映射，标量对应到任何一门语言的基础数据类型。

q:json的规则是什么？
- 序列用[]表示，序列的元素可以是一个标量，也可以是一个序列或者映射，但是元素必须一致。序列的元素用","分割
- 映射用{}表示，映射的元素是一个k-v pair，k-v pair用":"表示，映射的元素用","分割

注意，序列和映射都可以由多个元素构成，但是语义并不一样。序列的元素是并列的，映射的元素没有并列的语义，映射的元素本质上代表的是一个属性的维度。
最后说一下，Json是一种数据交换格式，和xml一样，具有描述对象的能力。

参考<br>
[数据类型和Json格式](http://www.ruanyifeng.com/blog/2009/05/data_types_and_json.html)<br>

## 操作系统(Linux)

### 编译问题集锦

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


### 高级语言的compile/link/load
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

### linux elf格式

- 引言

- 分类


## 数据库

## 工具

### 版本控制

- git


q:git修改上次提交?
>主要说下，怎么修改username和email
```c
git commit --amend --author="userName <userEmail>"
```

参考<br>
[git中 修改已提交的commit的用户名邮箱](https://www.jianshu.com/p/7def4f387e9f)

q: merge/rebase的区别
>先说下merge，merge比较简单，就是分叉的分支，进行一次三路合并。第3路是最近common ancestor.
需要注意的一点是，commit message会写在同一列。但是commit符号(*)并不在一起。我以前总看错

### 网络服务

- wireshark

### 构建(编译)

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

- blade

### Tex

对几个概念先进行简单描述：
>
TEX: 一种排版语言
Latex: 基于TEX实现的一个宏集，自然也是一种排版语言
>
Tex Live: TEX/TEX based 语言编译器的发行版，如g++, visual c++。目前使用的CTEX = Tex live + 中文支持
>
Tex Studio: IDE

## 质量/测试

这一节我觉得需要说的，如果开发不能保证好的质量，都交给测试去做。只会降低效率。打铁还需自身硬，一方面对于语言本身精益求精，另一方面对于测试理论的了解，我觉得有助于写出更好的代码。

### 测试理论

- 基本概念

q:什么是单元测试？
>单元测试（unit testing），是指对软件中的最小可测试单元进行检查和验证.
对于单元测试中单元的含义，一般来说，要根据实际情况去判定其具体含义，如C语言中单元指一个函数，Java里单元指一个类，图形化的软件中可以指一个窗口或一个菜单等。
总的来说，单元就是人为规定的最小的被测功能模块

q:什么是回归测试？
>在新版本上运行所有已通过的测试用例以验证有没有“退化”情况发生，这个过程就是一个 Regression Test

q:如何理解回归测试当中的回归？
>Regress 的英语定义是： return to a worse or less developed state。是倒退、退化、退步的意思。
在软件项目中，如果一个模块或功能以前是正常工作的，但是在一个新的构建中出了问题，那这个模块就出现了一个“退步”（Regression），从正常工作的稳定状态退化到不正常工作的不稳定状态。
所以，对于回归测试的理解主要在于，一个新的构建当中是否有模块测试失败。但是，我的疑问是，如果有测试失败，它连单测都过不了，所以对于单侧和回归测试的区别需要梳理清楚，再讨论这个问题。

参考<br>
[软件测试笔记（一）理论篇](https://testerhome.com/topics/8594)<br>
[单元测试和回归测试](http://www.uml.org.cn/Test/201112285.asp)<br>


q:什么是test case?
>In software engineering, a test case is a specification of the inputs, execution conditions, testing procedure, and expected results that define a single test to be executed to achieve a particular software testing objective, such as to exercise a particular program path or to verify compliance with a specific requirement.
>
>对于这句话的理解，我是这么想的，为了达到测试目的，一组程序输入，执行条件，测试过程，以及期待结果的说明。当然前半句话说的比较泛，后面这句比较具体：对于特定程序执行逻辑的测试，来验证是否满足预期的结果。
>
>总结下什么是测例，以一组输入的形式给出，为了测试程序的某条执行逻辑，是否符合预期。

q:什么是test suite?
>In software development, a test suite, less commonly known as a 'validation suite', is a collection of test cases that are intended to be used to test a software program to show that it has some specified set of behaviours.
>
>我的理解，一组相关的测试用例。或者说是，测试程序特定功能的测例集合。

我们可以看下gtest给出的例子，从名称上就能看出来。

```cpp
// gtest提供的测试方法
TEST(TestSuiteName, TestName) {
  ... test body ...
}

TEST(AddTest, HandlePositive) {
  EXPECT_EQ(add(1,2), 3);
}

TEST(AddTest, HandleZero) {
  EXPECT_EQ(add(0,3), 0);
  EXPECT_EQ(add(3,0), 0);
  EXPECT_EQ(add(0,0), 0);
}

TEST(AddTest, HandleNegative) {
  EXPECT_EQ(add(-1,1), -1);
  EXPECT_EQ(add(1,-1), -1);
  EXPECT_EQ(add(-1,-1), -1);
}
```

q:什么是xUnit architecture?
>xUnit is the collective name for several unit testing frameworks that derive their structure and functionality from Smalltalk's SUnit.
简言之，从SUnit发展而来的测试框架，我们都叫做xUnit测试框架

q:xUnit测试框架特点？
>All xUnit frameworks share the following basic component architecture, with some varied implementation details:
- Test runner: A test runner is an executable program that runs tests implemented using an xUnit framework and reports the test results
- Test case: A test case is the most elemental class. All unit tests are inherited from here.
- Test fixtures: A test fixture (also known as a test context) is the set of preconditions or state needed to run a test. The developer should set up a known good state before the tests, and return to the original state after the tests.
- Test suites: A test suite is a set of tests that all share the same fixture. The order of the tests shouldn't matter.
- Test execution: The execution of an individual unit test proceeds as follows: 
- Test result formatter: A test runner produces results in one or more output formats
- Assertions: An assertion is a function or macro that verifies the behavior (or the state) of the unit under test.Usually an assertion expresses a logical condition that is true for results expected in a correctly running system under test (SUT).

参考<br>
[XUnit](https://en.wikipedia.org/wiki/XUnit)<br>

### 测试框架


- gtest

[How to set up googletest library from source?](https://lchsk.com/how-to-set-up-googletest-library-from-source.html)


## 其他

- 回车换行(CRLF)

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
