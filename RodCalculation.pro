TEMPLATE = app
           CONFIG += console c++17 qt widgets
                     CONFIG -= app_bundle
                               QT += core gui widgets

                                     SOURCES += \
                                                main.cpp \
                                                rod.cpp

                                                HEADERS += \
                                                           rod.h
