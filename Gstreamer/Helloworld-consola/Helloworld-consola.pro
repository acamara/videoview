#-------------------------------------------------
#
# Project created by QtCreator 2011-04-09T18:00:11
#
#-------------------------------------------------

QT       -= gui

TARGET = Helloworld-consola
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include" \
            "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\gstreamer-0.10" \
            "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\glib-2.0" \
            "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\libxml2"

LIBS += -L"C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\lib" \
    -lgstreamer-0.10 \
    -lglib-2.0\
    -lgobject-2.0
