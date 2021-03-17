# c-programming-toys

记录一些在复习巩固c语言编程的过程中编写的有意思的程序

对于每一个程序都会有一篇简短的文章介绍**编程思路**与所需的**背景知识**

第一阶段的计划是记录Unix/Linux系统编程学习过程中编写的一些程序。本阶段以
《Unix/Linux编程实践教程》一书中的内容作为主线，并参考《Linux/Unix系统编程手册》
以及《Unix高级环境编程》两本书籍的内容进行深入学习。编写的程序涵盖书中部分例子的仿写与书中部分习题的实现，包括：

- [x] 应用文件io(系统调用io)实现一个最简易的cp命令
- [x] 应用标准io的各套读写函数实现cp命令
- [x] 为cp命令添加复制空洞文件(file holes)的功能
- [x] 参照man手册使用stdio实现自己的GNU getline()函数

...

第二阶段的计划是对《csapp》一书中后三个实验的实现，除了对实验本身的实现外，
还将对这些实验涉及到的一些相关概念与算法进行实现，包括：

- [x] Malloc Lab: Writing A Dynamic Storage Allocator（实现了基于隐式空闲链表与基于分离适配算法的两种allocator，对后者进行了完整优化）
- [ ] Malloc Lab相关：Buddy System的一个极简实现

...
此仓库将持续更新，欢迎⭐