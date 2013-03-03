CFLAGS = -Wall -Wno-deprecated-declarations

debug: CFLAGS += -O0 -g -Wextra -pedantic
debug: gcolor3

gcolor3: gcolor3.c
	gcc gcolor3.c -o gcolor3 ${CFLAGS} `pkg-config --cflags --libs gtk+-3.0`

install: gcolor3
	mkdir -p $(DESTDIR)/usr/bin/
	mkdir -p $(DESTDIR)/usr/share/applications/

	install -m 0755 gcolor3 $(DESTDIR)/usr/bin/
	install -m 0644 data/gcolor3.desktop $(DESTDIR)/usr/share/applications/

uninstall:
	rm -f /usr/bin/gcolor3
	rm -f /usr/share/applications/gcolor3.desktop

clean:
	rm -f gcolor3
