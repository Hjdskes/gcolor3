PROG    = gcolor3

CC      = gcc
LIBS    = `pkg-config --cflags --libs gtk+-3.0`
CFLAGS  = -std=c99 -Wall -Wextra -Wno-deprecated-declarations

PREFIX   ?= /usr/local
BINPREFIX = $(PREFIX)/bin

SRC = source/ui.c source/callbacks.c source/gcolor3.c
OBJ = $(SRC:.c=.o)

all: CFLAGS += -Os
all: $(PROG)

debug: CFLAGS += -O0 -g -pedantic -DDEBUG
debug: all

.c.o:
	$(CC) $(LIBS) $(CFLAGS) -c -o $@ $<

$(PROG): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

install:
	mkdir -p $(DESTDIR)$(BINPREFIX)
	mkdir -p $(DESTDIR)/usr/share/applications/
	install -m 755 $(PROG) $(DESTDIR)/$(BINPREFIX)/
	install -m 0644 data/$(PROG).desktop $(DESTDIR)/usr/share/applications/

uninstall:
	rm -f $(BINPREFIX)/$(PROG)
	rm -f /usr/share/applications/$(PROG).desktop

clean:
	rm -f $(OBJ) $(PROG)

.PHONY: all debug clean install uninstall
