TOPDIR = ../..

TARGET_O  = libcmdb.o
CP_FLAG = 1
OBJECTS_O = cmdb.o
DEPS      = $(OBJECTS_O:.o=.d)

.PHONY : all clean

all : $(TARGET_O)
include $(TOPDIR)/rules.Make

clean : 
	-$(RM) $(TARGET_O)
	-$(RM) $(OBJECTS_O)
	-$(RM) $(DEPS)
