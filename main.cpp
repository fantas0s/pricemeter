#include "models/devicelistmodel.h"
#include "datasources/fetcherfactory.h"

#include <QTranslator>
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
    QTranslator translator;
    if (!translator.load(QLocale(), QString("pricemeter"), QLatin1String("_"), QLatin1String(":/translations"))) {
        qWarning() << "No translation for locale" << QLocale() << "using en_US instead";
        translator.load(QLatin1String(":/translations/pricemeter_en_US.qm"));
    }
    QCoreApplication::installTranslator(&translator);
    QQmlApplicationEngine engine;
    Clock clock;
    QScopedPointer<PriceFetcher> priceSource{FetcherFactory::getFetcherImplementation(&clock)};
    qmlRegisterType<DeviceListModel>("listmodels", 1, 0, "DeviceListModel");
    qmlRegisterSingletonType(QUrl("qrc:/qml/singletons/Constants.qml"), "config", 1, 0, "Constants");
    qmlRegisterSingletonInstance("datasources", 1, 0, "Clock", &clock);
    qmlRegisterSingletonInstance("datasources", 1, 0, "PriceFetcher", priceSource.data());
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
