## 语言

#### lua

- local变量个数
local变量个数上限200个，使用local变量可以提高效率，但是应该避免滥用
```
lua 5.3.0: ttt.lua:202: too many local variables (limit is 200) in function at line 1 near '='
luajit 2.0.5: ttt.lua:202: function at line 1 has more than 200 local variables
```

参考<br>
[高性能 Lua 技巧（译）](https://segmentfault.com/a/1190000004372649)<br>