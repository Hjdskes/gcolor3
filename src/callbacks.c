/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * callbacks.c
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

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>

#include "gcolor3.h"
#include "ui.h"
#include "callbacks.h"

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
			if (save_selected_color ())
				add_color_to_treeview();
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
	if (delete_color (colorname, color_value))
		gtk_list_store_remove (GTK_LIST_STORE (gtk_tree_view_get_model (GTK_TREE_VIEW (tree))), &selection_iter);
	g_free(color_value);
}