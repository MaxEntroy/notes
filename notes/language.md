## lua

### local变量个数
```
local变量个数上限200个，使用local变量可以提高效率，但是应该避免滥用
lua 5.3.0: ttt.lua:202: too many local variables (limit is 200) in function at line 1 near '='
luajit 2.0.5: ttt.lua:202: function at line 1 has more than 200 local variables
```

参考<br>
[高性能 Lua 技巧（译）](https://segmentfault.com/a/1190000004372649)<br>

### colon and dot
先看个标准的例子，使用dot传递self参数
```lua
local obc = {
    x = 10,
    y = 20,
}

function obc.add(self, bias)
    self.x = self.x + bias
    self.y = self.y + bias
    return self.x + self.y
end

function obc.minus(self, bias)
    self.x = self.x + bias
    self.y = self.y + bias
    return self.x - self.y
end

local ret = 0
local bias = 0.01

ret = obc.add(obc, bias)
print(ret)

ret = obc.minus(obc, bias)
print(ret)
```
是否一定要传递self参数，看情况而定。如果函数不需要访问member，则没有必要
下面这种写法就是wrapper，这种多提供给别的client使用
```lua
local caculator = {}

function caculator.add(left, right)
    return left + right
end

function caculator.minus(left, right)
    return left - right
end

local r = 0
r = caculator.add(10, 20)
print(r)

r = caculator.minus(10, 20)
print(r)
```
我们再来看一种不是那么“规范”的写法
下面这种，也有实际成员，但是不传递参数。也可以
```lua
local obc = {
    x = 10,
    y = 20,
}

function obc.add(bias)
    obc.x = obc.x + bias
    obc.y = obc.y + bias
    return obc.x + obc.y
end

function obc.minus(bias)
    obc.x = obc.x + bias
    obc.y = obc.y + bias
    return obc.x - obc.y
end

local ret = 0
local bias = 0.01

ret = obc.add(bias)
print(ret)

ret = obc.minus(bias)
print(ret)
```
>dot的写法，是显示成员变量的写法，一般在用lua进行基于oo的编程时，考虑这么做。
这3种写法，对于第2种，有显著的用途，这好理解。第1种和第3种，我觉得第1种更好一点，但是实际中，没有这么用，因为这种是基于oo的写法，但是这么写没有泛化能力。
lua是一门很灵活的语言，没有太多的语法限制，这点要时刻清楚

下面我们来看colon的写法，这是lua语法提供的一种syntatic sugar的写法
```lua
local obc = {
    x = 10,
    y = 20,
}

function obc:add(bias)
    self.x = self.x + bias
    self.y = self.y + bias
    return self.x + self.y
end

function obc:minus(bias)
    self.x = self.x + bias
    self.y = self.y + bias
    return self.x - self.y
end

local ret = 0
local bias = 0.01

ret = obc:add(bias)
print(ret)

ret = obc:minus(bias)
print(ret)

-- 下面混用的写法正确
-- ret = obc.add(obc, bias)
-- print(ret)

-- ret = obc.minus(obc, bias)
-- print(ret)

```
优点是可以省略对于obj对象的传递
我们再进一步看下混用的例子：
```lua
--mymath.lua
local cal = {}

function cal:add(left, right)
    return left + right
end

function cal:minus(left, right)
    return left - right
end

return cal
```
```lua
--mymath1.lua
local cal1 = {}

function cal1.add(left, right)
    return left + right
end

function cal1.minus(left, right)
    return left - right
end

return cal1
```
```lua
--client.lua
local function SetScriptPath(script_path)
    local lua_package_path = package.path
    package.path = string.format("%s;%s?.lua;", lua_package_path, script_path)
end

local function Test1()
    local script_path = "/home/kang/tmp/lua-test/module-test"
    SetScriptPath(script_path)

    local cal = require("mymath")
    local ret = cal:add(3,4)
    print(ret)

    ret = cal.add(cal, 3, 4)
    print(ret)
end

local function Test2()
    local script_path = "/home/kang/tmp/lua-test/module-test"
    SetScriptPath(script_path)

    local cal = require("mymath1")
    local ret = cal.add(3,4)
    print(ret)

    ret = cal:add(3,4) -- !!!非法
    print(ret)
end

Test1()
Test2()

```
总结下，混用很容易出错。我不做区分，我只给出本质性的描述
- 定义
```lua
local cal = {}

function cal:add(left, right)
    return left + right
end

function cal:minus(left, right)
    return left - right
end

return cal
-- 上面这种定义，和下面等价
-- 上面定义其实就是省略了self参数
local cal1 = {}

function cal1.add(self, left, right)
    return left + right
end

function cal1.minus(self, left, right)
    return left - right
end

return cal1

```
- 调用
```lua
cal:add(3,4)
-- 这种调用，和下面等价
cal.add(cal, 3, 4)
```
所以，混用出错，主要就是参数对不上。这种要结合定义和调用的形式来看。
比如用colon定义，但是用dot调用。把colon定义转换成dot定义即可。
用dot定义，但是colon调用，把dot定义转换成colon定义即可。对于不带self的dot定义，没有办法，从调用的角度来看就好。

- 备注
1.colon定义时，调用要特别小心。因为，默认会把第一个参数，传递给self.因为colon定义，是显示的省略了self，但是self本身是存在的
2.cpp调用table当中的lua函数时，也需要小心。如果是colon定义，需要```lua_getglobal();```把table拿进来，当做函数的第一个参数
3.所以，目前代码写的少，这块没有特别多的经验。只能说我目前有认识到，如果c要调用lua，还是写成dot好

### lua热更新
热更新的概念不说了，basis当中有提到的。下面主要说下在lua当中进行热更新时，碰到的一些问题。
先看一个基本的例子
```lua
-- test.lua
local cal = {}

function cal:add(left, right)
    return left + right
end

function cal:minus(left, right)
    return left - right
end

return cal
```
```lua
-- main.lua
local function SetScriptPath(script_path)
    local lua_package_path = package.path
    package.path = string.format("%s;%s?.lua;", lua_package_path, script_path)
end

local function Sleep(n)
    os.execute("sleep " .. n)
end

local function DoServer()
    local ret = 0
    local left = 3
    local right = 3

    while true do
        if package.loaded["test"] then
            package.loaded["test"] = nil
        end
        local cal = require "test"
        ret = cal:add(left, right)
        print("ret = "..ret)

        Sleep(1);
    end
end

local function main()
    local script_path = "/home/kang/tmp/lua-test/hot-update-test"
    SetScriptPath(script_path)

    DoServer()
end

main()
```

## cpp

### 传值/传引用
```
c只有传值这一种方式，传指针本质上也只传值。
cpp引入了传引用这种方式。
对于非指针变量，这2种形式都非常容易理解。
```
但是，指针有一个非常重要的特性是，**当我们谈起指针时，我们通常关注的是指针所指向的变量**
看下面这段代码
```cpp
#include <stdio.h>
#include <iostream>
#include <string>

void print(const char* str) {
    printf("%s\n", str);
}

void print1(const char*& str) {
    printf("%s\n", str);
}

void printS(std::string str) {
    std::cout << str << std::endl;
}

void printS1(const std::string& str) {
    std::cout << str << std::endl;
}

int main(void) {
    const char* s = "hello, world";
    print(s);
    print1(s);

    std::string ss = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    printS(ss);
    printS1(ss);

    return 0;
}
```
printS的写法非常清晰：
1.字符串过长，值传递开销大，采用引用
2.值传递可以保证实参不受影响，const T&也可以保证实参不受影响

print的写法：
1.值传递，保证实参不受影响。const T*保证指针关联的变量不变。
2.传引用可以起到同样的效果。且少了值传递的开销。
但是，对于指针而言，其实不传递引用也是可以的
- 指针关注的是所指向的变量，关注本身较少
- 指向任何类型的指针，大小都是不定的。和机器字长有关系，所以值传递本身开销并不大
- 综上，使用指针作为参数，如果关注的是指针所指向的变量，没必要用引

ps：这个问题的背景是，google的cpp-style要求传入参数必须是引用。如果这个变量是指针呢？自己觉得不是非常必要。


### 类型转换

参考<br>
[强制转换运算符](https://docs.microsoft.com/zh-cn/cpp/cpp/casting-operators?view=vs-2019)

## c

### 缓冲区/fflush用法
- 清空stdin的正确做法

先看一段程序
```c
#include <stdio.h>

int main(void) {
    int val = 0;

    for(;;) {
        scanf("%d", &val);
        printf("%d\n", val);
    }
    return 0;
}
/*
输入3.2或者非整型数据
程序进入死循环无限输出
*/
```
>这是因为 scanf("%d", &i); 只能接受整数，如果用户输入了字母，则这个字母会遗留在“输入缓冲区”中。
因为缓冲中有数据，故而 scanf 函数不会等待用户输入，直接就去缓冲中读取，可是缓冲中的却是字母，
这个字母再次被遗留在缓冲中，如此反复

即，缓冲区的数据没有被清理。

问题的关键在于fflush(stdin)的做法是否可行？
直接给出结论，不行。因为这个操作不是标准c支持的，如果需要程序可移植，则不能这么做。

正确的做法
```c
#include <stdio.h>

int main(void) {
    int val = 0;
    char c;

    for(;;) {
        scanf("%d", &val);

        if(feof(stdin) || ferror(stdin)) break;

        while( (c=getchar()) != '\n' && c != EOF );

        printf("%d\n", val);
    }
    return 0;
}
/*
eof和\n不用丢弃的原因是，scanf对这个2个字符本身有处理能力
*/
```

参考<br>
[关于fflush(stdin)清空输入缓存流(C/C++) ](https://my.oschina.net/deanzhao/blog/79790)<br>

- fflush使用时机
1.对于输入/输出流，我理解是需要考虑fflush时机。虽然，我不确定这种流输入和输出使用的是同一个缓冲区。但是，看了有代码是这么考虑的。
```c
/* fflush example */
#include <stdio.h>
char mybuffer[80];
int main()
{
   FILE * pFile;
   pFile = fopen ("example.txt","r+");
   if (pFile == NULL) perror ("Error opening file");
   else {
     fputs ("test",pFile);
     fflush (pFile);    // flushing or repositioning required
     fgets (mybuffer,80,pFile);
     puts (mybuffer);
     fclose (pFile);
     return 0;
  }
}
```
上面这段代码的感觉是，输入/输出共用的是一个缓冲区，如果不清除输出缓冲区，数据会被输入获取。
因为清除缓冲区的时机是操作系统确定的，所以如果希望人为确定，还是主动fflush操作。

参考<br>
[fflush](http://www.cplusplus.com/reference/cstdio/fflush/?kw=fflush)<br>