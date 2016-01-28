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

#include "xfce_rc_style.h"
#include "xfce_style.h"

#define DEFAULT_SHADE_START     1.10
#define DEFAULT_SHADE_END       0.90

static void xfce_rc_style_init(XfceRcStyle * style);
static void xfce_rc_style_class_init(XfceRcStyleClass * klass);
static guint xfce_rc_style_parse(GtkRcStyle * rc_style, GtkSettings * settings, GScanner * scanner);
static void xfce_rc_style_merge(GtkRcStyle * dest, GtkRcStyle * src);

static struct
{
    gchar *name;
    guint token;
}

theme_symbols[] =
{
    { "smooth_edge", TOKEN_SMOOTHEDGE },
    { "boxfill", TOKEN_BOXFILL },
    { "fill_style", TOKEN_FILL_STYLE },
    { "grip_style", TOKEN_GRIP_STYLE },
    { "none", TOKEN_GRIP_NONE },
    { "rough", TOKEN_GRIP_ROUGH },
    { "slide", TOKEN_GRIP_SLIDE },
    { "gradient", TOKEN_GRADIENT },
    { "plain", TOKEN_PLAIN },
    { "orientation", TOKEN_ORIENTATION },
    { "auto", TOKEN_AUTO },
    { "vertical", TOKEN_VERTICAL },
    { "horizontal", TOKEN_HORIZONTAL },
    { "northern_diagonal", TOKEN_NORTHERN_DIAGONAL },
    { "southern_diagonal", TOKEN_SOUTHERN_DIAGONAL },
    { "shade_start", TOKEN_SHADE_START },
    { "shade_end", TOKEN_SHADE_END },
    { "flat_border", TOKEN_FLATBORDER },
    { "focus_color", TOKEN_FOCUSCOLOR },
    { "true", TOKEN_TRUE},
    { "TRUE", TOKEN_TRUE},
    { "false", TOKEN_FALSE},
    { "FALSE", TOKEN_FALSE}
};

static guint n_theme_symbols = sizeof(theme_symbols) / sizeof(theme_symbols[0]);

static GtkStyle *xfce_rc_style_create_style(GtkRcStyle * rc_style);

static GtkRcStyleClass *parent_class;

GType xfce_type_rc_style = 0;

void xfce_rc_style_register_type(GTypeModule * module)
{
    static const GTypeInfo object_info = {
        sizeof(XfceRcStyleClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) xfce_rc_style_class_init,
        NULL,                   /* class_finalize */
        NULL,                   /* class_data */
        sizeof(XfceRcStyle),
        0,                      /* n_preallocs */
        (GInstanceInitFunc) xfce_rc_style_init,
    };

    xfce_type_rc_style = g_type_module_register_type(module, GTK_TYPE_RC_STYLE, "XfceRcStyle", &object_info, 0);
}

static void xfce_rc_style_init(XfceRcStyle * style)
{
    style->flags = XFCE_RC_FLAGS_NONE;
    style->smooth_edge = FALSE;
    style->grip_style = XFCE_RC_GRIP_ROUGH;
    style->gradient = FALSE;
    style->gradient_style = XFCE_RC_GRADIENT_AUTO;
    style->shade_start = DEFAULT_SHADE_START;
    style->shade_end = DEFAULT_SHADE_END;
    style->flat_border = FALSE;
}

static void xfce_rc_style_class_init(XfceRcStyleClass * klass)
{
    GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS(klass);

    parent_class = g_type_class_peek_parent(klass);

    rc_style_class->parse = xfce_rc_style_parse;
    rc_style_class->merge = xfce_rc_style_merge;
    rc_style_class->create_style = xfce_rc_style_create_style;
}

static guint theme_parse_boolean(GScanner * scanner, GTokenType wanted_token, gboolean * retval)
{
    guint token;

    token = g_scanner_get_next_token(scanner);
    if (token != wanted_token)
    {
        return wanted_token;
    }

    token = g_scanner_get_next_token(scanner);
    if(token != G_TOKEN_EQUAL_SIGN)
    {
        return G_TOKEN_EQUAL_SIGN;
    }
    token = g_scanner_get_next_token(scanner);
    if(token == TOKEN_TRUE)
    {
        *retval = TRUE;
    }
    else if(token == TOKEN_FALSE)
    {
        *retval = FALSE;
    }
    else
    {
        return TOKEN_TRUE;
    }
    
    return G_TOKEN_NONE;
}

static guint theme_parse_float (GScanner *scanner, GTokenType wanted_token, gfloat return_default, gfloat lower_limit, gfloat upper_limit, gfloat *retval)
{
    guint token;

    token = g_scanner_get_next_token (scanner);
    if (token != wanted_token)
    {
        return wanted_token;
    }
    token = g_scanner_get_next_token (scanner);
    if (token != G_TOKEN_EQUAL_SIGN)
    {
        return G_TOKEN_EQUAL_SIGN;
    }

    token = g_scanner_get_next_token (scanner);

    if (token != G_TOKEN_FLOAT)
    {
      *retval = return_default;
    }
    else
    {
      *retval = scanner->value.v_float;
    }
    if (*retval < lower_limit) 
    {
        *retval = lower_limit; 
    }
    if ((*retval > upper_limit) && (upper_limit > lower_limit)) 
    {
        *retval = upper_limit; 
    }
    return G_TOKEN_NONE;
}

