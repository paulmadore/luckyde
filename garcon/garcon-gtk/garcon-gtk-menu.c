/* vi:set et ai sw=2 sts=2 ts=2: */
/*-
 * Copyright (c) 2013 Nick Schermer <nick@xfce.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libxfce4util/libxfce4util.h>
#include <libxfce4ui/libxfce4ui.h>

#include <garcon-gtk/garcon-gtk-menu.h>

#define STR_IS_EMPTY(str) ((str) == NULL || *(str) == '\0')


/**
 * SECTION: garcon-gtk-menu
 * @title: GarconGtkMenu
 * @short_description: Create a GtkMenu for a GarconMenu.
 * @include: garcon-gtk/garcon-gtk.h
 *
 * Create a complete GtkMenu for the given GarconMenu
 **/



/* Property identifiers */
enum
{
  PROP_0,
  PROP_MENU,
  PROP_SHOW_GENERIC_NAMES,
  PROP_SHOW_MENU_ICONS,
  PROP_SHOW_TOOLTIPS,
  N_PROPERTIES
};



static void                 garcon_gtk_menu_finalize                    (GObject                 *object);
static void                 garcon_gtk_menu_get_property                (GObject                 *object,
                                                                         guint                    prop_id,
                                                                         GValue                  *value,
                                                                         GParamSpec              *pspec);
static void                 garcon_gtk_menu_set_property                (GObject                 *object,
                                                                         guint                    prop_id,
                                                                         const GValue            *value,
                                                                         GParamSpec              *pspec);
static void                 garcon_gtk_menu_show                        (GtkWidget               *widget);
static void                 garcon_gtk_menu_load                        (GarconGtkMenu           *menu);



struct _GarconGtkMenuPrivate
{
  GarconMenu *menu;

  guint is_loaded : 1;

  /* reload idle */
  guint reload_id;

  /* settings */
  guint show_generic_names : 1;
  guint show_menu_icons : 1;
  guint show_tooltips : 1;
};



static const GtkTargetEntry dnd_target_list[] = {
  { "text/uri-list", 0, 0 }
};



static GParamSpec *menu_props[N_PROPERTIES] = { NULL, };



G_DEFINE_TYPE (GarconGtkMenu, garcon_gtk_menu, GTK_TYPE_MENU)



static void
garcon_gtk_menu_class_init (GarconGtkMenuClass *klass)
{
  GObjectClass   *gobject_class;
  GtkWidgetClass *gtkwidget_class;

  g_type_class_add_private (klass, sizeof (GarconGtkMenuPrivate));

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = garcon_gtk_menu_finalize;
  gobject_class->get_property = garcon_gtk_menu_get_property;
  gobject_class->set_property = garcon_gtk_menu_set_property;

  gtkwidget_class = GTK_WIDGET_CLASS (klass);
  gtkwidget_class->show = garcon_gtk_menu_show;

  /**
   * GarconMenu:menu:
   *
   *
   **/
  menu_props[PROP_MENU] =
    g_param_spec_object ("menu",
                         "menu",
                         "menu",
                         GARCON_TYPE_MENU,
                         G_PARAM_READWRITE
                         | G_PARAM_STATIC_STRINGS);

  /**
   * GarconMenu:show-generic-names:
   *
   *
   **/
  menu_props[PROP_SHOW_GENERIC_NAMES] =
    g_param_spec_boolean ("show-generic-names",
                          "show-generic-names",
                          "show-generic-names",
                          FALSE,
                          G_PARAM_READWRITE
                         | G_PARAM_STATIC_STRINGS);

  /**
   * GarconMenu:show-menu-icons:
   *
   *
   **/
  menu_props[PROP_SHOW_MENU_ICONS] =
    g_param_spec_boolean ("show-menu-icons",
                          "show-menu-icons",
                          "show-menu-icons",
                          TRUE,
                          G_PARAM_READWRITE
                          | G_PARAM_STATIC_STRINGS);

  /**
   * GarconMenu:show-tooltips:
   *
   *
   **/
  menu_props[PROP_SHOW_TOOLTIPS] =
    g_param_spec_boolean ("show-tooltips",
                          "show-tooltips",
                          "show-tooltips",
                          FALSE,
                          G_PARAM_READWRITE
                          | G_PARAM_STATIC_STRINGS);

  /* install all properties */
  g_object_class_install_properties (gobject_class, N_PROPERTIES, menu_props);
}



