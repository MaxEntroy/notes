如云风所言，Lua 作为一门嵌入式语言，提供了完备的 C API 供 Lua 代码和宿主程序交互。但是正确将 Lua 嵌入是不太容易做对的事情。本文尝试从源码角度展开，总结Lua CAPI的正确用法，以及该方法在lua-5.2.x前后的一些异同。

## Lua CAPI的正确用法
云风已经讲的很清楚了，分两种情况。其一，如果c编写lua库，此时Lua CAPI都是直接或间接被lua调用的，此时的异常可以被宿主当中的lua_pcall捕获。其二，在调用lua之前，此时分布在宿主程序当中的Lua CAPI，对于这部分接口，没有一个捕获异常的逻辑。所以，云风的建议是，对于这部分Lua CAPI，把主要业务逻辑写到一个lua_CFunction当中，再用逻辑上层的lua_pcall调用该函数。

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
从上面我们可以看出，主业务逻辑都被封装在pmain当中，这个函数里面有大量Lua CAPI。

## lua_cpcall被废弃的原因
对于lua-5.2.0的代码，如果我们和lua-5.1.x的代码进行对比，会发现他们在向lua vm传入c function和参数的时候，略有不同。我们先上一段lua-5.1.5的代码，
```lua
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
对比lua-5.1.5和lua-5.2.0的代码，很明显可以发现一些区别
- lua-5.2.0采用lua_pushcfunction和lua_pushxx向lua vm传入c function和参数
- lua-5.1.5lua_cpcall的方式，不进行显示c function和参数的传递

我们进一步分析，可以发现```lua_pushcfunction```在lua-5.1.x和lua-5.2.0这两个版本都有支持，而```lua_cpcall```只在lua-5.1.5支持，后续的版本都废弃了。所以我们有理由猜测可能```lua_pushcfunction```发生了一些变化，导致```lua_cpcall```的一些功能被替代，从而导致废弃。

```lua
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

紧接着，我们发现lua-5.1.5和lua-5.2.0在lua_pushcclosure实现上的区别，**前者lua_pushcfunction有gc的开销，但是后者没有**。
所以，当我们在lua-5.1.5这样的环境时，采用```lua_pushcfunction(L, &pmain);```这种写法，会由lua_pushcfunction带来额外的开销。而lua_cpcall则可以用来解决这个问题。

云风是这么说的：
>这就是为什么，之前版本的 Lua 都提供了一个叫 lua_cpcall 的 C API 的缘故。而在 Lua 5.2 支持了 light c function 后，对于无 upvalue 的 C function 都可以无额外成本的通过 lua_pushcfunction 传入 lua vm ，所以就不再需要一个单独的 lua_cpcall 了。

参考<br>
[Lua C API 的正确用法](https://blog.codingnow.com/2015/05/lua_c_api.html)