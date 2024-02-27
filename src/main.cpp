#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world490d.lib")
#else
#pragma comment(lib, "opencv_world490.lib")
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "processing_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