static guint theme_parse_fillstyle (GScanner * scanner, GTokenType wanted_token, gboolean * retval)
{
    guint token;

    token = g_scanner_get_next_token(scanner);
    if (token != wanted_token)
    {
        return wanted_token;
    }

    token = g_scanner_get_next_token(scanner);
    if(token != G_TOKEN_EQUAL_SIGN)
    {
        return G_TOKEN_EQUAL_SIGN;
    }
    
    token = g_scanner_get_next_token(scanner);
    if(token == TOKEN_GRADIENT)
    {
        *retval = TRUE;
    }
    else if (token == TOKEN_PLAIN)
    {
        *retval = FALSE;
    }
    else
    {
        return TOKEN_PLAIN;
    }
    
    return G_TOKEN_NONE;
}

static guint theme_parse_orientation (GScanner * scanner, GTokenType wanted_token, XfceRcGradientType * retval)
{
    guint token;

    token = g_scanner_get_next_token(scanner);
    if (token != wanted_token)
    {
        return wanted_token;
    }

    token = g_scanner_get_next_token(scanner);
    if(token != G_TOKEN_EQUAL_SIGN)
    {
        return G_TOKEN_EQUAL_SIGN;
    }
    token = g_scanner_get_next_token(scanner);
    if(token == TOKEN_HORIZONTAL)
    {
        *retval = XFCE_RC_GRADIENT_HORIZONTAL;
    }
    else if(token == TOKEN_VERTICAL)
    {
        *retval = XFCE_RC_GRADIENT_VERTICAL;
    }
    else if(token == TOKEN_NORTHERN_DIAGONAL)
    {
        *retval = XFCE_RC_GRADIENT_NORTHERN_DIAGONAL;
    }
    else if(token == TOKEN_SOUTHERN_DIAGONAL)
    {
        *retval = XFCE_RC_GRADIENT_SOUTHERN_DIAGONAL;
    }
    else
    {
        *retval = XFCE_RC_GRADIENT_AUTO;
    }
    
    return G_TOKEN_NONE;
}

static guint theme_parse_boxfill (GScanner * scanner, GTokenType wanted_token, XfceRcStyle * retval)
{
    guint token;
    gboolean b;
    gfloat f;
    XfceRcGradientType orientation;

    token = g_scanner_get_next_token(scanner);
    if (token != wanted_token)
    {
        return wanted_token;
    }

    token = g_scanner_get_next_token(scanner);
    if(token != G_TOKEN_LEFT_CURLY)
    {
        return G_TOKEN_LEFT_CURLY;
    }

    token = g_scanner_peek_next_token(scanner);
    while (token != G_TOKEN_RIGHT_CURLY)
    {
        switch (token)
        {
            case TOKEN_FILL_STYLE:
                token = theme_parse_fillstyle(scanner, TOKEN_FILL_STYLE, &b);
                if(token != G_TOKEN_NONE)
                {
                    break;
                }
                retval->gradient = b;
                break;
            case TOKEN_ORIENTATION:
                token = theme_parse_orientation(scanner, TOKEN_ORIENTATION, &orientation);
                if(token != G_TOKEN_NONE)
                {
                    break;
                }
                retval->gradient_style = orientation;
                break;
            case TOKEN_SHADE_START:
                token = theme_parse_float (scanner, TOKEN_SHADE_START, DEFAULT_SHADE_START, 0.0, 2.0, &f);
                if(token != G_TOKEN_NONE)
                {
                    break;
                }
                retval->shade_start = f;
                break;
            case TOKEN_SHADE_END:
                token = theme_parse_float (scanner, TOKEN_SHADE_END, DEFAULT_SHADE_END, 0.0, 2.0, &f);
                if(token != G_TOKEN_NONE)
                {
                    break;
                }
                retval->shade_end = f;
                break;
            default:
                g_scanner_get_next_token(scanner);
                token = G_TOKEN_RIGHT_CURLY;
                break;
        }
        if (token != G_TOKEN_NONE)
        {
            return token;
        }
        token = g_scanner_peek_next_token(scanner);
    }
    g_scanner_get_next_token (scanner);
    return G_TOKEN_NONE;
}

