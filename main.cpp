#include "models/devicelistmodel.h"
#include "datasources/pricefetcher.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    if (qEnvironmentVariableIsEmpty("QTGLESSTREAM_DISPLAY")) {
        qputenv("QT_QPA_EGLFS_PHYSICAL_WIDTH", QByteArray("213"));
        qputenv("QT_QPA_EGLFS_PHYSICAL_HEIGHT", QByteArray("120"));

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    }
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Clock clock;
    PriceFetcher priceSource(&clock);
    qmlRegisterType<DeviceListModel>("listmodels", 1, 0, "DeviceListModel");
    qmlRegisterSingletonType(QUrl("qrc:/singletons/Constants.qml"), "config", 1, 0, "Constants");
    qmlRegisterSingletonInstance("datasources", 1, 0, "Clock", &clock);
    qmlRegisterSingletonInstance("datasources", 1, 0, "PriceFetcher", &priceSource);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
