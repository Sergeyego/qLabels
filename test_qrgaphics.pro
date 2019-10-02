#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T09:01:06
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_qrgaphics
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lib/papers.cpp \
    lib/templates.cpp \
    lib/tools.cpp \
    dialognewlabel.cpp \
    lib/ean13.cpp \
    edtpositionwidget.cpp \
    edtbarcodewidget.cpp \
    edtimagewidget.cpp \
    edttextwidget.cpp \
    edtlinewidget.cpp \
    edtbrushwidget.cpp \
    items.cpp

HEADERS  += mainwindow.h \
    lib/tools.h \
    lib/papers.h \
    lib/templates.h \
    dialognewlabel.h \
    lib/ean13.h \
    edtpositionwidget.h \
    edtbarcodewidget.h \
    edtimagewidget.h \
    edttextwidget.h \
    edtlinewidget.h \
    edtbrushwidget.h \
    items.h

FORMS    += mainwindow.ui \
    dialognewlabel.ui \
    edtpositionwidget.ui \
    edtbarcodewidget.ui \
    edtimagewidget.ui \
    edttextwidget.ui \
    edtlinewidget.ui \
    edtbrushwidget.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    templates/avery-iso-templates.xml \
    templates/avery-other-templates.xml \
    templates/avery-us-templates.xml \
    templates/dvd-template.xml \
    templates/misc-iso-templates.xml \
    templates/misc-other-templates.xml \
    templates/misc-us-templates.xml \
    templates/paper-sizes.xml \
    templates/zweckform-iso-templates.xml
