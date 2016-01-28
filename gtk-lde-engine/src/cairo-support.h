/*  Helper functions for gtk-engines
 *
 *  Copyright (C) 2006 Andrew Johnson <acjgenius@earthlink.net>
 *  Copyright (C) 2006 Thomas Wood <thos@gnome.org>
 *  Copyright (C) 2006-2007 Benjamin Berg <benjamin@sipsolutions.net>
 *  Copyright (C) 2007 Andrea Cimitan <andrea.cimitan@gmail.com>
 *  Copyright (C) 2011 Peter de Ridder <peter@xfce.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *
 *  Written by Andrew Johnson <acjgenius@earthlink.net>
 *  Written by Benjamin Berg <benjamin@sipsolutions.net>
 *  Written by Thomas Wood <thos@gnome.org>
 *  modified by Andrea Cimitan <andrea.cimitan@gmail.com>
 *  modified by Peter de Ridder <peter@xfce.org>
 *
 */

#include <gtk/gtk.h>

G_GNUC_INTERNAL cairo_t * ge_gdk_drawable_to_cairo (GdkDrawable  *window, GdkRectangle *area);
