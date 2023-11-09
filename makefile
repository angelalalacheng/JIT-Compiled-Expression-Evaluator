#
# Tony Givargis
# Copyright (C), 2023
# University of California, Irvine
#
# CS 238P - Operating Systems
# Makefile
#

#specific version of c compiler
CC     = gcc 
#Extra flags to give to the C compile to expose bugs
CFLAGS = -ansi -pedantic -Wall -Wextra -Werror -Wfatal-errors -fpic -O3 
# GCC will report warnings and errors 
# Wall: report error message / Werror: make warning report as error / Wfatal-errors: give the first error not all
# -O3 option to optimize compiling (0~3)
LDLIBS = -lm # math library
DEST   = cs238
SRCS  := $(wildcard *.c)
OBJS  := $(SRCS:.c=.o) # convert .c file to .o 

all: $(OBJS)
	@echo "[LN]" $(DEST)
	@$(CC) -o $(DEST) $(OBJS) $(LDLIBS)

%.o: %.c
	@echo "[CC]" $<
	@$(CC) $(CFLAGS) -c $<
	@$(CC) $(CFLAGS) -MM $< > $*.d

clean:
	@echo "clean"
	@rm -f $(DEST) *.so *.o *.d *~ *#

-include $(OBJS:.o=.d)
