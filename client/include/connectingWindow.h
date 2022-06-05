#ifndef CONNECTINGWINDOW_H
#define CONNECTINGWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "../include/wordset.h"
#include "user.h"

class ConnectingWindow : public QMainWindow {
    Q_OBJECT

public:
    User *user;
    QStackedWidget allWindows;
    struct WindowIndexes {
        int library = -1, readNow = -1, dictionary = -1, auth = -1, learn = -1,
            cards = -1, collection = -1, serverProblems = -1;
    } windowIndexes;

    explicit ConnectingWindow(QWidget *parent = nullptr);

    void setUser(User *u);

    void showCollection();
    void showReadNow();
    void showDictionary();
    void showCards();
    void showAuth();
    void showLibrary();

    void updateLibrary() const;
    void updateReadNow(const std::string &title,
                       const std::string &author,
                       const std::string &text) const;
    void updateDictionary() const;
    void updateCards();
    void updateCollection() const;

private:
    QAction *collectionAction_;
    QAction *readNowAction_;
    QAction *authorizationAction_;
    QAction *dictionaryAction_;
    QAction *cardsAction_;
    QAction *libraryAction_;

    std::vector<WordSet> getWordSets() const;

    void addMenu();
};

#endif  // CONNECTINGWINDOW_H
