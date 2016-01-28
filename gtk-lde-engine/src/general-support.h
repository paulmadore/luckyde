/*  Helper functions for gtk-engines
 *
 *  Copyright (C) 2006 Andrew Johnson <acjgenius@earthlink.net>
 *  Copyright (C) 2006-2007 Benjamin Berg <benjamin@sipsolutions.net>
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
 *  modified by Peter de Ridder <peter@xfce.org>
 *
 */

#ifndef __GENERAL_SUPPORT_H
#define __GENERAL_SUPPORT_H

/* macros to make sure that things are sane ... */

#define CHECK_DETAIL(detail, value) ((detail) && (!strcmp(value, detail)))

#define CHECK_ARGS					\
  g_return_if_fail (window != NULL);			\
  g_return_if_fail (style != NULL);

#define SANITIZE_SIZE					\
  g_return_if_fail (width  >= -1);			\
  g_return_if_fail (height >= -1);			\
                                                        \
  if ((width == -1) && (height == -1))			\
    gdk_drawable_get_size (window, &width, &height);	\
  else if (width == -1)					\
    gdk_drawable_get_size (window, &width, NULL);	\
  else if (height == -1)				\
    gdk_drawable_get_size (window, NULL, &height);

#endif /* __GENERAL_SUPPORT_H */
