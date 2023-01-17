# 编译器选择
CC = gcc

# 编译选项 \
  -Wall 输出编译器警告信息 \
  -O2   优化等级 O2, 执行几乎所有支持的优化算法, 提高执行效率, 牺牲编译性能。
CFLAGS = -Wall -O2

# 当前目录下 符合模式规则的文件名 生成以空格分隔的字符串 "hello.c print.c"
SRCS   = $(wildcard *.c)

# 文件模式替换 "hello.o print.o"
OBJS   = $(patsubst %.c,%.o,$(SRCS))
DEPS   = $(patsubst %.c,%.d,$(SRCS))

TARGET = hello

# PHONY , 避免文件名和目标名(all, clean) 相同时, 无法编译的情况 
.PHONY : all clean

all : $(TARGET)

# 目标文件编译成可执行文件
$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ 

# 源文件编译成目标文件 \
  隐含规则

# 依赖文件模式规则 \
  @ : 不输出该行命令 \
  set -e : 该句命令执行失败(返回非 0)时立即退出 \
  $(CC) -MM $< > $@.$$$$ : 生成依赖关系, 写入临时文件 $@.$$$$, $$$$ 表示当前进程号, $< 表示第一个依赖文件 \
  sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@ : \
	sed '/s/match/replace/g' 模式, 文件中查找 match 关键字, 将其替换为 replace \
        实现的效果 : "hello.o : hello.c hello.h" 替换为 "hello.o hello.d : hello.c hello.h", 即插入了一个 "hello.d" 字符串 \
        "\($*\)\.o[ :]*" 匹配 "hello.o : ", 对应 match; "\1.o $@ " 就是 "hello.o hello.d : ", 对应 replace 
%.d : %.c
	@set -e; rm -f $@; \
	$(CC) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# 目标文件的依赖关系包含进来, "hello.o hello.d : hello.c hello.h" \
  -include 前的 - 表示执行失败时, 不打印提示信息, 继续执行
-include $(DEPS)

clean :
	-$(RM) $(TARGET)
	-$(RM) $(OBJS)
	-$(RM) $(DEPS)
