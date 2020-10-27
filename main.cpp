#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <MessangerClient.h>
#include <user.h>
#include <QQmlContext>
#include <QQuickStyle>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    MessangerClient *messangerclient = new MessangerClient();
    User *u = new User();

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("messangerclient", messangerclient);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
