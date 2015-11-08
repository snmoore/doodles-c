# See https://www.gnu.org/prep/standards/html_node/Makefile-Basics.html#Makefile-Basics
SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS +=-std=c99 -Wall -Wextra -Wpedantic

all: count_bits

clean:
	rm count_bits

count_bits: count_bits.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ 