static void
garcon_gtk_menu_init (GarconGtkMenu *menu)
{
  menu->priv = G_TYPE_INSTANCE_GET_PRIVATE (menu, GARCON_GTK_TYPE_MENU, GarconGtkMenuPrivate);

  menu->priv->show_generic_names = FALSE;
  menu->priv->show_menu_icons = TRUE;
  menu->priv->show_tooltips = FALSE;

  gtk_menu_set_reserve_toggle_size (GTK_MENU (menu), FALSE);
}



static void
garcon_gtk_menu_finalize (GObject *object)
{
  GarconGtkMenu *menu = GARCON_GTK_MENU (object);

  /* Stop pending reload */
  if (menu->priv->reload_id != 0)
    g_source_remove (menu->priv->reload_id);

  /* Release menu */
  if (menu->priv->menu != NULL)
    g_object_unref (menu->priv->menu);

  (*G_OBJECT_CLASS (garcon_gtk_menu_parent_class)->finalize) (object);
}



static void
garcon_gtk_menu_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  GarconGtkMenu *menu = GARCON_GTK_MENU (object);

  switch (prop_id)
    {
    case PROP_MENU:
      g_value_set_object (value, menu->priv->menu);
      break;

    case PROP_SHOW_GENERIC_NAMES:
      g_value_set_boolean (value, menu->priv->show_generic_names);
      break;

    case PROP_SHOW_MENU_ICONS:
      g_value_set_boolean (value, menu->priv->show_menu_icons);
      break;

    case PROP_SHOW_TOOLTIPS:
      g_value_set_boolean (value, menu->priv->show_tooltips);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



static void
garcon_gtk_menu_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  GarconGtkMenu *menu = GARCON_GTK_MENU (object);

  switch (prop_id)
    {
    case PROP_MENU:
      garcon_gtk_menu_set_menu (menu, g_value_get_object (value));
      break;

    case PROP_SHOW_GENERIC_NAMES:
      garcon_gtk_menu_set_show_generic_names (menu, g_value_get_boolean (value));
      break;

    case PROP_SHOW_MENU_ICONS:
      garcon_gtk_menu_set_show_menu_icons (menu, g_value_get_boolean (value));
      break;

    case PROP_SHOW_TOOLTIPS:
      garcon_gtk_menu_set_show_tooltips (menu, g_value_get_boolean (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



static void
garcon_gtk_menu_show (GtkWidget *widget)
{
  GarconGtkMenu *menu = GARCON_GTK_MENU (widget);

  /* try to load the menu if needed */
  if (!menu->priv->is_loaded)
    garcon_gtk_menu_load (menu);

  (*GTK_WIDGET_CLASS (garcon_gtk_menu_parent_class)->show) (widget);
}



static void
garcon_gtk_menu_append_quoted (GString     *string,
                               const gchar *unquoted)
{
  gchar *quoted;

  quoted = g_shell_quote (unquoted);
  g_string_append (string, quoted);
  g_free (quoted);
}



static void
garcon_gtk_menu_item_activate (GtkWidget      *mi,
                               GarconMenuItem *item)
{
  GString      *string;
  const gchar  *command;
  const gchar  *p;
  const gchar  *tmp;
  gchar       **argv;
  gboolean      result = FALSE;
  gchar        *uri;
  GError       *error = NULL;

  g_return_if_fail (GTK_IS_WIDGET (mi));
  g_return_if_fail (GARCON_IS_MENU_ITEM (item));

  command = garcon_menu_item_get_command (item);
  if (STR_IS_EMPTY (command))
    return;

  string = g_string_sized_new (100);

  if (garcon_menu_item_requires_terminal (item))
    g_string_append (string, "exo-open --launch TerminalEmulator ");

  /* expand the field codes */
  for (p = command; *p != '\0'; ++p)
    {
      if (G_UNLIKELY (p[0] == '%' && p[1] != '\0'))
        {
          switch (*++p)
            {
            case 'f': case 'F':
            case 'u': case 'U':
              /* TODO for dnd, not a regression, xfdesktop never had this */
              break;

            case 'i':
              tmp = garcon_menu_item_get_icon_name (item);
              if (!STR_IS_EMPTY (tmp))
                {
                  g_string_append (string, "--icon ");
                  garcon_gtk_menu_append_quoted (string, tmp);
                }
              break;

            case 'c':
              tmp = garcon_menu_item_get_name (item);
              if (!STR_IS_EMPTY (tmp))
                garcon_gtk_menu_append_quoted (string, tmp);
              break;

            case 'k':
              uri = garcon_menu_item_get_uri (item);
              if (!STR_IS_EMPTY (uri))
                garcon_gtk_menu_append_quoted (string, uri);
              g_free (uri);
              break;

            case '%':
              g_string_append_c (string, '%');
              break;
            }
        }
      else
        {
          g_string_append_c (string, *p);
        }
    }

  /* parse and spawn command */
  if (g_shell_parse_argv (string->str, NULL, &argv, &error))
    {
      result = xfce_spawn_on_screen (gtk_widget_get_screen (mi),
                                     garcon_menu_item_get_path (item),
                                     argv, NULL, G_SPAWN_SEARCH_PATH,
                                     garcon_menu_item_supports_startup_notification (item),
                                     gtk_get_current_event_time (),
                                     garcon_menu_item_get_icon_name (item),
                                     &error);

      g_strfreev (argv);
    }

  if (G_UNLIKELY (!result))
    {
      xfce_dialog_show_error (NULL, error, _("Failed to execute command \"%s\"."), command);
      g_error_free (error);
    }

  g_string_free (string, TRUE);
}



static void
garcon_gtk_menu_item_drag_begin (GarconMenuItem *item,
                                 GdkDragContext *drag_context)
{
  const gchar *icon_name;

  g_return_if_fail (GARCON_IS_MENU_ITEM (item));

  icon_name = garcon_menu_item_get_icon_name (item);
  if (!STR_IS_EMPTY (icon_name))
    gtk_drag_set_icon_name (drag_context, icon_name, 0, 0);
}



static void
garcon_gtk_menu_item_drag_data_get (GarconMenuItem   *item,
                                    GdkDragContext   *drag_context,
                                    GtkSelectionData *selection_data,
                                    guint             info,
                                    guint             drag_time)
{
  gchar *uris[2] = { NULL, NULL };

  g_return_if_fail (GARCON_IS_MENU_ITEM (item));

  uris[0] = garcon_menu_item_get_uri (item);
  if (G_LIKELY (uris[0] != NULL))
    {
      gtk_selection_data_set_uris (selection_data, uris);
      g_free (uris[0]);
    }
}



static void
garcon_gtk_menu_item_drag_end (GarconGtkMenu *menu)
{
  g_return_if_fail (GTK_IS_MENU (menu));

  /* make sure the menu is not visible */
  gtk_menu_popdown (GTK_MENU (menu));

  /* always emit this signal */
  g_signal_emit_by_name (G_OBJECT (menu), "selection-done", 0);
}



static void
garcon_gtk_menu_deactivate (GtkWidget     *submenu,
                            GarconGtkMenu *menu)
{
  garcon_gtk_menu_item_drag_end (menu);
}



static gboolean
garcon_gtk_menu_reload_idle (gpointer data)
{
  GarconGtkMenu *menu = GARCON_GTK_MENU (data);
  GList         *children;

  /* wait until the menu is hidden */
  if (gtk_widget_get_visible (GTK_WIDGET (menu)))
    return TRUE;

  /* destroy all menu item */
  children = gtk_container_get_children (GTK_CONTAINER (menu));
  g_list_free_full (children, (GDestroyNotify) gtk_widget_destroy);

  /* reload the menu */
  garcon_gtk_menu_load (menu);

  /* reset */
  menu->priv->reload_id = 0;

  return FALSE;
}



static void
garcon_gtk_menu_reload (GarconGtkMenu *menu)
{
  /* schedule a menu reload */
  if (menu->priv->reload_id == 0
      && menu->priv->is_loaded)
    {
      menu->priv->reload_id = g_timeout_add (100, garcon_gtk_menu_reload_idle, menu);
    }
}



static GtkWidget*
garcon_gtk_menu_load_icon (const gchar *icon_name)
{
  GtkWidget *image = NULL;
  gint w, h, size;
  gchar *p, *name = NULL;
  GdkPixbuf *pixbuf = NULL;
  GtkIconTheme *icon_theme = gtk_icon_theme_get_default ();

  gtk_icon_size_lookup (GTK_ICON_SIZE_MENU, &w, &h);
  size = MIN (w, h);

  if (gtk_icon_theme_has_icon (icon_theme, icon_name))
    {
      image = gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_MENU);
    }
  else
    {
      if (g_path_is_absolute (icon_name))
        {
          pixbuf = gdk_pixbuf_new_from_file_at_scale (icon_name, w, h, TRUE, NULL);
        }
      else
        {
          /* try to lookup names like application.png in the theme */
          p = strrchr (icon_name, '.');
          if (p)
            {
              name = g_strndup (icon_name, p - icon_name);
              pixbuf = gtk_icon_theme_load_icon (icon_theme, name, size, 0, NULL);
              g_free (name);
              name = NULL;
            }

          /* maybe they point to a file in the pixbufs folder */
          if (G_UNLIKELY (pixbuf == NULL))
            {
              gchar *filename;

              filename = g_build_filename ("pixmaps", icon_name, NULL);
              name = xfce_resource_lookup (XFCE_RESOURCE_DATA, filename);
              g_free (filename);
            }

          if (name)
            {
              pixbuf = gdk_pixbuf_new_from_file_at_scale (name, w, h, TRUE, NULL);
              g_free (name);
            }
        }

      /* Turn the pixbuf into a gtk_image */
      if (G_LIKELY (pixbuf))
        {
          /* scale the pixbuf down if it needs it */
          GdkPixbuf *tmp = gdk_pixbuf_scale_simple (pixbuf, w, h, GDK_INTERP_BILINEAR);
          g_object_unref (pixbuf);
          pixbuf = tmp;

          image = gtk_image_new_from_pixbuf (pixbuf);
          g_object_unref (G_OBJECT (pixbuf));
        }
    }

  return image;
}



static gboolean
garcon_gtk_menu_add (GarconGtkMenu *menu,
                     GtkMenu       *gtk_menu,
                     GarconMenu    *garcon_menu)
{
  GList               *elements, *li;
  GtkWidget           *mi, *image;
  const gchar         *name, *icon_name;
  const gchar         *comment;
  GtkWidget           *submenu;
  gboolean             has_children = FALSE;
  const gchar         *command;
  GarconMenuDirectory *directory;

  g_return_val_if_fail (GARCON_GTK_IS_MENU (menu), FALSE);
  g_return_val_if_fail (GTK_IS_MENU (gtk_menu), FALSE);
  g_return_val_if_fail (GARCON_IS_MENU (garcon_menu), FALSE);

  elements = garcon_menu_get_elements (garcon_menu);
  for (li = elements; li != NULL; li = li->next)
    {
      g_assert (GARCON_IS_MENU_ELEMENT (li->data));

      if (GARCON_IS_MENU_ITEM (li->data))
        {
          /* watch for changes */
          g_signal_connect_swapped (G_OBJECT (li->data), "changed",
              G_CALLBACK (garcon_gtk_menu_reload), menu);

          /* skip invisible items */
          if (!garcon_menu_element_get_visible (li->data))
            continue;

          /* get element name */
          name = NULL;
          if (menu->priv->show_generic_names)
            name = garcon_menu_item_get_generic_name (li->data);
          if (name == NULL)
            name = garcon_menu_item_get_name (li->data);

          if (G_UNLIKELY (name == NULL))
            continue;

          /* create item */
          mi = gtk_image_menu_item_new_with_label (name);
          gtk_menu_shell_append (GTK_MENU_SHELL (gtk_menu), mi);
          g_signal_connect (G_OBJECT (mi), "activate",
              G_CALLBACK (garcon_gtk_menu_item_activate), li->data);
          gtk_widget_show (mi);

          if (menu->priv->show_tooltips)
            {
              comment = garcon_menu_item_get_comment (li->data);
              if (!STR_IS_EMPTY (comment))
                gtk_widget_set_tooltip_text (mi, comment);
            }

          /* support for dnd item to for example the xfce4-panel */
          gtk_drag_source_set (mi, GDK_BUTTON1_MASK, dnd_target_list,
              G_N_ELEMENTS (dnd_target_list), GDK_ACTION_COPY);
          g_signal_connect_swapped (G_OBJECT (mi), "drag-begin",
              G_CALLBACK (garcon_gtk_menu_item_drag_begin), li->data);
          g_signal_connect_swapped (G_OBJECT (mi), "drag-data-get",
              G_CALLBACK (garcon_gtk_menu_item_drag_data_get), li->data);
          g_signal_connect_swapped (G_OBJECT (mi), "drag-end",
              G_CALLBACK (garcon_gtk_menu_item_drag_end), menu);

          /* doesn't happen, but anyway... */
          command = garcon_menu_item_get_command (li->data);
          if (STR_IS_EMPTY (command))
            gtk_widget_set_sensitive (mi, FALSE);

          if (menu->priv->show_menu_icons)
            {
              icon_name = garcon_menu_item_get_icon_name (li->data);
              if (STR_IS_EMPTY (icon_name))
                icon_name = "applications-other";

              image = garcon_gtk_menu_load_icon (icon_name);
              gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mi), image);
              gtk_widget_show (image);
            }

          /* atleast 1 visible child */
          has_children = TRUE;
        }
      else if (GARCON_IS_MENU_SEPARATOR (li->data))
        {
          mi = gtk_separator_menu_item_new ();
          gtk_menu_shell_append (GTK_MENU_SHELL (gtk_menu), mi);
          gtk_widget_show (mi);
        }
      else if (GARCON_IS_MENU (li->data))
        {
          /* the element check for menu also copies the item list to
           * check if all the elements are visible, we do that with the
           * return value of this function, so avoid that and only check
           * the visibility of the menu directory */
          directory = garcon_menu_get_directory (li->data);
          if (directory != NULL
              && !garcon_menu_directory_get_visible (directory))
            continue;

          submenu = gtk_menu_new ();
          if (garcon_gtk_menu_add (menu, GTK_MENU (submenu), li->data))
            {
              /* attach submenu */
              name = garcon_menu_element_get_name (li->data);
              mi = gtk_image_menu_item_new_with_label (name);
              gtk_menu_shell_append (GTK_MENU_SHELL (gtk_menu), mi);
              gtk_menu_item_set_submenu (GTK_MENU_ITEM (mi), submenu);
              g_signal_connect (G_OBJECT (submenu), "selection-done",
                  G_CALLBACK (garcon_gtk_menu_deactivate), menu);
              gtk_widget_show (mi);

              if (menu->priv->show_menu_icons)
                {
                  icon_name = garcon_menu_element_get_icon_name (li->data);
                  if (STR_IS_EMPTY (icon_name))
                    icon_name = "applications-other";

                  image = gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_MENU);
                  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mi), image);
                  gtk_widget_show (image);
                }
            }
          else
            {
              /* no visible element in the menu */
              gtk_widget_destroy (submenu);
            }
        }
    }

  g_list_free (elements);

  return has_children;
}



