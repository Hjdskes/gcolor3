-include $(top_srcdir)/common.mk
-include $(top_srcdir)/git.mk

bin_PROGRAMS = \
	gcolor3

gcolor3_SOURCES = \
	main.c \
	gcolor3-application.c \
	gcolor3-window.c \
	$(gcolor3_NOINST_H_FILES)

gcolor3_NOINST_H_FILES = \
	gcolor3-application.h \
	gcolor3-window.h

gcolor3_CPPFLAGS = \
	-DLOCALE_DIR=\"$(localedir)\"

gcolor3_CFLAGS = \
	$(GCOLOR3_CFLAGS) \
	$(AM_CFLAGS)

gcolor3_LDADD = \
	$(GCOLOR3_LIBS)

gcolor3_LDFLAGS = \
	-export-dynamic -no-undefined

