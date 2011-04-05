#-------------------------------------------------
#
# Project created by QtCreator 2011-03-26T12:26:40
#
#-------------------------------------------------

QT       -= gui

TARGET = provaGstreamer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
OSSBUILD = "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include"
INCLUDEPATH += $$[OSSBUILD] $$[OSSBUILD]"\gstreamer-0.10" $$[OSSBUILD]"\glib-2.0" $$[OSSBUILD]"\libxml2" \
            "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include" \
            "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\gstreamer-0.10" \
            "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\glib-2.0" \
            "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\libxml2"
LIBS += -L"C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\lib" -lgstreamer-0.10