static void
garcon_gtk_menu_load (GarconGtkMenu *menu)
{
  GError    *error = NULL;
  GtkWidget *mi;

  g_return_if_fail (GARCON_GTK_IS_MENU (menu));
  g_return_if_fail (menu->priv->menu == NULL || GARCON_IS_MENU (menu->priv->menu));

  if (menu->priv->menu == NULL)
    return;

  if (garcon_menu_load (menu->priv->menu, NULL, &error))
    {
      if (!garcon_gtk_menu_add (menu, GTK_MENU (menu), menu->priv->menu))
        {
          mi = gtk_menu_item_new_with_label (_("No applications found"));
          gtk_menu_shell_append (GTK_MENU_SHELL (menu), mi);
          gtk_widget_set_sensitive (mi, FALSE);
          gtk_widget_show (mi);
        }

      /* watch for changes */
      g_signal_connect_swapped (G_OBJECT (menu->priv->menu), "reload-required",
        G_CALLBACK (garcon_gtk_menu_reload), menu);
    }
  else
    {
       xfce_dialog_show_error (NULL, error, _("Failed to load the applications menu"));
       g_error_free (error);
    }

  menu->priv->reload_id = 0;
  menu->priv->is_loaded = TRUE;
}



/**
 * garcon_gtk_menu_new:
 * @garcon_menu  :
 *
 * Creates a new #GarconMenu for the .menu file referred to by @file.
 * This operation only fails @file is invalid. To load the menu
 * tree from the file, you need to call garcon_gtk_menu_load() with the
 * returned #GarconMenu.
 *
 * The caller is responsible to destroy the returned #GarconMenu
 * using g_object_unref().
 *
 * For more information about the usage @see garcon_gtk_menu_new().
 *
 * Returns: a new #GarconMenu for @file.
 **/
