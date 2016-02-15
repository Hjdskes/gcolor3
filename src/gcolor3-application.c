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

struct _Gcolor3ApplicationPrivate {
	GKeyFile *colors;
};

G_DEFINE_TYPE_WITH_PRIVATE (Gcolor3Application, gcolor3_application, GTK_TYPE_APPLICATION);

static void gcolor3_application_shutdown (GApplication *app);

static gchar *
get_user_file (void)
{
	return g_build_filename (g_get_home_dir (), ".rgb.ini", NULL);
}

static void
gcolor3_application_action_about (UNUSED GSimpleAction *action,
				  UNUSED GVariant      *parameter,
				  gpointer              user_data)
{
	GtkWindow *window;

	window = gtk_application_get_active_window (GTK_APPLICATION (user_data));
	g_return_if_fail (GCOLOR3_IS_WINDOW (window));

	gcolor3_window_show_about_dialog (GCOLOR3_WINDOW (window));
}

static void
gcolor3_application_action_quit (UNUSED GSimpleAction *action,
				 UNUSED GVariant      *parameter,
				 gpointer              user_data)
{
	GList *windows;

	windows = gtk_application_get_windows (GTK_APPLICATION (user_data));

	g_list_foreach (windows, (GFunc) gcolor3_window_close, NULL);
}

static GActionEntry app_entries[] = {
	{ "about", gcolor3_application_action_about, NULL, NULL, NULL },
	{ "quit",  gcolor3_application_action_quit,  NULL, NULL, NULL },
};

static void
gcolor3_application_init_app_menu (GtkApplication *application)
{
	GMenu *menu;

	g_action_map_add_action_entries (G_ACTION_MAP (application),
					 app_entries, G_N_ELEMENTS (app_entries),
					 application);

	menu = g_menu_new ();
	g_menu_append (menu, _("About"), "app.about");
	g_menu_append (menu, _("Quit"), "app.quit");
	gtk_application_set_app_menu (application, G_MENU_MODEL (menu));
	g_object_unref (menu);
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
	static const gchar *const accelmap[] = {
		"win.save", "<Ctrl>s", NULL,
		"win.delete", "<Shift>Delete", NULL,
		"win.change-page", "F9", NULL,
		NULL /* Terminating NULL */
	};

	const gchar *const *it;
	for (it = accelmap; it[0]; it += g_strv_length ((gchar **)it) + 1) {
		gtk_application_set_accels_for_action (application, it[0], &it[1]);
	}
}

static void
gcolor3_application_load_colors (Gcolor3Application *app) {
	Gcolor3ApplicationPrivate *priv;
	gchar *file;
	GError *error = NULL;

	g_return_if_fail (GCOLOR3_IS_APPLICATION (app));
	priv = gcolor3_application_get_instance_private (app);

	priv->colors = g_key_file_new ();
	file = get_user_file ();
	if (!(g_key_file_load_from_file (priv->colors,
					 file,
					 G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS,
					 &error))) {
		g_warning ("%s%s\n", _("Error opening file: "), error->message);
		g_clear_error (&error);
	}

	g_free (file);
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

	gcolor3_application_load_colors (app);
}

static void
gcolor3_application_activate (GApplication *application)
{
	Gcolor3Window *window;

	window = gcolor3_window_new (GCOLOR3_APPLICATION (application));
	gcolor3_window_add_colors (window);
	gtk_window_present_with_time (GTK_WINDOW (window), GDK_CURRENT_TIME);
}

static void
gcolor3_application_shutdown (GApplication *application)
{
	Gcolor3ApplicationPrivate *priv;
	GError *error = NULL;
	gchar *file;

	g_return_if_fail (GCOLOR3_IS_APPLICATION (application));
	priv = gcolor3_application_get_instance_private (GCOLOR3_APPLICATION (application));

	if (priv->colors != NULL) {
		// TODO: possibly only write to disk if contents changed?
		file = get_user_file ();
		if (!(g_key_file_save_to_file (priv->colors, file, &error))) {
			g_warning ("%s%s", _("Error writing file: "), error->message);
			g_clear_error (&error);
		}
		g_free (file);
		g_key_file_free (priv->colors);
	}

	G_APPLICATION_CLASS (gcolor3_application_parent_class)->shutdown (application);
}

static void
gcolor3_application_class_init (Gcolor3ApplicationClass *gcolor3_application_class)
{
	GApplicationClass *application_class = G_APPLICATION_CLASS (gcolor3_application_class);

	application_class->startup = gcolor3_application_startup;
	application_class->activate = gcolor3_application_activate;
	application_class->shutdown = gcolor3_application_shutdown;
}

static void
gcolor3_application_init (UNUSED Gcolor3Application *application)
{
}

Gcolor3Application *
gcolor3_application_new (void)
{
	GObject *application;

	application = g_object_new (GCOLOR3_TYPE_APPLICATION,
				    "application-id", "org.unia.gcolor3",
				    "flags", G_APPLICATION_FLAGS_NONE,
				    NULL);

	return GCOLOR3_APPLICATION (application);
}

GKeyFile *
gcolor3_application_get_colors (Gcolor3Application *application)
{
	Gcolor3ApplicationPrivate *priv;

	g_return_val_if_fail (GCOLOR3_IS_APPLICATION (application), NULL);
	priv = gcolor3_application_get_instance_private (application);

	return priv->colors;
}

