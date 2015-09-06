-include $(top_srcdir)/common.mk

bin_PROGRAMS = \
	gcolor3

gcolor3_SOURCES = \
	main.c \
	gcolor3-application.c \
	gcolor3-window.c

gcolor3_CFLAGS = \
	-DLOCALE_DIR=\"$(localedir)\" \
	$(GCOLOR3_CFLAGS) \
	$(AM_CFLAGS)

gcolor3_LDADD = \
	$(GCOLOR3_LIBS)

gcolor3_LDFLAGS = \
	-export-dynamic -no-undefined

