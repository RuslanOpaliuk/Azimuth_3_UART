greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

QT += charts
TARGET = terminal
TEMPLATE = app

SOURCES += \
    TextEditor.cpp \
    base_station_coord.cpp \
    callout.cpp \
    ll_protocol/sources/ll_Message.c \
    ll_protocol/sources/ll_protocol.c \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    sound_graph.cpp

HEADERS += \
    TextEditor.h \
    base_station_coord.h \
    callout.h \
    ll_protocol/sources/ll_Message.h \
    ll_protocol/sources/ll_protocol.h \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    sound_graph.h \
    version.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    terminal.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
