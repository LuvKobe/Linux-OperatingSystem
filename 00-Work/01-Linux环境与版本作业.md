### 1.  以下哪个命令输出Linux内核的版本信息

**🔍 选项**

```
A.uname -r

B.vmstat

C.sar

D.stat
```

**✨ 解析**

```
答案：A

vmstat：用来反映系统的负载情况，像有关进程、虚存、页面交换空间及CPU活动的信息。

sar：System ActivityReporter系统活动情况报告）是目前Linux上最为全面的系统性能分析工具之一，可以从多方面对系统的活动进行报告，包括：文件的读写情况、系统调用的使用情况、磁盘I/O、CPU效率、内存使用状况、进程活动及IPC有关的活动等。

stat：用于显示文件的状态信息
```



### 2. `linux 2.6.*` 内核默认支持的文件系统有哪些（多选）

**🔍 选项**

```
A.ext3

B.ext2

C.ext4

D.xfs

E.ufs
```

**✨ 解析**

```
答案：A、B、C、D、E
```



### 3. linux查看cpu占用的命令是什么

**🔍 选项**

```
A.top

B.netstat

C.free

D.df
```

**✨ 解析**

```
答案：top

netstat：用于显示各种网络相关信息，如网络连接，路由表，接口状态 (Interface Statistics)，masquerade 连接，多播成员 (Multicast Memberships) 等等。

free：显示Linux系统中空闲的、已用的物理内存及swap内存，及被内核使用的buffer

df：检查linux服务器的文件系统的磁盘空间占用情况。
```



### 4. 在Linux系统中, 为找到文件 `try_grep` 含有以a字母为行开头的内容, 可以使用命令

**🔍 选项**

```
A.grep -E #$ try_grep

B.grep -E #a try_grep

C.grep -E ^$ try_grep

D.grep -E ^a try_grep
```

**✨ 解析**

```
答案：D
```



### 5. 在使用mkdir命令创建新的目录时，在其父目录不存在时先创建父目录的选项是

**🔍 选项**

```
A.-m

B.-d

C.-f

D.-p
```

**✨ 解析**

```
答案：D

-p：创建多级目录
```



### 6. 批量删除当前目录下后缀名为 `.c` 的文件。如a.c、b.c

**🔍 选项**

```
A.rm *.c

B.find . -name "*.c" -maxdepth 1 | xargs rm

C.find . -name "*.c" | xargs rm

D.以上都不正确
```

**✨ 解析**

```
答案：A
```



### 7. Linux 有三个查看文件的命令，若希望在查看文件内容过程中可以用光标上下移动来查看文件内容，应使用那个命令

**🔍 选项**

```
A.cat

B.more

C.less

D.menu
```

**✨ 解析**

```
答案：C
```































