# See https://www.gnu.org/prep/standards/html_node/Makefile-Basics.html#Makefile-Basics
SHELL = /bin/sh

CFLAGS+=-std=c99 -g -Wall -Wextra -Wpedantic -pedantic-errors
LINT=scan-build -v
CEEDLING=/usr/local/bin/ceedling

.SUFFIXES:
.SUFFIXES: .c .o

.PHONY: all clean lint

all: $(target)

clean:
	-rm $(target)
	-rm -rf $(target).dSYM

lint: $(sources)
	$(LINT) $(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $? -o $(target)

test:
	$(CEEDLING) test:all
	
test_clean:
	$(CEEDLING) clobber
	-rm -rf build

$(target): $(sources)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ -o $@
