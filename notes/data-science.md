## 数据预处理

#### one hot encoding
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


#### embedding
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

#### Feed流

参考知乎杨亮的答案，写的比较合理
>是一种呈现内容给用户并持续更新的方式
我理解的feed，呈现内容给用户，这个过程，好比feed过程，把用户感兴趣的内容呈现(feed)给用户。
流又怎么理解呢?持续更新，持续的feed，这种时间上的连续性，我理解为流。因为flow这个概念本身就是有事件连续性的。

下面是水球泡的解释，我觉得也好，对于feed的理解大同小异。主要说下flow
>第二个词，流，就是他的呈现形式，就是这个信息怎么呈现的，大多数的都是用时间排列的形式呈现的，就是上面有人回答timeline形式，还有其他的呈现形式，就不多做赘述了。
和上面的描述类似，持续更新，即timeline的形式，呈现内容。这个叫做flow.

参考<br>
[什么叫feed流？](https://www.zhihu.com/question/20690652)