# AndroidBinderGotHook
android binder got hook

这个repo里只有一个so，注入工具参考另外一个库https://github.com/EggUncle/PtraceInject

这里就是hook了一下ioctl，关于binder协议的部分还没看，后续可能会补充这个。

### hook相关的表项
和gothook相关的elf有下面几项
- .dynsym 符号表
- .dynstr 字符串表，elf的符号表中的符号名字一项中，就是字符串表内容的索引
- .rela.plt 重定位表，在so和可执行文件中，重定位表的内容是一个虚拟地址，指向目标符号所在的位置

关于elf文件格式的详细信息https://paper.seebug.org/papers/Archive/refs/elf/Understanding_ELF.pdf

### hook流程
这里仅从注入后的入口开始描述hook的流程

1. 读取目标so文件，解析出符号表，字符串表，重定位表。

2. 遍历重定位表，根据重定位表的信息获取符号信息，再根据符号信息和字符串表信息获取到符号的名称，got表中存储ioctl的地址的项。

3. 替换got表中保存目标函数地址的项的内容。

### 踩到的坑
有个地方确实花了挺长时间的，在获取目标so的基地址的时候，一般来说就是maps里面对应项的地址，但是这是在大多数情况下。精确的获取目标地址的方式，应该是maps中获取到的对应项的地址，减去elf中第一个类型为PT_LOAD且vaddr以及offset为0的segment，大部分情况下，第一个PT_LOAD的vaddr都是0，但是这回要hook的libbinder不是。。。嗨呀真的菜。
