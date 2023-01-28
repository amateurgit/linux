# 编译器
CC = gcc

# 链接器
LD = ld

# 归档工具
AR = ar

# 编译选项
CFLAGS = -Wall -O2

# 头文件搜索路径
INCS = \
    -I$(TOPDIR)/cmf/include

# 库文件搜索路径
LDFLAGS = \
    -L$(TOP_DIR)/lib

# 依赖库
# LIBS =

# 递归执行 Makefile
ifdef SUBDIRS
.PHONY : $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)
endif

# 归档目标文件
ifdef TARGET_O
$(TARGET_O) : $(OBJECTS_O)
	$(LD) -r -o $@ $^; \
	if [ $(CP_FLAG) -eq 1 ]; then \
	    cp $@ $(TOPDIR)/obj; \
	fi; 
endif

# 可执行程序
ifdef TARGET_PROG
$(TARGET_PROG): $(TARGET_PROG_OBJECTS)
	$(CC) -o $@ $(LDFLAGS) $(TARGET_PROG_OBJECTS)
endif

# 编译目标文件 \
    -MMD : 生成依赖关系和目标文件, 预处理后不停止编译 \
    -MF  : 指定依赖文件的路径
%.o : %.c
	$(CC) $(CFLAGS) $(INCS) $(LDFLAGS) -c $< -o $@ -MMD -MF $(@:.o=.d)

# 目标文件依赖加入规则, 头文件更改即可自动重新编译
-include $(DEPS)
