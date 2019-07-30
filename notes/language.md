## 语言

#### lua

- local变量个数
```
local变量个数上限200个，使用local变量可以提高效率，但是应该避免滥用
lua 5.3.0: ttt.lua:202: too many local variables (limit is 200) in function at line 1 near '='
luajit 2.0.5: ttt.lua:202: function at line 1 has more than 200 local variables
```

参考<br>
[高性能 Lua 技巧（译）](https://segmentfault.com/a/1190000004372649)<br>

- colon and dot
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

ret = obc.add(obc, bias)
print(ret)

ret = obc.minus(obc, bias)
print(ret)

```
优点是可以省略对于obj对象的传递

#### cpp

- 传值/传引用
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