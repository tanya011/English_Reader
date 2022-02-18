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
    Collection* cll = new Collection;
    Library *lib = new Library;
    ReadingNow *rdn = new ReadingNow;
    Dictionary *dic = new Dictionary;
    Cards *crd = new Cards;
    Settings *stt = new Settings;
    Entrance *ent = new Entrance;

    rdn->show();

    set_titles(cll, lib, rdn, dic, crd, stt, ent);

    all_connections(cll, lib, rdn, dic, crd, stt, ent);

    return a.exec();
}