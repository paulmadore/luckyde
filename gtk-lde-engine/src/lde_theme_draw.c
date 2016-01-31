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
 *  Copyright (C) 2011-2012 Peter de Ridder (peter@xfce.org)
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

#include <math.h>
#include <gtk/gtk.h>
#include <string.h>

#include "xfce_style.h"
#include "xfce_rc_style.h"
#include "ge-support.h"
#include "gradient_draw.h"

#define DETAIL(s)   ((detail) && (!strcmp(s, detail)))

static void xfce_style_init(XfceStyle * style);
static void xfce_style_class_init(XfceStyleClass * klass);
static GtkStyleClass *parent_class = NULL;

extern GtkStyleClass xfce_default_class;

#define CHECK_MIN_SIZE 15
#define CHECK_DRAW_SIZE 11

/* internal functions */
static void xfce_fill_background(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height);
static void xfce_draw_grips(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gint x, gint y, gint width, gint height, GtkOrientation orientation);

static void draw_hline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x1, gint x2, gint y);
static void draw_vline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint y_1, gint y_2, gint x);
static void draw_shadow(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height);

static void draw_box(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height);
static void draw_check(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height);
static void draw_option(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height);
static void draw_shadow_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width);
static void draw_box_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width);
static void draw_extension(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side);
static void draw_slider(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation);
static void draw_handle(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation);

static void xfce_fill_background(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    GradientType gradient_style = GRADIENT_VERTICAL;
    gfloat shade_start = 1.0, shade_end = 1.0;
    gboolean draw_base = FALSE;
    cairo_t *cr;

    /* Spin buttons are a special case */
    if (widget && GTK_IS_SPIN_BUTTON (widget))
    {
        /* The entry part needs to be completed on the background */
        if (DETAIL("spinbutton"))
        {
            draw_base = TRUE;
            /* The entry sets the background to the base state color
             * This function is called with the state conditionally set according to state-hint */
            state_type = gtk_widget_get_state(widget);
        }
        else if (DETAIL("spinbutton_up") || DETAIL("spinbutton_down"))
        {
            /* Only draw the spinner buttons if you interact with them */
            if ((state_type != GTK_STATE_PRELIGHT) && (state_type != GTK_STATE_ACTIVE))
                return;
        }
    }

    cr = ge_gdk_drawable_to_cairo(window, area);

    if (DETAIL("trough-lower"))
    {
        state_type = GTK_STATE_SELECTED;
    }

    if ((!draw_base) && (XFCE_RC_STYLE(style->rc_style)->gradient))
    {
        switch (XFCE_RC_STYLE(style->rc_style)->gradient_style)
        {
            case XFCE_RC_GRADIENT_HORIZONTAL:
                gradient_style = GRADIENT_HORIZONTAL;
                break;
            case XFCE_RC_GRADIENT_VERTICAL:
                gradient_style = GRADIENT_VERTICAL;
                break;
            case XFCE_RC_GRADIENT_NORTHERN_DIAGONAL:
                gradient_style = GRADIENT_NORTHERN_DIAGONAL;
                break;
            case XFCE_RC_GRADIENT_SOUTHERN_DIAGONAL:
                gradient_style = GRADIENT_SOUTHERN_DIAGONAL;
                break;
            case XFCE_RC_GRADIENT_AUTO:
            default:
                if (DETAIL("hscrollbar"))
                {
                    gradient_style = GRADIENT_VERTICAL;
                }
                else if (DETAIL("vscrollbar"))
                {
                    gradient_style = GRADIENT_HORIZONTAL;
                }
                else if (height > width)
                {
                    gradient_style = GRADIENT_HORIZONTAL;
                }
                else
                {
                    gradient_style = GRADIENT_VERTICAL;
                }
                break;
        }
        if ((state_type == GTK_STATE_ACTIVE) || (DETAIL("trough")) || (DETAIL("trough-lower")) || (DETAIL("trough-upper")))
        {
            shade_start = MIN (XFCE_RC_STYLE(style->rc_style)->shade_start, XFCE_RC_STYLE(style->rc_style)->shade_end);
            shade_end = MAX (XFCE_RC_STYLE(style->rc_style)->shade_start, XFCE_RC_STYLE(style->rc_style)->shade_end);
        }
        else
        {
            shade_start = XFCE_RC_STYLE(style->rc_style)->shade_start;
            shade_end = XFCE_RC_STYLE(style->rc_style)->shade_end;
        }

        gradient_draw_shaded(cr, x, y, width, height, &style->bg[state_type], shade_start, shade_end, gradient_style);
    }
    else
    {
        gdk_cairo_set_source_color(cr, draw_base ? &style->base[state_type] : &style->bg[state_type]); 
        cairo_rectangle(cr, x, y, width, height);
        cairo_fill(cr);
    }

    cairo_destroy(cr);
}

static void xfce_draw_grip_rough (GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
    gint xx, yy;
    gint xthick, ythick;
    cairo_t *cr;
    GdkColor *light, *dark;

    xthick = style->xthickness;
    ythick = style->ythickness;

    light = &style->light[state_type];
    dark = &style->dark[state_type];

    cr = ge_gdk_drawable_to_cairo(window, area);

    if (orientation == GTK_ORIENTATION_HORIZONTAL)
    {
        if (width > 15 + xthick)
        {
            gint len = (height - 2 * (ythick + 2) > 4 ? height - 2 * (ythick + 2) :  height - 2 * ythick);
            gint delta = x + (width / 2) - 5;
            yy = y + (height - len) / 2;
            for(xx = 0; xx < 10; xx += 2)
            {
                gdk_cairo_set_source_color(cr, dark);   
                cairo_move_to(cr, xx + delta + 0.5, yy + 0.5);
                cairo_line_to(cr, xx + delta + 0.5, yy + len - 0.5);
                cairo_stroke(cr);
                gdk_cairo_set_source_color(cr, light);  
                cairo_move_to(cr, xx + delta + 1.5, yy + 0.5);
                cairo_line_to(cr, xx + delta + 1.5, yy + len - 0.5);
                cairo_stroke(cr);
            }
        }
    }
    else
    {
        if (height > 15 + ythick)
        {
            gint len = (width - 2 * (xthick + 2) > 4 ? width - 2 * (xthick + 2) :  width - 2 * xthick);
            gint delta = y + (height / 2) - 5;
            xx = x + (width - len) / 2;
            for(yy = 0; yy < 10; yy += 2)
            {
                gdk_cairo_set_source_color(cr, dark);   
                cairo_move_to(cr, xx + 0.5, yy + delta + 0.5);
                cairo_line_to(cr, xx + len - 0.5, yy + delta + 0.5);
                cairo_stroke(cr);
                gdk_cairo_set_source_color(cr, light);  
                cairo_move_to(cr, xx + 0.5, yy + delta + 1.5);
                cairo_line_to(cr, xx + len - 0.5, yy + delta + 1.5);
                cairo_stroke(cr);
            }
        }
    }

    cairo_destroy(cr);
}


