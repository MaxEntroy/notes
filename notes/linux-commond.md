## 常用命令

### 文件

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
ln -sf tmp tmp-target

ln:         make links between files
ln [OPTION]... [-T] TARGET LINK_NAME   (1st form)
ln [OPTION]... TARGET                  (2nd form)
ln [OPTION]... TARGET... DIRECTORY     (3rd form)
ln [OPTION]... -t DIRECTORY TARGET...  (4th form)
```

- 软连接与硬链接的区别
    - 硬链接可认为是一个文件拥有两个文件名;而软链接则是系统新建一个链接文件
    - 软链接可对文件和文件夹，而硬链接仅针对文件

参考
[软链接与硬链接的区别](https://blog.csdn.net/netwalk/article/details/38039155)

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

### 进程/网络

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

#### ld

参考<br>
[multiple Lua VMs detected](https://blog.csdn.net/u010144805/article/details/80776802)

### vim

#### 纵向操作

[技巧：Vim 的纵向编辑模式](https://www.ibm.com/developerworks/cn/linux/l-cn-vimcolumn/index.html)