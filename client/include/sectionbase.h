#ifndef SECTIONBASE_H
#define SECTIONBASE_H

#include <QMainWindow>
#include "library_window.h"
#include "readnow.h"
#include "authorization.h"
#include "db_manager.h"
#include "dictionary.h"
#include "cards.h"

namespace Ui {
class SectionBase;
}

class SectionBase : public QMainWindow
{
    Q_OBJECT

private:
    LibraryWindow libraryWindow;
    ReadNow readNow;
    Dictionary dictionaryWindow;
    Auth auth;
    DBManager& dbManager;
    Cards cards;


public:

    SectionBase(DBManager& m, QWidget *parent = nullptr);
    ~SectionBase();

    std::vector<WordSet> getWordSets(){
        return dictionaryWindow.getWordSets();
    }


    //кнопки на меню
    QAction* library;
    QAction* reading_now;
    QAction *auth_action;
    QAction* dictionary;
    QAction* cards_action;

private:

    Ui::SectionBase *ui;
};

#endif  // SECTIONBASE_H