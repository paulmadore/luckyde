
AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-DG_LOG_DOMAIN=\"gtk-xfce-engine\" \
	$(PLATFORM_CPPFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_BINARY_VERSION)/engines

engine_LTLIBRARIES = libxfce.la

libxfce_la_SOURCES = \
	cairo-support.c	 \
	cairo-support.h \
	ge-support.h \
	general-support.h \
	gradient_draw.c	 \
	gradient_draw.h \
	xfce_theme_draw.c \
	xfce_theme_main.c \
	xfce_rc_style.c \
	xfce_rc_style.h \
	xfce_style.h

libxfce_la_CFLAGS = \
	$(GTK_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GMODULE_CFLAGS) \
	$(PLATFORM_CFLAGS)

libxfce_la_LIBADD = \
	$(GTK_LIBS) \
	$(GLIB_LIBS) \
	$(GMODULE_LIBS) \
	$(LIBM)

libxfce_la_LDFLAGS = \
	-module \
	-avoid-version \
	-no-undefined \
	$(PLATFORM_LDFLAGS)
