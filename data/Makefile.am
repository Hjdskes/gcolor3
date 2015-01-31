-include $(top_srcdir)/common.mk

SUBDIRS = icons

@INTLTOOL_DESKTOP_RULE@
DESKTOP_IN_FILES = gcolor3.desktop.in.in
DESKTOP_FILES = $(DESKTOP_IN_FILES:.desktop.in.in=.desktop)

desktopdir = $(datadir)/applications
desktop_DATA = $(DESKTOP_FILES)

EXTRA_DIST = \
	gcolor3-app-menu.xml

DISTCLEANFILES = \
	$(DESKTOP_FILES)
