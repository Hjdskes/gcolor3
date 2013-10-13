/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
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

#include <config.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>

#include "gcolor3.h"
#include "callbacks.h"
#include "ui.h"

int main (void) {
#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	gtk_init (NULL, NULL);
	colorvalue.red = colorvalue.green = colorvalue.blue = 65535;

	window = create_window ();
	gtk_widget_show_all (window);

	user_filename = g_build_filename (g_getenv ("HOME"), ".rgb.txt", NULL);
	add_rgb_file (user_filename);

	gtk_main ();

	g_free (user_filename);

	return 0;
}
