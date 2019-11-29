/* HSV color selector for GTK
 *
 * Copyright (C) 1999 The Free Software Foundation
 *
 * Authors: Simon Budig <Simon.Budig@unix-ag.org> (original code)
 *          Federico Mena-Quintero <federico@gimp.org> (cleanup for GTK)
 *          Jonathan Blandford <jrb@redhat.com> (cleanup for GTK)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Modified by the GTK Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK at ftp://ftp.gtk.org/pub/gtk/.
 *
 * Updated and adapted for inclusion in Gcolor3 by Jente Hidskes 2018.
 */

#ifndef __GCOLOR3_HSV_H__
#define __GCOLOR3_HSV_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GCOLOR3_TYPE_HSV            (gcolor3_hsv_get_type ())
#define GCOLOR3_HSV(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GCOLOR3_TYPE_HSV, Gcolor3HSV))
#define GCOLOR3_HSV_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GCOLOR3_TYPE_HSV, Gcolor3HSVClass))
#define GCOLOR3_IS_HSV(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GCOLOR3_TYPE_HSV))
#define GCOLOR3_IS_HSV_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GCOLOR3_TYPE_HSV))
#define GCOLOR3_HSV_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GCOLOR3_TYPE_HSV, Gcolor3HSVClass))


typedef struct _Gcolor3HSV              Gcolor3HSV;
typedef struct _Gcolor3HSVPrivate       Gcolor3HSVPrivate;
typedef struct _Gcolor3HSVClass         Gcolor3HSVClass;

struct _Gcolor3HSV
{
  GtkWidget parent_instance;

  /*< private >*/
  Gcolor3HSVPrivate *priv;
};

struct _Gcolor3HSVClass
{
  GtkWidgetClass parent_class;

  /* Notification signals */
  void (* changed) (Gcolor3HSV       *hsv);

  /* Keybindings */
  void (* move)    (Gcolor3HSV       *hsv,
                    GtkDirectionType  type);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GType      gcolor3_hsv_get_type     (void) G_GNUC_CONST;
GtkWidget* gcolor3_hsv_new          (void);
void       gcolor3_hsv_set_color    (Gcolor3HSV    *hsv,
				     double         h,
				     double         s,
				     double         v);
void       gcolor3_hsv_get_color    (Gcolor3HSV    *hsv,
				     gdouble       *h,
				     gdouble       *s,
				     gdouble       *v);
void       gcolor3_hsv_set_metrics  (Gcolor3HSV    *hsv,
				     gint           size,
				     gint           ring_width);
void       gcolor3_hsv_get_metrics  (Gcolor3HSV    *hsv,
				     gint          *size,
				     gint          *ring_width);
gboolean   gcolor3_hsv_is_adjusting (Gcolor3HSV    *hsv);

G_END_DECLS

#endif /* __GCOLOR3_HSV_H__ */
