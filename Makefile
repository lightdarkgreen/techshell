# Author:      lightdarkgreen
# Description: Makefile for techshell.

# Compiler and compiler options to use.
CC = gcc
CFLAGS = --std=c99 -Wall -O3

# Build the shell when making the all target.
all: techshell

# Clean up the object files when making the clean target.
clean:
	rm -f techshell
	rm -rf obj

# The all and clean targets aren't real files.
.PHONY: all clean

# Object files that need to be linked together.
objs := $(patsubst src/%.c,obj/%.o,$(wildcard src/*.c))

# Make the object directory before the objects.
$(objs): | obj

# Create the object directory.
obj:
	[ -d $@ ] || mkdir $@

# Complie the objects from the C source code.
obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Link the objects together to make the executable.
techshell: $(objs)
	$(CC) $(CFLAGS) -o $@ $^
