# See https://www.gnu.org/prep/standards/html_node/Makefile-Basics.html#Makefile-Basics
SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS +=-std=c99 -Wall -Wextra -Wpedantic -O1

all: duff

clean:
	rm duff

duff: duff.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ 
