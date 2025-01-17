include(../../../../qtproject.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += printsupport

TEMPLATE = app
DESTDIR = $$IDE_APP_PATH

include($$QTCANPOOL_DIR/src/rpath.pri)

# lib
LIBS *= -l$$qtLibraryNameVersion(qribbon, 1)

HEADERS        = mainwindow.h

SOURCES        = main.cpp \
                 mainwindow.cpp

RESOURCES     = RibbonSystemMenu.qrc

include($$PWD/../../shared/aboutdialog.pri)

macx {
    CONFIG-=app_bundle
}
