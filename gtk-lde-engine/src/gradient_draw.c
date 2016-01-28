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
 *  Copyright (C) 2011 Peter de Ridder <peter@xfce.org>
 *
 *  Portions based Thinice port by
 *                       Tim Gerla <timg@rrv.net>,
 *                       Tomas Ögren <stric@ing.umu.se,
 *                       Richard Hult <rhult@codefactory.se>
 *  Portions based on Smooth theme by
 *                       Andrew Johnson <ajgenius@ajgenius.us>
 *  Portions based on IceGradient theme by
 *                       Tim Gerla <timg@means.net>
 *                       Tomas Ögren <stric@ing.umu.se>
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

#include <gtk/gtk.h>
#include <math.h>
#include "gradient_draw.h"

static void gradient_rgb_to_hls(gdouble * r, gdouble * g, gdouble * b)
{
    gdouble red = *r, green = *g, blue = *b;
    gdouble h, l, s;
    gdouble min, max, delta;

    if(red > green)
    {
        max = MAX(red, blue);
        min = MIN(green, blue);
    }
    else
    {
        max = MAX(green, blue);
        min = MIN(red, blue);
    }

    l = (max + min) / 2;
    s = 0;
    h = 0;

    if(max != min)
    {
        if(l <= 0.5)
        {
            s = (max - min) / (max + min);
        }
        else
        {
            s = (max - min) / (2 - max - min);
        }

        delta = max - min;
        if(red == max)
        {
            h = (green - blue) / delta;
        }
        else if(green == max)
        {
            h = 2 + (blue - red) / delta;
        }
        else if(blue == max)
        {
            h = 4 + (red - green) / delta;
        }

        h *= 60;
        if(h < 0.0)
        {
            h += 360;
        }
    }

    *r = h;
    *g = l;
    *b = s;
}

static void gradient_hls_to_rgb(gdouble * h, gdouble * l, gdouble * s)
{
    gdouble hue = *h + 120;
    gdouble lightness = *l;
    gdouble saturation = *s;
    gdouble m1, m2;
    gdouble r, g, b;

    if(lightness <= 0.5)
    {
        m2 = lightness * (1 + saturation);
    }
    else
    {
        m2 = lightness + saturation - lightness * saturation;
    }
    m1 = 2 * lightness - m2;

    if(saturation == 0)
    {
        *h = lightness;
        *l = lightness;
        *s = lightness;
    }
    else
    {
        while(hue > 360)
        {
            hue -= 360;
        }
        while(hue < 0)
        {
            hue += 360;
        }

        if(hue < 60)
        {
            r = m1 + (m2 - m1) * hue / 60;
        }
        else if(hue < 180)
        {
            r = m2;
        }
        else if(hue < 240)
        {
            r = m1 + (m2 - m1) * (240 - hue) / 60;
        }
        else
        {
            r = m1;
        }

        hue = *h;
        while(hue > 360)
        {
            hue -= 360;
        }
        while(hue < 0)
        {
            hue += 360;
        }

        if(hue < 60)
        {
            g = m1 + (m2 - m1) * hue / 60;
        }
        else if(hue < 180)
        {
            g = m2;
        }
        else if(hue < 240)
        {
            g = m1 + (m2 - m1) * (240 - hue) / 60;
        }
        else
        {
            g = m1;
        }

        hue = *h - 120;
        while(hue > 360)
        {
            hue -= 360;
        }
        while(hue < 0)
        {
            hue += 360;
        }
        if(hue < 60)
        {
            b = m1 + (m2 - m1) * hue / 60;
        }
        else if(hue < 180)
        {
            b = m2;
        }
        else if(hue < 240)
        {
            b = m1 + (m2 - m1) * (240 - hue) / 60;
        }
        else
        {
            b = m1;
        }

        *h = r;
        *l = g;
        *s = b;
    }
}

static void gradient_add_stop_color_shaded(cairo_pattern_t *gradient, gdouble offset, const GdkColor * a, gdouble k)
{
    gdouble red = (gdouble) a->red / 65535.0;
    gdouble green = (gdouble) a->green / 65535.0;
    gdouble blue = (gdouble) a->blue / 65535.0;

    gradient_rgb_to_hls(&red, &green, &blue);

    green = MIN(green * k, 1.0);
    green = MAX(green, 0.0);

    blue = MIN(blue * k, 1.0);
    blue = MAX(blue, 0.0);

    gradient_hls_to_rgb(&red, &green, &blue);

    cairo_pattern_add_color_stop_rgb(gradient, offset, red, green, blue);
}

void gradient_draw_shaded(cairo_t *cr, gint x, gint y, gint width, gint height, const GdkColor * color, gfloat shine_value, gfloat gradient_shade_value, GradientType gradient_style)
{
    cairo_pattern_t *gradient;
    gboolean horizontal = (gradient_style == GRADIENT_HORIZONTAL);
    gboolean northern = (gradient_style == GRADIENT_NORTHERN_DIAGONAL);
    gboolean diagonal = ((gradient_style == GRADIENT_NORTHERN_DIAGONAL) || (gradient_style == GRADIENT_SOUTHERN_DIAGONAL));

    if(diagonal)
    {
        gdouble size = (width + height) / 2.0;
        gdouble xx = (width - height)/4.0 + x;
        gdouble yy = (height - width)/4.0 + y;

        if(northern)
        {
            gradient = cairo_pattern_create_linear(xx, yy, xx + size, yy + size);
        }
        else
        {
            gradient = cairo_pattern_create_linear(xx + size, yy, xx, yy + size);
        }
    }
    else if(horizontal)
    {
        gradient = cairo_pattern_create_linear(x, y, x + width, y);
    }
    else
    {
        gradient = cairo_pattern_create_linear(x, y, x, y + height);
    }

    gradient_add_stop_color_shaded(gradient, 0, color, shine_value);
    gradient_add_stop_color_shaded(gradient, 1, color, gradient_shade_value);

    cairo_save(cr);

    cairo_set_source(cr, gradient);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    cairo_rectangle(cr, x, y, width, height);

    cairo_fill (cr);

    cairo_restore(cr);

    cairo_pattern_destroy(gradient);
}
