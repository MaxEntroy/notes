[TOC]

## 测试

这一节我觉得需要说的，如果开发不能保证好的质量，都交给测试去做。只会降低效率。打铁还需自身硬，一方面对于语言本身精益求精，另一方面对于测试理论的了解，我觉得有助于写出更好的代码。

### 测试理论

q:什么是单元测试？
>单元测试（unit testing），是指对软件中的最小可测试单元进行检查和验证.
对于单元测试中单元的含义，一般来说，要根据实际情况去判定其具体含义，如C语言中单元指一个函数，Java里单元指一个类，图形化的软件中可以指一个窗口或一个菜单等。
总的来说，单元就是人为规定的最小的被测功能模块

q:什么是回归测试？
>在新版本上运行所有已通过的测试用例以验证有没有“退化”情况发生，这个过程就是一个 Regression Test

q:如何理解回归测试当中的回归？
>Regress 的英语定义是： return to a worse or less developed state。是倒退、退化、退步的意思。
在软件项目中，如果一个模块或功能以前是正常工作的，但是在一个新的构建中出了问题，那这个模块就出现了一个“退步”（Regression），从正常工作的稳定状态退化到不正常工作的不稳定状态。
所以，对于回归测试的理解主要在于，一个新的构建当中是否有模块测试失败。但是，我的疑问是，如果有测试失败，它连单测都过不了，所以对于单侧和回归测试的区别需要梳理清楚，再讨论这个问题。

