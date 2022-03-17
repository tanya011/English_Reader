#include <QApplication>
#include "include/sectionbase.h"
#include "include/db_manager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    DBManager dbManager;
    SectionBase mainWindow(dbManager);
    mainWindow.showMaximized();
    mainWindow.show();


    return a.exec();
}
