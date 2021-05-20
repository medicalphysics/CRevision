
#include <QApplication>
#include <QSplashScreen>

#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    QPixmap pixmap(":/icons/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();

    QCoreApplication::setApplicationName("CRevision");
    QCoreApplication::setOrganizationName("SSHF");
    app.setWindowIcon(QIcon(":/icons/icon.png"));

    MainWindow win;
    win.show();
    splash.finish(&win);

    return app.exec();
}
