#include <QApplication>
#include <QPushButton>
#include <QStyleFactory>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    w.show();
    return QApplication::exec();
}