GtkWidget *
garcon_gtk_menu_new (GarconMenu *garcon_menu)
{
  g_return_val_if_fail (garcon_menu == NULL || GARCON_IS_MENU (garcon_menu), NULL);
  return g_object_new (GARCON_GTK_TYPE_MENU, "menu", garcon_menu, NULL);
}



/**
 * garcon_gtk_menu_get_menu:
 * @menu  : A #GarconGtkMenu
 *
 * The #GarconMenu used to create the #GtkMenu.
 *
 * The caller is responsible to releasing the returned #GarconMenu
 * using g_object_unref().
 *
 * Returns: the #GarconMenu for @menu.
 **/
void
garcon_gtk_menu_set_menu (GarconGtkMenu *menu,
                          GarconMenu    *garcon_menu)
{
  g_return_if_fail (GARCON_GTK_IS_MENU (menu));
  g_return_if_fail (garcon_menu == NULL || GARCON_IS_MENU (garcon_menu));

  if (menu->priv->menu == garcon_menu)
    return;

  if (menu->priv->menu != NULL)
    {
      g_signal_handlers_disconnect_by_func (G_OBJECT (menu->priv->menu), garcon_gtk_menu_reload, menu);
      g_object_unref (G_OBJECT (menu->priv->menu));
    }

  if (garcon_menu != NULL)
    menu->priv->menu = g_object_ref (G_OBJECT (garcon_menu));
  else
    menu->priv->menu = NULL;

  g_object_notify_by_pspec (G_OBJECT (menu), menu_props[PROP_MENU]);

  garcon_gtk_menu_reload (menu);
}



