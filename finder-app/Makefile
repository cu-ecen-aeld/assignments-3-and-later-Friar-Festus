# ?= is shorthand for ifndef VAR; statement; endif
CROSS_COMPILE ?=

GCC = gcc
CC = $(strip $(CROSS_COMPILE))$(strip $(GCC))
#CC += gcc
CFLAGS = -g
CFLAGS += -Wall
CFLAGS += -fPIC

target = writer
files = writer.c
objects = writer.o

$(target): $(objects)
	$(CC) $(CFLAGS) -o $(target) $(objects)

$(objects):
	$(CC) -c $(files) 

.PHONY: all

all:
	@echo CROSS_COMPILE is $(CROSS_COMPILE)

clean:
	@-rm -f writer writer.o ||:
