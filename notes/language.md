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

### Lua CAPI的正确用法/lua_cpcall

q:Lua CAPI的正确用法是什么？
>云风已经讲的很清楚了，分两种情况。其一，如果c编写lua库，此时Lua CAPI都是直接或间接被lua调用的，此时的异常肯定可以被lua_pcall捕获。其二，在调用lua之前，此时分布在宿主程序当中的Lua CAPI，对于这部分接口，没有一个捕获异常的逻辑。所以，云风的建议是，对于其二当中的情形，把主要业务逻辑写到一个lua_cfunction当中，然后用lua_pcall调用。

看一下lua-5.2.0的实现
```c
static int pmain (lua_State *L) {
  int argc = (int)lua_tointeger(L, 1);
  char **argv = (char **)lua_touserdata(L, 2);
  int script;
  int args[num_has];
  args[has_i] = args[has_v] = args[has_e] = args[has_E] = 0;
  if (argv[0] && argv[0][0]) progname = argv[0];
  script = collectargs(argv, args);
  if (script < 0) {  /* invalid arg? */
    print_usage(argv[-script]);
    return 0;
  }
  if (args[has_v]) print_version();
  if (args[has_E]) {  /* option '-E'? */
    lua_pushboolean(L, 1);  /* signal for libraries to ignore env. vars. */
    lua_setfield(L, LUA_REGISTRYINDEX, "LUA_NOENV");
  }
  /* open standard libraries */
  luaL_checkversion(L);
  lua_gc(L, LUA_GCSTOP, 0);  /* stop collector during initialization */
  luaL_openlibs(L);  /* open libraries */
  lua_gc(L, LUA_GCRESTART, 0);
  if (!args[has_E] && handle_luainit(L) != LUA_OK)
    return 0;  /* error running LUA_INIT */
  /* execute arguments -e and -l */
  if (!runargs(L, argv, (script > 0) ? script : argc)) return 0;
  /* execute main script (if there is one) */
  if (script && handle_script(L, argv, script) != LUA_OK) return 0;
  if (args[has_i])  /* -i option? */
    dotty(L);
  else if (script == 0 && !args[has_e] && !args[has_v]) {  /* no arguments? */
    if (lua_stdin_is_tty()) {
      print_version();
      dotty(L);
    }
    else dofile(L, NULL);  /* executes stdin as a file */
  }
  lua_pushboolean(L, 1);  /* signal no errors */
  return 1;
}


int main (int argc, char **argv) {
  int status, result;
  lua_State *L = luaL_newstate();  /* create state */
  if (L == NULL) {
    l_message(argv[0], "cannot create state: not enough memory");
    return EXIT_FAILURE;
  }
  /* call 'pmain' in protected mode */
  lua_pushcfunction(L, &pmain);
  lua_pushinteger(L, argc);  /* 1st argument */
  lua_pushlightuserdata(L, argv); /* 2nd argument */
  status = lua_pcall(L, 2, 1, 0);
  result = lua_toboolean(L, -1);  /* get result */
  finalreport(L, status);
  lua_close(L);
  return (result && status == LUA_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
}
```
从上面，我们可以看出，主业务逻辑都被封装在pmain当中，这个函数里面有大量Lua CAPI

