/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * callbacks.h
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

#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

GtkTreeIter selection_iter, iter;

G_BEGIN_DECLS

gchar *hex_value (GdkColor colorvalue);
gboolean save_selected_color (gchar *file);
gboolean delete_color (gchar *file, gchar *color_name, gchar *color_value);
void add_rgb_file (gchar *file);
void add_color_to_treeview (void);
void add_list_color (gchar *spec, gboolean is_new_color);
void on_colorselection_color_changed (void);
void on_save_entry_changed (void);
void on_list_selection_changed (void);
void on_save_button_clicked (void);
void on_delete_button_clicked (void);

G_END_DECLS

#endif
