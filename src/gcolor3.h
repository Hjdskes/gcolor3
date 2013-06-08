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

#ifndef _GCOLOR3H_
#define _GCOLOR3_H_

#include <gtk/gtk.h>

GtkWidget *window, *color_chooser, *save_dialog, *tree, *button_save, *save_label, *button_delete, *save_dialog_ok_button, *save_entry;
GtkTreeIter selection_iter, iter;
GtkTreeSelection *selection;
GtkListStore *liststore;
GdkColor colorvalue;
gchar *colorname, *user_filename;

enum {
	COLOR,
	COLOR_VALUE,
	COLOR_NAME,
	N_COLUMNS
};

G_BEGIN_DECLS

gboolean save_selected_color (void);
gboolean delete_color (gchar *color_name, gchar *color_value);
void add_rgb_file (void);

void on_colorselection_color_changed (void);
void on_save_entry_changed (void);
void on_list_selection_changed (void);
void on_save_button_clicked (void);
void on_delete_button_clicked (void);

gchar* hex_value (GdkColor colorvalue);
void add_color_to_treeview (void);
void add_list_color (gchar *spec, gchar *colorname, gboolean is_new_color);

G_END_DECLS

#endif