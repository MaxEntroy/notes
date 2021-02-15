[TOC]

## 常用命令

### 文件

#### awk

q:这个命令的作用
>mawk - pattern scanning and text processing language
>简单来说，就是文本处理工具

q:实际中的能力
1.对于输入进行逐行处理的能力

#### 磁盘大小

```shell
df -h # 查看整个系统各目录大小
du -h # 查看当前目录各子目录大小
du -h --max-depth=1 # 查看当前目录一级子目录大小
```
上面3个命令配合实用，对于清除文件日志时，快速定位大文件目录有帮助。

#### 2>&1
- 基本用法
```shell
nohup commond > /dev/null 2>&1 &

nohup       :后台执行，终端关闭会继续执行
/dev/null   :代表空设备文件
>           :代表重定向到哪里，例如：echo "123" > /home/123.txt
1           :表示stdout标准输出，系统默认值是1，所以">/dev/null"等同于"1>/dev/null"
2           :表示stderr标准错误
&           :表示等同于的意思，2>&1，表示2的输出重定向等同于1
&           :直接返回shell界面，如果终端退出，程序不会后台执行
```

参考
[Linux Shell 1>/dev/null 2>&1 含义](https://blog.csdn.net/ithomer/article/details/9288353)
[nohup ./start.sh> myout.file 2>&1 &](https://simon-9527.iteye.com/blog/2302675)

#### ln
- 基本用法
```shell
ln:         make links between files
ln [OPTION]... [-T] TARGET LINK_NAME   (1st form)
ln [OPTION]... TARGET                  (2nd form)
ln [OPTION]... TARGET... DIRECTORY     (3rd form)
ln [OPTION]... -t DIRECTORY TARGET...  (4th form)
```

- 软连接与硬链接的区别
    - 硬链接可认为是一个文件拥有两个文件名;而软链接则是系统新建一个链接文件
    - 软链接可对文件和文件夹，而硬链接仅针对文件

参考<br>
[软链接与硬链接的区别](https://blog.csdn.net/netwalk/article/details/38039155)<br>
[ln](https://github.com/MaxEntroy/notes/blob/master/notes/linux-command.md)<br>
[linux下创建和删除软、硬链接](https://www.cnblogs.com/xiaochaohuashengmi/archive/2011/10/05/2199534.html)<br>

#### ls
- 基本用法
```shell
ls [OPTION] [FILE]

-l       :long listing format
-h       :human-readable
-r       :reverse order when while listing
-t       :sort by modification time, newest first
```
- 格式说明
<img width="700"  src="img/ll.png"/>

```shell
文件类型
所有者权限
组用户权限
其他用户权限
链接数
用户名
组名
文件大小
最后修改时间
文件名
```
参考
[Linux 下 ls -l 命令执行显示结果的每一列含义](https://blog.csdn.net/zhuoya_/article/details/77418413)
[What do the fields in ls -al output mean?](https://unix.stackexchange.com/questions/103114/what-do-the-fields-in-ls-al-output-mean)

- 一些case

下午碰到写文件禁止权限的问题，借此我们再来详细谈一下文件的用户权限

q:如何理解文件权限？
>文件权限的理解，一定要结合用户来说，因为不同的用户具有不同的权限。所以需要先明确linux当中的用户管理

q:linux的身份等级(不从root角度来说，而是结合文件权限的角度来说)?
1. user(文件的属主)
2. group(文件的用户组)
3. others(其他)
这么分类的原因，还是牵扯到文件权限。比如user一般作为文件的创建者，具备可读可写的能力。而文件的用户组，一般具有可读的能力。而其他用户则没有查看权限

q:ls是如何查看这些信息的？
1. 第一列是不同用户组的权限
2. 第二列是user(表示user是谁)
3. 第三列是group
4. 没有other是因为可以根据前两个进行推断

q:下午碰到一个程序写权限不具备的案例。
>首先我们查看文件权限，发现是A。那么，我们就需要关心，当前程序是哪个用户在执行它。因为不同权限的用户对于同一个文件，具备不同的操作能力。
后来了解到，当前程序的执行用户是B。然后，对比了这个目录权限，发现B是other用户，但是对于他来说，权限只有读。所以不具备写权限。
解决办法就是为这个文件的other增加写权限

参考
[文件权限](https://xwjgo.github.io/2016/11/05/linux%E6%96%87%E4%BB%B6%E6%9D%83%E9%99%90/)

#### install

q:install命令有什么作用？
>install命令的作用是安装或升级软件或备份数据，它的使用权限是所有用户。install命令和cp命令类似，都可以将文件/目录拷贝到指定的地点。但是，install允许你控制目标文件的属性。install通常用于程序的makefile，使用它来将程序拷贝到目标（安装）目录。

参考<br>
[install命令](https://man.linuxde.net/install)<br>
[linux里install命令和cp命令](https://lingxiankong.github.io/2014-01-06-linux-install.html)<br>

---

### perf

- 初识

以下几个命令，可以帮助我们简单的分析程序的性能

```perf top -p pid```:查看运行程序的性能，具体每一列的内容，参照参考文献
```perf record -a -g -p pid```:对运行程序进行采用，dump一段时间的运行数据
```perf report -i perf.dat```:查看已经dump的perf性能数据

对于热点的查看，直接进入对应的cpu时间占用较高的函数内部即可。直到找到cpu占比时间较高代码部分

参考<br>
[系统级性能分析工具perf的介绍与使用](https://www.cnblogs.com/arnoldlu/p/6241297.html)<br>
[perf Examples](http://www.brendangregg.com/perf.html)


### gdb

- 初识

q:gdb需要准备什么?
> -g选项需要加入。这个是在compile time当中的compilation阶段，其实在linking阶段，没什么用。这也让我对Makefile的写法有了更进一步的理解

q:gdb调试core需要注意什么?
>设置unlimit
gdb bin corefile

参考<br>
[gdb 调试利器](https://linuxtools-rst.readthedocs.io/zh_CN/latest/tool/gdb.html)<br>

- 实践

q:对于tcp-ip-socket-programming/chapter05/demo-05/client是如何调试的?
>现象是，不知道挂在哪了？并且修改的是一个未调用的函数。所以，先判断挂哪了。
按照下文设置main断点，判断是否能进入main函数。
结果发现，没有进入main就挂了
>
>分析：
1.do_io_event函数并未调用，但是是否构造pb对象，会造成程序core.不合理，毕竟没有调用。
2.通过gdb确认了这一点，在main函数之前就已经core了，证明和1提到的逻辑没有关系。是否core和调用没有关系。
3.猜测编译器做了如下操作，检查text segment中的代码，如果有pb的构造(虽说没有执行构造)，编译器会做一些其他的工作，这些工作导致程序core
4.进一步发现，少链接了lphread库。补充后完善
>
>通过gdb的引入，验证了我的思想，即都没有执行main，就已经挂了。所以，是否core和是否调用io_event没有关系。原因是3中的猜测。

总结，我们对于编译器的了解很少。

```c
(gdb) b main
Breakpoint 1 at 0x40305c: file echo_client.cc, line 24.
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x000000000040305c in main(int, char**) at echo_client.cc:24
(gdb) run
Starting program: /home/kang/workspace/myspace/git-personal/tcp-ip-socket-programming/chapter05/demo-05/client/echo_client 
terminate called after throwing an instance of 'std::system_error'
  what():  Unknown error -1

Program received signal SIGABRT, Aborted.
0x00007ffff7096c37 in __GI_raise (sig=sig@entry=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:56
56	../nptl/sysdeps/unix/sysv/linux/raise.c: 没有那个文件或目录.
```

#### ulimit
q:ulimit什么作用?
>ulimit 用于限制 shell 启动进程所占用的资源

q:ulimit -c有什么用?
>-c <core文件上限>：设定core文件的最大值，单位为区块；(K)

q:hard limit 和 soft limit有什么区别?
>-H 设置硬资源限制;-S 设置软资源限制
There are two types of ulimit settings:
The hard limit is the maximum value that is allowed for the soft limit. Any changes to the hard limit require root access.
The soft limit is the value that Linux uses to limit the system resources for running processes. The soft limit cannot be greater than the hard limit.
>
>eg: 比如core文件大小的hard limit是10k, 这个是系统级别的。但是对于进程级别，可以设置soft limit为4K

参考
[ulimit](https://wangchujiang.com/linux-command/c/ulimit.html)<br>
[Linux ulimit Command](https://linuxhint.com/linux_ulimit_command/)<br>
[Linux Note – 通过ulimit和PAM来限制资源](https://blog.csdn.net/tianyue168/article/details/41896921)<br>
[Guidelines for configuring Linux ulimit settings for IBM Streams](https://www.ibm.com/support/knowledgecenter/SSCRJU_4.2.0/com.ibm.streams.install.doc/doc/ibminfospherestreams-install-operating-system-settings.html)

### 进程/网络

#### pgrep

q:作用？
>pgrep, pkill - look up or signal processes based on name and other attributes

q:用法？
- ```pgrep -f ProcessName```这个命令获得对应进程名的pid

#### xargs

q:这个命令的作用?
1. xargs - build and execute command lines from standard input
2. 翻译过来就是，把标准输入，转化为命令行参数。

q:命令的用法?
1. 管道可以把标准输出转换为标准输入，提供给右侧命令
2. 但是不是所有命令，都接受标准输入作为参数。
3. 对于不接受标准输入作为参数的命令，使用xargs将标准输入转化为命令行参数，从而执行该命令

>cat /etc/passwd | grep root, 这个命令ok，因为grep接受把标准输出转化为标准输入
echo "hello world" | echo，这个命令不ok，因为echo不接受标准输入作为参数
echo "hello world" | xargs echo，这么写就ok了

**xargs的作用在于，大多数命令（比如rm、mkdir、ls）与管道一起使用时，都需要xargs将标准输入转为命令行参数。**

#### 端口占用
- 基本用法
```shell
lsof -i:port 直接判断某个端口是否被占用
ps: 该命令只能查询当前用户启动的进程是否占用该端口, 如果查询整个系统，加上sudo选项
```

参考
[使用netstat lsof查看端口占用情况](http://lazybios.com/2015/03/netstat-notes/)

#### 判断进程占用端口

- linux
```shell
#获得进程号
ps -ef|grep [process name] 
#查看pid对应进程占用的端口
netstat -nlpt|grep pid
```

- macos
```shell
#获得进程号
ps -ef|grep [process name] 
#查看pid对应进程占用的端口
lsof -p pid | grep LISTEN
```
ps: macos netstat 功能和linux不完全一样

结合lsof，我自己又想出一种快捷的查询某个进程监听端口的办法
```
1.查询进程的pid
ps -ef|grep proname

2. 打开Pid关联的进程，打开的所有ipv4 socket
lsof -a pid -i 4
```

参考
[mac oxs 上查看进程监听的端口号 lsof](http://lazybios.com/2015/03/netstat-notes/)

#### lsof
这个命令的学习背景是如何查看一个进程占用的端口，但是发现和以前的认知有区别，有如下心得
- linux当中,everything is a file. 所以，socket，以及fd关联的文件，统统看做fd
- lsof这个命令本质是查看open files，本质其实就是通过fd信息，来查看关联的进程信息
- COMMAND,PID,USER是关联文件的应用程序信息，其余的是文件信息
- 这个命令，最重要的两个功能
    - 一个应用程序，打开了哪些文件
    - 一个文件，被哪些应用程序打开了

**这里特别强调，everything is a file in linux**，所以，如果我们想知道进程引用了哪些socket，.so，以及shared mem，都可以通过这种形式来查找。
因为后者都是file，所以这个概念一定要有

考虑如下场景，我用code打开了linux-comand.md
```
kang@ubuntu:~/workspace/myspace/git-personal/notes/notes(master)$ lsof -a -p 27626 -d cwd
COMMAND   PID USER   FD   TYPE DEVICE SIZE/OFF     NODE NAME
code    27626 kang  cwd    DIR    8,1     4096 13762658 /home/kang/workspace/myspace/git-personal/notes/notes

FD: 标识打开文件的fd,wd 值表示应用程序的当前工作目录,txt 类型的文件是程序代码,最后，数值表示应用程序的文件描述符，这是打开该文件时返回的一个整数
TYPE: 标识打开文件的类型，根据具体操作系统的不同，您会发现将文件和目录称为 REG 和 DIR，分别表示字符和块设备。UNIX、FIFO 和 IPv4标识网络设备
```

对于查找网络链接，参考当中给了特别好的例子，我们看下
```
kang@ubuntu:~$ lsof -i:xxxxx
COMMAND  PID USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
ssh     8778 kang    3u  IPv4  82495      0t0  TCP 000.000.000.000:xxxxx->11.111.111.111:yyyyy (ESTABLISHED)
注意，这个lsof -i:port,只是把网络链接当中port匹配的opening files找出来。
上面我们可以看到xxxxx出现的位置，8778这个进程是处于client的地位，是接受者。yyyyy是server port

[1.2.3.4@whoareyou /data/service]# lsof -i:xxxxx
COMMAND      PID USER   FD   TYPE     DEVICE SIZE/OFF NODE NAME
zzzzz 203771 root    106u  IPv4 11111111      0t0  TCP *:xxxxx (LISTEN)
我们来看xxxxx出现的位置，203771这个进程，是处于serving的地位，标识203711这个进程在xxxxx这个端口进行listening.
对于我来说重要的一点是。这个端口已经被一个服务给占了。所以别的服务不能开到这个端口上。
```

总结，这个命令的主要作用是，**显示打开的文件和控制它们的进程之间的关系**

这里进行更深一步的讨论，我认为主要搞定以下几个应用场景即可

q:对于某一个程序，如何知道它打开的文件？
1. 进程名(-c) ```lsof -c pname```
2. 进程号(-p) ```lsof -p pid```

>kang@ubuntu:~$ lsof -c vim
COMMAND PID USER   FD   TYPE DEVICE SIZE/OFF     NODE NAME
vim     704 kang  cwd    DIR    8,1     4096 12464319 /home/kang/tmp
vim     704 kang  rtd    DIR    8,1     4096        2 /
vim     704 kang  txt    REG    8,1  2191736 15074454 /usr/bin/vim.basic
vim     704 kang  mem    REG    8,1    43616  3145818 /lib/x86_64-linux-gnu/libnss_files-2.19.so
...
vim     704 kang    0u   CHR  136,8      0t0       11 /dev/pts/8
vim     704 kang    1u   CHR  136,8      0t0       11 /dev/pts/8
vim     704 kang    2u   CHR  136,8      0t0       11 /dev/pts/8
vim     704 kang    4u   REG    8,1     4096 12458216 /home/kang/tmp/.t.txt.swp


>kang@ubuntu:~$ lsof -p 704
COMMAND PID USER   FD   TYPE DEVICE SIZE/OFF     NODE NAME
vim     704 kang  cwd    DIR    8,1     4096 12464319 /home/kang/tmp
vim     704 kang  rtd    DIR    8,1     4096        2 /
vim     704 kang  txt    REG    8,1  2191736 15074454 /usr/bin/vim.basic
vim     704 kang  mem    REG    8,1    43616  3145818 /lib/x86_64-linux-gnu/libnss_files-2.19.so
...
vim     704 kang    0u   CHR  136,8      0t0       11 /dev/pts/8
vim     704 kang    1u   CHR  136,8      0t0       11 /dev/pts/8
vim     704 kang    2u   CHR  136,8      0t0       11 /dev/pts/8
vim     704 kang    4u   REG    8,1    12288 12458216 /home/kang/tmp/.t.txt.swp

q:对于一个文件，如何知道哪些程序打开了它
1. lsof |grep filename

当然，对于端口，则需要特别的命令，因为port并不是一个文件，但是它一般会关联一个file(sockcet)

参考<br>
[使用 lsof 查找打开的文件](https://www.ibm.com/developerworks/cn/aix/library/au-lsof.html#listing2)<br>
[ lsof 一切皆文件](https://linuxtools-rst.readthedocs.io/zh_CN/latest/tool/lsof.html)

### 环境配置

#### sz/rz

ps: rz/sz不是下载就能用，需要配置
参考<br>
[mac下rz/sz配置](https://segmentfault.com/a/1190000012166969)<br>

### 编译/链接

#### ldconfig

q:ldconfig的作用是什么?
>ldconfig creates the necessary links and cache to the most recent shared libraries found in the directories specified on the command line, in the file /etc/ld.so.conf, and in the trusted directories (/lib and /usr/lib).The cache is used by the run-time linker, ld.so or ld-linux.so.
>
>如何理解上面这句话，两层含义：
1.ldconfig创造了动态链接库的缓存信息，这个缓存信息会被run time linker使用。
2.ldconfig只会为 /lib,/usr/lib或者/etc/ld.so.conf里面所列出的的动态库创建cache信息

q:ldconfig如何使用？
>当我们新增.so时，比如加到了/lib,/usr/lib下面，需要执行ldconfig，即更新ldconfig创造的缓存信息。否则，
dynamic linking时，run-time linker根据ldconfig创造的cache，无法找到对应的动态链接库在哪，即链接失败。
>
>如果我们新增的.so，没有放到/lib或者/usr/lib，而是放到了other-path，那么需要把这个other-path加入到/etc/ld.so.conf里面。
再执行ldconfig更新cache.
>
>当我们新增的.so，既不想放到/usr/lib /lib，也不想更新/etc/ld.so.conf时，此时run-time linker无法找到对应.so。可以通过
增加一个LD_LIBRARY_PATH的全局变量，也能告诉run-time linker去哪找
>
>当我们的新增的.so，以上3种方案均不想采用时。比如自己项目的thirdparty/.so，此时编译时指定LDFLAGS:= -Wl,-rpath=$(LIB_PATH)
>
>总之，需要让run-time linker知道到哪里去找.so.
需要特别注意区别的是，-L是个compile time options，跟以上的讨论没有任何关系。compile time linking阶段，一定要找到符号定义，因此也需要知道库的路径。
但是，compile time知道不代表run-time也能知道。

参考<br>
[ldconfig(8) - Linux man page](https://linux.die.net/man/8/ldconfig)<br>
[ldconfig](https://wangchujiang.com/linux-command/c/ldconfig.html)<br>
[multiple Lua VMs detected](https://blog.csdn.net/u010144805/article/details/80776802)

#### pkg-config

q:这么命令的作用是什么？
>pkg-config is a helper tool used when compiling applications and libraries. 
It helps you insert the correct compiler options on the command line so an application can use.
>
>简言之，如果当你在使用一个比较大的库时，需要指定很多头文件，以及需要链接很多库时,如果一个一个手动写，比较麻烦，并且容易遗漏。
此时使用pkg-config可以帮助编译器自动识别这个库对应的头文件以及需要链接的库.
所以，当使用一个比较大的库，可以考虑使用pkg-config.

参考<br>
[freedesktop.org](https://www.freedesktop.org/wiki/Software/pkg-config/)<br>
[pkg-config的一些用法](https://blog.csdn.net/luotuo44/article/details/24836901)<br>

### vim

#### 纵向操作

[技巧：Vim 的纵向编辑模式](https://www.ibm.com/developerworks/cn/linux/l-cn-vimcolumn/index.html)
