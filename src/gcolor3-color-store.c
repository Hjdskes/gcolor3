/* Gcolor3ColorStore
 *
 * Copyright (C) 2016 Jente Hidskes
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

#include <glib.h>
#include <glib/gi18n.h>

#include "gcolor3-color-store.h"
#include "gcolor3-marshalers.h"

enum {
	SIGNAL_COLOR_ADDED,
	SIGNAL_COLOR_REMOVED,
	SIGNAL_LAST,
};

static guint signals[SIGNAL_LAST];

struct _Gcolor3ColorStorePrivate {
	GKeyFile *colors;
};

G_DEFINE_TYPE_WITH_PRIVATE (Gcolor3ColorStore, gcolor3_color_store, G_TYPE_OBJECT)

static inline gchar *
get_user_file (void)
{
	return g_build_filename (g_get_home_dir (), ".rgb.ini", NULL);
}

static void
gcolor3_color_store_dispose (GObject *object)
{
	Gcolor3ColorStorePrivate *priv;
	GError *error = NULL;
	gchar *file;

	priv = gcolor3_color_store_get_instance_private (GCOLOR3_COLOR_STORE (object));

	// TODO: possibly only write to disk if contents changed?
	file = get_user_file ();
	if (!(g_key_file_save_to_file (priv->colors, file, &error))) {
		g_warning (_("Error writing file: %s\n"), error->message);
		g_clear_error (&error);
	}
	g_free (file);
	g_key_file_free (priv->colors);

	G_OBJECT_CLASS (gcolor3_color_store_parent_class)->dispose (object);
}

static void
gcolor3_color_store_class_init (Gcolor3ColorStoreClass *gcolor3_color_store_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (gcolor3_color_store_class);

	object_class->dispose = gcolor3_color_store_dispose;

	signals[SIGNAL_COLOR_ADDED] =
		g_signal_new ("color-added", // FIXME: wrap in I_()?
			      GCOLOR3_TYPE_COLOR_STORE,
			      G_SIGNAL_RUN_LAST,
			      0,
			      NULL, NULL,
			      _gcolor3_marshal_VOID__STRING_STRING,
			      G_TYPE_NONE, 2,
			      G_TYPE_STRING, G_TYPE_STRING);

	signals[SIGNAL_COLOR_REMOVED] =
		g_signal_new ("color-removed", // FIXME: wrap in I_()?
			      GCOLOR3_TYPE_COLOR_STORE,
			      G_SIGNAL_RUN_LAST,
			      0,
			      NULL, NULL,
			      _gcolor3_marshal_VOID__STRING,
			      G_TYPE_NONE, 1,
			      G_TYPE_STRING);
}

static void
gcolor3_color_store_init (Gcolor3ColorStore *store)
{
	Gcolor3ColorStorePrivate *priv;
	gchar *file;
	GError *error = NULL;

	priv = gcolor3_color_store_get_instance_private (store);

	priv->colors = g_key_file_new ();
	file = get_user_file ();
	if (!(g_key_file_load_from_file (priv->colors,
					 file,
					 G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS,
					 &error))) {
		g_warning (_("Error opening file: %s. Colors likely won't be saved.\n"),
			   error->message);
		g_clear_error (&error);
	}

	g_free (file);
}

Gcolor3ColorStore *
gcolor3_color_store_new ()
{
	return g_object_new (GCOLOR3_TYPE_COLOR_STORE, NULL);
}

gboolean
gcolor3_color_store_add_color (Gcolor3ColorStore *store, const gchar *key, const gchar *hex)
{
	Gcolor3ColorStorePrivate *priv;

	g_return_val_if_fail (GCOLOR3_IS_COLOR_STORE (store), FALSE);
	g_return_val_if_fail (key != NULL && hex != NULL, FALSE);

	priv = gcolor3_color_store_get_instance_private (store);

	if (g_key_file_has_key (priv->colors, "Colors", key, NULL)) {
		g_warning (_("There is already a color name: %s\n"), key);
		return FALSE;
	}

	g_key_file_set_string (priv->colors, "Colors", key, hex);
	g_signal_emit (store, signals[SIGNAL_COLOR_ADDED], 0, key, hex);
	return TRUE;
}

gboolean
gcolor3_color_store_remove_color (Gcolor3ColorStore *store, const gchar *key)
{
	Gcolor3ColorStorePrivate *priv;
	GError *error = NULL;

	g_return_val_if_fail (GCOLOR3_IS_COLOR_STORE (store), FALSE);
	g_return_val_if_fail (key != NULL, FALSE);

	priv = gcolor3_color_store_get_instance_private (store);

	if (!(g_key_file_remove_key (priv->colors, "Colors", key, &error))) {
		g_warning (_("Error deleting key: %s\n"), error->message);
		g_clear_error (&error);
		return FALSE;
	}

	g_signal_emit (store, signals[SIGNAL_COLOR_REMOVED], 0, key);
	return TRUE;
}

void
gcolor3_color_store_foreach (Gcolor3ColorStore           *store,
			     Gcolor3ColorStoreForeachFunc func,
			     gpointer                     user_data)
{
	Gcolor3ColorStorePrivate *priv;
	GError *error = NULL;
	gchar **keys = NULL;
	gsize length;

	g_return_if_fail (GCOLOR3_IS_COLOR_STORE (store));
	priv = gcolor3_color_store_get_instance_private (store);

	if (!(keys = g_key_file_get_keys (priv->colors, "Colors", &length, &error))) {
		g_warning (_("Error reading keys: %s\n"), error->message);
		g_clear_error (&error);
		return;
	}

	for (guint i = 0; i < length; i++) {
		gchar *value;

		if (!(value = g_key_file_get_value (priv->colors, "Colors", keys[i], NULL))) {
			continue;
		}

		func (keys[i], value, user_data);
	}

	g_strfreev (keys);
}

