pixmapsdir = $(datadir)/icons/hicolor/scalable/apps
pixmaps_DATA = gcolor3.svg

UPDATE_ICONS = gtk-update-icon-cache -q -t -f $(datadir)/icons/hicolor || :

install-data-hook: 
	$(UPDATE_ICONS)
uninstall-hook: 
	$(UPDATE_ICONS)
