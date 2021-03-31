# c-programming-toys

记录一些在学习Unix/Linux系统编程与复习c语言的过程中编写的有意思的程序

对于每一个程序都会有一篇简短的文章介绍**编程思路**与所需的**背景知识**

第一阶段的计划是记录Unix/Linux系统编程学习过程中编写的一些程序。本阶段以
《Unix/Linux编程实践教程》一书以及视频[Linux系统编程](https://www.bilibili.com/video/BV1yJ411S7r6) 的内容作为主线，并参考《Linux/Unix系统编程手册》
以及《Unix高级环境编程》两本书籍的内容进行深入学习。编写的程序涵盖书中部分例子的仿写与书中部分习题的实现，包括：

- [x] 应用文件io(系统调用io)与标准io的各套读写函数实现简易`cp`命令，并添加复制空洞文件的功能（IO相关）
- [x] 参照man手册使用`stdio`实现自己的`GNU getline(`)函数（IO相关）
- [x] 使用共享文件的方法实现一个可以删除文件指定行内容的程序，其功能等同于命令（IO相关）：
  
    `sed -i 'Md' filename'`

- [x]  实现一个与`du -s`命令功能相同的程序（文件系统相关）
- [ ]  仿照`ls`实现`myls`命令（文件系统相关）

...

第二阶段的计划是对《csapp》一书中后三个实验的实现，除了对实验本身的实现外，
还将对这些实验涉及到的一些相关概念与算法进行实现，包括：

- [x] Malloc Lab: Writing A Dynamic Storage Allocator（实现了基于隐式空闲链表与基于分离适配算法的两种allocator，对后者进行了完整优化）
- [ ] Malloc Lab相关：Buddy System的一个极简实现

...
此仓库将持续更新，欢迎⭐