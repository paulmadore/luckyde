


#undef GTK_DISABLE_DEPRECATED
#define GTK_ENABLE_BROKEN
#include <exo/exo.h>
#include <exo/exo-alias.h>

/* enumerations from "exo-icon-view.h" */
GType
exo_icon_view_drop_position_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GEnumValue values[] = {
	{ EXO_ICON_VIEW_NO_DROP, "EXO_ICON_VIEW_NO_DROP", "no-drop" },
	{ EXO_ICON_VIEW_DROP_INTO, "EXO_ICON_VIEW_DROP_INTO", "drop-into" },
	{ EXO_ICON_VIEW_DROP_LEFT, "EXO_ICON_VIEW_DROP_LEFT", "drop-left" },
	{ EXO_ICON_VIEW_DROP_RIGHT, "EXO_ICON_VIEW_DROP_RIGHT", "drop-right" },
	{ EXO_ICON_VIEW_DROP_ABOVE, "EXO_ICON_VIEW_DROP_ABOVE", "drop-above" },
	{ EXO_ICON_VIEW_DROP_BELOW, "EXO_ICON_VIEW_DROP_BELOW", "drop-below" },
	{ 0, NULL, NULL }
	};
	type = g_enum_register_static ("ExoIconViewDropPosition", values);
  }
	return type;
}
GType
exo_icon_view_layout_mode_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GEnumValue values[] = {
	{ EXO_ICON_VIEW_LAYOUT_ROWS, "EXO_ICON_VIEW_LAYOUT_ROWS", "rows" },
	{ EXO_ICON_VIEW_LAYOUT_COLS, "EXO_ICON_VIEW_LAYOUT_COLS", "cols" },
	{ 0, NULL, NULL }
	};
	type = g_enum_register_static ("ExoIconViewLayoutMode", values);
  }
	return type;
}

/* enumerations from "exo-toolbars-model.h" */
GType
exo_toolbars_model_flags_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GFlagsValue values[] = {
	{ EXO_TOOLBARS_MODEL_NOT_REMOVABLE, "EXO_TOOLBARS_MODEL_NOT_REMOVABLE", "not-removable" },
	{ EXO_TOOLBARS_MODEL_ACCEPT_ITEMS_ONLY, "EXO_TOOLBARS_MODEL_ACCEPT_ITEMS_ONLY", "accept-items-only" },
	{ EXO_TOOLBARS_MODEL_OVERRIDE_STYLE, "EXO_TOOLBARS_MODEL_OVERRIDE_STYLE", "override-style" },
	{ 0, NULL, NULL }
	};
	type = g_flags_register_static ("ExoToolbarsModelFlags", values);
  }
	return type;
}

#define __EXO_ENUM_TYPES_C__
#include <exo/exo-aliasdef.c>



