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

#ifndef __GCOLOR3_COLOR_STORE_H__
#define __GCOLOR3_COLOR_STORE_H__

#include <gtk/gtk.h>
#include <glib.h>

G_BEGIN_DECLS

typedef struct _Gcolor3ColorStore        Gcolor3ColorStore;
typedef struct _Gcolor3ColorStoreClass   Gcolor3ColorStoreClass;
typedef struct _Gcolor3ColorStorePrivate Gcolor3ColorStorePrivate;

typedef void (*Gcolor3ColorStoreForeachFunc) (const gchar *key,
					      const gchar *hex,
					      gpointer     user_data);

#define GCOLOR3_TYPE_COLOR_STORE            (gcolor3_color_store_get_type ())
#define GCOLOR3_COLOR_STORE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GCOLOR3_TYPE_COLOR_STORE, Gcolor3ColorStore))
#define GCOLOR3_COLOR_STORE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GCOLOR3_TYPE_COLOR_STORE, Gcolor3ColorStoreClass))
#define GCOLOR3_IS_COLOR_STORE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GCOLOR3_TYPE_COLOR_STORE))
#define GCOLOR3_IS_COLOR_STORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GCOLOR3_TYPE_COLOR_STORE))
#define GCOLOR3_COLOR_STORE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GCOLOR3_TYPE_COLOR_STORE, Gcolor3ColorStoreClass))

struct _Gcolor3ColorStore {
	GObject base_instance;
};

struct _Gcolor3ColorStoreClass {
	GObjectClass parent_class;
};

GType              gcolor3_color_store_get_type (void);

Gcolor3ColorStore *gcolor3_color_store_new (void);

gboolean           gcolor3_color_store_add_color (Gcolor3ColorStore *store,
						  const gchar       *key,
						  const gchar       *hex);

gboolean           gcolor3_color_store_remove_color (Gcolor3ColorStore *store,
						     const gchar       *key);

void               gcolor3_color_store_foreach (Gcolor3ColorStore           *store,
						Gcolor3ColorStoreForeachFunc func,
						gpointer                     user_data);

G_END_DECLS

#endif /* __GCOLOR3_COLOR_STORE_H__ */

