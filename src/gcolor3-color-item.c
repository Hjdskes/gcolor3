/* Gcolor3ColorItem
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

#include "gcolor3-color-item.h"

enum {
      PROP_0,
      PROP_KEY,
      PROP_HEX,
};

struct _Gcolor3ColorItemPrivate {
	gchar *key;
	const gchar *hex;
};

G_DEFINE_TYPE_WITH_PRIVATE (Gcolor3ColorItem, gcolor3_color_item, G_TYPE_OBJECT)

static void
gcolor3_color_item_get_property (GObject    *object,
				 guint       prop_id,
				 GValue     *value,
				 GParamSpec *pspec)
{
	Gcolor3ColorItemPrivate *priv;

	priv = gcolor3_color_item_get_instance_private (GCOLOR3_COLOR_ITEM (object));

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
gcolor3_color_item_set_property (GObject      *object,
				 guint         prop_id,
				 const GValue *value,
				 GParamSpec   *pspec)
{
	Gcolor3ColorItemPrivate *priv;

	priv = gcolor3_color_item_get_instance_private (GCOLOR3_COLOR_ITEM (object));

	switch (prop_id) {
	case PROP_KEY:
		priv->key = g_value_dup_string (value);
		break;
	case PROP_HEX:
		priv->hex = g_value_dup_string (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gcolor3_color_item_finalize (GObject *object)
{
	Gcolor3ColorItemPrivate *priv;

	priv = gcolor3_color_item_get_instance_private (GCOLOR3_COLOR_ITEM (object));

	g_free (priv->key);

	G_OBJECT_CLASS (gcolor3_color_item_parent_class)->finalize (object);
}

static void
gcolor3_color_item_class_init (Gcolor3ColorItemClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = gcolor3_color_item_finalize;
	object_class->get_property = gcolor3_color_item_get_property;
	object_class->set_property = gcolor3_color_item_set_property;

	g_object_class_install_property (object_class, PROP_KEY,
					 g_param_spec_string ("key",
							      "key",
							      "Key of this row's color",
							      "Black",
							      G_PARAM_CONSTRUCT_ONLY |
							      G_PARAM_READWRITE |
							      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class, PROP_HEX,
					 g_param_spec_string ("hex",
							      "HexValue",
							      "Hex value of this row's color",
							      "#000000",
							      G_PARAM_CONSTRUCT_ONLY |
							      G_PARAM_READWRITE |
							      G_PARAM_STATIC_STRINGS));
}

static void
gcolor3_color_item_init (Gcolor3ColorItem *item)
{
}

Gcolor3ColorItem *
gcolor3_color_item_new (const gchar *key, const gchar *hex)
{
	g_return_val_if_fail (key != NULL, NULL);
	g_return_val_if_fail (hex != NULL, NULL);

	return g_object_new (GCOLOR3_TYPE_COLOR_ITEM, "key", key, "hex", hex, NULL);
}
