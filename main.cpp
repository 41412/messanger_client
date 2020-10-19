#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <MessangerClient.h>
#include <QQuickStyle>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    MessangerClient *client = new MessangerClient();

    QQuickStyle::setStyle("Material");
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
