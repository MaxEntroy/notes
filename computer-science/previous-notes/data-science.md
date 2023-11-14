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

q:embedding?
>看到一个非常好的解释：
Embed这个词，英文的释义为, fix (an object) firmly and deeply in a surrounding mass, 也就是“嵌入”之意。例如：One of the bullets passed through Andrea's chest before embedding itself in a wall.
>
>另外，这个词（的分词形式）在数学上也是一个专有名词，Embedding，它广泛存在于包括代数、拓扑与几何等诸多数学领域。它主要表征某个数学结构中的一个实例被包含在另外一个实例中，例如一个group它同时又是一个subgroup
当我们说某个对象 X 被嵌入到另外一个对象 Y 中, 那么 embedding 就由一个单射的、结构保持的（structure-preserving）映射 f : X → Y 来给定的。此处的结构保持的具体含义要依赖于X 和 Y 是哪种数学结构的实例而定

q:word embedding misunderstanding?
>们在把单词嵌入进另外一个空间时，要做到单射和structure-preserving，或者说我们更专注的是映射关系，而最终得到每个单词在另外一个空间中的表达也仅仅是之前设计好的映射关系的很自然的表达

>这里需要特助注意的是，由于wor2vec已经变成主流的word embedding方法，所以一般当大家在nlp领域说word embedding时，说的就是w2v.但是，不要误会了。w2v只是word embedding的一种方法。
本质要清楚的是，embedding是一种映射，并且这个映射具备它自己的特点。

q:embedding的作用？
>Embedding 是一个将离散变量转为连续向量表示的一个方式
结合我们上文提到的one-hot编码来看，看起来都是0,1，不是连续向量。
这里的理解有误，其实将字符串形式的分类，映射到实数空间，已经转换为连续向量了。你也可以把0,1替换了0.0和0.1

q:目前有哪些embedding方法?
- interger encoding
- one-hot encoding
- cocurrence
- w2v

q:embedding目前主要用在哪？
- word embedding
- entity embedding(类别数据)

参考<br>
[Word Embedding与Word2Vec](https://blog.csdn.net/baimafujinji/article/details/77836142)<br>
[Embedding 的理解](https://zhuanlan.zhihu.com/p/46016518)<br>
[深度学习推荐系统 | Embedding，从哪里来，到哪里去](https://zhuanlan.zhihu.com/p/138310401)

## 一些概念

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

## 推荐系统

### 推荐引擎
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

### 召回算法

下面的总结，我参照了厂内的一篇文章，在此做读书笔记。作者对于召回算法的分类，依据召回算法使用的数据来进行分类

- behavior information(协同类召回)

这一类基于用户的行为数据，通常也叫作协同过滤。协同过滤的本质是一种基于统计的算法，最初的做法并没有模型的能力在里面。
目前协同的做法主要分一下两类：
1. memory-based. 基于统计出来的u2u,i2i关系，计算相似关系，得到推荐结果
2. model-based. 基于隐变量模型，主要是一些矩阵分解，pLSA,LDA。通过模型计算出u2u,i2i关系，在这个基础上计算相似关系，得到推荐结果。

显然，model-based泛化能力相对更好，比如可以看下面这个列表
| 左对齐 | kobe | jordan | brian willams | winfrey |
| :-----| ----: | :----: | :----: | :----: |
| lee | 1 | 0 | 0 | 0 |
| jerry | 0 | 1 | 0 | 0 |
| marry | 0 | 0 | 1 | 0 |
| james | 0 | 0 | 0 | 1 |

对于这个矩阵，直接看起来，这4个user，他们没有任何交集，从某种意义上来说，无法推荐。但是，当我们提取tag的语义之后，我们其实可以发现，前2个是篮球运动员，后两个是主持人。
所以，这个共现矩阵我们可以近似的分解为下面的两个矩阵
| 左对齐 | player | host |
| :-----| ----: | :----: |
| lee | 1 | 0 | 
| jerry | 1 | 0 | 
| marry | 0 | 1 | 
| james | 0 | 1 | 
此时，计算u2u关系，更容易得到lee和jerry是相似的结论，从而可以进行推荐

| 左对齐 | kobe | jordan | brian willams | winfrey |
| :-----| ----: | :----: | :----: | :----: |
| player | 1 | 1 | 0 | 0 |
| host | 0 | 0 | 1 | 1 |
同理，此时计算i2i关系，更容易得到kobe和jordan是相似的结论，从而可以进行推荐

- side information(内容召回or模型召回)

除了用户行为数据，我们还可以拿到user infomation and item information，这个是用户或者物品本身的属性。
协同算法一般不会利用side information。当时当利用side information时，通常会和behavior information一起使用。

1.内容召回.
1.1.基础的做法是利用用户画像中的cat/tag信息，进行相关文章的召回
1.2.也可以做embedding召回。这里利用过去的文章信息，利用title正文信息，学习出一个item embedding信息，我们可以用来简介表示user.然后，和文章池当中item embedding进行匹配。主流的做法是可以利用faiss
2.模型召回。
2.1. 基础的就是上一个lr这样的模型
2.2. 目前主流的做法是上一个深度模型。

对于1.2和2.2的区别，我是了解的。前者是根据相似度进行pctr计算，后者是利用feature进行pctr计算

在第二类算法中，最常见的模型就是CTR模型.
1. CTR模型本质上是一个二分类的分类器，用得比较多的是LR、xgboost、lightGBM等分类器
2. 其中，behavior information和side information被用于构造训练样本的feature and label.
3. 分类器完成训练后，通过预测user点击item的概率，可以把topK概率最大的item作为推荐结果推送给用户

假设我们用user information, item informaton, behavior infomation训练一个lr，简单构造如下样本
(age, title, click_cnt)，这个样本有来自三方面的信息，我们训练完成一个lr.进行在线预测时，
1.age 从user profile获得
2.title 从进行打分的文章获得，click_cnt根据打分的文章获得
3.通过以上信息，我们可以给到lr进行pctr计算


q:key point?
>比起纯行为的协同过滤，使用了side information 的CTR模型通常会取得更好的推荐效果。而整个CTR模型取胜的关键，在于如何结合side information和行为数据构造出具有判别性的用户特征、物品特征以及交叉特征

q:当前主流的ctr模型？
>近五年来，基于深度学习的CTR模型逐渐发展起来，在不少应用场景下取得了比传统CTR模型更好的效果
相比于传统的CTR模型，深度CTR模型有着自己独到的优势