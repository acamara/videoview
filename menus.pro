HEADERS += configdialog.h \
    mainwindow.h \
    widgetvideo.h \
    qvumeter.h \
    eprocessat.h
SOURCES += mainwindow.cpp \
    main.cpp \
    configdialog.cpp \
    widgetvideo.cpp \
    qvumeter.cpp \
    eprocessat.cpp
FORMS += mainwindow.ui \
    configdialog.ui \
    widgetvideo.ui

win32 {
    INCLUDEPATH += "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include" \
                "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\gstreamer-0.10" \
                "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\glib-2.0" \
                "C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\include\libxml2"

    LIBS += -L"C:\Program Files (x86)\OSSBuild\GStreamer\v0.10.6\sdk\lib" \
        -lgstreamer-0.10 \
        -lglib-2.0\
        -lgobject-2.0\
        -lgstinterfaces-0.10

}
else {
    INCLUDEPATH += /usr/include \
                   /usr/include/gstreamer-0.10 \
                   /usr/include/glib-2.0 \
                   /usr/lib/glib-2.0/include \
                   /usr/lib/x86_64-linux-gnu/glib-2.0/include \
                   /usr/include/libxml2

    LIBS += -L/usr/local/lib \
        -lgstreamer-0.10 \
        -lglib-2.0\
        -lgobject-2.0\
        -lgstinterfaces-0.10
}

RESOURCES += \
    recursos.qrc
