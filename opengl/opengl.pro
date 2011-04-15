# -------------------------------------------------
# Project created by QtCreator 2011-02-04T18:14:12
# -------------------------------------------------
QT += opengl
TARGET = opengl
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    glwidget.cpp \
    configdialog.cpp
HEADERS += mainwindow.h \
    glwidget.h \
    configdialog.h
FORMS += mainwindow.ui \
    configdialog.ui
win32 { 
    INCLUDEPATH += "C:\OpenCV2.0\include\opencv"
    LIBS += -L"C:\OpenCV2.0\lib" \
        -lcv200 \
        -lcxcore200 \
        -lhighgui200
}
else { 
    INCLUDEPATH += /usr/local/include/opencv
    LIBS += -L/usr/local/lib
}
