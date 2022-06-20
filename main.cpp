#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setFixedSize(874,642);
    QIcon icon(":/resourse/img/image.png");
    w.setWindowIcon(icon);
    w.setWindowTitle("Genetic Algorithm");
    return a.exec();
}
