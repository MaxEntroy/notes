[TOC]

## 网络/通信/协议

### webservice
先来看一看阮一峰对这个概念的阐述：
- 什么是service
>通俗地说，"服务"就是计算机可以提供的某一种功能。
- 本地服务 or 网络服务
>根据来源不同，服务又分为2种。一种是"本地服务"（使用同一台机器提供的服务，不需要网络），另一种是"网络服务"（使用另一台计算机提供的服务，必须通过网络才能完成）。
**"网络服务"（Web Service）的本质，就是通过网络调用其他网站的资源。** by ruanyifeng

- why webserivce?
>Web Service架构的基本思想，就是尽量把非核心功能交给其他人去做，自己全力开发核心功能。比如，如果你要开发一个相册软件，完全可以使用Flickr的网络服务，把相片都储存到它上面，你只要全力做好相册本身就可以了。总体上看，凡是不属于你核心竞争力的功能，都应该把它"外包"出去。

- 个人理解
本人觉得，上面的解释有一点点问题。我觉得网络服务应该是包含web service，因为网络服务有很多种。ssh server, ftp server，都是网络服务。不是只有web server.
也可能是阮一峰说的web有泛指Internet的作用。

- 网络服务之间的区别？
像上文所说，web service不代表所有网络服务。但是，web service的使用是最多的，因为人类使用最多的就是浏览器去浏览各种页面来获得信息。这也是Internet最大的贡献。
其实各种网络行为，也就关联了各种网络服务。比如，浏览行为关联了web service，下载行为关联了ftp service。

具体来说，web service具有如下特性
- web service特性
    - 应用层使用http协议
    - 采用http文档格式
    - 使用url
    - 客户端使用浏览器(这个是显而易见的，浏览器就是为浏览而生，没人愿意在terminal当中浏览)

尹圣雨给的解释，我觉得非常赞,后者对web server的定义
- 基于http协议
- 将url对应的文件发送给client端
- 在服务端找到client端请求的url对应的文件

所以，这个解释非常清楚的说明白了几个东西
1.应用层采用什么协议 2.client端请求的是什么 3.server端发送什么结果回去

他进一步给出了作为一个初学者，应该如何理解web server
- 理解hypertext
- 理解http
- web服务器端是以http协议为基础传输hyptertext的服务器端

