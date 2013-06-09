/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2013 Jente Hidskes <jthidskes@outlook.com>
 * 
 * gcolor3 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gcolor3 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>

#include "gcolor3.h"
#include "ui.h"
#include "callbacks.h"

gboolean save_selected_color (void) {
	FILE *fp;
	size_t filesize1;
	long filesize2;
	gchar old[512] = "";

	colorname = g_strdup (gtk_entry_get_text (GTK_ENTRY (save_entry)));
	if (!strcmp (colorname, ""))
		return FALSE;

	/* if file exists already, get its contents, otherwise just write to it */
	if (g_file_test (user_filename, G_FILE_TEST_EXISTS)) {
		fp = fopen (user_filename, "r");
		filesize2 = ftell (fp);
		if (fp) {
			filesize1 = fread (old, sizeof (old), 1, fp);
			fclose (fp);
			if(filesize1 != filesize2)
				show_file_error(_("read"));
		} else {
			show_file_error (_("read"));
			return FALSE;
		}
	}

	fp = fopen (user_filename, "w");
	if (!fp) {
		show_file_error (_("write"));
		return FALSE;
	} else {
		g_fprintf (fp, "%3d %3d %3d\t\t%s\n%s", colorvalue.red/256, colorvalue.green/256, colorvalue.blue/256, colorname, old);
		fclose (fp);
		return TRUE;
	}
}

gboolean delete_color (gchar *color_name, gchar *color_value) {
	FILE *fp;
	gchar *p, *file_color_name = NULL, file_color_value[8], buffer[512] = "", newstuff[512] = "";
	gint r, g, b;
	gboolean found = FALSE;

	/* remove from file */
	fp = fopen (user_filename, "r");
	if (!fp) {
		show_file_error (_("read"));
		return FALSE;
	} else {
		while ((p = fgets (buffer, sizeof buffer, fp)) != NULL) {
			if (buffer[0] == '!')
				continue;
			r = g_ascii_strtoull (p, &p, 10);
			g = g_ascii_strtoull (p, &p, 10);
			b = g_ascii_strtoull (p, &p, 10);
			p += strspn (p, " \t");
			g_sprintf (file_color_value, "#%.2X%.2X%.2X", r, g, b);
			file_color_name = g_strchomp (g_strdup (p));

			/* make sure to only remove the first matching color; both value and name must match */
			if (found || strcmp (file_color_name, color_name) != 0 || strcmp (g_ascii_strup (file_color_value, -1), color_value) != 0)
				g_sprintf (newstuff+strlen(newstuff), "%3d %3d %3d\t\t%s\n", r, g, b, file_color_name);
			else
				found = TRUE;
		}
		g_free(p);
		g_free(file_color_name);
		fclose (fp);

		/* only rewrite the file if we found a match */
		if (found) {
			fp = fopen (user_filename, "w");
			if (!fp) {
				show_file_error (_("write"));
				return FALSE;
			} else {
				g_fprintf (fp, "%s", newstuff);
				fclose (fp);
				return TRUE;
			}
		}
		return FALSE;
	}
}

void add_rgb_file (void) {
	FILE *fp;
	gchar *p, buffer[512], spec[8];
	gint r, g, b;

	fp = fopen (user_filename, "r");
	if (!fp)
		return; /* silently fail */

	while ((p = fgets (buffer, sizeof buffer, fp)) != NULL) {
		if (buffer[0] == '!')
			continue;
		r = g_ascii_strtoull (p, &p, 10);
		g = g_ascii_strtoull (p, &p, 10);
		b = g_ascii_strtoull (p, &p, 10);
		p += strspn (p, " \t");
		g_sprintf (spec, "#%.2X%.2X%.2X", r, g, b);
		colorname = g_strchomp (g_strdup (p));
		add_list_color (spec, colorname, FALSE);
	}
	g_free(p);
	fclose (fp);
}

gchar* hex_value (GdkColor colorvalue) {
	return g_strdup_printf ("#%.2X%.2X%.2X", colorvalue.red/256, colorvalue.green/256, colorvalue.blue/256);
}

void add_color_to_treeview (void) {
	GtkTreePath *path;

	/* add color to tree view */
	add_list_color (hex_value (colorvalue), colorname, TRUE);

	/* scroll tree view so user sees new color */
	if (selection) {
		if (gtk_tree_selection_get_selected (selection, NULL, &iter)) {
			path = gtk_tree_model_get_path (gtk_tree_view_get_model (GTK_TREE_VIEW(tree)), &iter);
			gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(tree), path, NULL, FALSE, 0, 0);
			gtk_tree_path_free (path);
		}
	}
}

void add_list_color (gchar *spec, gchar *colorname, gboolean is_new_color) {
	GdkPixbuf *buf;

	gchar *swatchxpm[] = {
		"16 14 1 1",
		".      c #FFFFFF",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................" };
	gchar colorline[] = ".      c #FFFFFF";

	g_sprintf (colorline, ".      c %s", spec);
	swatchxpm[1] = colorline;
	buf = gdk_pixbuf_new_from_xpm_data ((gchar const **)swatchxpm);

	/* setting where to insert will only take effect if user didn't sort either of the columns yet */
	if (is_new_color)
		gtk_list_store_prepend (liststore, &iter);
	else
		gtk_list_store_append (liststore, &iter);
	gtk_list_store_set (liststore, &iter, COLOR, buf, COLOR_VALUE, spec, COLOR_NAME, colorname, -1);
	/* when adding a new color, select it in the treeview */
	if (is_new_color)
		gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (tree)), &iter);
}

int main (void) {
 	GtkWidget *window;

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	gtk_init (NULL, NULL);
	colorvalue.red = colorvalue.green = colorvalue.blue = 65535;

	window = create_window ();
	gtk_widget_show_all (window);

	user_filename = g_build_filename(g_getenv("HOME"), ".rgb.txt", NULL);
	add_rgb_file ();

	gtk_main ();

	g_free(user_filename);

	return 0;
}