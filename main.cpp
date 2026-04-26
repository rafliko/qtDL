#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (MainWindow::readJson("search_dirs.json")!=0) return 1;
    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