static void xfce_draw_grip_slide (GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
    gint gx, gy, gwidth, gheight;
    cairo_t *cr;
    GdkColor *light, *dark, *mid, *bg;

    light = &style->light[state_type];
    dark = &style->dark[state_type];
    mid = &style->mid[state_type];
    bg = &style->base[GTK_STATE_SELECTED];

    gx = gy = gwidth = gheight = 0;

    if (orientation == GTK_ORIENTATION_HORIZONTAL)
    {
        gint delta = ((gint)((height - 3) / 2));
        gx = x + delta;
        gy = y + delta;
        gwidth = width - 2 * delta - 1;
        gheight = height - 2 * delta - 1;
    }
    else
    {
        gint delta = ((gint)((width - 3) / 2));
        gx = x + delta;
        gy = y + delta;
        gwidth = width - 2 * delta - 1;
        gheight = height - 2 * delta - 1;
    }

    cr = ge_gdk_drawable_to_cairo(window, area);

    if ((gheight > 1) && (gwidth > 1))
    {
        gdk_cairo_set_source_color(cr, bg);     
        cairo_rectangle(cr, gx + 1, gy + 1, gwidth - 1, gheight - 1);
        cairo_fill(cr);

        gdk_cairo_set_source_color(cr, dark);   
        cairo_move_to(cr, gx + 0.5, gy + gheight + 0.5);
        cairo_line_to(cr, gx + 0.5, gy + 0.5);
        cairo_line_to(cr, gx + gwidth + 0.5, gy + 0.5);
        cairo_stroke(cr);

        gdk_cairo_set_source_color(cr, light);  
        cairo_move_to(cr, gx + 0.5, gy + gheight + 0.5);
        cairo_line_to(cr, gx + gwidth + 0.5, gy + gheight + 0.5);
        cairo_line_to(cr, gx + gwidth + 0.5, gy + 0.5);
        cairo_stroke(cr);

        gdk_cairo_set_source_color(cr, mid);    
        cairo_rectangle(cr, gx, gy, 1, 1);
        cairo_rectangle(cr, gx + gwidth, gy, 1, 1);
        cairo_rectangle(cr, gx, gy + gheight, 1, 1);
        cairo_rectangle(cr, gx + gwidth, gy + gheight, 1, 1);
        cairo_fill(cr);
    }

    cairo_destroy(cr);
}

static void xfce_draw_grips(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
    XfceRcStyle *rc_style;

    rc_style = XFCE_RC_STYLE(style->rc_style);

    if (rc_style)
    {
        if (rc_style->grip_style == XFCE_RC_GRIP_ROUGH)
        {
            xfce_draw_grip_rough (style, window, state_type, area, widget, x, y, width, height, orientation);
        }
        else if (rc_style->grip_style == XFCE_RC_GRIP_SLIDE)
        {
            xfce_draw_grip_slide (style, window, state_type, area, widget, x, y, width, height, orientation);
        }
    }
}

static void draw_hline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x1, gint x2, gint y)
{
    gint thickness_light;
    gint thickness_dark;
    cairo_t *cr;

    CHECK_ARGS;

    thickness_light = style->ythickness / 2;
    thickness_dark = style->ythickness - thickness_light;

    cr = ge_gdk_drawable_to_cairo(window, area);

    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);

    x2 += 1;

    cairo_set_line_width (cr, thickness_dark);
    gdk_cairo_set_source_color(cr, &style->dark[state_type]);
    cairo_move_to(cr, x1, y + (thickness_dark / 2.0));
    cairo_line_to(cr, x2, y + (thickness_dark / 2.0));
    cairo_stroke(cr);

    y += thickness_dark;

    cairo_set_line_width (cr, thickness_light);
    gdk_cairo_set_source_color(cr, &style->light[state_type]);
    cairo_move_to(cr, x1, y + (thickness_light / 2.0));
    cairo_line_to(cr, x2, y + (thickness_light / 2.0));
    cairo_stroke(cr);

    cairo_destroy(cr);
}

static void draw_vline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint y_1, gint y_2, gint x)
{
    gint thickness_light;
    gint thickness_dark;
    cairo_t *cr;

    CHECK_ARGS;

    thickness_light = style->xthickness / 2;
    thickness_dark = style->xthickness - thickness_light;

    cr = ge_gdk_drawable_to_cairo(window, area);

    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);

    y_2 += 1;

    cairo_set_line_width (cr, thickness_dark);
    gdk_cairo_set_source_color(cr, &style->dark[state_type]);
    cairo_move_to(cr, x + (thickness_dark / 2.0), y_1);
    cairo_line_to(cr, x + (thickness_dark / 2.0), y_2);
    cairo_stroke(cr);

    x += thickness_dark;

    cairo_set_line_width (cr, thickness_light);
    gdk_cairo_set_source_color(cr, &style->light[state_type]);
    cairo_move_to(cr, x + (thickness_light / 2.0), y_1);
    cairo_line_to(cr, x + (thickness_light / 2.0), y_2);
    cairo_stroke(cr);

    cairo_destroy(cr);
}

