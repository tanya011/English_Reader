#include "menuconnectuons.h"

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

    all_connections(cll, lib, rdn, dic, crd, stt, ent);

    return a.exec();
}
