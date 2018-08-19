/* Gcolor3ColorRow
 *
 * Copyright (C) 2018 Jente Hidskes
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

#include "config.h"

#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <glib/gi18n.h>

#include "gcolor3-color-row.h"

#define I_(string) g_intern_static_string (string)
#define THUMB_SIZE 24

enum {
      PROP_0,
      PROP_KEY,
      PROP_HEX,
};

enum {
      SIGNAL_COLOR_REMOVED,
      SIGNAL_COLOR_RENAMED,
      SIGNAL_LAST,
};

static guint signals[SIGNAL_LAST];

struct _Gcolor3ColorRowPrivate {
	GtkWidget *image;
	GtkWidget *label;
	GtkWidget *entry;

	gchar *key;
	const gchar *hex;
};

G_DEFINE_TYPE_WITH_PRIVATE (Gcolor3ColorRow, gcolor3_color_row, GTK_TYPE_LIST_BOX_ROW)

static void
set_color_thumbnail (Gcolor3ColorRow *row)
{
	Gcolor3ColorRowPrivate *priv;
	GdkPixbuf *pixbuf;
	GdkRGBA color;
	guint32 hex;

	priv = gcolor3_color_row_get_instance_private (row);

	if (!gdk_rgba_parse (&color, priv->hex)) {
		g_warning (_("Could not parse color to display thumbnail\n"));
		return;
	}

	hex = ((guint32) (color.red   * 255)) << 24 |
	      ((guint32) (color.green * 255)) << 16 |
	      ((guint32) (color.blue  * 255)) <<  8 |
	      ((guint32) (color.alpha * 255));

	// TODO: make circular?
	pixbuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, THUMB_SIZE, THUMB_SIZE);
	gdk_pixbuf_fill (pixbuf,  hex);
	gtk_image_set_from_pixbuf (GTK_IMAGE (priv->image), pixbuf);
	g_object_unref (pixbuf);
}

static void
set_color_in_clipboard (const gchar *color)
{
	GtkClipboard *clipboard;

	clipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text (clipboard, color, -1);
}

static void
gcolor3_color_row_copy_button_clicked (UNUSED GtkButton *button, gpointer user_data)
{
	Gcolor3ColorRowPrivate *priv;

	priv = gcolor3_color_row_get_instance_private (GCOLOR3_COLOR_ROW (user_data));

	set_color_in_clipboard (priv->hex);
}

static void
gcolor3_color_row_delete_button_clicked (UNUSED GtkButton *button, gpointer user_data)
{
	g_signal_emit (GCOLOR3_COLOR_ROW (user_data), signals[SIGNAL_COLOR_REMOVED], 0);
}

static void
gcolor3_color_row_entry_activated (GtkEntry *entry, gpointer user_data)
{
	Gcolor3ColorRowPrivate *priv;
	gchar *old_key;

	priv = gcolor3_color_row_get_instance_private (GCOLOR3_COLOR_ROW (user_data));

	old_key = g_strdup (priv->key);
	priv->key = g_strdup (gtk_entry_get_text (entry));
	g_signal_emit (GCOLOR3_COLOR_ROW (user_data), signals[SIGNAL_COLOR_RENAMED],
		       0, old_key, priv->key);
	g_free (old_key);
}

static gboolean
gcolor3_color_row_key_press_event (GtkWidget   *row,
				   GdkEventKey *event)
{
	Gcolor3ColorRowPrivate *priv;

	if (event->type != GDK_KEY_PRESS) {
		return GDK_EVENT_PROPAGATE;
	}

	priv = gcolor3_color_row_get_instance_private (GCOLOR3_COLOR_ROW (row));

	switch (event->keyval) {
	case GDK_KEY_c:
		if ((event->state & GDK_CONTROL_MASK) != GDK_CONTROL_MASK) {
			return GDK_EVENT_PROPAGATE;
		}
		set_color_in_clipboard (priv->hex);
		return GDK_EVENT_STOP;
	case GDK_KEY_Delete:
		g_object_ref (row);
		g_signal_emit (GCOLOR3_COLOR_ROW (row), signals[SIGNAL_COLOR_REMOVED], 0);
		return GDK_EVENT_STOP;
	default:
		return GDK_EVENT_PROPAGATE;
	}
}

static void
gcolor3_color_row_set_property (GObject      *object,
				guint         prop_id,
				const GValue *value,
				GParamSpec   *pspec)
{
	Gcolor3ColorRowPrivate *priv;

	priv = gcolor3_color_row_get_instance_private (GCOLOR3_COLOR_ROW (object));

	switch (prop_id) {
	case PROP_KEY:
		priv->key = g_value_dup_string (value);
		break;
	case PROP_HEX:
		priv->hex = g_value_dup_string (value);
		set_color_thumbnail (GCOLOR3_COLOR_ROW (object));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gcolor3_color_row_get_property (GObject    *object,
				guint       prop_id,
				GValue     *value,
				GParamSpec *pspec)
{
	Gcolor3ColorRowPrivate *priv;

	priv = gcolor3_color_row_get_instance_private (GCOLOR3_COLOR_ROW (object));

	switch (prop_id) {
	case PROP_KEY:
		g_value_set_string (value, priv->key);
		break;
	case PROP_HEX:
		g_value_set_string (value, priv->hex);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gcolor3_color_row_finalize (GObject *object)
{
	Gcolor3ColorRowPrivate *priv;

	priv = gcolor3_color_row_get_instance_private (GCOLOR3_COLOR_ROW (object));

	g_free (priv->key);

	G_OBJECT_CLASS (gcolor3_color_row_parent_class)->finalize (object);
}

static void
gcolor3_color_row_class_init (Gcolor3ColorRowClass *gcolor3_color_row_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (gcolor3_color_row_class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (gcolor3_color_row_class);

	object_class->set_property = gcolor3_color_row_set_property;
	object_class->get_property = gcolor3_color_row_get_property;
	object_class->finalize = gcolor3_color_row_finalize;

	widget_class->key_press_event = gcolor3_color_row_key_press_event;

	signals[SIGNAL_COLOR_REMOVED] =
		g_signal_new (I_("color-removed"),
			      GCOLOR3_TYPE_COLOR_ROW,
			      G_SIGNAL_RUN_LAST,
			      0,
			      NULL, NULL,
			      NULL,
			      G_TYPE_NONE, 0);

	signals[SIGNAL_COLOR_RENAMED] =
		g_signal_new (I_("color-renamed"),
			      GCOLOR3_TYPE_COLOR_ROW,
			      G_SIGNAL_RUN_LAST,
			      0,
			      NULL, NULL,
			      NULL,
			      G_TYPE_NONE, 2,
			      G_TYPE_STRING, G_TYPE_STRING);

	g_object_class_install_property (object_class, PROP_KEY,
					 g_param_spec_string ("key",
							      "key",
							      "Key of this row's color",
							      "Black",
							      G_PARAM_CONSTRUCT_ONLY |
							      G_PARAM_READWRITE |
							      G_PARAM_PRIVATE |
							      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class, PROP_HEX,
					 g_param_spec_string ("hex",
							      "HexValue",
							      "Hex value of this row's color",
							      "#000000",
							      G_PARAM_CONSTRUCT_ONLY |
							      G_PARAM_READWRITE |
							      G_PARAM_PRIVATE |
							      G_PARAM_STATIC_STRINGS));

	gtk_widget_class_set_template_from_resource (widget_class, "/nl/hjdskes/gcolor3/color-row.ui");

	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3ColorRow, image);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3ColorRow, label);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3ColorRow, entry);

	gtk_widget_class_bind_template_callback (widget_class, gcolor3_color_row_copy_button_clicked);
	gtk_widget_class_bind_template_callback (widget_class, gcolor3_color_row_delete_button_clicked);
	gtk_widget_class_bind_template_callback (widget_class, gcolor3_color_row_entry_activated);
}

static void
gcolor3_color_row_init (Gcolor3ColorRow *color_row)
{
	Gcolor3ColorRowPrivate *priv;

	priv = gcolor3_color_row_get_instance_private (color_row);

	gtk_widget_init_template (GTK_WIDGET (color_row));

	g_object_bind_property (color_row, "key", priv->entry, "text", G_BINDING_DEFAULT);
	g_object_bind_property (color_row, "hex", priv->label, "label", G_BINDING_DEFAULT);
}

Gcolor3ColorRow *
gcolor3_color_row_new (const gchar *key, const gchar *hex)
{
	g_return_val_if_fail (key != NULL, NULL);
	g_return_val_if_fail (hex != NULL, NULL);

	return g_object_new (GCOLOR3_TYPE_COLOR_ROW, "key", key, "hex", hex, NULL);
}
