/* GTK - The GIMP Toolkit
 * Copyright (C) 2000 Red Hat, Inc.
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
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
 * Updated and adapted for inclusion in Gcolor3 by Jente Hidskes 2017.
 */

#ifndef __GCOLOR3_COLOR_SELECTION_H__
#define __GCOLOR3_COLOR_SELECTION_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GCOLOR3_TYPE_COLOR_SELECTION            (gcolor3_color_selection_get_type ())
#define GCOLOR3_COLOR_SELECTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GCOLOR3_TYPE_COLOR_SELECTION, Gcolor3ColorSelection))
#define GCOLOR3_COLOR_SELECTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GCOLOR3_TYPE_COLOR_SELECTION, Gcolor3ColorSelectionClass))
#define GCOLOR3_IS_COLOR_SELECTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GCOLOR3_TYPE_COLOR_SELECTION))
#define GCOLOR3_IS_COLOR_SELECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GCOLOR3_TYPE_COLOR_SELECTION))
#define GCOLOR3_COLOR_SELECTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GCOLOR3_TYPE_COLOR_SELECTION, Gcolor3ColorSelectionClass))

typedef struct _Gcolor3ColorSelection        Gcolor3ColorSelection;
typedef struct _Gcolor3ColorSelectionPrivate Gcolor3ColorSelectionPrivate;
typedef struct _Gcolor3ColorSelectionClass   Gcolor3ColorSelectionClass;

/**
 * Gcolor3ColorSelectionChangePaletteFunc:
 * @colors: (array length=n_colors): Array of colors
 * @n_colors: Number of colors in the array
 */
typedef void (* Gcolor3ColorSelectionChangePaletteFunc) (const GdkRGBA *colors,
                                                         gint           n_colors);

/**
 * Gcolor3ColorSelectionChangePaletteWithScreenFunc:
 * @screen:
 * @colors: (array length=n_colors): Array of colors
 * @n_colors: Number of colors in the array
 *
 * Since: 2.2
 */
typedef void (* Gcolor3ColorSelectionChangePaletteWithScreenFunc) (GdkScreen     *screen,
							           const GdkRGBA *colors,
							           gint           n_colors);

struct _Gcolor3ColorSelection
{
  GtkBox parent_instance;

  /*< private >*/
  Gcolor3ColorSelectionPrivate *private_data;
};

/**
 * Gcolor3ColorSelectionClass:
 * @parent_class: The parent class.
 * @color_changed:
 */
struct _Gcolor3ColorSelectionClass
{
  GtkBoxClass parent_class;

  void (*color_changed)	(Gcolor3ColorSelection *color_selection);

  /*< private >*/

  /* Padding for future expansion */
  void (*_gcolor3_reserved1) (void);
  void (*_gcolor3_reserved2) (void);
  void (*_gcolor3_reserved3) (void);
  void (*_gcolor3_reserved4) (void);
};


/* ColorSelection */

GType      gcolor3_color_selection_get_type                (void) G_GNUC_CONST;
GtkWidget *gcolor3_color_selection_new                     (void);
gboolean   gcolor3_color_selection_get_has_opacity_control (Gcolor3ColorSelection *colorsel);
void       gcolor3_color_selection_set_has_opacity_control (Gcolor3ColorSelection *colorsel,
							    gboolean               has_opacity);
gboolean   gcolor3_color_selection_get_has_palette         (Gcolor3ColorSelection *colorsel);
void       gcolor3_color_selection_set_has_palette         (Gcolor3ColorSelection *colorsel,
							    gboolean               has_palette);


void     gcolor3_color_selection_set_current_alpha   (Gcolor3ColorSelection *colorsel,
						      guint16                alpha);
guint16  gcolor3_color_selection_get_current_alpha   (Gcolor3ColorSelection *colorsel);
void     gcolor3_color_selection_set_previous_alpha  (Gcolor3ColorSelection *colorsel,
						      guint16                alpha);
guint16  gcolor3_color_selection_get_previous_alpha  (Gcolor3ColorSelection *colorsel);

void     gcolor3_color_selection_set_current_rgba    (Gcolor3ColorSelection *colorsel,
                                                      const GdkRGBA         *rgba);
void     gcolor3_color_selection_get_current_rgba    (Gcolor3ColorSelection *colorsel,
                                                      GdkRGBA               *rgba);
void     gcolor3_color_selection_set_previous_rgba   (Gcolor3ColorSelection *colorsel,
                                                      const GdkRGBA         *rgba);
void     gcolor3_color_selection_get_previous_rgba   (Gcolor3ColorSelection *colorsel,
                                                      GdkRGBA               *rgba);

gboolean gcolor3_color_selection_is_adjusting        (Gcolor3ColorSelection *colorsel);

gboolean gcolor3_color_selection_palette_from_string (const gchar      *str,
                                                      GdkRGBA         **colors,
                                                      gint             *n_colors);
gchar*   gcolor3_color_selection_palette_to_string   (const GdkRGBA    *colors,
                                                      gint              n_colors);

Gcolor3ColorSelectionChangePaletteWithScreenFunc gcolor3_color_selection_set_change_palette_with_screen_hook (Gcolor3ColorSelectionChangePaletteWithScreenFunc func);

void     gcolor3_color_selection_set_current_color   (Gcolor3ColorSelection *colorsel,
                                                      const GdkRGBA         *color);
void     gcolor3_color_selection_get_current_color   (Gcolor3ColorSelection *colorsel,
                                                      GdkRGBA               *color);
void     gcolor3_color_selection_set_previous_color  (Gcolor3ColorSelection *colorsel,
                                                      const GdkRGBA         *color);
void     gcolor3_color_selection_get_previous_color  (Gcolor3ColorSelection *colorsel,
                                                      GdkRGBA               *color);

G_END_DECLS

#endif /* __GCOLOR3_COLOR_SELECTION_H__ */
