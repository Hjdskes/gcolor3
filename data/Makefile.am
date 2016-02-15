SUBDIRS = icons

# Desktop file
@INTLTOOL_DESKTOP_RULE@
desktop_in_files = gcolor3.desktop.in.in
desktopdir = $(datadir)/applications
desktop_DATA = $(desktop_in_files:.desktop.in.in=.desktop)

EXTRA_DIST = \
	$(desktop_in_files)

DISTCLEANFILES = \
	$(desktop_DATA)

-include $(top_srcdir)/git.mk

