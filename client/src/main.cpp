#include <QApplication>
#include "include/sectionbase.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    SectionBase mainWindow;
    mainWindow.showMaximized();
    return a.exec();
}