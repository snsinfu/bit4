#include <QGuiApplication>
#include <QQuickWindow>
#include <QtQml>
#include <QtWebEngine/QtWebEngine>


int main(int argc, char** argv)
{
    QGuiApplication app{argc, argv};
    QtWebEngine::initialize();

    QQmlApplicationEngine engine{"qrc:///main.qml"};
    auto qmlroot = engine.rootObjects().value(0);
    auto window = qobject_cast<QQuickWindow*>(qmlroot);
    if (!window) {
        return 1;
    }
    window->show();

    return app.exec();
}