static void draw_shadow(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    gint xt, yt;
    cairo_t *cr;
    GtkWidget *parent;
    GtkStyle *pstyle;
    GdkColor *pbg;

    CHECK_ARGS;

    if (shadow_type == GTK_SHADOW_NONE)
        return;

    SANITIZE_SIZE;

    parent = gtk_widget_get_parent(widget);
    if (parent)
    {
        /* Get the background color from the parent widget.
         * This makes a nicer corner if the background colors differ.
         */
        pstyle = gtk_widget_get_style(parent);
        pbg = &pstyle->bg[gtk_widget_get_state(parent)];
    }
    else
    {
        pbg = &style->mid[state_type];
    }

    /* Spin buttons are a special case */
    if (widget && GTK_IS_SPIN_BUTTON (widget))
    {
        /* The button part of the spin button is a separate window which isn't drawn correctly */
        if (DETAIL("spinbutton"))
        {
            gint line_width = 0;
            gboolean interior_focus = FALSE;
            if (gtk_widget_has_focus(widget))
                gtk_widget_style_get(widget, "focus-line-width", &line_width, "interior-focus", &interior_focus, NULL);
            if (interior_focus)
                line_width = 0;

            if (gtk_widget_get_direction(widget) != GTK_TEXT_DIR_RTL)
            {
                x -= widget->allocation.width - width;
            }
            width = widget->allocation.width;
            if (line_width)
                gtk_paint_focus(style, window, state_type, area, widget, detail, x, y, width, height);
            x += line_width;
            width -= 2 * line_width;
            y += line_width;
            height -= 2 * line_width;
        }
    }

    cr = ge_gdk_drawable_to_cairo(window, area);

    xt = MIN(style->xthickness, width - 1);
    yt = MIN(style->ythickness, height - 1);

    if (XFCE_RC_STYLE(style->rc_style)->flat_border)
    {
        if ((DETAIL("spinbutton_up") || DETAIL("spinbutton_down")) && (state_type != GTK_STATE_PRELIGHT && state_type != GTK_STATE_ACTIVE))
        {
            /* Do nothing */
        }
        else if (DETAIL("menubar") || DETAIL ("handlebox_bin") || DETAIL ("handlebox") || DETAIL ("toolbar"))
        {
            if (yt > 0)
            {
                gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                cairo_move_to(cr, x + 0.5, y + height - 0.5);
                cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                cairo_stroke(cr);
            }
        }
        else
        {
            if (((xt > 0) && (yt > 0)) || DETAIL("bar") || DETAIL("trough") || DETAIL("trough-lower") || DETAIL("trough-upper"))
            {
                if (DETAIL("trough-lower"))
                {
                    state_type = GTK_STATE_SELECTED;
                }

                gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
                cairo_stroke(cr);

                if (XFCE_RC_STYLE(style->rc_style)->smooth_edge)
                {
                    if (DETAIL("menu"))
                    {
                        /* Sharp edges */
                    }
                    else
                    {
                        if (DETAIL("bar"))
                        {
                            state_type = GTK_STATE_NORMAL;
                        }

                        gdk_cairo_set_source_color(cr, pbg);
                        cairo_rectangle(cr, x, y, 1, 1);
                        cairo_rectangle(cr, x + width - 1, y, 1, 1);
                        cairo_rectangle(cr, x, y + height - 1, 1, 1);
                        cairo_rectangle(cr, x + width - 1, y + height - 1, 1, 1);
                        cairo_fill(cr);
                    }
                }
            }
        }
    }
    else
    {
        switch (shadow_type)
        {
            case GTK_SHADOW_NONE:
                break;
            case GTK_SHADOW_ETCHED_IN:
                if ((xt > 1) && (yt > 1))
                {
                    gdk_cairo_set_source_color(cr, &style->light[state_type]);
                    cairo_move_to(cr, x + 0.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                    cairo_move_to(cr, x + width - 1.5, y + 0.5);
                    cairo_line_to(cr, x + 0.5, y + 0.5);
                    cairo_line_to(cr, x + 0.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, &style->light[state_type]);
                    cairo_move_to(cr, x + width - 1.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                    cairo_move_to(cr, x + 1.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + 1.5);
                    cairo_stroke(cr);
                }
                else if ((xt > 0) && (yt > 0))
                {
                    gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                    cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
                    cairo_stroke(cr);
                }
                break;
            case GTK_SHADOW_ETCHED_OUT:
                if ((xt > 1) && (yt > 1))
                {
                    gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                    cairo_move_to(cr, x + 0.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, &style->light[state_type]);
                    cairo_move_to(cr, x + width - 1.5, y + 0.5);
                    cairo_line_to(cr, x + 0.5, y + 0.5);
                    cairo_line_to(cr, x + 0.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                    cairo_move_to(cr, x + width - 1.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, &style->light[state_type]);
                    cairo_move_to(cr, x + 1.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + 1.5);
                    cairo_stroke(cr);
                }
                else if ((xt > 0) && (yt > 0))
                {
                    gdk_cairo_set_source_color(cr, &style->light[state_type]);
                    cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
                    cairo_stroke(cr);
                }
                break;
            case GTK_SHADOW_IN:
                if (XFCE_RC_STYLE(style->rc_style)->smooth_edge)
                {
                    if (DETAIL("trough") || DETAIL("trough-lower") || DETAIL("trough-upper"))
                    {
                        gdk_cairo_set_source_color(cr, &style->bg[DETAIL("trough-lower") ? GTK_STATE_SELECTED : GTK_STATE_NORMAL]);
                        cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, pbg);
                        cairo_rectangle(cr, x, y, 1, 1);
                        cairo_rectangle(cr, x + width - 1, y, 1, 1);
                        cairo_rectangle(cr, x, y + height - 1, 1, 1);
                        cairo_rectangle(cr, x + width - 1, y + height - 1, 1, 1);
                        cairo_fill(cr);
                    }
                    else if ((xt > 1) && (yt > 1))
                    {
                        gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                        cairo_move_to(cr, x + 1.5, y + 0.5);
                        cairo_line_to(cr, x + width - 1.5, y + 0.5);
                        cairo_move_to(cr, x + 0.5, y + 1.5);
                        cairo_line_to(cr, x + 0.5, y + height - 1.5);

                        cairo_move_to(cr, x + 1.5, y + height - 0.5);
                        cairo_line_to(cr, x + width - 1.5, y + height - 0.5);
                        cairo_move_to(cr, x + width - 0.5, y + 1.5);
                        cairo_line_to(cr, x + width - 0.5, y + height - 1.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, &style->mid[state_type]);
                        cairo_move_to(cr, x + width - 1.5, y + 1.5);
                        cairo_line_to(cr, x + 1.5, y + 1.5);
                        cairo_line_to(cr, x + 1.5, y + height - 1.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, &style->light[state_type]);
                        cairo_move_to(cr, x + 2.5, y + height - 1.5);
                        cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                        cairo_line_to(cr, x + width - 1.5, y + 2.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, pbg);
                        cairo_rectangle(cr, x, y, 1, 1);
                        cairo_rectangle(cr, x + width - 1, y, 1, 1);
                        cairo_rectangle(cr, x, y + height - 1, 1, 1);
                        cairo_rectangle(cr, x + width - 1, y + height - 1, 1, 1);
                        cairo_fill(cr);
                    }
                    else if ((xt > 0) && (yt > 0))
                    {
                        gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                        cairo_move_to(cr, x + 1.5, y + 0.5);
                        cairo_line_to(cr, x + width - 0.5, y + 0.5);
                        cairo_move_to(cr, x + 0.5, y + 1.5);
                        cairo_line_to(cr, x + 0.5, y + height - 0.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, &style->light[state_type]);
                        cairo_move_to(cr, x + 1.5, y + height - 0.5);
                        cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                        cairo_line_to(cr, x + width - 0.5, y + 1.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, pbg);
                        cairo_rectangle(cr, x, y, 1, 1);
                        cairo_rectangle(cr, x + width - 1, y, 1, 1);
                        cairo_rectangle(cr, x, y + height - 1, 1, 1);
                        cairo_rectangle(cr, x + width - 1, y + height - 1, 1, 1);
                        cairo_fill(cr);
                    }
                }
                else
                {
                    if (DETAIL("trough") || DETAIL("trough-lower") || DETAIL("trough-upper"))
                    {
                        gdk_cairo_set_source_color(cr, &style->dark[DETAIL("trough-lower") ? GTK_STATE_SELECTED : GTK_STATE_ACTIVE]);
                        cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
                        cairo_stroke(cr);
                    }
                    else if ((xt > 1) && (yt > 1))
                    {
                        gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                        cairo_move_to(cr, x + width - 0.5, y + 0.5);
                        cairo_line_to(cr, x + 0.5, y + 0.5);
                        cairo_line_to(cr, x + 0.5, y + height - 0.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, &style->light[state_type]);
                        cairo_move_to(cr, x + 1.5, y + height - 0.5);
                        cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                        cairo_line_to(cr, x + width - 0.5, y + 1.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, &style->black);
                        cairo_move_to(cr, x + width - 1.5, y + 1.5);
                        cairo_line_to(cr, x + 1.5, y + 1.5);
                        cairo_line_to(cr, x + 1.5, y + height - 1.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                        cairo_move_to(cr, x + 2.5, y + height - 1.5);
                        cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                        cairo_line_to(cr, x + width - 1.5, y + 2.5);
                        cairo_stroke(cr);
                    }
                    else if ((xt > 0) && (yt > 0))
                    {
                        gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                        cairo_move_to(cr, x + width - 0.5, y + 0.5);
                        cairo_line_to(cr, x + 0.5, y + 0.5);
                        cairo_line_to(cr, x + 0.5, y + height - 0.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, &style->light[state_type]);
                        cairo_move_to(cr, x + 1.5, y + height - 0.5);
                        cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                        cairo_line_to(cr, x + width - 0.5, y + 1.5);
                        cairo_stroke(cr);
                    }
                }
                break;
            case GTK_SHADOW_OUT:
                if (XFCE_RC_STYLE(style->rc_style)->smooth_edge)
                {
                    if ((DETAIL("spinbutton_up") || DETAIL("spinbutton_down")) && (state_type != GTK_STATE_PRELIGHT))
                    {
                        /* Do nothing */
                    }
                    else if (DETAIL("bar"))
                    {
                        gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                        cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, pbg);
                        cairo_rectangle(cr, x, y, 1, 1);
                        cairo_rectangle(cr, x + width - 1, y, 1, 1);
                        cairo_rectangle(cr, x, y + height - 1, 1, 1);
                        cairo_rectangle(cr, x + width - 1, y + height - 1, 1, 1);
                        cairo_fill(cr);
                    }
                    else if (DETAIL("menu"))
                    {
                        if ((xt > 0) && (yt > 0))
                        {
                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
                            cairo_stroke(cr);
                        }
                    }
                    else if (DETAIL("menubar"))
                    {
                        if ((xt > 1) && (yt > 1))
                        {
                            gdk_cairo_set_source_color(cr, &style->mid[state_type]);
                            cairo_move_to(cr, x + 0.5, y + height - 1.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 1.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_stroke(cr);
                        }
                        else if ((xt > 0) && (yt > 0))
                        {
                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_stroke(cr);
                        }
                    }
                    else if (DETAIL ("handlebox_bin") || DETAIL ("handlebox") || DETAIL ("toolbar"))
                    {
                        if ((xt > 1) && (yt > 1))
                        {
                            gdk_cairo_set_source_color(cr, &style->light[state_type]);
                            cairo_move_to(cr, x + 0.5, y + 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + 0.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->mid[state_type]);
                            cairo_move_to(cr, x + 0.5 , y + height - 1.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 1.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_stroke(cr);
                        }
                        else if ((xt > 0) && (yt > 0))
                        {
                            gdk_cairo_set_source_color(cr, &style->light[state_type]);
                            cairo_move_to(cr, x + 0.5, y + 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + 0.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_stroke(cr);
                        }
                    }
                    else if (DETAIL("vscrollbar") || DETAIL("hscrollbar") || DETAIL("slider") || DETAIL("vscale") || DETAIL("hscale"))
                    {
                        if ((xt > 1) && (yt > 1))
                        {
                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + width - 0.5, y + 0.5);
                            cairo_line_to(cr, x + 0.5, y + 0.5);
                            cairo_line_to(cr, x + 0.5, y + height - 0.5);

                            cairo_move_to(cr, x + 1.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + 1.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->light[state_type]);
                            cairo_move_to(cr, x + width - 2.5, y + 1.5);
                            cairo_line_to(cr, x + 1.5, y + 1.5);
                            cairo_line_to(cr, x + 1.5, y + height - 2.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->mid[state_type]);
                            cairo_move_to(cr, x + 1.5, y + height - 1.5);
                            cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                            cairo_line_to(cr, x + width - 1.5, y + 1.5);
                            cairo_stroke(cr);
                        }
                        else if ((xt > 0) && (yt > 0))
                        {
                            gdk_cairo_set_source_color(cr, &style->light[state_type]);
                            cairo_move_to(cr, x + width - 0.5, y + 0.5);
                            cairo_line_to(cr, x + 0.5, y + 0.5);
                            cairo_line_to(cr, x + 0.5, y + height - 0.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 1.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + 1.5);
                            cairo_stroke(cr);
                        }
                    }
                    else if (DETAIL("frame"))
                    {
                        if ((xt > 1) && (yt > 1))
                        {
                            gdk_cairo_set_source_color(cr, &style->light[state_type]);
                            cairo_move_to(cr, x + width - 1.5, y + 0.5);
                            cairo_line_to(cr, x + 0.5, y + 0.5);
                            cairo_line_to(cr, x + 0.5, y + height - 1.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + 0.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->bg[state_type]);
                            cairo_move_to(cr, x + width - 1.5, y + 1.5);
                            cairo_line_to(cr, x + 1.5, y + 1.5);
                            cairo_line_to(cr, x + 1.5, y + height - 1.5);

                            cairo_move_to(cr, x + 2.5, y + height - 1.5);
                            cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                            cairo_line_to(cr, x + width - 1.5, y + 2.5);
                            cairo_stroke(cr);
                        }
                        else if ((xt > 0) && (yt > 0))
                        {
                            gdk_cairo_set_source_color(cr, &style->light[state_type]);
                            cairo_move_to(cr, x + width - 0.5, y + 0.5);
                            cairo_line_to(cr, x + 0.5, y + 0.5);
                            cairo_line_to(cr, x + 0.5, y + height - 0.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 1.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + 1.5);
                            cairo_stroke(cr);
                        }
                    }
                    else
                    {
                        if ((xt > 1) && (yt > 1))
                        {
                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 1.5, y + 0.5);
                            cairo_line_to(cr, x + width - 1.5, y + 0.5);
                            cairo_move_to(cr, x + 0.5, y + 1.5);
                            cairo_line_to(cr, x + 0.5, y + height - 1.5);

                            cairo_move_to(cr, x + 1.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 1.5, y + height - 0.5);
                            cairo_move_to(cr, x + width - 0.5, y + 1.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 1.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->light[state_type]);
                            cairo_move_to(cr, x + width - 2.5, y + 1.5);
                            cairo_line_to(cr, x + 1.5, y + 1.5);
                            cairo_line_to(cr, x + 1.5, y + height - 2.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->mid[state_type]);
                            cairo_move_to(cr, x + 1.5, y + height - 1.5);
                            cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                            cairo_line_to(cr, x + width - 1.5, y + 1.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, pbg);
                            cairo_rectangle(cr, x, y, 1, 1);
                            cairo_rectangle(cr, x + width - 1, y, 1, 1);
                            cairo_rectangle(cr, x, y + height - 1, 1, 1);
                            cairo_rectangle(cr, x + width - 1, y + height - 1, 1, 1);
                            cairo_fill(cr);
                        }
                        else if ((xt > 0) && (yt > 0))
                        {
                            gdk_cairo_set_source_color(cr, &style->light[state_type]);
                            cairo_move_to(cr, x + 1.5, y + 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + 0.5);
                            cairo_move_to(cr, x + 0.5, y + 1.5);
                            cairo_line_to(cr, x + 0.5, y + height - 0.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 1.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + 1.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, pbg);
                            cairo_rectangle(cr, x, y, 1, 1);
                            cairo_rectangle(cr, x + width - 1, y, 1, 1);
                            cairo_rectangle(cr, x, y + height - 1, 1, 1);
                            cairo_rectangle(cr, x + width - 1, y + height - 1, 1, 1);
                            cairo_fill(cr);
                        }
                    }
                }
                else
                {
                    if ((DETAIL("spinbutton_up") || DETAIL("spinbutton_down")) && (state_type != GTK_STATE_PRELIGHT))
                    {
                        /* Do nothing */
                    }
                    else if (DETAIL("menubar"))
                    {
                        if ((xt > 1) && (yt > 1))
                        {
                            gdk_cairo_set_source_color(cr, &style->mid[state_type]);
                            cairo_move_to(cr, x + 0.5, y + height - 1.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 1.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_stroke(cr);
                        }
                        else if ((xt > 0) && (yt > 0))
                        {
                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_stroke(cr);
                        }
                    }
                    else if (DETAIL ("handlebox_bin") || DETAIL ("handlebox") || DETAIL ("toolbar"))
                    {
                        if ((xt > 1) && (yt > 1))
                        {
                            gdk_cairo_set_source_color(cr, &style->light[state_type]);
                            cairo_move_to(cr, x + 0.5, y + 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + 0.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->mid[state_type]);
                            cairo_move_to(cr, x + 0.5 , y + height - 1.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 1.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_stroke(cr);
                        }
                        else if ((xt > 0) && (yt > 0))
                        {
                            gdk_cairo_set_source_color(cr, &style->light[state_type]);
                            cairo_move_to(cr, x + 0.5, y + 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + 0.5);
                            cairo_stroke(cr);

                            gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                            cairo_move_to(cr, x + 0.5, y + height - 0.5);
                            cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                            cairo_stroke(cr);
                        }
                    }
                    else if ((xt > 1) && (yt > 1))
                    {
                        gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                        cairo_move_to(cr, x + width - 0.5, y + 0.5);
                        cairo_line_to(cr, x + 0.5, y + 0.5);
                        cairo_line_to(cr, x + 0.5, y + height - 0.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, &style->black);
                        cairo_move_to(cr, x + 1.5, y + height - 0.5);
                        cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                        cairo_line_to(cr, x + width - 0.5, y + 1.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, &style->light[state_type]);
                        cairo_move_to(cr, x + width - 1.5, y + 1.5);
                        cairo_line_to(cr, x + 1.5, y + 1.5);
                        cairo_line_to(cr, x + 1.5, y + height - 1.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                        cairo_move_to(cr, x + 2.5, y + height - 1.5);
                        cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                        cairo_line_to(cr, x + width - 1.5, y + 2.5);
                        cairo_stroke(cr);
                    }
                    else if ((xt > 0) && (yt > 0))
                    {
                        gdk_cairo_set_source_color(cr, &style->light[state_type]);
                        cairo_move_to(cr, x + width - 0.5, y + 0.5);
                        cairo_line_to(cr, x + 0.5, y + 0.5);
                        cairo_line_to(cr, x + 0.5, y + height - 0.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, &style->dark[state_type]);
                        cairo_move_to(cr, x + 1.5, y + height - 0.5);
                        cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                        cairo_line_to(cr, x + width - 0.5, y + 1.5);
                        cairo_stroke(cr);
                    }
                }
                break;
        }
    }

    cairo_destroy(cr);
}

static void draw_box(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    CHECK_ARGS;
    SANITIZE_SIZE;

    if (!style->bg_pixmap[state_type])
    {
        xfce_fill_background(style, window, state_type, area, widget, detail, x, y, width, height);
    }
    else
    {
        gtk_style_apply_default_background(style, window, 1, state_type, area, x, y, width, height);
    }
    draw_shadow(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
}

static void draw_dash(cairo_t * cr, const GdkColor * c, gdouble x, gdouble y, guint size)
{
    guint w, b;

    b = (size + 7) / 10;

    w = size / 4;
    if ((w % 2) != (size % 2))
    {
        w += 1;
    }

    gdk_cairo_set_source_color(cr, c);

    cairo_set_line_width (cr, w);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);

    cairo_move_to (cr, x + b, y + size / 2.0);
    cairo_line_to (cr, x + size - b, y + size / 2.0);

    cairo_stroke(cr);
}

static void draw_check(GtkStyle * style, GdkWindow * window, GtkStateType state, GtkShadowType shadow, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    cairo_t *cr;
    guint size;
    guint w, b;
    GdkColor * c;

    CHECK_ARGS;
    SANITIZE_SIZE;

    /* Make sure it doesn't get to small */
    if (width < CHECK_MIN_SIZE)
        width = CHECK_DRAW_SIZE;
    else
    {
        width -= CHECK_MIN_SIZE - CHECK_DRAW_SIZE;
        x += (CHECK_MIN_SIZE - CHECK_DRAW_SIZE) / 2;
    }
    if (height < CHECK_MIN_SIZE)
        height = CHECK_DRAW_SIZE;
    else
    {
        height -= CHECK_MIN_SIZE - CHECK_DRAW_SIZE;
        y += (CHECK_MIN_SIZE - CHECK_DRAW_SIZE) / 2;
    }

    /* Make it square */
    if (width > height)
    {
        x += width - height;
        size = height;
    }
    else
    {
        y += height - width;
        size = width;
    }

    cr = ge_gdk_drawable_to_cairo(window, area);

    cairo_set_antialias (cr, CAIRO_ANTIALIAS_NONE);

    cairo_rectangle (cr, x + 0.5, y + 0.5, size - 1, size - 1);

    if (!DETAIL("check"))    /* not Menu item */
    {
        /* Draw the background */
        gdk_cairo_set_source_color(cr, &style->base[state]);
        cairo_fill_preserve(cr);
        c = &style->text[state];
    }
    else
    {
        c = &style->fg[state];
    }

    /* Draw the border */
    gdk_cairo_set_source_color(cr, &style->dark[state]);
    cairo_stroke(cr);

    x += 1;
    y += 1;
    size -= 2;

    if (shadow == GTK_SHADOW_IN)
    {
        b = (size + 7) / 10;
        w = ((size + 4 - b) / 6);

        /* Draw the check */
        gdk_cairo_set_source_color(cr, c);

        cairo_move_to (cr, x + b, y + floor(size / 2 - 1.5));

        cairo_line_to (cr, x + b, y + size - b);
        cairo_line_to (cr, x + b + w, y + size - b);

        cairo_line_to (cr, x + size - b, y + b + w);
        cairo_line_to (cr, x + size - b, y + b);
        cairo_line_to (cr, x + size - b + 1 - w, y + b);

        cairo_line_to (cr, x + b + w, y + size - b + 1 - 2 * w);

        cairo_line_to (cr, x + b + w, y + floor(size / 2 - 1.5));

        cairo_close_path (cr);
        cairo_fill(cr);
    }
    else if (shadow == GTK_SHADOW_ETCHED_IN)
    {
        draw_dash(cr, c, x, y, size);
    }

    cairo_destroy(cr);
}

static void draw_option(GtkStyle * style, GdkWindow * window, GtkStateType state, GtkShadowType shadow, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    cairo_t *cr;
    guint size;
    GdkColor * c;

    CHECK_ARGS;
    SANITIZE_SIZE;

    /* Make sure it doesn't get to small */
    if (width < CHECK_MIN_SIZE)
        width = CHECK_DRAW_SIZE;
    else
    {
        width -= CHECK_MIN_SIZE - CHECK_DRAW_SIZE;
        x += (CHECK_MIN_SIZE - CHECK_DRAW_SIZE) / 2;
    }
    if (height < CHECK_MIN_SIZE)
        height = CHECK_DRAW_SIZE;
    else
    {
        height -= CHECK_MIN_SIZE - CHECK_DRAW_SIZE;
        y += (CHECK_MIN_SIZE - CHECK_DRAW_SIZE) / 2;
    }

    /* Make it square */
    if (width > height)
    {
        x += width - height;
        size = height;
    }
    else
    {
        y += height - width;
        size = width;
    }

    cr = ge_gdk_drawable_to_cairo(window, area);

    cairo_set_antialias (cr, CAIRO_ANTIALIAS_NONE);

    cairo_arc (cr, x + (size / 2.0), y + (size / 2.0), (size - 1) / 2.0, 0, 2 * M_PI);

    if (!DETAIL("option"))   /* not Menu item */
    {
        /* Draw the background */
        gdk_cairo_set_source_color(cr, &style->base[state]);
        cairo_fill_preserve(cr);
        c = &style->text[state];
    }
    else
    {
        c = &style->fg[state];
    }

    /* Draw the border */
    gdk_cairo_set_source_color(cr, &style->dark[state]);
    cairo_stroke(cr);

    x += 1;
    y += 1;
    size -= 2;

    if (shadow == GTK_SHADOW_IN)
    {
        /* Draw the dot */
        gdk_cairo_set_source_color(cr, c);

        cairo_arc (cr, x + (size / 2.0), y + (size / 2.0), (size / 2.0) - ((size + 2) / 5), 0, 2 * M_PI);
        cairo_fill(cr);
    }
    else if (shadow == GTK_SHADOW_ETCHED_IN)
    {
        draw_dash(cr, c, x, y, size);
    }

    cairo_destroy(cr);
}

static void draw_shadow_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width)
{
    GdkRectangle rect;

    CHECK_ARGS;

    if (shadow_type == GTK_SHADOW_NONE)
        return;

    SANITIZE_SIZE;

    draw_shadow(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);

    switch (gap_side)
    {
        case GTK_POS_TOP:
            rect.x = x + gap_x;
            rect.y = y;
            rect.width = gap_width;
            rect.height = 2;
            break;
        case GTK_POS_BOTTOM:
            rect.x = x + gap_x;
            rect.y = y + height - 2;
            rect.width = gap_width;
            rect.height = 2;
            break;
        case GTK_POS_LEFT:
            rect.x = x;
            rect.y = y + gap_x;
            rect.width = 2;
            rect.height = gap_width;
            break;
        default: /* GTK_POS_RIGHT */
            rect.x = x + width - 2;
            rect.y = y + gap_x;
            rect.width = 2;
            rect.height = gap_width;
            break;
    }

    gtk_style_apply_default_background(style, window, 1, state_type, area, rect.x, rect.y, rect.width, rect.height);
}

static void draw_box_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width)
{
    GdkColor *c1;
    GdkColor *c2;
    GdkColor *c3;
    GdkColor *c4;
    cairo_t *cr;

    CHECK_ARGS;
    SANITIZE_SIZE;

    gtk_style_apply_default_background(style, window, widget && !GTK_WIDGET_NO_WINDOW(widget), state_type, area, x, y, width, height);

    if (XFCE_RC_STYLE(style->rc_style)->smooth_edge)
    {
        c1 = &style->dark[state_type];
        c2 = &style->bg[state_type];
        c3 = &style->bg[state_type];
        c4 = &style->dark[state_type];
    }
    else
    {
        c1 = &style->dark[state_type];
        c2 = &style->light[state_type];
        c3 = &style->dark[state_type];
        c4 = &style->black;
    }

    cr = ge_gdk_drawable_to_cairo(window, area);

    switch (shadow_type)
    {
        case GTK_SHADOW_NONE:
        case GTK_SHADOW_IN:
        case GTK_SHADOW_OUT:
        case GTK_SHADOW_ETCHED_IN:
        case GTK_SHADOW_ETCHED_OUT:
            switch (gap_side)
            {
                case GTK_POS_TOP:
                    gdk_cairo_set_source_color(cr, c1);
                    cairo_move_to(cr, x + 0.5, y + 0.5);
                    cairo_line_to(cr, x + 0.5, y + height - 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c2);
                    cairo_move_to(cr, x + 1.5, y + 0.5);
                    cairo_line_to(cr, x + 1.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c3);
                    cairo_move_to(cr, x + 2.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c4);
                    cairo_move_to(cr, x + 1.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + 1.5);
                    cairo_stroke(cr);

                    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
                    if (gap_x > 0)
                    {
                        gdk_cairo_set_source_color(cr, c1);
                        cairo_move_to(cr, x, y + 0.5);
                        cairo_line_to(cr, x + gap_x, y + 0.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, c2);
                        cairo_move_to(cr, x + 1, y + 1.5);
                        cairo_line_to(cr, x + gap_x, y + 1.5);
                        cairo_stroke(cr);

                        cairo_move_to(cr, x + gap_x, y + 0.5);
                        cairo_line_to(cr, x + gap_x + 1, y + 0.5);
                        cairo_stroke(cr);
                    }
                    if ((width - (gap_x + gap_width)) > 0)
                    {
                        gdk_cairo_set_source_color(cr, c1);
                        cairo_move_to(cr, x + gap_x + gap_width, y + 0.5);
                        cairo_line_to(cr, x + width, y + 0.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, c2);
                        cairo_move_to(cr, x + gap_x + gap_width, y + 1.5);
                        cairo_line_to(cr, x + width - 1, y + 1.5);

                        cairo_move_to(cr, x + gap_x + gap_width - 1, y + 0.5);
                        cairo_line_to(cr, x + gap_x + gap_width, y + 0.5);
                        cairo_stroke(cr);
                    }
                    break;
                case GTK_POS_BOTTOM:
                    gdk_cairo_set_source_color(cr, c1);
                    cairo_move_to(cr, x + width - 0.5, y + 0.5);
                    cairo_line_to(cr, x + 0.5, y + 0.5);
                    cairo_line_to(cr, x + 0.5, y + height - 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c2);
                    cairo_move_to(cr, x + width - 1.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + height - 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c3);
                    cairo_move_to(cr, x + width - 1.5, y + 2.5);
                    cairo_line_to(cr, x + width - 1.5, y + height - 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c4);
                    cairo_move_to(cr, x + width - 0.5, y + 1.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                    cairo_stroke(cr);

                    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
                    if (gap_x > 0)
                    {
                        gdk_cairo_set_source_color(cr, c4);
                        cairo_move_to(cr, x + 1, y + height - 0.5);
                        cairo_line_to(cr, x + gap_x, y + height - 0.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, c3);
                        cairo_move_to(cr, x + 2, y + height - 1.5);
                        cairo_line_to(cr, x + gap_x, y + height - 1.5);

                        cairo_move_to(cr, x + gap_x, y + height - 0.5);
                        cairo_line_to(cr, x + gap_x + 1, y + height - 0.5);
                        cairo_stroke(cr);
                    }
                    if ((width - (gap_x + gap_width)) > 0)
                    {
                        gdk_cairo_set_source_color(cr, c4);
                        cairo_move_to(cr, x + gap_x + gap_width, y + height - 0.5);
                        cairo_line_to(cr, x + width - 1, y + height - 0.5);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, c3);
                        cairo_move_to(cr, x + gap_x + gap_width, y + height - 1.5);
                        cairo_line_to(cr, x + width - 1, y + height - 1.5);

                        cairo_move_to(cr, x + gap_x + gap_width - 1, y + height - 0.5);
                        cairo_line_to(cr, x + gap_x + gap_width, y + height - 0.5);
                        cairo_stroke(cr);
                    }
                    break;
                case GTK_POS_LEFT:
                    gdk_cairo_set_source_color(cr, c1);
                    cairo_move_to(cr, x + 0.5, y + 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c2);
                    cairo_move_to(cr, x + 0.5, y + 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c3);
                    cairo_move_to(cr, x + 0.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + 2.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c4);
                    cairo_move_to(cr, x + 1.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + 1.5);
                    cairo_stroke(cr);

                    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
                    if (gap_x > 0)
                    {
                        gdk_cairo_set_source_color(cr, c1);
                        cairo_move_to(cr, x + 0.5, y + 1);
                        cairo_line_to(cr, x + 0.5, y + gap_x);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, c2);
                        cairo_move_to(cr, x + 1.5, y + 2);
                        cairo_line_to(cr, x + 1.5, y + gap_x);

                        cairo_move_to(cr, x + 0.5, y + gap_x);
                        cairo_line_to(cr, x + 0.5, y + gap_x + 1);
                        cairo_stroke(cr);
                    }
                    if ((width - (gap_x + gap_width)) > 0)
                    {
                        gdk_cairo_set_source_color(cr, c1);
                        cairo_move_to(cr, x + 0.5, y + gap_x + gap_width);
                        cairo_line_to(cr, x + 0.5, y + height);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, c2);
                        cairo_move_to(cr, x + 1.5, y + gap_x + gap_width);
                        cairo_line_to(cr, x + 1.5, y + height - 1);

                        cairo_move_to(cr, x + 0.5, y + gap_x + gap_width - 1);
                        cairo_line_to(cr, x + 0.5, y + gap_x + gap_width);
                        cairo_stroke(cr);
                    }
                    break;
                case GTK_POS_RIGHT:
                    gdk_cairo_set_source_color(cr, c1);
                    cairo_move_to(cr, x + width - 0.5, y + 0.5);
                    cairo_line_to(cr, x + 0.5, y + 0.5);
                    cairo_line_to(cr, x + 0.5, y + height - 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c2);
                    cairo_move_to(cr, x + width - 0.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c3);
                    cairo_move_to(cr, x + 2.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c4);
                    cairo_move_to(cr, x + 1.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                    cairo_stroke(cr);

                    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
                    if (gap_x > 0)
                    {
                        gdk_cairo_set_source_color(cr, c4);
                        cairo_move_to(cr, x + width - 0.5, y + 1);
                        cairo_line_to(cr, x + width - 0.5, y + gap_x);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, c3);
                        cairo_move_to(cr, x + width - 1.5, y + 2);
                        cairo_line_to(cr, x + width - 1.5, y + gap_x);

                        cairo_move_to(cr, x + width - 0.5, y + gap_x);
                        cairo_line_to(cr, x + width - 0.5, y + gap_x + 1);
                        cairo_stroke(cr);
                    }
                    if ((width - (gap_x + gap_width)) > 0)
                    {
                        gdk_cairo_set_source_color(cr, c4);
                        cairo_move_to(cr, x + width - 0.5, y + gap_x + gap_width);
                        cairo_line_to(cr, x + width - 0.5, y + height - 1);
                        cairo_stroke(cr);

                        gdk_cairo_set_source_color(cr, c3);
                        cairo_move_to(cr, x + width - 1.5, y + gap_x + gap_width);
                        cairo_line_to(cr, x + width - 1.5, y + height - 1);

                        cairo_move_to(cr, x + width - 0.5, y + gap_x + gap_width - 1);
                        cairo_line_to(cr, x + width - 0.5, y + gap_x + gap_width);
                        cairo_stroke(cr);
                    }
                    break;
            }
    }

    cairo_destroy(cr);
}

static void draw_extension(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side)
{
    GdkColor *c1;
    GdkColor *c2;
    GdkColor *c3;
    GdkColor *c4;
    cairo_t *cr;

    CHECK_ARGS;
    SANITIZE_SIZE;

    if (XFCE_RC_STYLE(style->rc_style)->smooth_edge)
    {
        c1 = &style->dark[state_type];
        c2 = &style->bg[state_type];
        c3 = &style->bg[state_type];
        c4 = &style->dark[state_type];
    }
    else
    {
        c1 = &style->dark[state_type];
        c2 = &style->light[state_type];
        c3 = &style->dark[state_type];
        c4 = &style->black;
    }

    cr = ge_gdk_drawable_to_cairo(window, area);

    switch (shadow_type)
    {
        case GTK_SHADOW_NONE:
        case GTK_SHADOW_IN:
        case GTK_SHADOW_OUT:
        case GTK_SHADOW_ETCHED_IN:
        case GTK_SHADOW_ETCHED_OUT:
            switch (gap_side)
            {
                case GTK_POS_TOP:
                    gtk_style_apply_default_background(style, window, widget && !GTK_WIDGET_NO_WINDOW(widget), state_type, area, x + 2, y, width - 4, height - 2);

                    gdk_cairo_set_source_color(cr, c1);
                    cairo_move_to(cr, x + 0.5, y + 0.5);
                    cairo_line_to(cr, x + 0.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c2);
                    cairo_move_to(cr, x + 1.5, y + 0.5);
                    cairo_line_to(cr, x + 1.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c3);
                    cairo_move_to(cr, x + 2.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c4);
                    cairo_move_to(cr, x + 1.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 1.5, y + height - 0.5);
                    cairo_move_to(cr, x + width - 0.5, y + 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 1.5);
                    cairo_stroke(cr);
                    break;
                case GTK_POS_BOTTOM:
                    gtk_style_apply_default_background(style, window, widget && !GTK_WIDGET_NO_WINDOW(widget), state_type, area, x + 2, y + 2, width - 4, height - 2);

                    gdk_cairo_set_source_color(cr, c1);
                    cairo_move_to(cr, x + 1.5, y + 0.5);
                    cairo_line_to(cr, x + width - 1.5, y + 0.5);
                    cairo_move_to(cr, x + 0.5, y + 1.5);
                    cairo_line_to(cr, x + 0.5, y + height - 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c2);
                    cairo_move_to(cr, x + width - 1.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + height - 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c3);
                    cairo_move_to(cr, x + width - 1.5, y + 2.5);
                    cairo_line_to(cr, x + width - 1.5, y + height - 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c4);
                    cairo_move_to(cr, x + width - 0.5, y + 1.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                    cairo_stroke(cr);
                    break;
                case GTK_POS_LEFT:
                    gtk_style_apply_default_background(style, window, widget && !GTK_WIDGET_NO_WINDOW(widget), state_type, area, x, y + 2, width - 2, height - 4);

                    gdk_cairo_set_source_color(cr, c1);
                    cairo_move_to(cr, x + 0.5, y + 0.5);
                    cairo_line_to(cr, x + width - 1.5, y + 0.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c2);
                    cairo_move_to(cr, x + 0.5, y + 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c3);
                    cairo_move_to(cr, x + 0.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 1.5, y + 2.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c4);
                    cairo_move_to(cr, x + 0.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 1.5, y + height - 0.5);
                    cairo_move_to(cr, x + width - 0.5, y + 1.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 1.5);
                    cairo_stroke(cr);
                    break;
                case GTK_POS_RIGHT:
                    gtk_style_apply_default_background(style, window, widget && !GTK_WIDGET_NO_WINDOW(widget), state_type, area, x + 2, y + 2, width - 2, height - 4);

                    gdk_cairo_set_source_color(cr, c1);
                    cairo_move_to(cr, x + 1.5, y + 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + 0.5);
                    cairo_move_to(cr, x + 0.5, y + 1.5);
                    cairo_line_to(cr, x + 0.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c2);
                    cairo_move_to(cr, x + width - 0.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + 1.5);
                    cairo_line_to(cr, x + 1.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c3);
                    cairo_move_to(cr, x + 2.5, y + height - 1.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 1.5);
                    cairo_stroke(cr);

                    gdk_cairo_set_source_color(cr, c4);
                    cairo_move_to(cr, x + 1.5, y + height - 0.5);
                    cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
                    cairo_stroke(cr);
                    break;
            }
    }

    cairo_destroy(cr);
}

static void draw_slider(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
    CHECK_ARGS;
    SANITIZE_SIZE;

    orientation = GTK_ORIENTATION_HORIZONTAL;
    if (height > width)
        orientation = GTK_ORIENTATION_VERTICAL;

    draw_box(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
    xfce_draw_grips(style, window, state_type, area, widget, x, y, width, height, orientation);
}

static void draw_handle(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
    CHECK_ARGS;
    SANITIZE_SIZE;

    orientation = GTK_ORIENTATION_HORIZONTAL;
    if (height > width)
        orientation = GTK_ORIENTATION_VERTICAL;

#if 0
    draw_box(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
#endif
    xfce_draw_grips(style, window, state_type, area, widget, x, y, width, height, orientation);
}

static void draw_focus(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    cairo_t *cr;
    gboolean free_dash_list = FALSE;
    gint line_width = 1;
    gint8 *dash_list = (gint8 *) "\1\1";

    CHECK_ARGS;
    SANITIZE_SIZE;

    if (widget)
    {
        gtk_widget_style_get (widget,
                "focus-line-width", &line_width,
                "focus-line-pattern", (gchar *)&dash_list,
                NULL);

        free_dash_list = TRUE;
    }

    if (DETAIL("add-mode"))
    {
        if (free_dash_list)
            g_free (dash_list);

        dash_list = (gint8 *) "\4\4";
        free_dash_list = FALSE;
    }

    cr = gdk_cairo_create (window);

    if (DETAIL("colorwheel_light"))
        cairo_set_source_rgb (cr, 0., 0., 0.);
    else if (DETAIL("colorwheel_dark"))
        cairo_set_source_rgb (cr, 1., 1., 1.);
    else if (XFCE_RC_STYLE(style->rc_style)->flags & XFCE_RC_FOCUS_COLOR)
        gdk_cairo_set_source_color (cr, &XFCE_RC_STYLE(style->rc_style)->focus_color);
    else
        gdk_cairo_set_source_color (cr, &style->fg[state_type]);

    cairo_set_line_width (cr, line_width);

    if (dash_list[0])
    {
        gint n_dashes = strlen ((const gchar *) dash_list);
        gdouble *dashes = g_new (gdouble, n_dashes);
        gdouble total_length = 0;
        gdouble dash_offset;
        gint i;

        for (i = 0; i < n_dashes; i++)
        {
            dashes[i] = dash_list[i];
            total_length += dash_list[i];
        }

        if (XFCE_RC_STYLE(style->rc_style)->smooth_edge)
        {
          dash_offset = 0;
        }
        else
        {
            /* The dash offset here aligns the pattern to integer pixels
             * by starting the dash at the right side of the left border
             * Negative dash offsets in cairo don't work
             * (https://bugs.freedesktop.org/show_bug.cgi?id=2729)
             */
            dash_offset = - line_width / 2.;
            while (dash_offset < 0)
                dash_offset += total_length;
        }

        cairo_set_dash (cr, dashes, n_dashes, dash_offset);
        g_free (dashes);
    }

    if (area)
    {
        gdk_cairo_rectangle (cr, area);
        cairo_clip (cr);
    }

    if (XFCE_RC_STYLE(style->rc_style)->smooth_edge)
    {
        cairo_move_to (cr,
                x + 1,
                y + line_width / 2.);
        cairo_line_to (cr,
                x + width - 1,
                y + line_width / 2.);
        cairo_move_to (cr,
                x + width - line_width / 2.,
                y + 1);
        cairo_line_to (cr,
                x + width - line_width / 2.,
                y + height - 1);
        cairo_move_to (cr,
                x + width - 1,
                y + height - line_width / 2.);
        cairo_line_to (cr,
                x + 1,
                y + height - line_width / 2.);
        cairo_move_to (cr,
                x + line_width / 2.,
                y + height - 1);
        cairo_line_to (cr,
                x + line_width / 2.,
                y + 1);
    }
    else
    {
        cairo_rectangle (cr,
                x + line_width / 2.,
                y + line_width / 2.,
                width - line_width,
                height - line_width);
    }
    cairo_stroke (cr);
    cairo_destroy (cr);

    if (free_dash_list)
        g_free (dash_list);
}

GType xfce_type_style = 0;

void xfce_style_register_type(GTypeModule * module)
{
    static const GTypeInfo object_info = {
        sizeof(XfceStyleClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) xfce_style_class_init,
        NULL,                   /* class_finalize */
        NULL,                   /* class_data */
        sizeof(XfceStyle),
        0,                      /* n_preallocs */
        (GInstanceInitFunc) xfce_style_init,
    };

    xfce_type_style = g_type_module_register_type(module, GTK_TYPE_STYLE, "XfceStyle", &object_info, 0);
}

static void xfce_style_init(XfceStyle * style)
{
}

static void xfce_style_class_init(XfceStyleClass * klass)
{
    GtkStyleClass *style_class = GTK_STYLE_CLASS(klass);

    parent_class = g_type_class_peek_parent(klass);
    style_class->draw_hline = draw_hline;
    style_class->draw_vline = draw_vline;
    style_class->draw_shadow = draw_shadow;
    style_class->draw_box = draw_box;
    style_class->draw_check = draw_check;
    style_class->draw_option = draw_option;
    style_class->draw_shadow_gap = draw_shadow_gap;
    style_class->draw_box_gap = draw_box_gap;
    style_class->draw_extension = draw_extension;
    style_class->draw_slider = draw_slider;
    style_class->draw_handle = draw_handle;
    style_class->draw_focus = draw_focus;
}
