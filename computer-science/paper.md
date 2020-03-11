## Why Google Stores Billions of Lines of Code in a Single Repository

### Google Scale
这一部分主要说了google他们提了很多代码，代码规模非常之大

### Background

- Piper and CitC
1. google代码仓库叫做Piper，讲了一些这个仓库的一些运行情况。
2. 然后开始说之前的实践是在Perforce上，开发Piper的主要目的是因为代码量增加的很快，前者显得捉襟见肘
3. 开始讲pipter的设计，这其中提到安全性是google特别考虑的。因为公司把代码视作非常重要的财产。
4. piper-workflow
5. 开始说CitC(client in the cloud)的作用。
5.1. piper的客户端，类似云上的一个文件系统
5.2. piper支持快照功能
5.3. 5.1当中提到，citc类似一个云上的文件系统，所以开发者所有的工作都保存在云上，这是google workflow process很重要一点
5.4. piper支持了比较多的工具，代码检查，代码浏览，周围生态工具比较齐全。
5.5. piper也能不通过citc使用
5.6. 总结下，上文提到的各种优点，piper和citc联合使用，威力强大

- Trunk-based development
1. 这么做的优点在于避免分支的合并。
2. 新老功能并存的情形，通过加开关的逻辑来避免新分支的开发，但是这同时也引入了额外的复杂度。A/B test也是用了类似的办法