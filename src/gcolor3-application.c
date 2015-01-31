/* Gcolor3Application
 *
 * Copyright (C) 2015 Jente Hidskes
 *
 * Author: Jente Hidskes <hjdskes@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "gcolor3-application.h"
#include "gcolor3-window.h"

G_DEFINE_TYPE (Gcolor3Application, gcolor3_application, GTK_TYPE_APPLICATION);

static void
gcolor3_application_action_about (GSimpleAction *action,
				  GVariant      *parameter,
				  gpointer       user_data)
{
	//GtkWindow *window;

	//window = gtk_application_get_active_window (GTK_APPLICATION (user_data));
	//g_return_if_fail (GCOLOR3_IS_WINDOW (window));

	//gcolor3_window_show_about_dialog (GCOLOR3_WINDOW (window));
	g_print ("About");
}

static void
gcolor3_application_action_quit (GSimpleAction *action,
				 GVariant      *parameter,
				 gpointer       user_data)
{
	//GList *windows;

	//windows = gtk_application_get_windows (GTK_APPLICATION (user_data));

	//g_list_foreach (windows, (GFunc) gcolor3_window_close, NULL);
	g_print ("Quit");
}

static GActionEntry app_entries[] = {
	{ "about", gcolor3_application_action_about, NULL, NULL, NULL },
	{ "quit",  gcolor3_application_action_quit,  NULL, NULL, NULL },
};

static void
gcolor3_application_init_app_menu (GtkApplication *application)
{
	GtkBuilder *builder;
	GObject *builder_object;

	g_action_map_add_action_entries (G_ACTION_MAP (application),
					 app_entries, G_N_ELEMENTS (app_entries),
					 application);

	builder = gtk_builder_new_from_resource ("/org/unia/gcolor3/ui/gcolor3-app-menu.xml");
	builder_object = gtk_builder_get_object (builder, "app-menu");
	gtk_application_set_app_menu (application, G_MENU_MODEL (builder_object));

	g_object_unref (builder);
}

static void
gcolor3_application_init_accelerators (GtkApplication *application)
{
	/* Taken from eog, which in turn has this based on a simular
	 * construct in Evince (src/ev-application.c).
	 * Setting multiple accelerators at once for an action
	 * is not very straight forward in a static way.
	 *
	 * This gchar* array simulates an action<->accels mapping.
	 * Enter the action name followed by the accelerator strings
	 * and terminate the entry with a NULL-string.*/
	static const gchar * const accelmap[] = {
		NULL /* Terminating NULL */
	};

	const gchar *const *it = accelmap;
	for (it = accelmap; it[0]; it += g_strv_length ((gchar **)it) + 1) {
		gtk_application_set_accels_for_action(application, it[0], &it[1]);
	}
}

static void
gcolor3_application_startup (GApplication *application)
{
	Gcolor3Application *app = GCOLOR3_APPLICATION (application);

	G_APPLICATION_CLASS (gcolor3_application_parent_class)->startup (application);

	gtk_window_set_default_icon_name ("gcolor3");
	g_set_application_name (_("Color picker"));

	gcolor3_application_init_app_menu (GTK_APPLICATION (app));
	gcolor3_application_init_accelerators (GTK_APPLICATION (app));
}

static void
gcolor3_application_activate (GApplication *application)
{
	gcolor3_application_open_window (GCOLOR3_APPLICATION (application),
					 GDK_CURRENT_TIME);
}

static void
gcolor3_application_class_init (Gcolor3ApplicationClass *gcolor3_application_class)
{
	GApplicationClass *application_class;

	application_class = (GApplicationClass *) gcolor3_application_class;

	application_class->startup = gcolor3_application_startup;
	application_class->activate = gcolor3_application_activate;
}

static void
gcolor3_application_init (Gcolor3Application *application)
{
}

gboolean
gcolor3_application_open_window (Gcolor3Application *application,
				 guint32             timestamp)
{
	Gcolor3Window *window = NULL;

	g_return_val_if_fail (GCOLOR3_IS_APPLICATION (application), FALSE);

	window = gcolor3_window_new (application);
	gtk_window_present_with_time (GTK_WINDOW (window), timestamp);

	return TRUE;
}

Gcolor3Application *
gcolor3_application_new (void) {
	GObject *application;

	application = g_object_new (GCOLOR3_TYPE_APPLICATION,
				    "application-id", "org.unia.gcolor3",
				    "flags", G_APPLICATION_FLAGS_NONE,
				    NULL);

	return GCOLOR3_APPLICATION (application);
}
