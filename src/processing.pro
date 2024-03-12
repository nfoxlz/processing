QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    capture_form.cpp \
    doc_form.cpp \
    image_doc.cpp \
    main.cpp \
    mainwindow.cpp \
    opencv_form.cpp

HEADERS += \
    capture_form.h \
    doc_form.h \
    image_doc.h \
    mainwindow.h \
    opencv_form.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    processing_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += ../include
LIBS += -L../lib

CONFIG(debug, debug|release): {
    DEFINES += _DEBUG
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
