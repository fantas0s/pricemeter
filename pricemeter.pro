QT += quick

C++=13
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS += \
    translations/pricemeter_en_US.ts \
    translations/pricemeter_fi_FI.ts

CONFIG += embed_translations

SOURCES += \
        datasources/clock.cpp \
        datasources/pricefetcher.cpp \
        main.cpp \
        models/consumption.cpp \
        models/devicelistitem.cpp \
        models/devicelistmodel.cpp \
        models/operationlistitem.cpp \
        models/operationsmodel.cpp \
        utils/xmlfilereader.cpp

RESOURCES += qml.qrc \
    appdata.qrc \
    images.qrc \
    translations.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    datasources/clock.h \
    datasources/pricefetcher.h \
    datasources/fetcherfactory.h \
    models/consumption.h \
    models/devicelistitem.h \
    models/devicelistmodel.h \
    models/operationlistitem.h \
    models/operationsmodel.h \
    utils/xmlfilereader.h

html_api {
    HEADERS += \
        deprecated/htmlfetcher.h \
        deprecated/htmltableextractor.h

    SOURCES += \
        deprecated/fetcherfactory.cpp \
        deprecated/htmlfetcher.cpp \
        deprecated/htmltableextractor.cpp
}

rest_api {
    HEADERS += \
        restapi/xmlfetcher.h

    SOURCES += \
        restapi/xmlfetcher.cpp \
        restapi/fetcherfactory.cpp
}
