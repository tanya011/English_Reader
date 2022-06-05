#ifndef DICTIONARY_WINDOW_H
#define DICTIONARY_WINDOW_H

#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include <QWidget>

#include "../include/wordset.h"
#include "connectingWindow.h"
#include "wordRep.h"
#include "wordSetContentRep.h"
#include "wordSetRep.h"

class DictionaryWindow : public QWidget {
    Q_OBJECT


    ConnectingWindow *parent_;

    std::vector<std::pair<QLabel *, QLabel *>> wordLabels_;


    QMenuBar *toolBar_;
    QGridLayout *layout_;
    QWidget *box_;
    QScrollArea *scrollArea;


    std::map<int, QAction *>
        wordSetIconForMenu_;

    QMenu *wordSets_;
    QAction *allWords_;

    std::vector<QPushButton *> wordBtnsDeleteFromDictionary_;
    std::vector<QPushButton *> wordBtnsDeleteFromWordSet_;

    WordRep *wordRep_;
    WordSetRep *wordSetRep_;
    WordSetContentRep *wordSetContentRep_;

public:
    explicit DictionaryWindow(WordRep *wordRep,
                              WordSetRep *wordSetRep,
                              WordSetContentRep *wordSetContentRep,
                              ConnectingWindow *parent = nullptr);

    void showWordSet(int wordSetId);

    void makeWordSetIcons();

    void removeWordSetIcons();

    std::vector<WordSet> getWordSets();

    void executeRequestFromReadNow(const std::string &original,
                                   const std::string &translation,
                                   const std::string &wordSetTitle,
                                   const std::string &context = "");

    void updateDictionaryChanges();
};

#endif  // DICTIONARY_WINDOW_H