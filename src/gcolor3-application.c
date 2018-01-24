/* Gcolor3Application
 *
 * Copyright (C) 2015-2016, 2018 Jente Hidskes
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
#include "gcolor3-color-store.h"
#include "gcolor3-window.h"

struct _Gcolor3ApplicationPrivate {
	Gcolor3ColorStore *colors;
};

G_DEFINE_TYPE_WITH_PRIVATE (Gcolor3Application, gcolor3_application, GTK_TYPE_APPLICATION);

static void
gcolor3_application_action_about (UNUSED GSimpleAction *action,
				  UNUSED GVariant      *parameter,
				  gpointer              user_data)
{
	static const char *authors[] = {
		"Jente Hidskes <hjdskes@gmail.com>",
		NULL,
	};

	static const char *artists[] = {
		"hhh",
		NULL,
	};

	gtk_show_about_dialog (gtk_application_get_active_window (GTK_APPLICATION (user_data)),
			       "program-name", g_get_application_name (),
			       "version", PACKAGE_VERSION,
			       "copyright", "Copyright \xc2\xa9 "COPYRIGHT" Jente Hidskes",
			       "comments", _("Choose colors from the picker or the screen"),
			       "authors", authors,
			       "artists", artists,
			       /* Translators: translate this to give yourself credits. */
			       "translator-credits", _("translator-credits"),
			       "website-label", _("Website"),
			       "website", PACKAGE_URL,
			       "logo-icon-name", gtk_window_get_default_icon_name (),
			       "wrap-license", TRUE,
			       "license-type", GTK_LICENSE_GPL_2_0,
			       NULL);
}

static void
gcolor3_application_action_quit (UNUSED GSimpleAction *action,
				 UNUSED GVariant      *parameter,
				 gpointer              user_data)
{
	GList *windows;

	windows = gtk_application_get_windows (GTK_APPLICATION (user_data));

	g_list_foreach (windows, (GFunc) gtk_widget_destroy, NULL);
}

static GActionEntry app_entries[] = {
	{ "about", gcolor3_application_action_about, NULL, NULL, NULL },
	{ "quit",  gcolor3_application_action_quit,  NULL, NULL, NULL },
};

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
		"win.copy", "<Ctrl>c", NULL,
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
gcolor3_application_startup (GApplication *application)
{
	Gcolor3Application *app = GCOLOR3_APPLICATION (application);

	G_APPLICATION_CLASS (gcolor3_application_parent_class)->startup (application);

	gtk_window_set_default_icon_name ("gcolor3");
	g_set_application_name (_("Color picker"));

	g_action_map_add_action_entries (G_ACTION_MAP (application),
					 app_entries, G_N_ELEMENTS (app_entries),
					 application);

	gcolor3_application_init_accelerators (GTK_APPLICATION (app));
}

static void
gcolor3_application_activate (GApplication *application)
{
	Gcolor3ApplicationPrivate *priv;
	Gcolor3Window *window;

	priv = gcolor3_application_get_instance_private (GCOLOR3_APPLICATION (application));

	window = gcolor3_window_new (GCOLOR3_APPLICATION (application), priv->colors);
	gtk_window_present_with_time (GTK_WINDOW (window), GDK_CURRENT_TIME);
}

static void
gcolor3_application_dispose (GObject *object)
{
	Gcolor3ApplicationPrivate *priv;

	priv = gcolor3_application_get_instance_private (GCOLOR3_APPLICATION (object));

	g_clear_object (&priv->colors);

	G_OBJECT_CLASS (gcolor3_application_parent_class)->dispose (object);
}

static void
gcolor3_application_class_init (Gcolor3ApplicationClass *gcolor3_application_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (gcolor3_application_class);
	GApplicationClass *application_class = G_APPLICATION_CLASS (gcolor3_application_class);

	object_class->dispose = gcolor3_application_dispose;

	application_class->startup = gcolor3_application_startup;
	application_class->activate = gcolor3_application_activate;
}

static void
gcolor3_application_init (Gcolor3Application *application)
{
	Gcolor3ApplicationPrivate *priv;

	priv = gcolor3_application_get_instance_private (application);

	priv->colors = gcolor3_color_store_new ();
}

Gcolor3Application *
gcolor3_application_new (void)
{
	return g_object_new (GCOLOR3_TYPE_APPLICATION,
			     "application-id", "org.hjdskes.gcolor3",
			     "flags", G_APPLICATION_FLAGS_NONE,
			     NULL);
}

