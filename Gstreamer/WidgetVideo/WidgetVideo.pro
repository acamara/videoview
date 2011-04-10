#-------------------------------------------------
#
# Project created by QtCreator 2011-04-10T12:33:33
#
#-------------------------------------------------

TARGET = WidgetVideo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include" \
            "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\gstreamer-0.10" \
            "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\glib-2.0" \
            "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\libxml2"

LIBS += -L"C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\lib" \
    -lgstreamer-0.10 \
    -lglib-2.0\
    -lgobject-2.0\
    -lgstinterfaces-0.10
