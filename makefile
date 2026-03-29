.POSIX:
.SUFFIXES:
.PHONY: all clean install check

PROJECT   =c-slot-machine
VERSION   =1.0.0

PREFIX    =/usr/local
DESTDIR   =$(HOMEDRIVE)

TPREFIX   =$(HOMEDRIVE:C:=x86_64-w64-mingw32-)
EXE       =$(HOMEDRIVE:C:=.exe)

CC        =cc
CFLAGS    =-Wall -g3 -std=c99

BUILDDIR ?=.build
PROGS     =$(BUILDDIR)/slot-machine$(EXE)

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
	$(CC) -o $@ $(CFLAGS) $(CPPFLAGS) slot-machine.c

