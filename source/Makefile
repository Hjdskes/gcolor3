PROG    = gcolor3
VERSION = 1.0

CC      = gcc
LIBS    = `pkg-config --cflags --libs gtk+-3.0`
CFLAGS  = -std=c99 -Wall -Wextra -Wno-deprecated-declarations
CFLAGS += -DVERSION=\"$(VERSION)\"

PREFIX   ?= /usr/local
BINPREFIX = $(PREFIX)/bin

SRC = ui.c callbacks.c gcolor3.c
OBJ = $(SRC:.c=.o)

all: CFLAGS += -Os
all: LDFLAGS += -s
all: $(PROG)

debug: CFLAGS += -O0 -g -pedantic -DDEBUG
debug: all

.c.o:
	$(CC) $(LIBS) $(CFLAGS) -c -o $@ $<

gcolor3: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

install:
	mkdir -p $(DESTDIR)$(BINPREFIX)
	mkdir -p $(DESTDIR)/usr/share/applications/
	install -m 755 $(PROG) $(DESTDIR)$(BINPREFIX)/
	install -m 0644 data/gcolor3.desktop $(DESTDIR)/usr/share/applications/

uninstall:
	rm -f $(DESTDIR)$(BINPREFIX)/$(PROG)
	rm -f /usr/share/applications/gcolor3.desktop

clean:
	rm -f $(OBJ) $(PROG)

.PHONY: all debug clean install uninstall
