#ifndef DICTIONARY_WINDOW_H
#define DICTIONARY_WINDOW_H

#include <QGridLayout>
#include <QGroupBox>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <fstream>
#include "connectingWindow.h"
#include "wordRep.h"
#include "wordSetContentRep.h"
#include "wordSetRep.h"
#include "../include/wordset.h"
#include <QScrollArea>

class DictionaryWindow : public QWidget {
Q_OBJECT

    ConnectingWindow *parent_;

    QMenuBar *wordSetsToolsBar_ = new QMenuBar(this);

    //QGroupBox *wordsPlacement_ = new QGroupBox(this);

    QPushButton *serverSync_;

    QGridLayout *layout_ = new QGridLayout();
    QWidget *box_;
    QScrollArea *scrollArea =  new QScrollArea(this);

    int curOpenWordSetId_ = 1;

    std::map<int, QAction *>
            wordSetIconForMenu_;  // Helps to add name of wordSet in wordSet menu

    QMenu *wordSets_ = new QMenu("Выбор группы", wordSetsToolsBar_);

    QAction* allWords_ = new QAction("Все группы", wordSets_);

    std::vector<QPushButton *> wordBtnsDeleteFromDictionary_;
    std::vector<QPushButton *> wordBtnsDeleteFromWordSet_;

    WordRep *wordRep_;
    WordSetRep *wordSetRep_;
    WordSetContentRep *wordSetContentRep_;

public:
    explicit DictionaryWindow(WordRep *wordRep, WordSetRep *wordSetRep, WordSetContentRep *wordSetContentRep ,  ConnectingWindow *parent = nullptr);

    void showWordSet(int wordSetId);

    std::vector<WordSet> getWordSets();

    void dictSyncButtonConnect();


    void clearTables();

    void executeRequestFromReadNow(const std::string& original,
                                   const std::string& translation,
                                   const std::string& wordSetTitle,
                                   const std::string& context = "");

    void updateDictionaryChanges();

    void makeIcons();

    void removeIcons();

public slots:
    void addWordSetIconToMenu(int wordSetId, const std::string &title);

    void deleteWordSetIconFromMenu(int wordSetId);
};

#endif  // DICTIONARY_WINDOW_H