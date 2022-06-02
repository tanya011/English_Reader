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

class DictionaryWindow : public QWidget {
Q_OBJECT

    ConnectingWindow *parent_;

    QMenuBar *wordSetsToolsBar_ = new QMenuBar(this);

    QGroupBox *wordsPlacement_ = new QGroupBox(this);

    QPushButton *serverSync_;

    QGridLayout *layout_ = new QGridLayout;

    int curOpenWordSetId_ = 1;

    std::map<int, QAction *>
            wordSetIconForMenu_;  // Helps to add name of wordSet in wordSet menu

    QMenu *wordSets_ = new QMenu("Выбор группы", wordSetsToolsBar_);

    std::vector<QPushButton *> wordBtnsDeleteFromDictionary_;
    std::vector<QPushButton *> wordBtnsDeleteFromWordSet_;

    bool firstOpening_ = true;

    WordRep wordRep_;
    WordSetRep wordSetRep_;
    WordSetContentRep wordSetContentRep_;

public:
    explicit DictionaryWindow(DBManager &m,  ConnectingWindow *parent = nullptr);

    void showWordSet(int wordSetId);

    std::vector<WordSet> getWordSets();

    void dictSyncButtonConnect();

    void downloadDictDataFromServer();

    void clearTables();

    void executeRequestFromReadNow(const std::string& original,
                                   const std::string& translation,
                                   const std::string& wordSetTitle,
                                   const std::string& context = "");

    void updateDictionaryChanges();

    void makeIcons();

public slots:
    void addWordSetIconToMenu(int wordSetId, const std::string &title);

    void deleteWordSetIconFromMenu(int wordSetId);
};

#endif  // DICTIONARY_WINDOW_H