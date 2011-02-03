# -------------------------------------------------
# Project created by QtCreator 2011-01-29T11:15:27
# -------------------------------------------------
QT += opengl
TARGET = menus
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    GravarThread.cpp \
    glwidget.cpp \
    configdialog.cpp \
    CapturaThread.cpp
HEADERS += mainwindow.h \
    GravarThread.h \
    glwidget.h \
    configdialog.h \
    CapturaThread.h
FORMS += mainwindow.ui \
    configdialog.ui

win32 {

INCLUDEPATH += "C:\OpenCV2.0\include\opencv"
LIBS += -L"C:\OpenCV2.0\lib" \
    -lcv200 \
    -lcxcore200 \
    -lhighgui200
 }

else{

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib

}

