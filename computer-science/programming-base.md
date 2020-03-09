[TOC]

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
[异步消息的传递-回调机制](https://www.ibm.com/developerworks/cn/linux/l-callback/index.html)<br>

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