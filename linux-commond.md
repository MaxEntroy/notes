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

<HR style="FILTER: progid:DXImageTransform.Microsoft.Glow(color=#987cb9,strength=10)" width="80%" color=#987cb9

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