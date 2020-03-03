[TOC]

## 常用命令

### 文件

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

#### install

q:install命令有什么作用？
>install命令的作用是安装或升级软件或备份数据，它的使用权限是所有用户。install命令和cp命令类似，都可以将文件/目录拷贝到指定的地点。但是，install允许你控制目标文件的属性。install通常用于程序的makefile，使用它来将程序拷贝到目标（安装）目录。

参考<br>
[install命令](https://man.linuxde.net/install)<br>
[linux里install命令和cp命令](https://lingxiankong.github.io/2014-01-06-linux-install.html)<br>

---

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
