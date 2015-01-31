-include $(top_srcdir)/common.mk

bin_PROGRAMS = \
	gcolor3

gcolor3_SOURCES = \
	main.c \
	gcolor3-application.c \
	gcolor3-application.h \
	$(BUILT_SOURCES)

gcolor3_CFLAGS = \
	-DLOCALE_DIR=\""$(localedir)"\" \
	-DSRC_DIR=\""$(srcdir)"\" \
	-DPKG_DATA_DIR=\""$(pkgdatadir)"\" \
	-DDATA_DIR=\""$(datadir)"\" \
	$(GTK3_CFLAGS) \
	$(AM_CFLAGS)

gcolor3_LDADD = \
	$(GTK3_LIBS)

gcolor3_LDFLAGS = \
	-export-dynamic -no-undefined

BUILT_SOURCES = \
	gcolor3-resources.c

gcolor3-resources.c: gcolor3.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=$(top_srcdir)/data --generate-dependencies gcolor3.gresource.xml)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) "$<" --target=$@ \
         --sourcedir=$(top_srcdir)/data --c-name _gcolor3 --generate-source

EXTRA_DIST = \
	gcolor3.gresource.xml

CLEANFILES = \
	$(BUILT_SOURCES)
