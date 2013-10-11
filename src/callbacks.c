/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * callbacks.c
 * Copyright (C) 2013 Jente Hidskes <jthidskes@outlook.com>
 *
 * Gcolor3 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gcolor3 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>

#include "gcolor3.h"
#include "callbacks.h"
#include "ui.h"

gchar *hex_value (GdkColor colorvalue) {
	return g_strdup_printf ("#%.2X%.2X%.2X", colorvalue.red/256, colorvalue.green/256, colorvalue.blue/256);
}

gboolean save_selected_color (gchar *file) {
	FILE *fp;
	size_t filesize1;
	long filesize2;
	gchar old[512] = "";

	colorname = g_strdup (gtk_entry_get_text (GTK_ENTRY (save_entry)));
	if (!strcmp (colorname, ""))
		return FALSE;

	/* if file exists already, get its contents, otherwise just write to it */
	if (g_file_test (file, G_FILE_TEST_EXISTS)) {
		fp = fopen (file, "r");
		filesize2 = ftell (fp);
		if (fp) {
			filesize1 = fread (old, sizeof (old), 1, fp);
			fclose (fp);
			if(filesize1 != filesize2)
				show_file_error(file, _("read"));
		} else {
			show_file_error (file, _("read"));
			return FALSE;
		}
	}

	fp = fopen (file, "w");
	if (!fp) {
		show_file_error (file, _("write"));
		return FALSE;
	} else {
		g_fprintf (fp, "%3d %3d %3d\t\t%s\n%s", colorvalue.red/256, colorvalue.green/256, colorvalue.blue/256, colorname, old);
		fclose (fp);
		return TRUE;
	}
}

gboolean delete_color (gchar *file, gchar *color_name, gchar *color_value) {
	FILE *fp;
	gchar *p, *file_color_name = NULL, file_color_value[8], buffer[512] = "", newstuff[512] = "";
	gint r, g, b;
	gboolean found = FALSE;

	/* remove from file */
	fp = fopen (file, "r");
	if (!fp) {
		show_file_error (file, _("read"));
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
			fp = fopen (file, "w");
			if (!fp) {
				show_file_error (file, _("write"));
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

void add_rgb_file (gchar *file) {
	FILE *fp;
	gchar *p, buffer[512], spec[8];
	gint r, g, b;

	fp = fopen (file, "r");
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
		add_list_color (spec, FALSE);
	}
	g_free(p);
	fclose (fp);
}

void add_color_to_treeview (void) {
	GtkTreePath *path;

	/* add color to tree view */
	add_list_color (hex_value (colorvalue), TRUE);

	/* scroll tree view so user sees new color */
	if (selection) {
		if (gtk_tree_selection_get_selected (selection, NULL, &iter)) {
			path = gtk_tree_model_get_path (gtk_tree_view_get_model (GTK_TREE_VIEW(tree)), &iter);
			gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(tree), path, NULL, FALSE, 0, 0);
			gtk_tree_path_free (path);
		}
	}
}

void add_list_color (gchar *spec, gboolean is_new_color) {
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

void on_colorselection_color_changed (void) {
	gtk_color_selection_get_current_color (GTK_COLOR_SELECTION (color_chooser), &colorvalue);
}

void on_save_entry_changed (void) {
	gchar *text;

	text = g_strdup (gtk_entry_get_text (GTK_ENTRY (save_entry)));
	if (strlen (text) > 0)
		gtk_widget_set_sensitive (save_dialog_ok_button, TRUE);
	else
		gtk_widget_set_sensitive (save_dialog_ok_button, FALSE);
}

void on_list_selection_changed (void) {
	GdkColor new_color, curr_color;
	GtkTreeModel *model;
	gchar *color;

	if (gtk_tree_selection_get_selected (selection, &model, &selection_iter)) {
		gtk_tree_model_get (model, &selection_iter, COLOR_VALUE, &color, -1);
		gdk_color_parse (color, &new_color);

		/* save the old color in color wheel */
		gtk_color_selection_get_current_color (GTK_COLOR_SELECTION (color_chooser), &curr_color);
		gtk_color_selection_set_previous_color (GTK_COLOR_SELECTION (color_chooser), &curr_color);
		gtk_color_selection_set_current_color (GTK_COLOR_SELECTION (color_chooser), &new_color);
		g_free (color);

		gtk_widget_set_sensitive (button_delete, TRUE);
	} else
		gtk_widget_set_sensitive (button_delete, FALSE);
}

void on_save_button_clicked (void) {
	gchar *labeltext;
	gint result;

	save_dialog = save_dialog_open ();
	gtk_window_set_transient_for (GTK_WINDOW (save_dialog), GTK_WINDOW (window));

	labeltext = g_strdup_printf (_("Enter a color name for %s:"), hex_value (colorvalue));
	gtk_label_set_markup (GTK_LABEL (save_label), labeltext);
	g_free(labeltext);
	result = gtk_dialog_run (GTK_DIALOG (save_dialog));
	switch (result) {
		case GTK_RESPONSE_OK:
			if (save_selected_color (user_filename))
				add_color_to_treeview ();
			break;
		case GTK_RESPONSE_CANCEL:
		default:
			colorname = "";
			break;
	}
	gtk_widget_destroy (save_dialog);
}

void on_delete_button_clicked (void) {
	gchar *color_value;

	gtk_tree_model_get (gtk_tree_view_get_model (GTK_TREE_VIEW (tree)), &selection_iter, COLOR_NAME, &colorname, COLOR_VALUE, &color_value, -1);
	if (delete_color (user_filename, colorname, color_value))
		gtk_list_store_remove (GTK_LIST_STORE (gtk_tree_view_get_model (GTK_TREE_VIEW (tree))), &selection_iter);
	g_free(color_value);
}
