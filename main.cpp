#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <MessangerClient.h>
#include <McUser.h>
#include <mcdebug.h>
#include <QQmlContext>
#include <QQuickStyle>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<MessangerClient>("MessangerClient", 1, 0, "MessangerClient");
    qmlRegisterType<McUser>("McUser", 1, 0, "McUser");
    qmlRegisterType<McDebug>("McDebug", 1, 0, "McDebug");

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    McUser mcuser;
    MessangerClient messangerclient(&mcuser);

    engine.rootContext()->setContextProperty("messangerclient", &messangerclient);
    engine.rootContext()->setContextProperty("mcuser", &mcuser);
    engine.rootContext()->setContextProperty("friendmodel", mcuser.getFriendModel());
    engine.rootContext()->setContextProperty("chatroommodel", mcuser.getChatroomModel());
//    engine.rootContext()->setContextProperty("chatmodel", mcuser.getChatModel());
//    engine.rootContext()->setContextProperty("chatmodel", mcuser.getChatModelList());

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
