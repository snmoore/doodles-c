# See https://www.gnu.org/prep/standards/html_node/Makefile-Basics.html#Makefile-Basics
SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS +=-std=c99 -Wall -Wextra -Wpedantic

all: matrix_multiply

clean:
	rm matrix_multiply

matrix_multiply: matrix_multiply.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ 
