# See https://www.gnu.org/prep/standards/html_node/Makefile-Basics.html#Makefile-Basics
SHELL = /bin/sh

CFLAGS+=-std=c99 -g -Wall -Wextra -Wpedantic -pedantic-errors
LINT=scan-build -v

.SUFFIXES:
.SUFFIXES: .c .o

.PHONY: all clean lint

all: single double

clean:
	-rm single
	-rm single.o
	-rm -rf single.dSYM
	-rm double
	-rm double.o
	-rm -rf double.dSYM

lint: single.c double.c
	$(LINT) $(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $? -c

single: single.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $? -o $@

double: double.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $? -o $@
