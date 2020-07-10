[TOC]

## Overview

q:什么是软件工程?
>Software engineering is the systematic application of engineering approaches to the development of software.
>
>把工程管理的方法应用到软件开发中。计算机科学和管理科学的交集。

q:软件工程诞生的背景？
>上世界70-80年代，随着软件开发的复杂度急剧提升，导致传统的软件开发的方法难以为继。比如项目非常庞大，需求分析如果不做好，
开发返工大家非常大，以及测试做的不好，线上带来的问题非常多。所以希望引入一种科学的方法来对软件开发的流程进行管理，这就是软件工程引入的背景。

q:软件工程的流程大体分几个阶段?
- Software requirements
- Software design
- Software development
- Software testing
- Software maintenance

## UML

### Basis

q:什么是uml?
>The Unified Modeling Language (UML) is a general-purpose, developmental, modeling language in the field of software engineering that is intended to provide a standard way to visualize the design of a system
>
>以上wiki的定义说的很清楚了，我再简化下，
1.建模语言
2.一种标准版化的工具
3.对系统设计进行可视化

q:uml是如何描述系统的?
>UML diagrams represent two different views of a system model:
* Static (or structural) view: emphasizes the static structure of the system using objects, attributes, operations and relationships. It includes class diagrams and composite structure diagrams.
* Dynamic (or behavioral) view: emphasizes the dynamic behavior of the system by showing collaborations among objects and changes to the internal states of objects. This view includes sequence diagrams, activity diagrams and state machine diagrams.

q:uml具体由哪些图?
- Structure diagrams
    - Class Diagram
    - Object Diagram
    - Component Diagram
    - Composite Structure Diagram
    - Deploy Diagram
    - Package Diagram
    - Profile Diagram
- Behavior diagrams
    - Activity Diagram
    - User Case Diagram
    - State Machine Diagram
    - Interaction Diagram
        - Communication Diagram
        - Sequence Diagram
        - Timing Diagram
        - Interaction Overview Diagram

### Class Diagram

- classes,
- their attributes,
- operations (or methods),
- and the relationships among objects.

下面我们主要说下类之间的关系

#### Class-level relationships

- Association

Associations are relationships between classes in a UML Class Diagram.

q:如何理解association?
>association其实表达的是一种非常泛的关联，只要两个对象之间有关系，我们就说存在association.
其实也可以看到，association细化后才有了后续的各种关系.

q:现实的例子有哪些？
>由于，association是最泛的一个概念，所以更加细化的关系，又其它关系表达就好。
我理解，一般用association表达的关系，通常都是逻辑上的关联，在代码上没有特殊的反映。
>比如，两个类Client and Server,A server can process the requests of a client.
这其中，更强调server对于client请求的处理，因此，uml是server指向client的箭头

- Dependency

An object of one class might use an object of another class in the code of a method. If the object is not stored in any field, then this is modeled as a dependency relationship.

1. A special type of association.
2. Exists between two classes if changes to the definition of one may cause changes to the other (but not the other way around).

```cpp
class Book {};

class Person {
 public:
  void ReadBook(const Book&);
};
```

- Aggregation

A special type of association.

1. It represents a "part of" relationship.
2. Objects of Class1 and Class2 have separate lifetimes.

```cpp
class Wheel {};

class Car {
private:
  Wheel wheel_arr[4];
};

class Bicycle {
 private:
  Wheel wheel_arr_[2]
};
```

- Composition

1. A special type of aggregation where parts are destroyed when the whole is destroyed.
2. Class2 cannot stand by itself.

```cpp
class Departments {};

class Universtiy {
 private:
  Departments department_arr_[4];  
};
```

#### Instance-level relationships

- Inheritance (or Generalization)

1. Represents an "is-a" relationship.
2. SubClass1 and SubClass2 are specializations of SuperClass.

- Realization

1. Realization is special type of inheritance.
2. Realization is a relationship between the blueprint class and the object containing its respective implementation level details.(abstract class and implementation class.)

参考<br>
[Class diagram](https://en.wikipedia.org/wiki/Class_diagram#Association)<br>
[UML Class Diagram Tutorial](https://www.visual-paradigm.com/guide/uml-unified-modeling-language/uml-class-diagram-tutorial/)<br>
[UML Association vs Aggregation vs Composition with EXAMPLE](https://www.guru99.com/association-aggregation-composition-difference.html)

#### Sequence Diagram

q:什么是Sequence Diagram
>A sequence diagram shows object interactions arranged in time sequence.
>
根据定义，我们知道有两点是很重要的：
1. object interactions
2. time sesquence

q:时序图本质是相对什么建模?
>时序图通过消息，注意，是消息(或者说事件)来对对象之间的交互进行建模，对象之间的交互指的是其实就是对象之间的方法调用。

q:序列图由哪些部分组成
- Messages. written with horizontal arrows with the message name written above them, display interaction.
  - synchronous message
  - asynchronous message
  - reply messages
- Activation boxes.are opaque rectangles drawn on top of lifelines to represent that processes are being performed in response to the message 
- Objects

[Sequence diagram](https://en.wikipedia.org/wiki/Sequence_diagram)<br>

## 编程范式(Programming paradigm)

### 基础
q:什么是编程范式?
>A style of building the structure and elements of computer programs.

q:当我们对编程范式进行区分的时候，主要从哪些特征进行区分?
>Some paradigms are concerned mainly with implications for the execution model of the language, such as allowing side effects, or whether the sequence of operations is defined by the execution model. Other paradigms are concerned mainly with the way that code is organized, such as grouping a code into units along with the state that is modified by the code. Yet others are concerned mainly with the style of syntax and grammar.

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

### OO

## 设计模式(Software design pattern)

### 基础

q:什么是设计模式?
>In software engineering, a software design pattern is a general, reusable solution to a commonly occurring problem within a given context in software design.
>
>对于给定上下文经常出现的某类问题，一种通用，可重用的解决办法。
对于某一类通用问题的程序设计最佳实践。

q:什么是模式？
>泛泛的来说是，一些事物所展示出来的非常典型、通用、具体的特点，这些特点的汇总叫做模式。

## 通用概念

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