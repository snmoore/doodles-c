# See https://www.gnu.org/prep/standards/html_node/Makefile-Basics.html#Makefile-Basics
SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS +=-std=c99 -Wall -Wextra -Wpedantic

all: stack

clean:
	rm stack

reverse: stack.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ 
