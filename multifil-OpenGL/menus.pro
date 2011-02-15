QT += opengl

HEADERS += renderthread.h \
    mainwindow.h \
    glwidget.h
SOURCES += renderthread.cpp \
    mainwindow.cpp \
    main.cpp \
    glwidget.cpp
FORMS += mainwindow.ui

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
