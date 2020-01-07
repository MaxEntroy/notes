## lua

### lua table用法注意

q:lua table对于元素的方式有几种？
>线性表(数组)只有一种，使用下标运算符[]; 对于哈希表，则有两种，dot and subscript.

q:lua table做哈希表时，对于元素的访问有哪些注意？
>lua table做哈希表时，key可以是任何除了nil之外的任意元素。对于访问时，要特别注意dot的方式，这本质是一种grammar sugar.

```
local tb = {}
tb.name = "kungli" -- tb["name"] = kungli

local key = "age"
tb.key = 18 -- tb["key"] = 18 dot方式把key直接扩展成字符串常量充当key
```

### local变量个数
```
local变量个数上限200个，使用local变量可以提高效率，但是应该避免滥用
lua 5.3.0: ttt.lua:202: too many local variables (limit is 200) in function at line 1 near '='
luajit 2.0.5: ttt.lua:202: function at line 1 has more than 200 local variables
```

参考<br>
[高性能 Lua 技巧（译）](https://segmentfault.com/a/1190000004372649)<br>

### colon and dot
q:混用是需要注意什么？
对于colon，Lua程序设计p228给了本质性描述
- 方法定义时，增加一个额外的形参
- 方法调用时，增加一个额外的实参

如果进行混用，主要就是判断，**调用时和定义时的参数是否对的上**即可。

q:c-lua交互时需要注意什么？
> cpp调用table当中的lua函数时，也需要小心。如果是colon定义，需要lua_getglobal()把table拿进来，当做函数的第一个参数

我们来看一个例子
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

q:colon和dot使用时机是什么？
>如果函数中需要用到self，那么定义时采用colon，调用时采用colon。因为这么做可以避免显示的定义一个参数。如果函数中不用到self，采用dot定义，因为如果用colon定义，增加一个形参没有用，调用时采用dot，如果colon会多传一个实参，导致参数个数不匹配

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
从上面，我们可以看出，主业务逻辑都被封装在pmain当中，这个函数里面有大量Lua CAPI。并且，我们可以看到，直接通过lua_pushcfunction把pmain入栈，然后再通过lua_pushxx传入参数。这个需要我们对比lua-5.1.5和lua-5.2.0的代码才可以看出其中的区别。

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

q:lua_pcall和lua_cpcall在使用上有何区别？
>文档说，lua_cpcall的func即pmain，开始的时候，只有一个参数在func的栈里面。那就是ud即s
pmain是lua_CFunction，本质是lua调用c函数的形式，只不过由c发起。
pmain的特点就是有一个private stack，只用来跟lua交互
真正想表达的事，对于lua-5.2.x开始的版本：
1.func不用显示入栈
2.ud不用显示入栈
3.后续版本需要lua_pushcfunction and lua_pushlightuserdata
4.原理我暂时不深究，知道这么用就好

参考<br>
[Lua C API 的正确用法](https://blog.codingnow.com/2015/05/lua_c_api.html)

### lua函数/lua闭包

q:什么是定界(scope)?
>变量与变量所对应的实体之间绑定关系的有效范围，在部分情况下，也常与可见性混用。对于这句话，要小心后半句，我们对可见性是非常熟悉的，即变量可以用的范围。但是定界不完全是这个意思

q:静态定界vs动态定界
>静态定界，一个变量的可见性范围仅严格的与组成程序的静态具体词法上下文有关，而与运行时的调用堆栈无关；动态定界，一个变量的可见性范围在编译时无法确定，依赖于运行时的实际堆栈调用情况

q:什么是词法定界？
>词法定界即静态定界，一个变量的可见性范围仅依赖于编译时程序的具体词法上下文

q:词法定界意味着什么？
>当编写一个被其它函数B包含的函数A时，被包含的函数A可以访问包含其的函数B的所有局部变量，这种特性为词法定界所拥有(lexical scoping means that functions can assess variables of their enclosing functions)

关于上面这个描述，我们还可以补充以下细节(第1点是按自己理解确定的)

- 函数B称作enclosing function
- 函数A称作enclosure
- 函数B中的局部变量称作non-local variable(lua中也称作up value)

对于第3点，也好理解，函数B中的局部变量，在函数A中不是local variable，也不是global variable,而是non-local variable.
通过词法定界，我们其实引入了闭包。这个概念不好直接理解，上文中其实是通过一种追本溯源的方式去理解。

q:到底什么是闭包？
>闭包是一种机制，一个闭包就是一个函数外加能够使该函数访问non-local variables所需要的其他机制。本质来说，遵循词法定界的函数，就是一个闭包。

q:lua函数是什么？
>lua函数严格遵循词法定界和第一类值

详细解释下
- 词法定界
  - lua函数可以访问包含其自身的函数当中的局部变量
  - 技术上来说，lua只有闭包，没有函数。因为Lua函数遵循词法定界，这就导致lua当中所谓的函数，不过是闭包的特例
- 第一类值
  - 函数可以被保存到变量中
  - 函数可以作为参数，传递给其它函数
  - 函数可以作为返回值，在某个函数中被返回

q:lua闭包使用心得?
>Lua CAPI是另外一回事，如果在lua语言中使用，一定要注意闭包的语法特性，即一定发生在嵌套函数定义中，这个在c/c++/java这样的语言中是不支持的。简而言之，就是函数内部定义的函数。严格意义来说，还包括了他所能截获的外部变量。外部变量，即外部函数内的局部变量。

q:什么是upvalue
>狭义来说，外部函数的局部变量就是upvalue.

q:如何理解upvalue?
>功能上类似c语言当中的static变量，用来描述函数的状态。所以闭包是有状态的，状态通过upvalue来体现。upvalue的意义在于状态记录和共享。

q:闭包和upvalue的关系？
- 同时返回的多个闭包，共享一个upvalue，修改其中一个，会影响其它
- 不同次返回的闭包，状态独立

### 环境
q:required chunk运行在什么环境？
>默认是全局环境(A required chunk runs in global environment by default.)

q:环境这个东西怎么理解？
>pil有过清晰的解释，起因是为了避免全局变量的滥用引入的。我今天在排查问题的时候，有了新的认知。闭包 = lua chunk + upvalue + env
因为我之前不理解的是，require的lua chunk，和宿主用的不是同一个env，在看过云风的一篇lua require扩展性为的文章后才知道，require的时候，会运行lua chunk，此时肯定需要一个env.
这个env是default env，云风要做的就是，在宿主当中，绑定env给到required lua chunk

```lua
-- client.lua
local function foo()
    local print = print
    local require = require
    
    _ENV = {}
    _G = {}

    print("foo:")
    print(_ENV)
    print(_G)

    local cal = require("cal")
    cal:add(3,4)
end

foo()

local cal = {}

function cal:add(left, right)
    print("cal: ")
    print(_ENV)
    print(_G)
    return left + right
end

function cal:minus(left, right)
    return left - right
end

return cal
--[[
  foo:
table: 0x1955410
table: 0x1954ce0
cal: 
table: 0x194e5f0
table: 0x194e5f0
]]
```

参考<br>
[Lua require chunk environment](https://stackoverflow.com/questions/28502827/lua-require-chunk-environment)<br>
[扩展lua require的行为](https://blog.codingnow.com/2015/10/lua_require_env.html)<br>
[以自定义方式加载 lua 模块](https://blog.codingnow.com/2007/04/user_define_lua_loader.html)<br>

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

### std::function

q:std::function有什么用？
>Class template std::function is a general-purpose polymorphic function wrapper.

q:如何理解？
>我理解是对于函数作为一等公民的支持，cpp本身不支持，所以通过一个function wrapper来实现这种语义。

q:函数指针类型不安全？
>还有很重要一点在于，std::function是类型安全的，但是诸如函数指针这样的可调用对象，并不是类型安全的。
对于函数指针，定义int(*)(int, int)，如果我们实际传递int(*)(int)，也是可以的，fn不过是一个指向函数的指针，我们无法判断这个指针实际指向什么，像参数和返回类型等无从知晓。

```cpp
#include <iostream>
#include <functional>

int add(int a, int b) {
    return a + b;
}

// 编译通过 正常执行 但逻辑错误
void test_func_pointer() {

    typedef int(*func1)(int);
    func1 f1 = (func1)add;
    int ret = f1(20);
    std::cout << "ret1 = " << ret << std::endl;

    typedef int(*func2)(int, int, int);
    func2 f2 = (func2)add;
    int ret2 = f2(20, 30, 10);
    std::cout << "ret2 = " << ret2 << std::endl;
}

// 编译不通过 无法执行
void test_function_object() {

    typedef std::function<int(int)> func1;
    func1 f1 = (func1)add;
    int ret = f1(20);
    std::cout << "ret1 = " << ret << std::endl;

    typedef std::function<int(int, int, int)> func2;
    func2 f2 = (func2)add;
    int ret2 = f2(20, 30, 10);
    std::cout << "ret2 = " << ret2 << std::endl;
}

int main(int argc, const char * argv[])
{
    test_func_pointer();
    test_function_object();
    return 0;
}
/*
引入std::function，可以获得确切的类型信息，类型不匹配无法转换。
但是函数指针不具备这个能力。
ps:为什么int,float,double类型不一致，也可以强转。因为int,float,double之间的本质类型都是"number"，区别在于精度。
但是，函数标签不一样，根本就不是同一种类型
*/
```

参考<br>
[C/C++函数指针参数不匹配问题](https://blog.csdn.net/oktears/article/details/39452341)<br>

## c

### 头文件/源文件分离

q:不带头文件，编译有什么影响？
>从下面的代码中，我们明显可以看出：
1.main.c缺少了foo.h
2.foo.c缺少了stdio.h
从编译结果来看，main.o,foo.o的编译，都提示了函数未声明的告警，但是并不是error，最后已编译没问题。
我的理解，可以的。
编译过程没问题，只是缺少另一个模块的函数。链接过程中，都有缺少函数的定义，也没问题。

```makefile
CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra
LDFLAGS=
LIBS=

main: main.o foo.o
	$(CC) -o $@ $(LDFLAGS) main.o foo.o $(LIBS)

main.o: main.c
foo.o: foo.c

clean:
	rm -rf main main.o foo.o

.PHONY: clean
```

```c
// main.c
int main(void) {
    myprint("main", "hello, world");
    return 0;
}

// foo.c
void myprint(const char* pname, const char* msg) {
    printf("%s:%s\n", pname, msg);
}

```

编译结果
```makefile
gcc -std=gnu99 -Wall -Wextra   -c -o main.o main.c
main.c: In function ‘main’:
main.c:2:5: warning: implicit declaration of function ‘myprint’ [-Wimplicit-function-declaration]
     myprint("main", "hello, world");
     ^
gcc -std=gnu99 -Wall -Wextra   -c -o foo.o foo.c
foo.c: In function ‘myprint’:
foo.c:4:5: warning: implicit declaration of function ‘printf’ [-Wimplicit-function-declaration]
     printf("%s:%s\n", pname, msg);
     ^
foo.c:4:5: warning: incompatible implicit declaration of built-in function ‘printf’ [enabled by default]
gcc -std=gnu99 -o main  main.o foo.o 
```

参考<br>
[warning-implicit-declaration-of-function(https://stackoverflow.com/questions/8440816/warning-implicit-declaration-of-function)

**结论**：没有头文件，一样可以运行。结合我这两天看书的心得，我们知道
- 源文件的编译都是相互独立的，如果遇见未定义的符号，待链接时计算该符号地址
- 链接时，对于未定义的符号，会在机器.o当中寻找，重新计算改符号地址。
从这个较多来说，有没有头文件，确实没什么帮助。

q:c/cpp为什么要采用头文件源文件这样的一种写法呢？java,c#,lua,python都没有这样实现？
>历史原因肯定是有的，可以仔细看看后面说的语言？java,lua,python都是解释型语言，不会生成.o这样的文件。
最直观的解释，接口和实现分离！这个是有道理的。
1.如果我写个第三方库，给你个.a，不给你.h，你怎么用？
2.编译器会帮我们做必要的检查，比如函数声明和函数定义是否匹配。如果没有，则不检查。执行时会core.

看下面代码，makefile保持不变
```c
// main.c
int main(void) {
    myprint("main");
    return 0;
}

// foo.c
void myprint(const char* pname, const char* msg) {
    printf("%s:%s\n", pname, msg);
}
/*
结果
main:}�?��
*/
```
很明显，输出了异常值。这本质上来说，破坏了栈空间，访问了垃圾值

如果我们加上头文件，则会有如下编译结果。即，编译器帮助我们检查了声明，和调用是否相符。提前帮助我们解决了问题。
```c
gcc -std=gnu99 -Wall -Wextra   -c -o main.o main.c
main.c: In function ‘main’:
main.c:3:5: error: too few arguments to function ‘myprint’
     myprint("main");
     ^
In file included from main.c:1:0:
foo.h:4:6: note: declared here
 void myprint(const char* pname, const char* msg);
      ^
make: *** [main.o] 错误 1
```

可以参考下，下面两个链接。
参考<br>
[为什么c++要“在头文件中声明，在源文件中定义”？](https://www.zhihu.com/question/58547318)<br>
[为什么C/C++要分为头文件和源文件？](https://www.zhihu.com/question/280665935)<br>
[C语言的头文件是必须的吗？](https://blog.csdn.net/blow_jj/article/details/42219709)

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

### 全局变量/keyword extern
q:什么是全局变量?
>External variables are also known as global variables.These variables are defined outside the function. 

q:全局变量的scope?lifetime?default value?
>scope: They are not bound by any function. They are everywhere in the program i.e. global.(说的更详细一点，定义在函数之外，没有被static修饰的变量，作用于是全局的。否则就是文件级别)
lifetime: Till the end of the execution of the program.
default value: Default initialized value of global variables are Zero.

q:extern的作用是什么？
>“extern” keyword is used to declare and define the external variables.(严格来说，gcc-4.8.4extern只用来声明全局变量，定义时使用extern会报一个warning)

q:extern在c中有哪些注意点？
- External variables can be declared number of times but defined only once.
- “extern” keyword is used to extend the visibility of function or variable.

extern最主要的作用，**通过声明全局变量，来扩展全局变量的作用域**。

- By default the functions are visible throughout the program, there is no need to declare or define extern functions. It just increase the redundancy.
- Variables with “extern” keyword are only declared not defined.(定义时，使用extern也行，只是不建议)

注意，定义在函数之外的变量(全局变量)，默认是extern，定义是无须再使用extern，只有声明一个全局变量时才使用。

- Initialization of extern variable is considered as the definition of the extern variable.

```c
#include <stdio.h>
extern int g_a = 1; // D 定义全局变量 有初始值 extern 没必要
extern int g_b;     // 声明全局变量

int g_c = 2;        // D 定义全局变量 有初始值
int g_d;            // C 定义全局变量 无初始值

static int g_e = 3; // d 定义全局变量 有初始值 文件级别
static int g_f;     // b 定义全局变量 无初始值 文件级别

int main(void) {
    printf("g_a: %d\n", g_a);
    printf("g_c: %d\n", g_c);
    printf("g_d: %d\n", g_d);
    printf("g_e: %d\n", g_e);
    printf("g_f: %d\n", g_f);
    return 0;
}
/*
0000000000000000 D g_a
0000000000000004 D g_c
0000000000000004 C g_d
0000000000000008 d g_e
0000000000000000 b g_f
0000000000000000 T main
                 U printf
*/
```

参考<br>
[“extern” keyword in C](https://www.tutorialspoint.com/extern-keyword-in-c)<br>
[warning in extern declaration](https://stackoverflow.com/questions/4268589/warning-in-extern-declaration)

### 低维信息用高维表示

```c
#include <stdio.h>
#include <stdint.h>

int main() {
  uint32_t val11 = 0xffffffff;
  uint32_t val12 = -1;

  printf("%#x\n", val11);
  printf("%#x\n", val12);

  if(val11 == val12) {
    printf("val11 == val12\n");
  }
  else {
    printf("val11 != val12");
  }

  uint64_t val21 = 0xffffffff;
  uint64_t val22 = -1;

  printf("%#lx\n", val21);
  printf("%#lx\n", val22);

  if(val21 == val22) {
    printf("val21 == val22\n");
  }
  else {
    printf("val21 != val22\n");
  }
  return 0;
}
/*
0xffffffff
0xffffffff
val11 == val12
0xffffffff
0xffffffffffffffff
val21 != val22
*/
```