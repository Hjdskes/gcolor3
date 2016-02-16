SUBDIRS = icons

# Desktop file
@INTLTOOL_DESKTOP_RULE@
desktop_in_files = gcolor3.desktop.in.in
desktopdir = $(datadir)/applications
desktop_DATA = $(desktop_in_files:.desktop.in.in=.desktop)

# Resource files
GRESDEPS = $1 $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(dir $1) $1)
GRESGEN = $(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target="$@" --sourcedir="$(dir $<)" --generate-source "$<"

gcolor3_resource_deps = $(call GRESDEPS,gcolor3.gresource.xml)
gcolor3-resources.c: $(gcolor3_resource_deps)
	$(GRESGEN)

BUILT_SOURCES = gcolor3-resources.c

EXTRA_DIST = \
	$(desktop_in_files) \
	gcolor3.gresource.xml \
	$(gcolor3_resource_deps)

CLEANFILES = \
	$(BUILT_SOURCES)

DISTCLEANFILES = \
	$(desktop_DATA) \
	$(BUILT_SOURCES)

-include $(top_srcdir)/git.mk

