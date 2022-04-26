#ifndef SECTIONBASE_H
#define SECTIONBASE_H

#include <QMainWindow>
#include <QStackedWidget>
#include "wordset.h"

class ConnectingWindow : public QMainWindow {
    Q_OBJECT

public:
    QStackedWidget allWindows;
    struct WindowIndexes {
        int library=-1, readNow=-1, dictionary=-1, auth=-1, learn=-1, cards=-1;
    } windowIndexes;

    explicit ConnectingWindow(QWidget *parent = nullptr);

    void showLibrary();
    void showReadNow();
    void showDictionary();
    void showCards();
    void showCardsDisplay();
    void showAuth();

    void updateLibrary();
    void updateReadNow(const std::string &title,
                       const std::string &author,
                       const std::string &text);
    void updateDictionary();
    void updateCards();

    void updateAuth();



private:
    QAction *libraryAction_;
    QAction *readNowAction_;
    QAction *authorizationAction_;
    QAction *dictionaryAction_;
    QAction *cardsAction_;

    std::vector<WordSet> getWordSets();

    void addMenu();


};

#endif  // SECTIONBASE_H
