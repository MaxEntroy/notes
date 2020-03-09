## 数学

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

### 完备(completeness)
一个抽象的数学概念，我们先来看看wikipedia的一些通用解释，方便有一个基础的认知：
>The completeness of the real numbers, which implies that there are no "holes" in the real numbers
这个解释我觉得特别好，特别直观，我们可以得到一个简单通用的描述，一个完备的系统是一个没有漏洞的系统。没有数学描述，不严谨，但是好了解，直观

参考<br>
[Completeness](https://en.wikipedia.org/wiki/Completeness)

## 数据挖掘

### one hot encoding
- 定义
```
One-hot encoding refers to a way of transforming data into vectors where all components are 0, except for one component with a value of 1.
eg:
red = [1,0,0]
blue = [0,1,0]
yellow = [0,0,1]
```
- 类别数据
```
A "pet" variable with the values: “dog” and “cat“.
A "color" variable with the values: “red“, “green” and “blue“.
A "place" variable with the values: “first”, “second” and “third“.
```
简单来说，pet这个特征，它的数据是可枚举的。即离散不连续。
但是对于机器学习来说，有时候需要离散数据。
- integer encoding
显然，对于上述的类别数据，我们可以有如下的编码
```
pet: dog = 0, cat = 1
color: red = 0, green = 1, blue = 2
place: first = 1, second = 2, third = 3
```
对于这种编码，0,1,2本来是有ordinal关系的，所以像place就可以很好的表达。因为place这个变量的值是有ordinal关系的。
但是，pet,color的值，是没有ordinal关系的。那么这种编码对于机器学习算法来说，可能会误用编码之间的关系。

- one hot encoding
引入二进制编码，对于color而言，它有三个枚举值，因此需要三个二进制变量，没一位用来表示一个变量。
所以，会有如下的结果
```
red   green   blue
1     0       0
0     1       0
0     0       1
```
引入二进制编码的根本原因是为了避免interger encoding所代理的natural ordering.

参考<br>
[One-hot encoding](https://machinelearning.wtf/terms/one-hot-encoding/)<br>
[Why One-Hot Encode Data in Machine Learning?](https://machinelearningmastery.com/why-one-hot-encode-data-in-machine-learning/)<br>
[机器学习之one hot encoding详解](https://www.jianshu.com/p/cb344e1c860a)<br>


### embedding
- word representation
上述谈到的one-hot encoding有一个很重要的作用，就是可以对词进行编码。
考虑如下的两句话：
Have a good day.
Have a great day.
```
 V = {Have, a, good, great, day}.

 Have = [1,0,0,0,0]`; a=[0,1,0,0,0]` ; good=[0,0,1,0,0]` ; great=[0,0,0,1,0]` ; day=[0,0,0,0,1]` (` represents transpose)
```

- embed是什么意思？
```
先给出朗文高阶(6th)的解释
to put something firmly and deeply into something.
那么，我们不妨这么说：
to put word firmly and deeply into vector space.
所以，word embedding本质是一种word representation.
```

参考<br>
[Introduction to Word Embedding and Word2Vec](https://towardsdatascience.com/introduction-to-word-embedding-and-word2vec-652d0c2060fa)<br>
[【NLP】 詞嵌入 (word embeddings) 的基本概念](https://www.kesci.com/home/project/5b7a359e31902f000f55152f)<br>
[What the heck is Word Embedding](https://towardsdatascience.com/what-the-heck-is-word-embedding-b30f67f01c81)<br>

## 业务场景

### Feed流

参考知乎杨亮的答案，写的比较合理
>是一种呈现内容给用户并持续更新的方式
我理解的feed，呈现内容给用户，这个过程，好比feed过程，把用户感兴趣的内容呈现(feed)给用户。
流又怎么理解呢?持续更新，持续的feed，这种时间上的连续性，我理解为流。因为flow这个概念本身就是有事件连续性的。

下面是水球泡的解释，我觉得也好，对于feed的理解大同小异。主要说下flow
>第二个词，流，就是他的呈现形式，就是这个信息怎么呈现的，大多数的都是用时间排列的形式呈现的，就是上面有人回答timeline形式，还有其他的呈现形式，就不多做赘述了。
和上面的描述类似，持续更新，即timeline的形式，呈现内容。这个叫做flow.

参考<br>
[什么叫feed流？](https://www.zhihu.com/question/20690652)

### 推荐系统

- 引擎(Engine)
我们先来看剑桥英文字典给的定义，
>a machine that uses the energy from liquid fuel or steam to produce movement.
这个解释很直观了，把燃料变成热能，促进物体运动的机器，就是发动机。这也是最基本的定义

我们再来看一个引申的定义，
>something that provides power, often economic power, for other things
提供动力的部分。

我们结合系统来看，对于推荐系统而言，推荐引擎又是什么呢？综合以上两个结论，我们可以知道，推荐引擎是为推荐系统提供动力的部分，即驱动推荐系统工作的部分。
那么，当我们说一个系统当中的引擎时，我们其实说的是，为这个系统提供动力，驱动系统工作的部分。比如，汽车系统的引擎就是发动机。

再说回我们的推荐系统，推荐引擎具体是什么呢？又或者说，任何一个系统，它的引擎又具体是什么呢？上文我们说了，是驱动整个系统工作的部分，落地到代码层面，其实就是这个系统的整体**框架设计**部分代码，因为这一部分代码是驱动整个系统工作的部分。它并不描述每个部分应该怎样工作，而是描述这些部分组合到一起是怎样工作的，即整个系统的各个部分，是在怎样的组织设计下，被驱动工作的。这是引擎的真正含义。

总结，从逻辑上来说，推荐引擎是推荐系统的核心，承担的是驱动推荐系统工作的部分。物理上来说，推荐引擎是推荐系统整个框架设计部分的代码，它并不描述每个部分是怎样工作的，而是描述每个部分是在怎样的组织设计下，被驱动工作的。

## 计算科学

### 图灵机

q:为什么之前一直不理解？
>以前读书的时候，发现tm模型很简单，但是没有理解这种简单的含义在哪里？即它到底抽象了什么东西？比如回调这个思想非常简单好理解，但是大家看回调总觉得很难，因为没有理解到回调的思想到底是什么？即控制反转的思路，所以以至于回调看起来那么简单，但就是大家不理解。对于tm，我面临了一样的问题，我一向是崇尚简单可解释的，但是tm这个简单可解释的机器，我一直没有理解其简单之道在哪里？还是不懂它到底抽象了什么东西？

我是在看了[非数学，非计算机专业的我，对编程什么的一窍不通的我要怎么理解图灵机的概念？](https://www.zhihu.com/question/22602657) Invalid s的回答之后，豁然开朗。
他主要对于以下节问题做了清晰的解答。

q:什么是计算？
>计算就是通过一些机械动作，把问题映射到答案的过程。(从集合论的角度去考虑，我发现形式语言的东西也多从集合论入手，为什么是集合论，这个暂时也不深究)至于你的机械动作是数手指，还是用算盘，还是更复杂的电子计算机，本质都是这一系列的机械动作。

q:既然一切计算归根结底都可以是机械过程，那么有没有什么机械过程可以模拟一切运算呢？
>图灵说了，可以做一个机械装置，他包含以下部分
1.从纸带上读入内容或输出信息到纸带上
2.移动到纸带上的其他位置
3.有一套控制规则和一个内部状态寄存器。可根据状态寄存器内容以及当前纸带格子里的内容，决定下一步动作(在纸带上移动、读取或者写入，停机)

显然，具有如上构造的tm，可以完成计算这一过程。
这其中非常重要的一点是，然后呢，问题虽然是无限的；但解答某个问题需要的“控制规则”的数目是有限的，所以这些“控制规则”总可以编码然后存到纸带上。

q:什么又是通用图灵机？
>图灵证明了，可以设计一些特殊的控制规则，使得它控制下的图灵机可以从纸带读入任意的控制规则M，然后仿照控制规则M去处理纸带内容。这种可以模拟任意其他计算规则的图灵机就是“通用图灵机”；我们把“足以模拟任意其他计算规则”这个要求称为“图灵完备”。

q:如何理解通用图灵机？
>对于特定问题的规则，总是有限可编码，即可被tm当做输入读取，读取后按照这些规则进行执行，所以通用tm不负责直接计算所有问题，而是读入算子，执行算子，从而达到可以解决任何问题的能力。这么看，人一定是tm。因为tm能做到的，人都能做到。我不会加法，你告诉我加法的规则，我按着规则进行计算，就能有结果，你告诉我减法，我也能做减法。严格来说，规则都是死的，是在课本上的。但是tm和人都是活的。

结论：
说的更浅显一些，就是：支持“通用控制规则”的图灵机，不管它看起来有多简单，但它的确就是一种万能机器。这里抢到，一定要支持通用规则，这代表什么规则动能执行。不过，放在这个角度，人还是不是通用图灵机，因为人不支持通用控制规则，即对于我而言，有我不理解的无法学习的特殊规则，那我我就无法完成这个计算。

q:如何理解imitation game(turing test)?
>从上面对于图灵机的理解来看，通用图灵机在读入规则之后，就是按照(模仿)控制规则去执行。所以，图灵机的过程是一个imitation process。那么，如果这个process 能达到一定的程度(阈值)，我们认为这台tm具备智能。
当然，这是一个简单的理解，我也不能确定理解是否正确，只能说是按着tm去理解了模仿。

参考<br>
[非数学，非计算机专业的我，对编程什么的一窍不通的我要怎么理解图灵机的概念？](https://www.zhihu.com/question/22602657)<br>
[智能哲学：“第三问题”与图灵的“模仿游戏”](https://cloud.tencent.com/developer/article/1133181)<br>


### Lambda calculus
q:什么是lamda calculus
>Lambda calculus (also written as λ-calculus) is a formal system in mathematical logic for expressing computation based on function abstraction and application using variable binding and substitution.
直接给出了wiki的定义，我觉得我还没有直接描述的能力，我们可有总结下他到底是什么：1.一套系统，在梳理逻辑当中用来表示计算 2.基于函数抽象和函数应用

q:如何理解lambda calculus
>It is a universal model of computation that can be used to simulate any Turing machine.简言之，通用计算模型，和tm等价，这是我能理解到的。

q:函数抽象和函数应用怎么理解？
>这两个特性保证了一门函数式语言是图灵完备的。
与图灵机对应，λ-演算的直接影响是函数式编程语言，如lisp、Haskell等，如果说这些函数式语言图灵完备，需要有以下两个特征：
能够进行函数抽象（也就是函数定义）
能够进行函数应用（也就是函数调用）
鉴别一个语言是不是函数式的标准是：这个语言能否在运行时创建函数，如果能，就是函数式语言。

参考<br>
[Lambda calculus](https://en.wikipedia.org/wiki/Lambda_calculus)
[编程语言的基石——Lambda calculus](https://liujiacai.net/blog/2014/10/12/lambda-calculus-introduction/)

### Software framework

q:先简单谈一下自己对于框架和库的理解？
>我理解库还是针对一些非常具体、细节的问题，比如网络通信，这是一个非常落地的，非常细节的问题，我们可以开发一套网络库来解决网络通信
的问题，此时我们不说这是套网络框架。
对于框架，主要针对的是一些相对系统，不是那么具体、细节的问题。比如我们想实现一个推荐系统，工程上需要实现一个推荐框架，这个框架由
各个模块以及各个模块之间的联系构成。对于这其中的某些模块，他们可能会涉及网络通信的问题，此时可以利用之前开发的网络库进行开发。
所以，框架指的是的是更加系统、宏观的问题。而库则是相对具体、细致的问题。

q:wiki是怎么解释软件框架的?
>Software frameworks may include support programs, compilers, code libraries, tool sets, and application programming interfaces (APIs) 
that bring together all the different components to enable development of a project or system.
>
>这句话主要表达了框架的构成，最后一句表明了框架的根本目的是为了解决对于一个庞大工程或者系统的开发。

q:wiki是怎么描述differences between Frameworks and normal libraries?
Frameworks have key distinguishing features that separate them from normal libraries:
- inversion of control: In a framework, unlike in libraries or in standard user applications, the overall program's flow of control is not dictated by the caller, but by the framework.
- extensibility: A user can extend the framework – usually by selective overriding – or programmers can add specialized user code to provide specific functionality.
- non-modifiable framework code: The framework code, in general, is not supposed to be modified, while accepting user-implemented extensions. In other words, users can extend the framework, but cannot modify its code.



