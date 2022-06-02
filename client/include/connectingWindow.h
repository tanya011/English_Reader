#ifndef CONNECTINGWINDOW_H
#define CONNECTINGWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "user.h"
#include "../include/wordset.h"

class ConnectingWindow : public QMainWindow {
    Q_OBJECT

public:
    User *user;  // Maybe it's bad, that its public, but at the same time it
                 // doesn't even belong to this window
    QStackedWidget allWindows;
    struct WindowIndexes {
        int library = -1, readNow = -1, dictionary = -1, auth = -1, learn = -1,
            cards = -1, collection = -1;
    } windowIndexes;

    explicit ConnectingWindow(QWidget *parent = nullptr);

    void setUser(User *u);

    void showCollection();
    void showReadNow();
    void showDictionary();
    void showCards();
    void showAuth();
    void showLibrary();

    void authAddExit(){

    }

    void updateLibrary();
    void updateReadNow(const std::string &title,
                       const std::string &author,
                       const std::string &text);
    void updateDictionary();
    void updateCards();
    void updateCollection();


private:
    QAction *collectionAction_;
    QAction *readNowAction_;
    QAction *authorizationAction_;
    QAction *dictionaryAction_;
    QAction *cardsAction_;
    QAction *libraryAction_;

    std::vector<WordSet> getWordSets();

    void addMenu();
};

#endif  // CONNECTINGWINDOW_H