参考<br>
[软件测试笔记（一）理论篇](https://testerhome.com/topics/8594)<br>
[单元测试和回归测试](http://www.uml.org.cn/Test/201112285.asp)<br>


q:什么是test case?
>In software engineering, a test case is a specification of the inputs, execution conditions, testing procedure, and expected results that define a single test to be executed to achieve a particular software testing objective, such as to exercise a particular program path or to verify compliance with a specific requirement.
>
>对于这句话的理解，我是这么想的，为了达到测试目的，一组程序输入，执行条件，测试过程，以及期待结果的说明。当然前半句话说的比较泛，后面这句比较具体：对于特定程序执行逻辑的测试，来验证是否满足预期的结果。
>
>总结下什么是测例，以一组输入的形式给出，为了测试程序的某条执行逻辑，是否符合预期。

q:什么是test suite?
>In software development, a test suite, less commonly known as a 'validation suite', is a collection of test cases that are intended to be used to test a software program to show that it has some specified set of behaviours.
>
>我的理解，一组相关的测试用例。或者说是，测试程序特定功能的测例集合。

我们可以看下gtest给出的例子，从名称上就能看出来。

```cpp
// gtest提供的测试方法
TEST(TestSuiteName, TestName) {
  ... test body ...
}

TEST(AddTest, HandlePositive) {
  EXPECT_EQ(add(1,2), 3);
}

TEST(AddTest, HandleZero) {
  EXPECT_EQ(add(0,3), 0);
  EXPECT_EQ(add(3,0), 0);
  EXPECT_EQ(add(0,0), 0);
}

TEST(AddTest, HandleNegative) {
  EXPECT_EQ(add(-1,1), -1);
  EXPECT_EQ(add(1,-1), -1);
  EXPECT_EQ(add(-1,-1), -1);
}
```

q:什么是xUnit architecture?
>xUnit is the collective name for several unit testing frameworks that derive their structure and functionality from Smalltalk's SUnit.
简言之，从SUnit发展而来的测试框架，我们都叫做xUnit测试框架

q:xUnit测试框架特点？
>All xUnit frameworks share the following basic component architecture, with some varied implementation details:
- Test runner: A test runner is an executable program that runs tests implemented using an xUnit framework and reports the test results
- Test case: A test case is the most elemental class. All unit tests are inherited from here.
- Test fixtures: A test fixture (also known as a test context) is the set of preconditions or state needed to run a test. The developer should set up a known good state before the tests, and return to the original state after the tests.
- Test suites: A test suite is a set of tests that all share the same fixture. The order of the tests shouldn't matter.
- Test execution: The execution of an individual unit test proceeds as follows: 
- Test result formatter: A test runner produces results in one or more output formats
- Assertions: An assertion is a function or macro that verifies the behavior (or the state) of the unit under test.Usually an assertion expresses a logical condition that is true for results expected in a correctly running system under test (SUT).

参考<br>
[XUnit](https://en.wikipedia.org/wiki/XUnit)<br>

## Devops

### 版本控制

#### 基础理论

q:什么是版本控制？
>本控制是一种记录一个或若干文件内容变化，以便将来查阅特定版本修订情况的系统

q:版本控制系统经历了怎样的变更?
- 本地版本控制系统
- 集中化版本控制系统(cvcs)
- 分布式版本控制系统(dvcs)

q:他们各自优缺点?
>- 本地版本控制系统，不方便协作
- cvcs, 如果中心节点挂掉，有可能无法恢复且无法恢复，有丢失所有历史的可能。
- dvsc, 解决了cvcs的问题，每个节点保存远程仓库的镜像，远程服务器挂掉，可用本地的进行恢复。缺点是，保存整个项目，有空间消耗。

参考<br>
[why-is-git-better-than-subversion](https://stackoverflow.com/questions/871/why-is-git-better-than-subversion)

#### git

q:如何理解分支?
>分支本质上关联的是commit对象的链表。master,test这些分支是链表的头指针，从逻辑上关联整个链表。
HEAD则指向了当前的工作链表，即工作分支

q:如何理解remote branches?
- Remote branches are references to the state of branches on your remote repositories.
- They’re local branches that you can’t move.
    - they’re moved automatically whenever you do any network communication
从这两点，我们能看出来，远程分支是一种本地分支，它是远程仓库分支的引用，在本地不能进行操作。因为它只是引用。
远程分支的作用，主要是多人协作时告诉我们某一个分支在远程仓库的情形，即别人的共享如何。

q:如何理解tracking branches?
>Checking out a local branch from a remote branch automatically creates what is called a tracking branch.
说的很清楚了，从远程分支checkout出来的本地分支，叫做tracking branch.它的特性就是和一个远程分支保持直接的联系，
最直观的体现在git fetch以及git push时，自动甄别进行同步的远程分支。
racking branches are local branches that have a direct relationship to a remote branch.

q:git修改上次提交?
>主要说下，怎么修改username和email
```c
git commit --amend --author="userName <userEmail>"
```
q: merge/rebase的区别
>先说下merge，merge比较简单，就是分叉的分支，进行一次三路合并。第3路是最近common ancestor.
需要注意的一点是，commit message会写在同一列。但是commit符号(*)并不在一起。我以前总看错

参考<br>
[git中 修改已提交的commit的用户名邮箱](https://www.jianshu.com/p/7def4f387e9f)

#### git workflow

参考<br>
[大家如何选择 git 工作流模型？](http://mk.oa.com/q/255762)

### CI/CD

q:CI指什么？
>Continuous Integration (CI) is a development practice where developers integrate code into a shared repository frequently, preferably several times a day.
>
>简单来说，一天之内，频繁的将代码集成(提交)到主干(仓库)

q:CI的这种实践，有什么优点？
- 快速发现错误。每完成一点更新，就集成到主干，可以快速发现错误，定位错误也比较容易
- 防止分支大幅偏离主干。

q:CI的意义是什么？
>持续集成的目的，就是让产品可以快速迭代，同时还能保持高质量。它的核心措施是，代码集成到主干之前，必须通过自动化测试。只要有一个测试用例失败，就不能集成。

q:Continuous Deliver指什么?
>指的是，频繁地将软件的新版本，交付给质量团队或者用户，以供评审。如果评审通过，代码就进入生产阶段。

q:Continuous Deployment指什么?
>是持续交付的下一步，指的是代码通过评审以后，自动部署到生产环境

参考<br>
[持续集成是什么？](http://www.ruanyifeng.com/blog/2015/09/continuous-integration.html)<br>
[CONTINUOUS INTEGRATION ESSENTIALS](https://codeship.com/continuous-integration-essentials)