/**
 * garcon_gtk_menu_get_menu:
 * @menu  : A #GarconGtkMenu
 *
 * The #GarconMenu used to create the #GtkMenu.
 *
 * The caller is responsible to releasing the returned #GarconMenu
 * using g_object_unref().
 *
 * Returns: the #GarconMenu for @menu.
 **/
GarconMenu *
garcon_gtk_menu_get_menu (GarconGtkMenu *menu)
{
  g_return_val_if_fail (GARCON_GTK_IS_MENU (menu), NULL);
  if (menu->priv->menu != NULL)
    return g_object_ref (G_OBJECT (menu->priv->menu));
  return NULL;
}



/**
 * garcon_gtk_menu_set_show_generic_names:
 * @menu               : A #GarconGtkMenu
 * @show_generic_names : new value
 *
 **/
void
garcon_gtk_menu_set_show_generic_names (GarconGtkMenu *menu,
                                        gboolean       show_generic_names)
{
  g_return_if_fail (GARCON_GTK_IS_MENU (menu));

  if (menu->priv->show_generic_names == show_generic_names)
    return;

  menu->priv->show_generic_names = !!show_generic_names;
  g_object_notify_by_pspec (G_OBJECT (menu), menu_props[PROP_SHOW_GENERIC_NAMES]);

  garcon_gtk_menu_reload (menu);
}



