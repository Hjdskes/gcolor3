-include $(top_srcdir)/common.mk
-include $(top_srcdir)/git.mk

bin_PROGRAMS = \
	gcolor3

# FIXME: make dist fails on data files if Gcolor3 hasn't been make'd yet.
gcolor3_SOURCES = \
	../data/gcolor3-resources.c \
	../data/gcolor3-marshalers.c \
	main.c \
	gcolor3-application.c \
	gcolor3-color-selection.c \
	gcolor3-color-store.c \
	gcolor3-window.c \
	$(gcolor3_NOINST_H_FILES)

# FIXME: make dist fails on data files if Gcolor3 hasn't been make'd yet.
gcolor3_NOINST_H_FILES = \
	../data/gcolor3-marshalers.h \
	gcolor3-application.h \
	gcolor3-color-selection.h \
	gcolor3-color-store.h \
	gcolor3-window.h

gcolor3_CPPFLAGS = \
	-I$(top_srcdir)/data \
	-DLOCALE_DIR=\"$(localedir)\"

gcolor3_CFLAGS = \
	$(GCOLOR3_CFLAGS) \
	$(AM_CFLAGS)

gcolor3_LDADD = \
	$(GCOLOR3_LIBS)

gcolor3_LDFLAGS = \
	-export-dynamic -no-undefined

