[TOC]

## 基础算法

### 概率算法

#### 随机数相关

q:已知rand生成1-7随机数，如何生成1-5随机数
>先说一个错误的做法：
1.1-5不动
2.6-7对5取mod
这么做的问题在于，P(x=1) = 2/7, P(x=2) = 2/7, P(x=3)=P(x=4)=P(x=5) = 1/7
>
>正确的做法：
对于6和7，拒绝即可。这样保证1-5的概率一致。但是这有一个问题是，6和7的生成是无效生成，需要再次生成。增加耗时

q:已知rand生成1-7随机数，如何生成1-10随机数
>直接说一个答案，我觉得非常好。我之前的考虑是，两次随机，得到49个数，我只要分布在2-10的。多余的剪枝，2-10概率不均的，剪枝。
但是这样会产生大量无用的试探。增加耗时。
>答案给了一个非常好的思路，他不在乎每次的数值，每次的数值只是用来当做一个坐标。这个坐标标识了40个数字当中的顺序，具体可以映射到某一个1-10的数子。
前40个坐标是，A11-A17,A21-A27,A31-A37,A41-A47,A51-A57,A61-A65，这里总共是40个坐标。他们对应1-40这40个整数。每个数mod10，得到了他们的数字。
具体流程：
1.抛两次筛子，获得坐标
2.获得坐标对应的序列当中的num
3.num%10即可

```cpp
int rand10() {
  int row, col, idx;
  do {

  }while(idx > 40);
  return idx % 10 + 1;
}
```

>这里还有一种办法，比较trick.这个办法的思路是，把10分成两部分，第一次判断落在那个区间，第二个给出这次的值
1.把rand7()想象成一个7面的骰子🎲，现在我们有两个骰子
2.第一次如果掷出1,2,3,4,5,6就掷下一个骰子；如果掷出7，就继续掷这个骰子，直到不是7为止
3.第二次如果掷出1,2,3,4,5就结束；如果掷出6,7，就继续掷这个骰子，直到不是6,7为止
4.如果第一次掷出1,2,3，则first记为0；如果第一次掷出4,5,6，则记first为5；记第二次掷出的为second
5.最后返回first+second

```cpp
int rand10 {
  int first, second;
  do {
    first = rand7();
  }while(first == 7);

  do {
    second = rand7();
  }while(second > 5);

  return (first/4)?5+second:second;ssssss
}
```

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