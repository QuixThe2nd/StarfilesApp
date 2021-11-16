QT += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    main.cpp \
    starfiles.cpp \
    uploaded.cpp \

HEADERS += \
    starfiles.h \
    uploaded.h \

FORMS += \
    starfiles.ui \
    uploaded.ui

RESOURCES += \
    assets/resources.qrc

RC_ICONS += assets/images/logo/favicon.ico
