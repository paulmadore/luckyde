
AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-DG_LOG_DOMAIN=\"lde-panel\" \
	-DWNCK_I_KNOW_THIS_IS_UNSTABLE \
	-DDATADIR=\"$(datadir)/lde\" \
	-DLIBDIR=\"$(libdir)/lde\" \
	-DHELPERDIR=\"$(HELPER_PATH_PREFIX)/lde/panel\" \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	-DDBUS_API_SUBJECT_TO_CHANGE \
	-DWNCK_I_KNOW_THIS_IS_UNSTABLE \
	$(PLATFORM_CPPFLAGS)

bin_PROGRAMS = \
	lde-panel

lde_panel_built_sources = \
	panel-dbus-service-infos.h \
	panel-dbus-client-infos.h \
	panel-marshal.c \
	panel-marshal.h \
	panel-plugin-external-wrapper-infos.h \
	panel-preferences-dialog-ui.h

lde_panel_SOURCES = \
	$(lde_panel_built_sources) \
	main.c \
	panel-application.c \
	panel-application.h \
	panel-base-window.c \
	panel-base-window.h \
	panel-dbus-client.c \
	panel-dbus-client.h \
	panel-dbus-service.c \
	panel-dbus-service.h \
	panel-dialogs.c \
	panel-dialogs.h \
	panel-item-dialog.c \
	panel-item-dialog.h \
	panel-itembar.c \
	panel-itembar.h \
	panel-module.c \
	panel-module.h \
	panel-module-factory.c \
	panel-module-factory.h \
	panel-plugin-external.c \
	panel-plugin-external.h \
	panel-plugin-external-wrapper.c \
	panel-plugin-external-wrapper.h \
	panel-plugin-external-46.c \
	panel-plugin-external-46.h \
	panel-preferences-dialog.c \
	panel-preferences-dialog.h \
	panel-tic-tac-toe.c \
	panel-tic-tac-toe.h \
	panel-window.c \
	panel-window.h

lde_panel_CFLAGS = \
	$(GTK_CFLAGS) \
	$(GMODULE_CFLAGS) \
	$(EXO_CFLAGS) \
	$(LIBldeUTIL_CFLAGS) \
	$(LIBldeUI_CFLAGS) \
	$(LIBWNCK_CFLAGS) \
	$(DBUS_CFLAGS) \
	$(ldeCONF_CFLAGS) \
	$(LIBX11_CFLAGS) \
	$(LIBWNCK_CFLAGS) \
	$(PLATFORM_CFLAGS)

lde_panel_LDFLAGS = \
	-no-undefined \
	$(PLATFORM_LDFLAGS)

lde_panel_LDADD = \
	$(top_builddir)/libldepanel/libldepanel-$(LIBldePANEL_VERSION_API).la \
	$(top_builddir)/common/libpanel-common.la \
	$(GTK_LIBS) \
	$(EXO_LIBS) \
	$(GMODULE_LIBS) \
	$(LIBldeUTIL_LIBS) \
	$(LIBldeUI_LIBS) \
	$(LIBWNCK_LIBS) \
	$(ldeCONF_LIBS) \
	$(DBUS_LIBS) \
	$(LIBX11_LIBS) \
	$(LIBWNCK_LIBS) \
	-lm

lde_panel_DEPENDENCIES = \
	$(top_builddir)/libldepanel/libldepanel-$(LIBldePANEL_VERSION_API).la \
	$(top_builddir)/common/libpanel-common.la

if MAINTAINER_MODE

panel-marshal.h: panel-marshal.list Makefile
	$(AM_V_GEN) glib-genmarshal --prefix=_panel_marshal --internal --header $< > $@

panel-marshal.c: panel-marshal.list Makefile
	$(AM_V_GEN) echo "#include <panel/panel-marshal.h>" > $@ \
	&& glib-genmarshal --prefix=_panel_marshal --body $< >> $@

panel-dbus-service-infos.h: $(srcdir)/panel-dbus-service-infos.xml Makefile
	$(AM_V_GEN) dbus-binding-tool --prefix=panel_dbus_service --mode=glib-server $< > $@

panel-dbus-client-infos.h: $(srcdir)/panel-dbus-service-infos.xml Makefile
	$(AM_V_GEN) dbus-binding-tool --mode=glib-client $< > $@

panel-plugin-external-wrapper-infos.h: $(srcdir)/panel-plugin-external-wrapper-infos.xml Makefile
	$(AM_V_GEN) dbus-binding-tool --prefix=panel_plugin_external_wrapper --mode=glib-server $< > $@

panel-preferences-dialog-ui.h: $(srcdir)/panel-preferences-dialog.glade Makefile
	$(AM_V_GEN) exo-csource --static --strip-comments --strip-content --name=panel_preferences_dialog_ui $< >$@

#panel-marshal.h: $(srcdir)/panel-marshal.list Makefile
#	$(AM_V_GEN) glib-genmarshal --header --prefix=panel_marshal $< > $@
#
#panel-marshal.c: $(srcdir)/panel-marshal.list Makefile
#	$(AM_V_GEN) echo "#include <panel/panel-marshal.h>" > $@ \
#	&& glib-genmarshal --body --prefix=panel_marshal $< >> $@

DISTCLEANFILES = \
	$(lde_panel_built_sources)

BUILT_SOURCES = \
	$(lde_panel_built_sources)

endif

EXTRA_DIST = \
	panel-dbus-service-infos.xml \
	panel-plugin-external-wrapper-infos.xml \
	panel-preferences-dialog.glade \
	panel-marshal.list

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
