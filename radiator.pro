QT += quick network quickcontrols2
CONFIG += c++14 warn_on

win32 {
    CONFIG += console

    #TODO on Windows and with Qt <= 5.9, install OpenSSL 1.0
    #     from https://slproweb.com/products/Win32OpenSSL.html3
    # For Qt >= 5.10 install OpenSSL 1.1
}

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_USE_QSTRINGBUILDER

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SOURCES += \
    main.cpp \
    utils.cpp \
    buildmodel.cpp \
    jenkinsclient.cpp \
    modelaccessor.cpp \
    branchfiltermodel.cpp \
    Config/config.cpp \
    Config/configuser.cpp \
    Config/configproxy.cpp \
    Config/configremote.cpp \
    Config/configradiator.cpp \
    Config/configremotemodel.cpp \
    Config/configradiatorheader.cpp \
    Entity/job.cpp \
    Entity/build.cpp \
    Entity/buildcause.cpp \
    Entity/branchfilter.cpp \
    Entity/culprit.cpp \

HEADERS += \
    utils.h \
    buildmodel.h \
    modelaccessor.h \
    jenkinsclient.h \
    jenkinsclient_p.h \
    branchfiltermodel.h \
    Config/config.h \
    Config/configuser.h \
    Config/configproxy.h \
    Config/configremote.h \
    Config/configradiator.h \
    Config/configremotemodel.h \
    Config/configradiatorheader.h \
    Entity/job.h \
    Entity/build.h \
    Entity/culprit.h \
    Entity/buildcause.h \
    Entity/branchfilter.h \

RESOURCES += qml/qml.qrc
RESOURCES += assets/assets.qrc
