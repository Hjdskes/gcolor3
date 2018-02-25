/* Utilities
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>

#include <glib.h>
#include <gdk/gdk.h>

#include "utils.h"

inline double
scale_round (double val, double factor)
{
  val = floor (val * factor + 0.5);
  val = MAX (val, 0);
  val = MIN (val, factor);
  return val;
}

inline gchar *
hex_value (GdkRGBA *color) {
	return g_strdup_printf ("#%.2X%.2X%.2X",
			       (unsigned int) (scale_round (color->red, 255)),
			       (unsigned int) (scale_round (color->green, 255)),
			       (unsigned int) (scale_round (color->blue, 255)));
}

