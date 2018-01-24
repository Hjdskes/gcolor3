SUBDIRS = icons

# Desktop file
@INTLTOOL_DESKTOP_RULE@
desktop_in_files = org.hjdskes.gcolor3.desktop.in.in
desktopdir = $(datadir)/applications
desktop_DATA = $(desktop_in_files:.desktop.in.in=.desktop)

# AppData file
@INTLTOOL_XML_RULE@
appdata_in_files = org.hjdskes.gcolor3.appdata.xml.in
appdatadir = $(datadir)/appdata
appdata_DATA = $(appdata_in_files:.xml.in=.xml)

# Resource files
GRESDEPS = $1 $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(dir $1) $1)
GRESGEN = $(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target="$@" --sourcedir="$(dir $<)" --generate-source "$<"

gcolor3_resource_deps = $(call GRESDEPS,gcolor3.gresource.xml)
gcolor3-resources.c: $(gcolor3_resource_deps)
	$(GRESGEN)

# Marshals
gcolor3-marshalers.c: gcolor3-marshalers.list
	$(AM_V_GEN) $(GLIB_GENMARSHAL) --prefix=_gcolor3_marshal $< --body > $@

gcolor3-marshalers.h: gcolor3-marshalers.list
	$(AM_V_GEN) $(GLIB_GENMARSHAL) --prefix=_gcolor3_marshal $< --header > $@

BUILT_SOURCES = \
	gcolor3-resources.c \
	gcolor3-marshalers.c \
	gcolor3-marshalers.h

EXTRA_DIST = \
	$(desktop_in_files) \
	$(appdata_in_files) \
	gcolor3.gresource.xml \
	$(gcolor3_resource_deps) \
	gcolor3-marshalers.list

CLEANFILES = \
	$(BUILT_SOURCES)

DISTCLEANFILES = \
	$(desktop_DATA) \
	$(appdata_DATA) \
	$(BUILT_SOURCES)

-include $(top_srcdir)/git.mk

