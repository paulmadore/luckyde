
AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-DG_LOG_DOMAIN=\"lde4-panel-migrate\" \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	-DXDGCONFIGDIR=\"$(sysconfdir)/xdg\" \
	$(PLATFORM_CPPFLAGS)

confdir = $(sysconfdir)/xdg/lde4/panel
conf_DATA = \
	default.xml

migratedir = $(HELPER_PATH_PREFIX)/lde4/panel

migrate_PROGRAMS = \
	migrate

migrate_SOURCES = \
	main.c \
	migrate-46.c \
	migrate-46.h \
	migrate-config.c \
	migrate-config.h \
	migrate-default.c \
	migrate-default.h

migrate_CFLAGS = \
	$(GTK_CFLAGS) \
	$(XFCONF_CFLAGS) \
	$(LIBLDE4UTIL_CFLAGS) \
	$(LIBLDE4UI_CFLAGS) \
	$(PLATFORM_CFLAGS)

migrate_LDFLAGS = \
	-no-undefined \
	$(PLATFORM_LDFLAGS)

migrate_LDADD = \
	$(top_builddir)/liblde4panel/liblde4panel-$(LIBLDE4PANEL_VERSION_API).la \
	$(GTK_LIBS) \
	$(XFCONF_LIBS) \
	$(LIBLDE4UTIL_LIBS) \
	$(LIBLDE4UI_LIBS)

migrate_DEPENDENCIES = \
	$(top_builddir)/liblde4panel/liblde4panel-$(LIBLDE4PANEL_VERSION_API).la

EXTRA_DIST = \
	default.xml

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
