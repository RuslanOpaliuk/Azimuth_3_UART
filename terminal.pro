greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = terminal
TEMPLATE = app

SOURCES += \
    TextEditor.cpp \
    ll_protocol/ll_protocol.c \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp

HEADERS += \
    TextEditor.h \
    ll_protocol/ll_protocol.h \
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
