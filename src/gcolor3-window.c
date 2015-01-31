/* Gcolor3Window
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

#include "gcolor3-window.h"

struct _Gcolor3WindowPrivate {
	GtkWidget *palette;
};

G_DEFINE_TYPE_WITH_PRIVATE (Gcolor3Window, gcolor3_window, GTK_TYPE_APPLICATION_WINDOW);

static void
gcolor3_window_palette_changed ()
{

}

static void
gcolor3_window_dispose (GObject *object)
{
	Gcolor3Window *window;
	Gcolor3WindowPrivate *priv;

	window = GCOLOR3_WINDOW (object);
	priv = gcolor3_window_get_instance_private (window);

	G_OBJECT_CLASS (gcolor3_window_parent_class)->dispose(object);
}

static void
gcolor3_window_class_init (Gcolor3WindowClass *gcolor3_window_class)
{
	GObjectClass *object_class;
	GtkWidgetClass *widget_class;

	object_class = (GObjectClass *) gcolor3_window_class;
	widget_class = (GtkWidgetClass *) gcolor3_window_class;

	object_class->dispose = gcolor3_window_dispose;

	//widget_class->delete_event = gcolor3_window_delete_event;
	//widget_class->key_press_event = gcolor3_window_key_press_event;

	gtk_widget_class_set_template_from_resource (widget_class,
						     "/org/unia/gcolor3/ui/gcolor3-window.ui");
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, palette);

	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_palette_changed);
}

static void
gcolor3_window_init (Gcolor3Window *window)
{
	gtk_widget_init_template (GTK_WIDGET (window));
}

Gcolor3Window *
gcolor3_window_new (Gcolor3Application *application)
{
	return g_object_new (GCOLOR3_TYPE_WINDOW, "application", application, NULL);
}

