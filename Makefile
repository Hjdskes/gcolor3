CFLAGS = -Wall
debug: CFLAGS += -O0 -g -pedantic -Wextra
debug: gcolor3

gcolor3: gcolor3.c
	gcc gcolor3.c -o gcolor3 ${CFLAGS} `pkg-config --cflags --libs gtk+-3.0`

install: gcolor3
	mkdir -p $(DESTDIR)/usr/bin/
	mkdir -p $(DESTDIR)/usr/share/applications/
	#mkdir -p $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

	install -m 0755 gcolor3 $(DESTDIR)/usr/bin/
	install -m 0644 data/gcolor3.desktop $(DESTDIR)/usr/share/applications/
	#install -m 0644 data/gcolor3.png $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

uninstall:
	rm -rf /usr/bin/gcolor3
	rm -rf /usr/share/applications/gcolor3.desktop
	#rm -rf /usr/share/icons/hicolor/scalable/apps/gcolor3.png

clean:
	rm -rf gcolor3
