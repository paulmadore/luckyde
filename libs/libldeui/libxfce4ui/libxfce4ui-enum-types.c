


#include <libxfce4ui/libxfce4ui.h>
#include <libxfce4ui/libxfce4ui-alias.h>

/* enumerations from "xfce-sm-client.h" */

GType
xfce_sm_client_error_enum_get_type(void)
{
    static GType type = 0;

    if(!type) {
        static const GEnumValue values[] = {
            { XFCE_SM_CLIENT_ERROR_FAILED, "XFCE_SM_CLIENT_ERROR_FAILED", "failed" },
            { XFCE_SM_CLIENT_ERROR_INVALID_CLIENT, "XFCE_SM_CLIENT_ERROR_INVALID_CLIENT", "invalid-client" },
            { 0, NULL, NULL }
	};
	type = g_enum_register_static("XfceSmCLientErrorEnum", values);
    }

    return type;
}

GType
xfce_sm_client_restart_style_get_type(void)
{
    static GType type = 0;

    if(!type) {
        static const GEnumValue values[] = {
            { XFCE_SM_CLIENT_RESTART_NORMAL, "XFCE_SM_CLIENT_RESTART_NORMAL", "normal" },
            { XFCE_SM_CLIENT_RESTART_IMMEDIATELY, "XFCE_SM_CLIENT_RESTART_IMMEDIATELY", "immediately" },
            { 0, NULL, NULL }
	};
	type = g_enum_register_static("XfceSMClientRestartStyle", values);
    }

    return type;
}

GType
xfce_sm_client_shutdown_hint_get_type(void)
{
    static GType type = 0;

    if(!type) {
        static const GEnumValue values[] = {
            { XFCE_SM_CLIENT_SHUTDOWN_HINT_ASK, "XFCE_SM_CLIENT_SHUTDOWN_HINT_ASK", "ask" },
            { XFCE_SM_CLIENT_SHUTDOWN_HINT_LOGOUT, "XFCE_SM_CLIENT_SHUTDOWN_HINT_LOGOUT", "logout" },
            { XFCE_SM_CLIENT_SHUTDOWN_HINT_HALT, "XFCE_SM_CLIENT_SHUTDOWN_HINT_HALT", "halt" },
            { XFCE_SM_CLIENT_SHUTDOWN_HINT_REBOOT, "XFCE_SM_CLIENT_SHUTDOWN_HINT_REBOOT", "reboot" },
            { 0, NULL, NULL }
	};
	type = g_enum_register_static("XfceSMClientShutdownHint", values);
    }

    return type;
}


#define __LIBXFCE4UI_ENUM_TYPES_C__
#include <libxfce4ui/libxfce4ui-aliasdef.c>



