greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = terminal
TEMPLATE = app

SOURCES += \
    Graphic/Analiz.cpp \
    Graphic/Control_Graf.cpp \
    Graphic/Data_Graf.cpp \
    Graphic/Graphic_Widget.cpp \
    Graphic/graphic.cpp \
    graphic_window.cpp \
    main.cpp \
    main_window.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp

HEADERS += \
    Graphic/Analiz.h \
    Graphic/Control_Graf.h \
    Graphic/Data_Graf.h \
    Graphic/Graphic_Widget.h \
    Graphic/graphic.h \
    graphic_window.h \
    main_window.h \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    version.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    terminal.qrc

RC_FILE = myapp.rc

OTHER_FILES += \
    myapp.rc
