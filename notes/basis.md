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
参考
[Statements and Expressions](http://interactivepython.org/runestone/static/CS152f17/SimplePythonData/StatementsandExpressions.html)

#### 类型

参考
[Magic lies here - Statically vs Dynamically Typed Languages](https://android.jlelse.eu/magic-lies-here-statically-typed-vs-dynamically-typed-languages-d151c7f95e2b)

#### 回调

参考
[Callback](https://en.wikipedia.org/wiki/Callback_%28computer_programming%29)
[回调函数（callback）是什么？](https://www.zhihu.com/question/19801131)
[如何浅显地解释回调函数](https://bot-man-jl.github.io/articles/?post=2017/Callback-Explained)

## 网络/通信

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

参考
[Mathematical model](https://en.wikipedia.org/wiki/Mathematical_model)
[General linear model](https://en.wikipedia.org/wiki/General_linear_model)
[Set](https://en.wikipedia.org/wiki/Set_(mathematics))