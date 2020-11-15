#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "qcvdetectfilter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qDebug() << "App path : " << qApp->applicationDirPath();

    qmlRegisterType<QCvDetectFilter>("com.amin.classes", 1, 0, "CvDetectFilter");
    qRegisterMetaType<QList<QRect>>("QList<QRect>");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
