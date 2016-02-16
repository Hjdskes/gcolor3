/* Gcolor3Application
 *
 * Copyright (C) 2015-2016 Jente Hidskes
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

#ifndef __GCOLOR3_APPLICATION_H__
#define __GCOLOR3_APPLICATION_H__

#include <gtk/gtk.h>
#include <glib.h>

G_BEGIN_DECLS

typedef struct _Gcolor3Application        Gcolor3Application;
typedef struct _Gcolor3ApplicationClass   Gcolor3ApplicationClass;
typedef struct _Gcolor3ApplicationPrivate Gcolor3ApplicationPrivate;

#define GCOLOR3_TYPE_APPLICATION            (gcolor3_application_get_type ())
#define GCOLOR3_APPLICATION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GCOLOR3_TYPE_APPLICATION, Gcolor3Application))
#define GCOLOR3_APPLICATION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GCOLOR3_TYPE_APPLICATION, Gcolor3ApplicationClass))
#define GCOLOR3_IS_APPLICATION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GCOLOR3_TYPE_APPLICATION))
#define GCOLOR3_IS_APPLICATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GCOLOR3_TYPE_APPLICATION))
#define GCOLOR3_APPLICATION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GCOLOR3_TYPE_APPLICATION, Gcolor3ApplicationClass))

struct _Gcolor3Application {
	GtkApplication base_instance;
};

struct _Gcolor3ApplicationClass {
	GtkApplicationClass parent_class;
};

GType               gcolor3_application_get_type (void) G_GNUC_CONST;

Gcolor3Application *gcolor3_application_new (void);

G_END_DECLS

#endif /* __GCOLOR3_APPLICATION_H__ */

