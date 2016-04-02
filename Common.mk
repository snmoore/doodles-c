# See https://www.gnu.org/prep/standards/html_node/Makefile-Basics.html#Makefile-Basics
SHELL = /bin/sh

CFLAGS+=-std=c99 -g -Wall -Wextra -Wpedantic -pedantic-errors
LINT=scan-build -v

.SUFFIXES:
.SUFFIXES: .c .o

.PHONY: all clean lint

all: $(target)

clean:
	-rm $(target)
	-rm -rf $(target).dSYM

lint: $(sources)
	$(LINT) $(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $? -o $(target)

$(target): $(sources)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $? -o $@
