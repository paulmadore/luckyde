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
 *                       Tomas �gren <stric@ing.umu.se,
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

#include <gmodule.h>
#include <gtk/gtk.h>
#include "xfce_rc_style.h"
#include "xfce_style.h"

G_MODULE_EXPORT void theme_init(GTypeModule * module);
G_MODULE_EXPORT void theme_exit(void);
G_MODULE_EXPORT GtkRcStyle *theme_create_rc_style(void);
G_MODULE_EXPORT const gchar * g_module_check_init (GModule * module);



G_MODULE_EXPORT void theme_init(GTypeModule * module)
{
    xfce_rc_style_register_type(module);
    xfce_style_register_type(module);
}

G_MODULE_EXPORT void theme_exit(void)
{
}

G_MODULE_EXPORT GtkRcStyle *theme_create_rc_style(void)
{
    void *ptr;
    ptr = GTK_RC_STYLE(g_object_new(XFCE_TYPE_RC_STYLE, NULL));
    return (GtkRcStyle *) ptr;
}

G_MODULE_EXPORT const gchar * g_module_check_init (GModule * module)
{
  return gtk_check_version (GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION - GTK_INTERFACE_AGE);
}