/**
 * garcon_gtk_menu_get_show_generic_names:
 * @menu  : A #GarconGtkMenu
 *
 * Return value: if generic names are shown
 **/
gboolean
garcon_gtk_menu_get_show_generic_names (GarconGtkMenu *menu)
{
  g_return_val_if_fail (GARCON_GTK_IS_MENU (menu), FALSE);
  return menu->priv->show_generic_names;
}



/**
 * garcon_gtk_menu_set_show_menu_icons:
 * @menu            : A #GarconGtkMenu
 * @show_menu_icons : new value
 *
 *
 **/
void
garcon_gtk_menu_set_show_menu_icons (GarconGtkMenu *menu,
                                     gboolean       show_menu_icons)
{
  g_return_if_fail (GARCON_GTK_IS_MENU (menu));

  if (menu->priv->show_menu_icons == show_menu_icons)
    return;

  menu->priv->show_menu_icons = !!show_menu_icons;
  g_object_notify_by_pspec (G_OBJECT (menu), menu_props[PROP_SHOW_MENU_ICONS]);

  garcon_gtk_menu_reload (menu);
}



/**
 * garcon_gtk_menu_get_show_menu_icons:
 * @menu  : A #GarconGtkMenu
 *
 * Return value: if menu icons are shown
 **/
