/*  $Id$
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
 *  Copyright (C) 1999-2004 Olivier Fourdan (fourdan@xfce.org)
 *
 *  Portions based Thinice port by 
 *                       Tim Gerla <timg@rrv.net>,
 *                       Tomas Ögren <stric@ing.umu.se,
 *                       Richard Hult <rhult@codefactory.se>
 *  Portions based on Smooth theme by
 *                       Andrew Johnson <ajgenius@ajgenius.us>
 *  Portions based on IceGradient theme by  
 *                       Tim Gerla <timg@means.net>
 *                       Tomas Ã–gren <stric@ing.umu.se>
 *                       JM Perez <jose.perez@upcnet.es>
 *  Portions based on Wonderland theme by   
 *                       Garrett LeSage
 *                       Alexander Larsson
 *                       Owen Taylor <otaylor@redhat.com>
 *  Portions based on Raleigh theme by 
 *                       Owen Taylor <otaylor@redhat.com>
 *  Portions based on Notif theme
 *  Portions based on Notif2 theme
 *  Portions based on original GTK theme
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gtk/gtkrc.h>

typedef struct _XfceRcStyle XfceRcStyle;
typedef struct _XfceRcStyleClass XfceRcStyleClass;

extern GType xfce_type_rc_style;

#define XFCE_TYPE_RC_STYLE              xfce_type_rc_style
#define XFCE_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), XFCE_TYPE_RC_STYLE, XfceRcStyle))
#define XFCE_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XFCE_TYPE_RC_STYLE, XfceRcStyleClass))
#define XFCE_IS_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), XFCE_TYPE_RC_STYLE))
#define XFCE_IS_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XFCE_TYPE_RC_STYLE))
#define XFCE_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XFCE_TYPE_RC_STYLE, XfceRcStyleClass))

typedef enum
{
    XFCE_RC_GRADIENT_HORIZONTAL,
    XFCE_RC_GRADIENT_VERTICAL,
    XFCE_RC_GRADIENT_NORTHERN_DIAGONAL,
    XFCE_RC_GRADIENT_SOUTHERN_DIAGONAL,
    XFCE_RC_GRADIENT_AUTO
} XfceRcGradientType;

typedef enum
{
    XFCE_RC_HANDLER_NONE,
    XFCE_RC_GRIP_ROUGH,
    XFCE_RC_GRIP_SLIDE
} XfceRcHandlerType;

typedef enum
{
    XFCE_RC_FLAGS_NONE = 0,
    XFCE_RC_FOCUS_COLOR = 1<<0
} XfceRcFlags;

struct _XfceRcStyle
{
    GtkRcStyle parent_instance;

    XfceRcFlags flags;

    gboolean smooth_edge;
    XfceRcHandlerType grip_style;
    gboolean gradient;
    XfceRcGradientType gradient_style;
    gfloat shade_start;
    gfloat shade_end;
    gboolean flat_border;
    GdkColor focus_color;
};

struct _XfceRcStyleClass
{
    GtkRcStyleClass parent_class;
};

void xfce_rc_style_register_type(GTypeModule * module);

enum
{
    TOKEN_SMOOTHEDGE = G_TOKEN_LAST + 1,
    TOKEN_BOXFILL,
    TOKEN_FILL_STYLE,
    TOKEN_GRIP_STYLE,
    TOKEN_GRIP_NONE,
    TOKEN_GRIP_ROUGH,
    TOKEN_GRIP_SLIDE,
    TOKEN_GRADIENT,
    TOKEN_PLAIN,
    TOKEN_ORIENTATION,
    TOKEN_AUTO,
    TOKEN_HORIZONTAL,
    TOKEN_VERTICAL,
    TOKEN_NORTHERN_DIAGONAL,
    TOKEN_SOUTHERN_DIAGONAL,
    TOKEN_SHADE_START,
    TOKEN_SHADE_END,
    TOKEN_FLATBORDER,
    TOKEN_FOCUSCOLOR,
    TOKEN_TRUE,
    TOKEN_FALSE
};
