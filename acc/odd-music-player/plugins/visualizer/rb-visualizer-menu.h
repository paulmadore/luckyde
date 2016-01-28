/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 *  Copyright (C) 2010  Jonathan Matthew <jonathan@d14n.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  The Rhythmbox authors hereby grant permission for non-GPL compatible
 *  GStreamer plugins to be used and distributed together with GStreamer
 *  and Rhythmbox. This permission is above and beyond the permissions granted
 *  by the GPL license by which Rhythmbox is covered. If you modify this code
 *  you may extend this exception to your version of the code, but you are not
 *  obligated to do so. If you do not wish to do so, delete this exception
 *  statement from your version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA.
 *
 */

#ifndef RB_VISUALIZER_MENU_H
#define RB_VISUALIZER_MENU_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

/* quality settings */
typedef struct {
	const char *name;
	const char *setting;
	int width;
	int height;
	gint fps_n;
	gint fps_d;
} VisualizerQuality;

extern const VisualizerQuality rb_visualizer_quality[];

int	rb_visualizer_menu_clip_quality 	(int value);

GMenu *rb_visualizer_create_popup_menu 	(const char *fullscreen_action);

G_END_DECLS

#endif /* RB_VISUALIZER_MENU_H */
