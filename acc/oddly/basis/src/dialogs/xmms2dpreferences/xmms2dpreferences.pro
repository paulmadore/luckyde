TEMPLATE = lib
CONFIG += static plugin
include (../../../config.pri)

FORMS +=	xmms2dprefdialog.ui

HEADERS +=		plugin.h \
				xmms2dprefdialog.h \
				xmms2dprefmodel.h

SOURCES +=		plugin.cpp \
				xmms2dprefdialog.cpp \
				xmms2dprefmodel.cpp