q:lua_cpcall废弃的原因是什么？
>从lua-5.2.0当中的代码，我们可以看到，直接通过lua_pushcfunction把pmain入栈，然后再通过lua_pushxx传入参数。这个需要我们对比lua-5.1.5和lua-5.2.0的代码才可以看出其中的区别。
```c
// lua-5.1.5
#define lua_pushcfunction(L,f)	lua_pushcclosure(L, (f), 0)

LUA_API void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n) {
  Closure *cl;
  lua_lock(L);
  luaC_checkGC(L);
  api_checknelems(L, n);
  cl = luaF_newCclosure(L, n, getcurrenv(L));
  cl->c.f = fn;
  L->top -= n;
  while (n--)
    setobj2n(L, &cl->c.upvalue[n], L->top+n);
  setclvalue(L, L->top, cl);
  lua_assert(iswhite(obj2gco(cl)));
  api_incr_top(L);
  lua_unlock(L);
}

// lua-5.2.0
#define lua_pushcfunction(L,f)	lua_pushcclosure(L, (f), 0)

LUA_API void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n) {
  lua_lock(L);
  if (n == 0) {
    setfvalue(L->top, fn);
  }
  else {
    Closure *cl;
    api_checknelems(L, n);
    api_check(L, n <= MAXUPVAL, "upvalue index too large");
    luaC_checkGC(L);
    cl = luaF_newCclosure(L, n);
    cl->c.f = fn;
    L->top -= n;
    while (n--)
      setobj2n(L, &cl->c.upvalue[n], L->top + n);
    setclCvalue(L, L->top, cl);
  }
  api_incr_top(L);
  lua_unlock(L);
}

```
通过代码对比，我们可以发现一些区别，以及一些api设计上的原因：
- lua_pushcfunction的实现都是一样的
- lua语言支持了closure，function也算作一种特殊的closure.
- c语言不支持closure，只支持function
- 当lua和c交互时，存在两种语言数据类型不一致的情形，需要在交互时解决这个问题。具体，lua是这么解决的:
  - 提供lua_pushcclosure来支持lua语言当中的closure
  - 提供lua_pushcfunction来帮助c function转换成lua当中的closure
  - 基于前两点，我们发现Lua CAPI在设计的时候，对于第一点是直接进行支持，而第二点则是间接地支持
  - 对于lua_pushcfunction的实现，因为c function就是closure without upvalue，所以，lua_pushcclosure的最后一个参数肯定是0，这个是由c function和closure本身的关系决定的。 

紧接着，我们发现lua-5.1.5和lua-5.2.0在lua_pushcclosure实现上的区别，导致前者lua_pushcfunction有gc的开销，但是后者没有。
所以，当我们在lua-5.1.5这样的环境时，采用```lua_pushcfunction(L, &pmain);```这种写法，会由lua_pushcfunction带来额外的开销。而lua_cpcall则可以用来解决这个问题。

云风是这么说的：
>这就是为什么，之前版本的 Lua 都提供了一个叫 lua_cpcall 的 C API 的缘故。而在 Lua 5.2 支持了 light c function 后，对于无 upvalue 的 C function 都可以无额外成本的通过 lua_pushcfunction 传入 lua vm ，所以就不再需要一个单独的 lua_cpcall 了。

```c
// Calls the C function func in protected mode. 
// func starts with only one element in its stack, a light userdata containing ud. 
// In case of errors, lua_cpcall returns the same error codes as lua_pcall, plus the error object on the top of the stack; otherwise, it returns zero, and does not change the stack. 
// All values returned by func are discarded.
int lua_cpcall (lua_State *L, lua_CFunction func, void *ud);

static int pmain (lua_State *L) {
  struct Smain *s = (struct Smain *)lua_touserdata(L, 1);
  char **argv = s->argv;
  int script;
  int has_i = 0, has_v = 0, has_e = 0;
  globalL = L;
  if (argv[0] && argv[0][0]) progname = argv[0];
  lua_gc(L, LUA_GCSTOP, 0);  /* stop collector during initialization */
  luaL_openlibs(L);  /* open libraries */
  lua_gc(L, LUA_GCRESTART, 0);
  s->status = handle_luainit(L);
  if (s->status != 0) return 0;
  script = collectargs(argv, &has_i, &has_v, &has_e);
  if (script < 0) {  /* invalid args? */
    print_usage();
    s->status = 1;
    return 0;
  }
  if (has_v) print_version();
  s->status = runargs(L, argv, (script > 0) ? script : s->argc);
  if (s->status != 0) return 0;
  if (script)
    s->status = handle_script(L, argv, script);
  if (s->status != 0) return 0;
  if (has_i)
    dotty(L);
  else if (script == 0 && !has_e && !has_v) {
    if (lua_stdin_is_tty()) {
      print_version();
      dotty(L);
    }
    else dofile(L, NULL);  /* executes stdin as a file */
  }
  return 0;
}


int main (int argc, char **argv) {
  int status;
  struct Smain s;
  lua_State *L = lua_open();  /* create state */
  if (L == NULL) {
    l_message(argv[0], "cannot create state: not enough memory");
    return EXIT_FAILURE;
  }
  s.argc = argc;
  s.argv = argv;
  status = lua_cpcall(L, &pmain, &s);
  report(L, status);
  lua_close(L);
  return (status || s.status) ? EXIT_FAILURE : EXIT_SUCCESS;
}
```

