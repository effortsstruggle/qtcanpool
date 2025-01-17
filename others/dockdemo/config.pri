!isEmpty(CONFIG_PRI_INCLUDED):error("config.pri already included")
CONFIG_PRI_INCLUDED = 1

isEmpty(QTPROJECT_VERSION):         QTPROJECT_VERSION = 2.0.0
isEmpty(QTPROJECT_COMPAT_VERSION):  QTPROJECT_COMPAT_VERSION = 2.0.0
isEmpty(QTPROJECT_DISPLAY_VERSION): QTPROJECT_DISPLAY_VERSION = 2.0.0
isEmpty(QTPROJECT_COPYRIGHT_YEAR):  QTPROJECT_COPYRIGHT_YEAR = 2022

isEmpty(QTPROJECT_DIR):             QTPROJECT_DIR = $$PWD
isEmpty(QTPROJECT_OUT_PWD):         QTPROJECT_OUT_PWD = $$OUT_PWD
isEmpty(QTPROJECT_PRO_FILE_PWD):    QTPROJECT_PRO_FILE_PWD = $$_PRO_FILE_PWD_
isEmpty(QTPROJECT_PRO_FILE):        QTPROJECT_PRO_FILE = $$_PRO_FILE_

isEmpty(QTCANPOOL_DIR):             QTCANPOOL_DIR = $$quote($$PWD/../..)

!exists($$QTCANPOOL_DIR) {
    error("QTCANPOOL_DIR was not configured correctly")
}

include($$QTCANPOOL_DIR/qtconfig.pri)
