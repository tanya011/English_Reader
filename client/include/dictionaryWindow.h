#ifndef DICTIONARY_WINDOW_H
#define DICTIONARY_WINDOW_H

#include <QGridLayout>
#include <QGroupBox>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QTextEdit>
#include <QScrollArea>
#include <QWidget>
#include <fstream>
#include "connectingWindow.h"
#include "dictionary.h"

class DictionaryWindow : public QWidget {
    QMenuBar *wordSetsToolsBar_ = new QMenuBar(this);

    QPushButton *button ;

   // QGroupBox *wordsPlacement_ = new QGroupBox(this);

    //QGridLayout *layout_ = new QGridLayout;

    QGridLayout *layout1 = new QGridLayout;


    QScrollArea *scrollArea =  new QScrollArea(this);

    int curOpenWordSetId_ = 1;

    std::map<int, QAction *>
        wordSetIconForMenu_;  // Helps to add name of wordSet in wordSet menu

    QMenu *wordSets_ = new QMenu("Выбор группы", wordSetsToolsBar_);

    std::vector<QPushButton *> wordBtnsDeleteFromDictionary_;
    std::vector<QPushButton *> wordBtnsDeleteFromWordSet_;

public:
    explicit DictionaryWindow(ConnectingWindow *parent = nullptr);

    Dictionary mLogic_;

    void showWordSet(int wordSetId);

    std::vector<WordSet> getWordSets();

private slots:
    void update();

public slots:
    void addWordSetIconToMenu(int wordSetId, const std::string &title);

    void deleteWordSetIconFromMenu(int wordSetId);

    void makeWordIcon(int wordId,
                      int wordSetId,
                      std::string textForVisualisation);

    void removeWordIcon(int wordId, int wordSetId);

    void removeWordSetsIcons(int wordSetId);


};

#endif  // DICTIONARY_WINDOW_H
