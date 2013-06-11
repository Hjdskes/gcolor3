## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DDATA_DIR=\""$(datadir)"\" \
	$(GCOLOR3_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gcolor3

gcolor3_SOURCES = \
	main.c \
	callbacks.c \
	callbacks.h \
	ui.c \
	ui.h \
	gcolor3.h

gcolor3_LDFLAGS = \
	-Wl,--export-dynamic

gcolor3_LDADD = $(GCOLOR3_LIBS)


