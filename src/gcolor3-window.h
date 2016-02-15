/* Gcolor3Window
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

#ifndef __GCOLOR3_WINDOW_H__
#define __GCOLOR3_WINDOW_H__

#include <gtk/gtk.h>
#include <glib.h>

#include "gcolor3-application.h"

G_BEGIN_DECLS

typedef struct _Gcolor3Window        Gcolor3Window;
typedef struct _Gcolor3WindowClass   Gcolor3WindowClass;
typedef struct _Gcolor3WindowPrivate Gcolor3WindowPrivate;

#define GCOLOR3_TYPE_WINDOW            (gcolor3_window_get_type ())
#define GCOLOR3_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GCOLOR3_TYPE_WINDOW, Gcolor3Window))
#define GCOLOR3_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GCOLOR3_TYPE_WINDOW, Gcolor3WindowClass))
#define GCOLOR3_IS_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GCOLOR3_TYPE_WINDOW))
#define GCOLOR3_IS_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GCOLOR3_TYPE_WINDOW))
#define GCOLOR3_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GCOLOR3_TYPE_WINDOW, Gcolor3WindowClass))

struct _Gcolor3Window {
	GtkApplicationWindow base_instance;
};

struct _Gcolor3WindowClass {
	GtkApplicationWindowClass parent_class;
};

GType          gcolor3_window_get_type (void) G_GNUC_CONST;

Gcolor3Window *gcolor3_window_new (Gcolor3Application *application);

void           gcolor3_window_show_about_dialog (Gcolor3Window *window);

void           gcolor3_window_add_colors (Gcolor3Window *window);

G_END_DECLS

#endif /* __GCOLOR3_WINDOW_H__ */

