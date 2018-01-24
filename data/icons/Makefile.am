appiconsdir = $(datadir)/icons/hicolor/scalable/apps
dist_appicons_DATA = org.hjdskes.gcolor3.svg

gtk_update_icon_cache = gtk-update-icon-cache -t -f $(datadir)/icons/hicolor || :

install-data-hook: update-icon-cache
uninstall-hook: update-icon-cache

update-icon-cache:
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating GTK+ icon cache."; \
		$(gtk_update_icon_cache); \
	else \
		echo "*** Icon cache not updated. After (un)install, run this:"; \
		echo "***   $(gtk_update_icon_cache)"; \
	fi

-include $(top_srcdir)/git.mk