static guint theme_parse_grip_style (GScanner * scanner, GTokenType wanted_token, XfceRcStyle * retval)
{
    guint token;

    token = g_scanner_get_next_token(scanner);
    if (token != wanted_token)
    {
        return wanted_token;
    }

    token = g_scanner_get_next_token(scanner);
    if(token != G_TOKEN_EQUAL_SIGN)
    {
        return G_TOKEN_EQUAL_SIGN;
    }
    token = g_scanner_get_next_token(scanner);
    if(token == TOKEN_GRIP_NONE)
    {
        retval->grip_style = XFCE_RC_HANDLER_NONE;
    }
    else if(token == TOKEN_GRIP_SLIDE)
    {
        retval->grip_style = XFCE_RC_GRIP_SLIDE;
    }
    else
    {
        retval->grip_style = XFCE_RC_GRIP_ROUGH;
    }
    
    return G_TOKEN_NONE;
}

static guint theme_parse_color (GScanner *scanner, GTokenType wanted_token, GtkRcStyle *style, GdkColor *retval)
{
    guint token;

    token = g_scanner_get_next_token (scanner);
    if (token != wanted_token)
    {
        return wanted_token;
    }
    token = g_scanner_get_next_token (scanner);
    if (token != G_TOKEN_EQUAL_SIGN)
    {
        return G_TOKEN_EQUAL_SIGN;
    }

    return gtk_rc_parse_color_full (scanner, style, retval);
}

static guint xfce_rc_style_parse(GtkRcStyle * rc_style, GtkSettings * settings, GScanner * scanner)
{
    static GQuark scope_id = 0;
    XfceRcStyle *theme_data = XFCE_RC_STYLE(rc_style);
    guint old_scope;
    guint token;
    guint i;
    gboolean b;
    GdkColor c;

    /* Set up a new scope in this scanner. */
    if(!scope_id)
    {
        scope_id = g_quark_from_string("theme_engine");
    }
    old_scope = g_scanner_set_scope(scanner, scope_id);
    if(!g_scanner_lookup_symbol(scanner, theme_symbols[0].name))
    {
        for(i = 0; i < n_theme_symbols; i++)
        {
            g_scanner_scope_add_symbol(scanner, scope_id, theme_symbols[i].name, GINT_TO_POINTER(theme_symbols[i].token));
        }
    }
    token = g_scanner_peek_next_token(scanner);
    while(token != G_TOKEN_RIGHT_CURLY)
    {
        switch (token)
        {
            case TOKEN_SMOOTHEDGE:
                token = theme_parse_boolean(scanner, TOKEN_SMOOTHEDGE, &b);
                if(token != G_TOKEN_NONE)
                {
                    break;
                }
                theme_data->smooth_edge = b;
                break;
            case TOKEN_GRIP_STYLE:
                token = theme_parse_grip_style(scanner, TOKEN_GRIP_STYLE, theme_data);
                if(token != G_TOKEN_NONE)
                {
                    break;
                }
                break;
            case TOKEN_BOXFILL:
                token = theme_parse_boxfill(scanner, TOKEN_BOXFILL, theme_data);
                if(token != G_TOKEN_NONE)
                {
                    break;
                }
                break;
            case TOKEN_FLATBORDER:
                token = theme_parse_boolean(scanner, TOKEN_FLATBORDER, &b);
                if(token != G_TOKEN_NONE)
                {
                    break;
                }
                theme_data->flat_border = b;
                break;
            case TOKEN_FOCUSCOLOR:
                token = theme_parse_color (scanner, TOKEN_FOCUSCOLOR, rc_style, &c);
                if(token != G_TOKEN_NONE)
                {
                    break;
                }
                theme_data->focus_color = c;
                theme_data->flags |= XFCE_RC_FOCUS_COLOR;
                break;
            default:
                g_scanner_get_next_token(scanner);
                token = G_TOKEN_RIGHT_CURLY;
                break;
        }

        if(token != G_TOKEN_NONE)
        {
            return token;
        }
        token = g_scanner_peek_next_token(scanner);
    }
    g_scanner_get_next_token(scanner);
    g_scanner_set_scope(scanner, old_scope);

    return G_TOKEN_NONE;
}

static void xfce_rc_style_merge(GtkRcStyle * dest, GtkRcStyle * src)
{
    XfceRcStyle *src_data, *dest_data;
    
    parent_class->merge(dest, src);
    if(!XFCE_IS_RC_STYLE(src))
    {
        return;
    }

    src_data = XFCE_RC_STYLE(src);
    dest_data = XFCE_RC_STYLE(dest);

    dest_data->flags = src_data->flags;
    dest_data->smooth_edge = src_data->smooth_edge;
    dest_data->grip_style = src_data->grip_style;
    dest_data->gradient = src_data->gradient;
    dest_data->gradient_style = src_data->gradient_style;
    dest_data->shade_start = src_data->shade_start;
    dest_data->shade_end = src_data->shade_end;
    dest_data->flat_border = src_data->flat_border;
    dest_data->focus_color = src_data->focus_color;
}

/* Create an empty style suitable to this RC style
 */
static GtkStyle *xfce_rc_style_create_style(GtkRcStyle * rc_style)
{
    void *ptr;
    ptr = GTK_STYLE(g_object_new(XFCE_TYPE_STYLE, NULL));
    return (GtkStyle *) ptr;
}
