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

#ifndef __GCOLOR3_COLOR_ROW_H__
#define __GCOLOR3_COLOR_ROW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _Gcolor3ColorRow        Gcolor3ColorRow;
typedef struct _Gcolor3ColorRowClass   Gcolor3ColorRowClass;
typedef struct _Gcolor3ColorRowPrivate Gcolor3ColorRowPrivate;

#define GCOLOR3_TYPE_COLOR_ROW            (gcolor3_color_row_get_type ())
#define GCOLOR3_COLOR_ROW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GCOLOR3_TYPE_COLOR_ROW, Gcolor3ColorRow))
#define GCOLOR3_COLOR_ROW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GCOLOR3_TYPE_COLOR_ROW, Gcolor3ColorRowClass))
#define GCOLOR3_IS_COLOR_ROW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GCOLOR3_TYPE_COLOR_ROW))
#define GCOLOR3_IS_COLOR_ROW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GCOLOR3_TYPE_COLOR_ROW))
#define GCOLOR3_COLOR_ROW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GCOLOR3_TYPE_COLOR_ROW, Gcolor3ColorRowClass))

struct _Gcolor3ColorRow {
	GtkListBoxRow base_instance;
};

struct _Gcolor3ColorRowClass {
	GtkListBoxRowClass parent_class;
};

GType            gcolor3_color_row_get_type (void) G_GNUC_CONST;

Gcolor3ColorRow *gcolor3_color_row_new (const gchar *key, const gchar *hex);

G_END_DECLS

#endif /* __GCOLOR3_COLOR_ROW_H__ */
