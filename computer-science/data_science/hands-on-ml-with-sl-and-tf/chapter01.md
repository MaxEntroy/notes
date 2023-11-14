
### What Is Machine Learning?

Machine Learning is the science (and art) of programming computers so they can
learn from data.

Here is a slightly more general definition:
- [Machine Learning is the] field of study that gives computers the ability to learn
without being explicitly programmed.(Arthur Samuel, 1959)
- A computer program is said to learn from experience E with respect to some task T
and some performance measure P, if its performance on T, as measured by P, improves
with experience E.(Tom Mitchell, 1997)

定义2比较学术化，定义1的阐述更容易理解，具备两个特点
1. 不用显示求解
2. 从数据中学习

- The examples that the system uses to learn arec alled the training set.
- Each training example is called a training instance (or sample).

### Why Use Machine Learning?

To summarize, Machine Learning is great for:
- Problems for which existing solutions require a lot of hand-tuning or long lists of
rules: one Machine Learning algorithm can often simplify code and perform better.
- Complex problems for which there is no good solution at all using a traditional
approach: the best Machine Learning techniques can find a solution.
- Fluctuating environments: a Machine Learning system can adapt to new data.
- Getting insights about complex problems and large amounts of data.

作者这里着重说了第一点，举了垃圾邮件分类的例子，这类问题如果直接用规则求解，规则需要频繁更新。同时，如果已有数据发生变化，规则改动非常困难。总之，维护很困难。机器学习的做法胜在更新少，除了模型需要更新外，不需要其他更新，同时可以做到一段时间的一劳永逸。

### Types of Machine Learning Systems

There are so many different types of Machine Learning systems that it is useful to
classify them in broad categories based on:

- Whether or not they are trained with human supervision (supervised, unsupervised,
semisupervised, and Reinforcement Learning)
- Whether or not they can learn incrementally on the fly (online versus batch
learning)
- Whether they work by simply comparing new data points to known data points,
or instead detect patterns in the training data and build a predictive model, much
like scientists do (instance-based versus model-based learning)

作者这里先说了下分类的依据，1/2/3在工作中都非常常见。拿推荐来说
- 监督学习
- 批量学习(定时更新)，希望过度到增量学习(实时性更高)
- 基于模型

#### Supervised/Unsupervised Learning

- In supervised learning, the training data you feed to the algorithm includes the desired
solutions, called labels.
    - A typical supervised learning task is classification.
    - Another typical task is to predict a target numeric value, such as the price of a car,
given a set of features (mileage, age, brand, etc.) called predictors.This sort of task is
called regression
    - In Machine Learning an attribute is a data type (e.g., “Mileage”),while a feature has several meanings depending on the context, but generally means an attribute plus its value (e.g., "Mileage = 15,000").

这里说下，有监督学习思路非常简单，就是sample是否有label。拿推荐来说，显然是监督学习，所以有正负样本一说。
这其中，特征比较重要，我们一般都理解为属性，作者这里特地强调了data type + its value

这里多说一下特征，区别 feature and attribute。比如判断一个人是否患有糖尿病，显然，可以查的属性非常多，比如身高，体重，肺活量，是否秃头，血糖。这写东西都是属性，但是不是每一个属性都是特征，根据现有的只是，我们知道是否秃头显然和一个人是否患有糖尿病没有关系。所以，这个属性就不是特征。那到底什么是特征呢？我自己的总结，和label有因果联系的属性，或者说，有用的属性就是特征。

此时，再回过头来，我们看样本。样本和特征又有什么关系？显然，特征构成了样本。所以特征也有sample unit的说法。单个样本又构成了训练集，给到模型输入。大致是如下的关系：

**属性->特征->样本->训练集**

#### Unsupervised learning

In unsupervised learning, as you might guess, the training data is unlabeled. The system tries to learn without a teacher.

#### Semisupervised learning

Some algorithms can deal with partially labeled training data, usually a lot of unlabeled
data and a little bit of labeled data. This is called semisupervised learning.

#### Reinforcement Learning

Reinforcement Learning is a very different beast. The learning system, called an agent
in this context, can observe the environment, select and perform actions, and get
rewards in return.

后面这3种机器学习方式，没有纤细展开，因为暂时和我的工作关系不大。

### Batch and Online Learning

Another criterion used to classify Machine Learning systems is whether or not the
system can learn incrementally from a stream of incoming data.

这里主要还是围绕实时性去说，增量学习vs批量学习

#### Batch learning