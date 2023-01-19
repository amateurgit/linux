TOPDIR = ..

TARGET_PROG  = init
OBJS = init.o
TARGET_PROG_OBJECTS += $(OBJS)
TARGET_PROG_OBJECTS += $(TOPDIR)/obj/libcmdb.o
DEPS      = init.d

.PHONY : all clean

all : $(TARGET_PROG)
include $(TOPDIR)/rules.Make

clean : 
	-$(RM) $(TARGET_PROG)
	-$(RM) $(OBJS)
	-$(RM) $(DEPS)
