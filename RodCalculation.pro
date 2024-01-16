TEMPLATE = app
CONFIG += console c++17 qt widgets
CONFIG -= app_bundle
QT += core gui widgets

SOURCES += \
    beamdrawer.cpp \
    main.cpp \
    rod.cpp

    HEADERS += \
    beamdrawer.h \
        rod.h
