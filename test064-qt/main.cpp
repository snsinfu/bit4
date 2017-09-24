#include <exception>
#include <iostream>

#include <QApplication>
#include <QQuickWindow>
#include <QMessageBox>
#include <QtQml>

namespace
{
    int run(int argc, char** argv)
    {
        QApplication app{argc, argv};

        QQmlApplicationEngine engine(QUrl("qrc:///hello.qml"));
        QObject* top = engine.rootObjects().value(0);
        QQuickWindow* window = qobject_cast<QQuickWindow*>(top);
        if (!window) {
            return 1;
        }
        window->show();

        return app.exec();
    }
}

int main(int argc, char** argv)
{
    try {
        return run(argc, argv);
    } catch (std::exception const& e) {
        std::cerr << "error: " << e.what() << '\n';
    }
    return 1;
}
