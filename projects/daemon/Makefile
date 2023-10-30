TOPDIR = .

TARGET_PROG  = appd
OBJS = appd.o daemonize.o 
TARGET_PROG_OBJECTS += $(OBJS)
DEPS      = appd.d daemonize.d

.PHONY : all clean

all : $(TARGET_PROG)
include $(TOPDIR)/rules.Make

clean : 
	-$(RM) $(TARGET_PROG)
	-$(RM) $(OBJS)
	-$(RM) $(DEPS)
