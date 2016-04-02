# See https://www.gnu.org/prep/standards/html_node/Makefile-Basics.html#Makefile-Basics
SHELL = /bin/sh

SOURCES=static.c
TARGET=static
CFLAGS+=-std=c99 -g -Wall -Wextra -Wpedantic -pedantic-errors
LINT=scan-build -v

.SUFFIXES:
.SUFFIXES: .c .o

.PHONY: all clean lint

all: $(TARGET)

clean:
	-rm $(TARGET)
	-rm -rf $(TARGET).dSYM

lint: $(SOURCES)
	$(LINT) $(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $? -o $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $? -o $@
