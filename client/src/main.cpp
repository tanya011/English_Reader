#include <QApplication>
#include "include/collection.h"
#include "include/library.h"
#include "include/readnow.h"
#include "include/dictionary.h"
#include "include/cards.h"
#include "include/settings.h"
#include "include/entrance.h"
#include "include/menuconnectuons.h"

void set_titles(Collection* cll, Library *lib, ReadingNow *rdn, Dictionary *dic, Cards *crd, Settings *stt, Entrance *ent){
    cll->setWindowTitle("Коллекция");
    lib->setWindowTitle("Каталог");
    rdn->setWindowTitle("Читаю сейчас");
    dic->setWindowTitle("Словарь");
    crd->setWindowTitle("Карточки");
    stt->setWindowTitle("Настройки");
    ent->setWindowTitle("Вход/Выход");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Создаём окна разделов
    auto* cll = new Collection;
    auto *lib = new Library;
    auto *rdn = new ReadingNow;
    rdn->showMaximized();
    auto *dic = new Dictionary;
    auto *crd = new Cards;
    auto *stt = new Settings;
    auto *ent = new Entrance;

    rdn->show();

    set_titles(cll, lib, rdn, dic, crd, stt, ent);

    all_connections(cll, lib, rdn, dic, crd, stt, ent);

    return a.exec();
}