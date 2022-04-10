#ifndef DICTIONARY_WINDOW_H
#define DICTIONARY_WINDOW_H


#include "dictionary.h"

#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QGroupBox>
#include <QTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QInputDialog>
#include <fstream>


class DictionaryWindow : public QWidget {

    QMenuBar* wordSetsToolsBar_ = new QMenuBar(this);

    QGroupBox* wordsPlacement_ = new QGroupBox(this);

    QGridLayout* layout_ = new QGridLayout;

    int curOpenWordSetId_ = 1;

    std::map<int, QAction*>
        wordSetIconForMenu_;  // Helps to add name of wordSet in wordSet menu


    QMenu* wordSets_ = new QMenu("Выбор группы", wordSetsToolsBar_);

    std::vector<QPushButton *> wordBtnsDeleteFromDictionary_;
    std::vector<QPushButton *> wordBtnsDeleteFromWordSet_;

public:
    explicit DictionaryWindow(QWidget *parent = nullptr);

    Dictionary mLogic_;

    void showWordSet(int wordSetId);

    inline std::vector<WordSet> getWordSets(){
        std::vector<WordSet> wordSets;
        for(auto g: mLogic_.wordSets_){
            wordSets.push_back(g.second);
        }
        return wordSets;
    }

public slots:
    void addWordSetIconToMenu(int wordSetId, const std::string& title);

    void deleteWordSetIconFromMenu(int wordSetId);

    void makeWordIcon(int wordId, int wordSetId, std::string textForVisualisation);

    void removeWordIcon(int wordId, int wordSetId);

    void removeWordSetsIcons(int wordSetId);
};


#endif // DICTIONARY_WINDOW_H
