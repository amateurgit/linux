ROOT = ..
include $(ROOT)/Make.defines

DIRS   = $(shell find . -type d)
SRCS   = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
OBJS   = $(patsubst %.c, %.o, $(SRCS))
DEPS   = $(patsubst %.c,%.d, $(SRCS))
TARGET = main

.PHONY: all clean

all: $(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.d: %.c 
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(DEPS)

clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJS)
	-$(RM) $(DEPS)
