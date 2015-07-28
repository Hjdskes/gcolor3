-include $(top_srcdir)/common.mk

bin_PROGRAMS = \
	gcolor3

gcolor3_SOURCES = \
	main.c \
	gcolor3-application.c \
	gcolor3-window.c

gcolor3_CFLAGS = \
	-DLOCALE_DIR=\"$(localedir)\" \
	-DDATA_DIR=\"$(datadir)\" \
	-DGETTEXT_PACKAGE=\"$(GETTEXT_PACKAGE)\" \
	$(GTK3_CFLAGS) \
	$(AM_CFLAGS)

gcolor3_LDADD = \
	$(GTK3_LIBS)

gcolor3_LDFLAGS = \
	-export-dynamic -no-undefined