q:lua_cpall废弃的原因是什么？
>lua-5.2.0开始以及之后的版本废弃lua_cpcall的原因是因为，对应的lua版本在lua_pushcclosure支持了无upvalue的写法，使得对于无 upvalue 的 C function 都可以无额外成本的通过 lua_pushcfunction 传入 lua vm


q:lua_cpcall废弃前后Lua CAPI的正确写法是什么？
>废弃后，pmain这样的函无upvalue函数，直接通过lua_pushcfunction传入lua vm。废弃前，则采用lua_cpcall。还有一点是，对于lua_cpcall，它会丢弃所有返回值。所以在lua_cpcall内部handle_script的错误，没法通过lua_cpcall来返回，因为lua_cpcall调用lua_scripit已经捕获这个错误了。所以需要再增加一个状态位来标识。对于lua_pcall则不用，可以把脚本错误值在pmain内部入栈，pmain的异常由高一层lua_pcall来监控。

q:注意lua-5.1.5和lua-5.2.0在返回值设计上的区别。
>lua-5.1.5的设计非常好理解，pmain内部，能发现的错误，证明已经被捕获了。返回0标识lua运行状态没问题，内部逻辑错误由s->status标识即可。如果pmain运行错误，此时的错误码被lua_cpcall捕获到。

参考
[[Lua C API 的正确用法]](https://blog.codingnow.com/2015/05/lua_c_api.html)

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

- 清空stdout

对于输入/输出流，需要使用fflush。此时，读写共用一个fd(FILE*)，关联同一个文件，自然而然使用同一个缓冲区。
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
上面这段代码先写，此时文件指针位于文件尾。如果此时读取，什么都读不到。
fflush感觉的作用是，可以调整文件指针位置。gcc version 4.8.4 (Ubuntu 4.8.4-2ubuntu1~14.04.4) 这个版本不行。

尝试下面的写法，是可以的。
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
     //fflush (pFile);    // flushing or repositioning required
     fseek(pFile, 0, SEEK_SET);
     fgets (mybuffer,80,pFile);
     puts (mybuffer);
     fclose (pFile);
     return 0;
  }
}
```

至于为什么不行，显然是移植性导致的。各自版本的实现，我暂时不细追。知道结论，如何使用即可。
所以这个版本，我理解清空缓冲区的目的是，写完成，让文件处于可读状态，即文件指针归位。

我们再看一个例子：
```c
#include<stdio.h>
#include<unistd.h>//unix环境
int main()
{
    printf("hello: ");
    //fflush(stdout);
    pid_t pid = fork();
    if(pid != 0) {
        printf("father\n");
    }
    else {
        printf("child\n");
    }
    return 0;
}
/*
期望的结果：
hello: father
child
或者
hello: child
father
总之，"hello: "的输出在创建进程前，肯定只能有父进程能输出。

实际结果：
hello: father
hello: child
*/
```
分析上面的结果，父进程先输出"hello: "，但是所谓的输出，只是把他放到了stdout的缓冲区，没有实际落终端。这个时机是在os控制。
然后，子进程copy了一份父进程，缓冲区自然也copy,最后程序结束的时候，os进行缓冲区的刷新。

我们进行修复：
```c
#include<stdio.h>
#include<unistd.h>//unix环境
int main()
{
    printf("hello: ");
    fflush(stdout);
    pid_t pid = fork();
    if(pid != 0) {
        printf("father\n");
    }
    else {
        printf("child\n");
    }
    return 0;
}
/*
无论如何，只会输出一个hello.
*/
```
这是个非常好的例子，在设计多进程/线程程序时，缓冲区是否共享，是否需要及时刷新，是需要特别考虑的。


参考<br>
[fflush](http://www.cplusplus.com/reference/cstdio/fflush/?kw=fflush)<br>
[一些 C 函数，痛苦的移植性](https://tnie.github.io/2017/03/07/portability-about-C-functions/)<br>