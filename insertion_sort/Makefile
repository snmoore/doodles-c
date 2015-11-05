# See https://www.gnu.org/prep/standards/html_node/Makefile-Basics.html#Makefile-Basics
SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS +=-std=c99 -Wall -Wextra -Wpedantic

all: issort

clean:
	rm issort

issort: issort.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ 
