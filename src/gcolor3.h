/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * gcolor3.h
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

#ifndef _GCOLOR3H_
#define _GCOLOR3_H_

#include <gtk/gtk.h>

GtkWidget *window, *stack, *color_chooser, *save_dialog, *tree, *save_label, *button_save, *save_dialog_ok_button, *save_entry;
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

#endif
