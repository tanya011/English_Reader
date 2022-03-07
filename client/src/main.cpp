
/*
void set_titles(Collection *cll,
                LibraryWindow *lib,
                ReadNow *rdn,
                Dictionary *dic,
                Cards *crd,
                Settings *stt,
                Entrance *ent) {
    cll->setWindowTitle("Коллекция");
    lib->setWindowTitle("Библиотека");
    rdn->setWindowTitle("Читаю сейчас");
    dic->setWindowTitle("Словарь");
    crd->setWindowTitle("Карточки");
    stt->setWindowTitle("Настройки");
    ent->setWindowTitle("Вход/Выход");
}*/

#include <QApplication>
#include "include/sectionbase.h"
#include "include/db_manager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    DBManager dbManager;
    SectionBase mainWindow(dbManager);
    mainWindow.show();

    return a.exec();
}