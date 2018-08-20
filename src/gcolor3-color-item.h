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

#ifndef __GCOLOR3_COLOR_ITEM_H__
#define __GCOLOR3_COLOR_ITEM_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _Gcolor3ColorItem        Gcolor3ColorItem;
typedef struct _Gcolor3ColorItemClass   Gcolor3ColorItemClass;
typedef struct _Gcolor3ColorItemPrivate Gcolor3ColorItemPrivate;

#define GCOLOR3_TYPE_COLOR_ITEM            (gcolor3_color_item_get_type ())
#define GCOLOR3_COLOR_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GCOLOR3_TYPE_COLOR_ITEM, Gcolor3ColorItem))
#define GCOLOR3_COLOR_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GCOLOR3_TYPE_COLOR_ITEM, Gcolor3ColorItemClass))
#define GCOLOR3_IS_COLOR_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GCOLOR3_TYPE_COLOR_ITEM))
#define GCOLOR3_IS_COLOR_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GCOLOR3_TYPE_COLOR_ITEM))
#define GCOLOR3_COLOR_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GCOLOR3_TYPE_COLOR_ITEM, Gcolor3ColorItemClass))

struct _Gcolor3ColorItem {
	GObject base_instance;
};

struct _Gcolor3ColorItemClass {
	GObjectClass parent_class;
};

GType             gcolor3_color_item_get_type (void) G_GNUC_CONST;

Gcolor3ColorItem *gcolor3_color_item_new (const gchar *key, const gchar *hex);

G_END_DECLS

#endif /* __GCOLOR3_COLOR_ITEM_H__ */
