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
heuristics: the study of how people user their experience to find answers to questions or to improve performance
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
从以上的定义中，我们可以看出该定义对启发式这个东西本身是什么，没有好的解释。而是更多的强调了，在经典方法失效的情况下(np问题，算力达不到)，我们采用启发式方法，在效果上做tradeoff，但是可以换来更快的一个近似解，当然前提是这个近似解可以接受。

综上，启发式算法我们可以给出一个自己的解释：
- 应用场景通常是算力不可解的场景，比如常见的优化类问题，全局解往往是NP-hard问题
- 具体策略不是告知直观的解决策略(算力不可解)，而是通过给出自学习的策略(目标+经验)进行解决
- 一般是一个近似解

- AI/ML
wikipedia多给出了一条启发式算法和AI的关系
```
Heuristics underlie the whole field of Artificial Intelligence and the computer simulation of thinking, 
as they may be used in situations where there are no known algorithms
```
显然，机器学习算法均属于启发式算法。因为，机器学习本质就是自学习，给出的目标函数对应的就是自学习的策略。而最优化问题，一般是算力不可解，所以，采用启发式算法来解决。常建的比如有sgd。具体来说，KMeasn算法，N个点给出K个类别，我们可以枚举所有可行解，但是算力不可解。所以，提出目标函数，采用sgd的方法得到了每个质心的坐标

## 网络/通信

#### protobuffer

参考<br>
[Pb语法](https://developers.google.com/protocol-buffers/docs/proto#scalar)<br>
[Pb-cpp相关](https://developers.google.com/protocol-buffers/docs/cpptutorial )<br>
[Pb-generated code](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated#repeatedmessage)<br>
[Pb-反射](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.descriptor)<br>
[Pb-反射-demo](https://yrczone.com/blog/article/45)<br>

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
