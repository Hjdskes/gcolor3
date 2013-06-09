/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ui.h
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

#ifndef _UI_H_
#define _UI_H_

#include <gtk/gtk.h>

G_BEGIN_DECLS

void show_file_error (gchar *type);
void about_dialog_close (GtkWidget *about_dialog);
void about_dialog_open (void);
GtkWidget* save_dialog_open (void);
GtkWidget* create_window (void);

G_END_DECLS

#endif
