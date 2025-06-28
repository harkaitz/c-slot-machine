.POSIX:
.SUFFIXES:
.PHONY: all clean install check

PROJECT   =c-slot-machine
VERSION   =1.0.0
CC        =$(shell which $(TPREFIX)cc $(TPREFIX)gcc 2>/dev/null | head -n 1)
CFLAGS    =-Wall -g3 -std=c99
PREFIX    =/usr/local
BUILDDIR ?=.build
UNAME_S  ?=$(shell uname -s)
EXE      ?=$(shell uname -s | awk '/Windows/ || /MSYS/ || /CYG/ { print ".exe" }')
PROGS     =\
    $(BUILDDIR)/slot-machine$(EXE)

all: $(PROGS)
clean:
	rm -f $(PROGS)
install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(PROGS) $(DESTDIR)$(PREFIX)/bin
check:
	test @"hello" = @$$($(BUILDDIR)/slot-machine$(EXE) -s 3 < examples/words)

$(BUILDDIR)/slot-machine$(EXE): slot-machine.c
	mkdir -p $(BUILDDIR)
	$(CC) -o $@ $(CFLAGS) $(CPPFLAGS) $<

