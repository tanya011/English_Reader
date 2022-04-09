#ifndef SECTIONBASE_H
#define SECTIONBASE_H

#include <QMainWindow>
#include "library_window.h"
#include "readNowWindow.h"
#include "authorizationWindow.h"
#include "db_manager.h"
#include "dictionary.h"
#include "cards.h"

namespace Ui {
    class SectionBase;
}

class SectionBase : public QMainWindow {
Q_OBJECT

public:
    SectionBase(DBManager &m, QWidget *parent = nullptr);
    ~SectionBase();

private:
    QAction *libraryAction_;
    QAction *readNowAction_;
    QAction *authorizationAction_;
    QAction *dictionaryAction_;
    QAction *cardsAction_;

    LibraryWindow libraryWindow_;
    ReadNowWindow readNowWindow_;
    Dictionary dictionaryWindow_;
    AuthorizationWindow authorizationWindow_;
    Cards cardsWindow_;
    DBManager &dbManager_;

    Ui::SectionBase *ui;

    std::vector<WordSet> getWordSets();

    void creatMenu();
};

#endif  // SECTIONBASE_H