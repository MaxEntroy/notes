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