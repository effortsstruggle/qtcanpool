QTC_LIB_DEPENDS += qxribbon
include(../../qtproject.pri)

QT += widgets testlib

CONFIG += c++11
TARGET = Test_QxRibbon
DESTDIR = $$IDE_APP_PATH

include($$QTCANPOOL_DIR/src/rpath.pri)

SOURCES += \
    main.cpp \
    tst_ribbonquickaccessbar.cpp

HEADERS += \
    tst_ribbonquickaccessbar.h