gboolean
garcon_gtk_menu_get_show_menu_icons (GarconGtkMenu *menu)
{
  g_return_val_if_fail (GARCON_GTK_IS_MENU (menu), FALSE);
  return menu->priv->show_menu_icons;
}



/**
 * garcon_gtk_menu_set_show_tooltips:
 * @menu  : A #GarconGtkMenu
 *
 *
 **/
void
garcon_gtk_menu_set_show_tooltips (GarconGtkMenu *menu,
                                   gboolean       show_tooltips)
{
  g_return_if_fail (GARCON_GTK_IS_MENU (menu));

  if (menu->priv->show_tooltips == show_tooltips)
    return;

  menu->priv->show_tooltips = !!show_tooltips;
  g_object_notify_by_pspec (G_OBJECT (menu), menu_props[PROP_SHOW_TOOLTIPS]);

  garcon_gtk_menu_reload (menu);
}



/**
 * garcon_gtk_menu_get_show_tooltips:
 * @menu  : A #GarconGtkMenu
 *
 * Return value: if descriptions are shown in tooltip
 **/
gboolean
garcon_gtk_menu_get_show_tooltips (GarconGtkMenu *menu)
{
  g_return_val_if_fail (GARCON_GTK_IS_MENU (menu), FALSE);
  return menu->priv->show_tooltips;
}