参考<br>
[Web service是什么](http://www.ruanyifeng.com/blog/2009/08/what_is_web_service.html)<br>
[web服务器与ftp服务器的区别](https://blog.csdn.net/w252064/article/details/77581691)<br>
[Web service](https://en.wikipedia.org/wiki/Web_service)<br>

### proto buffer

- pb初识

q:什么是pb?
>Protocol buffers are Google's language-neutral, platform-neutral, extensible mechanism for serializing structured data.
按照英文的描述，pb只是一种机制，这种机制可以用来序列化结构化数据，并且这种机制是跨语言，跨平台，可扩展的。
跨语言，跨平台，可扩展；三大特性把握好.

q:pb对比json,xml的定义上，有什么区别?
>从上文看，pb是一种机制，但是这种描述，我以为是强调它在序列化时的能力较好，对于更好的把握这个概念，并不好。
我们来看看json的定义：
JavaScript Object Notation is an open-standard file format or data interchange format that uses human-readable text to transmit data objects consisting of attribute–value pairs and array data types
json的这个定义就好多了，显然pb在使用上，也是好json是一样的，作为一种跨语言，跨平台，可扩展的数据交换格式。只不过,json显然没有提供序列化的能力，但是pb提供了。
>
>xml的定义如下：
Extensible Markup Language (XML) is a markup language that defines a set of rules for encoding documents in a format that is both human-readable and machine-readable. 

q:pb自己的理解?
>1.跨语言，跨平台，可扩展的数据交换格式(当然，叫protocal也没毛病)
2.支持序列化(json就不支持)

参考<br>
[Pb语法](https://developers.google.com/protocol-buffers/docs/proto#scalar)<br>
[Pb-cpp相关](https://developers.google.com/protocol-buffers/docs/cpptutorial )<br>
[Pb-generated code](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated#repeatedmessage)<br>
[Pb-序列化](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.message_lite#MessageLite.ByteSize)
[Pb-反射](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.descriptor)<br>
[Pb-反射-demo](https://yrczone.com/blog/article/45)<br>

- pb 语法(proto2)

q:how to defining a message?
>1.Specifying Field Types
2.Assigning Field Numbers
3.Specifying Field Rules
>
>需要注意的点:
1.required字段慎用，pb2里面，必须有定义。但是，如果这个字段在语义上废弃，并不能像optional那样不填即可，还是要写。

q:What's Generated From Your .proto?
>the compiler generates the code, mainly include:
1.getting and setting field values
2.serializing your messages to an output stream, and parsing your messages from an input stream.


- pb字段没对对齐
```
问题背景：约定好协议，进行联调
问题表现：接收端看到可以收到数据，字段a和字段b的值明显是有的。但是，在debug pb的时候，发现没有Key.比如a,b,c,d,e5个field，c,d,e
都能打印出相应的key和value，但是a和b只能打印出value，没有key，只有数字5和6
问题解决：上游同学没有以来pb 的debug string，而是手动把key和value进行解析，打印。返现确实a,b没有key.没有置位。但是，value也是有。
后来发现，这个数字5和6是index.至于为什么是5和6，对照了Pb的field，暂时觉得不会是这2个值。
总之：如果出现类似情形。就是字段没有对其。
```

- pb深入理解

q:pb的优点有哪些？
> A lot of the time we have no control over the network that the application is on and therefore our only control over this performance factor is the amount of data we are sending across it.
Binary message formats allow us to compress data much more efficiently than other formats and therefore are a good match to c++.

ps:第一篇参考文献有一点特别棒，对比了xml和json，需要翻作者的blog


q:序列化接口的选择?
```ParseFromArray/SerializeToArray```

```cpp
void do_io_event(int clnt_sfd) {
  // recv from client
  char buf[BUF_SZ];
  ssize_t nread = read(clnt_sfd, buf, BUF_SZ);

  cal::CalRequest req;
  req.ParseFromArray(buf, nread);

  if(!req.has_seqno() || !req.has_left() || !req.has_right() || !req.has_optr()) {
    fprintf(stderr, "%s\n", "Invalid CalRequest.");
    return;
  }

  // do caculation
  cal::CalResponse res;
  do_cal(req, &res);

  // send to client
  int sz = res.ByteSize();
  res.SerializeToArray(buf, sz);

  write(clnt_sfd, buf, sz);
}
```

```cpp
bool	
ParseFromArray(const void * data, int size)
//Parse a protocol buffer contained in an array of bytes.

bool	
SerializeToArray(void * data, int size) const
//Serialize the message and store it in the given byte array.
```

通过这个demo，我看可以看出。```SerializeToArray```这个接口的使用，buf和sz都没有什么问题。```ParseFromArray```这个接口的问题在于，sz如何确定，还是需要知道

```ParseFromString/SerializeToString```

```cpp
bool SerializeToString(string* output) const 
// serializes the message and stores the bytes in the given string. Note that the bytes are binary, not text; we only use the string class as a convenient container.
// 这点特别要注意，不是unp当中提到的text strings，就是binary bytes，不是character bytes.即前者不具有可解释性
bool ParseFromString(const string& data);
// parses a message from the given string.
```

```cpp
void do_io_event(int clnt_sfd) {
  // recv from client
  char buf[BUF_SZ];
  ssize_t nread = read(clnt_sfd, buf, BUF_SZ);

  cal::CalRequest req;
  std::string recv_buf(buf, nread);
  req.ParseFromString(recv_buf);

  if(!req.has_seqno() || !req.has_left() || !req.has_right() || !req.has_optr()) {
    fprintf(stderr, "%s\n", "Invalid CalRequest.");
    return;
  }

  // do caculation
  cal::CalResponse res;
  do_cal(req, &res);

  // send to client
  std::string send_buf;
  res.SerializeToString(&send_buf);

  write(clnt_sfd, send_buf.data(), send_buf.size());
}
```

对于这两个接口，注意如下：
1.SerializeToString没什么说的，write时，注意是send_buf.data()，之前一直写的是send_buf.c_str(). 通过c_str()我们知道，这个方法，返回的是包括null-terminated sequence of characters.
通过上面的接口，我们知道，string做的只是一个convenient container，是binary bytes.
data：returns a pointer to the first character of a string 
c_str：returns a non-modifiable standard C character array version of the string 

2.ParseFromString这个接口，虽然是对string进行反序列化，但是string的构造要注意，不能直接通过buf，否则会截断

参考<br>
[Binary Message Format C++ Examples](https://www.thomaswhitton.com/blog/2013/07/03/binary-message-format-c-plus-plus-examples/)

### 序列化(Serialization)

q:序列化的概念是什么?
>Serialization is the process of converting an object into a stream of bytes to store the object or transmit it to memory, a database, or a file.
简单来说，就是把对象专户为字节流进行传输的过程.

q:序列化为什么一定要把对象转化为字节流？直接发送binary structures不行吗?
>考虑unp 5.18 data format的例子，如果直接发送结构体会导致如下问题：
1.字节序的问题，会导致对于binary numbers(binary structures)有不同的解释,大端小端，导致接受到的数据出错。
2.不同平台导致的类型解释不一致。比如有些平台，整型4 bytes，有些是2 bytes，导致接受到的数据出错。
3.结构体对齐不一致，导致对于binary numbers(binary structures)有不同的解释，导致接受到的数据出错
>
>下面给出英文的解释：
1.Different implementations store binary numbers in different formats.
2.Different implementations can store the same C datatype differently.
3.Different implementations pack structures differently
>导致这一问题的根本原因是，发送的是binary structures，接收端会直接解释binary structures.
>
>比如，int a = 3;write(sfd, &a, sizoef(a)); 直接直接这么发送，发送了4个字节。但是对端是16 bit int，那么肯定会接受错误。包括字节序的问题。
>所以，正是因为不能直接发送binary structures，才是我们需要序列化的原因

q:是不是只有序列化这一种方法才能解决上述问题呢?
>unp 给了一些建议：
1.passing all numering data as text strings.
2.Explicitly define the binary formats of the supported datatypes (number of bits, bigor
little-endian) and pass all data between the client and server in this format. RPC
packages normally use this technique.
>
>先来解释第一种，第一种方法可行的原因在于，利用char进行发送，相当于用ascii对于binary structure的numeric data进行编码，当然，也只能是singular data，结构体还是不行。
因为，char类型就占一个字节，不存在不同平台解释不一致的原因，也没有字节序的问题。
第二种，我不知道是不是现在pb的做法。

q:序列化的目的是什么?
>通过上面的讨论，我们知道如果直接发送对象(binary structures)，其实会破坏对象的状态，因为接收端无法得到正确的对象。
所以，序列化这个过程是希望在进行对象传输的时候，可以把对象的状态进行正确保存，然后再进行发送。在接收端再恢复对象的状态。
Its main purpose is to save the state of an object in order to be able to recreate it when needed. The reverse process is called deserialization.
>
>整个过程有点类似于快递的过程，比如快递要给你发一张桌子，他不可能直接把拼好的桌子发送给你，这样运输的流程中，这个对象肯定会遭到破坏。发送方会先把桌子拆了，然后给出安装图纸。把桌子和图纸，一起通过快递
发送给客户。客户拿到图纸和被拆了的桌子，根据图纸进行重新拼接，然后恢复。

q:序列化的过程都有哪些操作?
>1.拆桌子，肯定需要把对象转化为字节流
2.画图纸，给出字节序，大小端，可能也有结构对其，即从字节流重新构造这个对象的信息。
The object is serialized to a stream that carries the data. 
The stream may also have information about the object's type, such as its version, culture, and assembly name. From that stream, the object can be stored in a database, a file, or memory.

参考<br>
[Serialization ](https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/serialization/)


### 同步/异步

对于这个问题的讨论，要分几个角度来说才能说清楚
1. 基本概念角度，同步/异步分别是什么，阻塞/非阻塞分别指什么
2. 从一个调用的角度(函数调用，函数内部涉及cpu计算/io操作又可以进一步划分为cpu调用,io调用)
3. 我们单独从io调用的角度讨论，这几个概念又指什么

#### 基本概念

这个概念是一对比较容易混淆的概念，主要在于这一对概念应用的场景比较多。区别不同的场景，就容易理解。
- 原始定义
参照longman 6th给出的解释
```
syn-chro-nize: 
1. to happen at exactly the same time, or arrange for two or more actions to happen at exactly the same time.
eg: The show was designed so that the lights synchronized with the music.
基本概念还是强调一个同时，其实内在的逻辑没有说明白。
同步，本质还是保持内在一个“一样”的状态，即一致性的状态
```

还是同步本身，我们来看wikipedia的定义
```
Synchronization is the coordination of events to operate a system in unison. 
The conductor of an orchestra keeps the orchestra synchronized or in time. 
Systems that operate with all parts in synchrony are said to be synchronous or in sync and those that are not are asynchronous.
这个定义和上面基本一直，同步就是对不同事件的协调，使得他们在某一个指标上(声音是否整齐)保持一致。
```
- 通信当中的同步
下面讲到比较熟需的一个场景，通信当中的一致。和时序电路一个意思。
```
In electrical engineering terms, for digital logic and data transfer, a synchronous circuit requires a clock signal. 
However, the use of the word "clock" in this sense is different from the typical sense of a clock as a device that keeps track of time-of-day; 
the clock signal simply signals the start and/or end of some time period, often very minute (measured in microseconds or nanoseconds), that has an arbitrary relationship to sidereal, solar, or lunar time, or to any other system of measurement of the passage of minutes, hours, and days.
举个例子，打一下拍子，开始；再打一下拍子，停止。
```

- synchronization of processes(computer science)
```
Process synchronization refers to the idea that multiple processes are to join up or handshake at a certain point, in order to reach an agreement or commit to a certain sequence of action.
这个定义的内在也是强调了某种程度的一致性，比如哲学家进餐问题，如果不按照同步序列进行操作，大家都吃不了饭。如果按照同步序列，大家处于“都能吃到饭”的一致性状态
```

- synchronization of data(computer science)
```
keeping multiple copies of a dataset in coherence with one another, or to maintain data integrity.
这个概念也是非常清晰的，就是保持多份副本数据的一致性
```
**说下异步**，上面讲了几个常用的同步场景。基本定义，通信当中的同步，计算机科学当中的同步。显然，异步的定义也简单。不是同步的就是异步。
- 本质定义
```
同步需要维护不同事件之间，内在的某种状态的一致性。比如，交响乐团，每个人的声音需要保持一致，才能齐奏。
异步，不需要维护不同事件之前，内在状态的一致性。即，不同事件之间，内在的状态是不一致的。
```

在wiki上，异步的定义很多。我们直接说重点，我们一般在讨论同步/异步时，主要是指同步io/异步io(本质上说函数调用)
在这个背景下，我们再继续讨论，会牵扯阻塞和非阻塞进来。

q:阻塞非阻塞是指什么？
>这个概念也是需要单独看待，阻塞指的是进程执行的5种状态之一。和同步，异步，调用，没有任何关系。

#### 调用


- 阻塞调用/非阻塞调用

>一个线程/进程经历的5个状态， 创建，就绪，运行，阻塞，终止
阻塞调用是指调用结果返回之前，调用者(caller process)会进入阻塞状态。只有在得到结果之后才会返回。
非阻塞调用是指在不能立刻得到结果之前，该函数不会阻塞当前线程，而会立刻返回。
所以，阻塞调用和非阻塞调用是指当前调用是否会让进程/线程进入阻塞状态。伴随而来的就是，函数是否立即返回。显然，阻塞调用不会立即返回，非阻塞调用会立即返回。
实际上来说，只有涉及io函数时，我们才会特意讨论一个函数调用的同步，异步，阻塞，非阻塞。涉及cpu的计算，一般都是同步调用，此时cpu在实际计算，进程处于运行状态。和阻塞没有任何关系

- 同步调用/异步调用

>这里要和阻塞/非阻塞调用区别开。同步和异步的基础概念好理解，但是和调用集合，还是需要仔细理解，下面给出我的解释。
同步调用：这个调用不完成，不去做下一件事情。具体来说，可以同步阻塞，也可以同步非阻塞。实现的方式不一样，但是根本都是要完成这一个调用。从时间轴上来看，是线性调用的体现，时间线是顺序的。
异步调用：不是同步调用的就是异步调用。具体来说，可以一个调用没有完成，取执行别的调用，非线性调用的体现，时间线是乱序的。我目前见到的体现形式只有回调(c/c++只有这种支持，可能js,go这样的语言有别的体现)。多说一点，对于foo我们要异步调用，形式上来看，业务代码里不会直接调用。而是向系统/框架进行注册，当对应事件发生时，由系统唤醒线程来执行回调。
从这个角度来说，不是直接自己的调用，都是回调。

- cpu调用/io调用

>严格来说，上面几组概念只有当我们讨论io调用时，才有效。因为io调用很慢，会浪费处理机的能力，所以会导致低效的问题。此时，讨论io模型就是为了解决这个问题。
对于cpu调用则没有什么好说的，肯定是同步阻塞模型。因为cpu很快，都不会有阻塞的情况出现，因为cpu的分片一直进行计算。

#### io模型

这里采用李辉老师上课的讲义，以及botman的讲义，来进行总结

- 同步阻塞

这里我沿用授课老师的观点，同步就是一个线程只做一件事。阻塞和非阻塞的区别就在于线程状态，但是都是要把一件事做好

```cpp
ssize_t ret = send(fd, buffer, len, flags);
```

同步调用，send这件事要做完。
阻塞调用，send没返回，线程阻塞等待事件完成.(这里要特别注意，线程会一直阻塞到io完成，不调度。从而保证只做一件事)

- 同步非阻塞

```
evutil_make_socket_nonblocking(fd);

while (len) {
  ssize_t ret = send(fd, buffer, len, flags);
  // case1: ready to send
  if (ret >= 0) {
    len -= ret;
    continue;
  }
  // case2: not ready
  if (EVUTIL_SOCKET_ERROR() == EAGAIN)
    continue;
  // case3: socket error
}
```

同步调用：send这件事要做完
非阻塞调用，send没做完，线程并不阻塞。(由于线程不阻塞，线程就有做其他事的可能，所以需要程序进行轮询，来保证send这件事做完)

- 异步非阻塞

这里也是沿用授课考试的观点，异步就是一个线程可以同时(一段时间，并发的概念)做多件事情。

```js
fs.readFile(filename, (err, data) => {
  if (err) {
    // handle |err|
  }
  // use |data|
});
console.log('start file I/O, and continue');
```

上面这个形式，和我们上一小结总结的异步调用有一点区别，上面我们讲在c/c++这种形式下面，一个异步调用是不可能自己调用的。必须要进行注册，
其实上面js的这个异步调用是匿名函数的内容。只不过，在函数调用里可以直接注册。

非阻塞调用：数据没有ready，直接返回
异步调用：程序会继续向下走去做别的事，不会等待在这一件事上。当数据ready时，调用回调。

- 异步阻塞

```js
const readFileAsync = util.promisify(fs.readFile);

try {
  const data = await readFileAsync(filename);
  // use |data|
} catch (err) {
  // handle |err|
}
```

阻塞调用：readFileAsyncr发起io请求，然后阻塞到io完成。
异步调用：这里很关键，需要对比。异步阻塞，此时协程会被挂起，处理机发生调度，但是即使调度，执行的代码都是在当前线程。对于同步阻塞，处理机发生调度，会执行别的线程代码。这里导致的问题就是，同步阻塞会面临线程同步的问题。但是异步阻塞不会面临这个问题，因为调度还是在一个线程内。
异步阻塞的优点是，阻塞从某种程度上，保证了空间的连续性，即数据准备ready，才会返回。异步则是使时间变的不连续，提高了处理机的性能。
异步阻塞，相对于异步非阻塞的优点是，空间连续，代码的可读性提高。

以上讨论，参考[从时空维度看 I/O 模型](https://bot-man-jl.github.io/articles/?post=2019/IO-Models) 这篇文章实在是讲的太好了，彻底解决了这里我所有的困惑。botman毫无疑问是非常佩服的同事，
对于技术细节的理解非常到位，不敷衍。

当然，早上的培训对这里的理解深入也有帮助：
1. 阻塞和非阻塞针对接口来说
2. 同步和异步针对进程来说，是不是一段时间内，只能做一件事

参考<br>
[Asynchronous I/O](https://en.wikipedia.org/wiki/Asynchronous_I/O)<br>
[Synchronous and Asynchronous I/O](https://docs.microsoft.com/en-us/windows/win32/fileio/synchronous-and-asynchronous-i-o)<br>
[同步，异步，阻塞，非阻塞等关系轻松理解 ](https://github.com/calidion/calidion.github.io/issues/40)<br>

### uri/url/urn
这是3个非常容易混淆的概念，但是理解的层次其实非常容易。
uri是抽象类，告诉我们，网络上的资源同样需要唯一的标识。这件事是重要的，但是怎么实现，没有说。
url和urn分别是两种实现。
url靠位置来表明一个资源,urn靠名称来表明一个资源。

参考<br>
[HTTP 协议中 URI 和 URL 有什么区别](https://www.zhihu.com/question/21950864)

### HTTP

- http怎么来的

- http现在都怎么用
这个感觉需要特别说下，和我们平时使用息息相关。先说直观体验，我理解http为浏览器而生，但是它又大量应用在我们的服务交互当中，所以我不是很理解这是怎么回事。
直到看到知乎上给了一个好的解释

>HTTP最早被用来做浏览器与服务器之间交互HTML和表单的通讯协议；后来又被被广泛的扩充到接口格式的定义上;
所以在讨论GET和POST区别的时候，需要现确定下到底是浏览器使用的GET/POST还是用HTTP作为接口传输协议的场景。

从上面这个定义当中，很容易得到http的2种用法。目前工作当中是用作第二种，即用http作为接口定义。
这又会带来另一个问题，为什么要有接口定义。回想我以前写的echo server，socket连接之后，就直接在socket上进行read/write。说下我自己的理解啊，如果服务简单，
这么写到没事。如果服务复杂，比如一个Server可能提供多种子服务，如果还是之前的方式，不清晰。我们自己看brpc搭建http/h2服务，通过proto来暴露对外接口，客户端通过
http_client或者curl进行请求，语义上非常清晰。

参考<br>
[GET 和 POST 到底有什么区别？](https://www.zhihu.com/question/28586791/answer/767316172?hb_wx_block=1&utm_source=wechat_session&utm_medium=social&utm_oi=658626377876639744)

- http的过程
    - 数据请求(client)
        - 建立TCP连接(TCP,TCP,TCP)
        - 发送http请求(HTTP)
    - 数据响应(server)
        - 发送http响应(TCP,HTTP)
    - 断开连接(server)
        - server主动断开(TCP,TCP)

特别注意，
1.其实数据请求和数据响应，都是要维护TCP的，因为TCP和HTTP不在同一个层次。
发送HTTP肯定得有TCP状态才可以。数据请求这里只不过单独拿出来说下。
2.server发送响应之后主动断开连接，所以没有四次挥手。
- http协议
协议主要就是说些彼此之间约定的数据发送格式
    - http_request
        - 请求行
        - 消息头
        - 空行
        - 消息体
    - http_response
        - 状态行
        - 消息头
        - 空行
        - 消息体

- GET/POST

### RPC

说一下我的这个简单理解，socket和http都比较好理解，是很具体的东西。
socket就是os提供的进行网络交互的handle，至于应用层什么协议使用，和socket本身没有关系。http底层用socket，ftp底层也使用socket，所以这点很清晰。
http也好理解，应用层协议，应用程序之间约定好**业务数据**的协议以及一些细节。http主要是web service这类业务进行的应用层协议，当然http我在上面也提到了，也可以被当做接口传输协议场景。
rpc我理解，不是一个非常具体的概念。是在基础网络基础上，抽象出来的高级逻辑。不仅有网络通信，还有服务发现，服务治理等这些东西。

补充一下自己的理解，这两天对于RPC有了一定新的认识。首先RPC肯定不是socket也不是http，socket是内核提供的网络服务fd，http是一种应用层协议，这两个东西都是实现网络服务的基础，socket的层次比http更低。对于RPC而言，则是在网络服务的基础上，又增加了一部分东西(这部分东西暂时我还不能清晰描述)，提供的一种远程服务调用。作为对比，考虑这样一个例子：
>本地服务，可以启一个echo server，当然，我这个echo server支持并发的功能，然后添加一个client，执行这个过程socket.send(clt_fd, message)，然后server调用echo(msg)这个函数去处理，这个函数内部会把msg丢给一个线程池，然后fd也丢给任务线程，由任务线程最后把结果发回给client。这是个非常清晰的过程。
我们考虑，如果是rpc，是不是也是一样的过程。本地服务没有太多变化，但是client端，不是显示的的发送数据，而是调用远程的echo函数，这个过程，对于下层屏蔽了网络这一部分。所以，rpc调用，肯定有网络服务作为基础来进行支撑的。之所以说这个过程，其实想表达的是，对于服务端的写法，逻辑上应该没有太多变化，但是需要考虑暴露接口，这个是肯定的，我们从brpc中已经可以看到，这一部分内容也是通过pb进行暴露的。

参考<br>
[RPC、HTTP、Socket区别](https://zhuanlan.zhihu.com/p/48760074)

q:结合服务方来看，使用rpc暴露服务接口，但是既然是服务，为什么看不到while(1)的部分呢?
>所谓请求服务，到底请求的是什么？发送数据，指定ip:port
对应的port，也一定有一个服务监听在这个port上。请求的服务，就是对方监听这个port的服务。
这样，数据发送到对应的port，对面的服务可以从port里读数据，之前对应的服务可能读阻塞在这个port上.
port理解为进程标识，socket理解成一个文件。进程显然是程序的执行实体。所以，对端的进程阻塞在一个文件上。
发送端向对端的进程发送数据，数据会发送到对端进程绑定的文件上，对端之前会一直阻塞在文件的读取上，有数据，
则会读取，然后处理，发送结果。这就是一次服务的请求过程。
>
>rpc暴露的接口，本质上，把那一段while(1)代码抽出去，当做对外暴露的接口罢了。
这个接口在调用其他函数，完成具体逻辑。只不过while(1)的部分又更底层的部分实现了，暴露的接口
单纯是处理业务逻辑。


### brpc

参考
[brpc学习 ParallelChannel的使用](https://www.twblogs.net/a/5c00fc12bd9eee7aed339fe4/zh-cn)

### Json

参考阮一峰对于json的理解，对于数据格式和json都有更进一步的理解。
阮一峰谈到，从结构上看，所有的数据(data)都可以分解成三种类型
>第一种类型是标量（scalar），也就是一个单独的字符串（string）或数字（numbers），比如"北京"这个单独的词。
>
>第二种类型是序列（sequence），也就是若干个相关的数据按照一定顺序并列在一起，又叫做数组（array）或列表（List），比如"北京，上海"。
>
>第三种类型是映射（mapping），也就是一个名/值对（Name/value），即数据有一个名称，还有一个与之相对应的值，这又称作散列（hash）或字典（dictionary），比如"首都：北京"。

q:对于他的这个划分，我们怎么理解？
>从已有的知识来看，当我们谈数据的结构的时候，我们就是在说数据结构，常见的数据结构有，集合，线性表，树，图，哈希表(散列)这5种，这个毋庸置疑。上文中提到的结构，标量，序列和映射，是物理层面进行分解后最终不能再进行划分的数据结构。其实也对。你想想，集合，线性表，树，图都是可以用序列来表示的。而哈希表就是映射，标量对应到任何一门语言的基础数据类型。

q:json的规则是什么？
- 序列用[]表示，序列的元素可以是一个标量，也可以是一个序列或者映射，但是元素必须一致。序列的元素用","分割
- 映射用{}表示，映射的元素是一个k-v pair，k-v pair用":"表示，映射的元素用","分割

注意，序列和映射都可以由多个元素构成，但是语义并不一样。序列的元素是并列的，映射的元素没有并列的语义，映射的元素本质上代表的是一个属性的维度。
最后说一下，Json是一种数据交换格式，和xml一样，具有描述对象的能力。

参考<br>
[数据类型和Json格式](http://www.ruanyifeng.com/blog/2009/05/data_types_and_json.html)<br>
