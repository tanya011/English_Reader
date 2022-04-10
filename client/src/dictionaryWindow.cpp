#include "include/dictionaryWindow.h"
#include <QPushButton>
#include<iostream>
#include <QDebug>
#include <QInputDialog>


void DictionaryWindow::showWordSet(int wordsetId){
    //qDebug() << wordsetId;
    for (auto &previousIcon: mLogic_.wordSets_[curOpenWordSetId_].getWords()){
        QLayoutItem* item;
        while ( (item = layout_->takeAt(0)) != nullptr){
            delete item->widget();
            delete item;
        }
        wordBtnsDeleteFromDictionary_.clear();
        wordBtnsDeleteFromWordSet_.clear();
    }
    curOpenWordSetId_ = wordsetId;
    wordBtnsDeleteFromDictionary_.resize(
        mLogic_.wordSets_[wordsetId].getWords().size());
    wordBtnsDeleteFromWordSet_.resize(
        mLogic_.wordSets_[wordsetId].getWords().size());
    int index = 0;
    int height = 0;
    for (auto &curWord: mLogic_.wordSets_[wordsetId].getWords()) {
        std::string fullWord = curWord.second->getOriginal() + " - " +  curWord.second->getTranslation();
        auto *word = new QTextEdit(fullWord.c_str());
        word->setMaximumHeight(100);
        height += word->height();
        layout_->addWidget(word, index, 0);
        wordBtnsDeleteFromDictionary_[index] = new QPushButton("Удалить из словаря");
        wordBtnsDeleteFromWordSet_[index] = new QPushButton("Удалить из группы");
        QObject::connect(wordBtnsDeleteFromDictionary_[index], &QPushButton::clicked, &mLogic_, [=](){
                             mLogic_.deleteWordFromDictionary(curWord.first);
                             showWordSet(wordsetId);
        });
        QObject::connect(wordBtnsDeleteFromWordSet_[index], &QPushButton::clicked, &mLogic_, [=](){
                             mLogic_.deleteWordFromWordSet(curWord.first, wordsetId);
                showWordSet(wordsetId);
        });
        layout_->addWidget(wordBtnsDeleteFromDictionary_[index], index, 1);
        layout_->addWidget(wordBtnsDeleteFromWordSet_[index], index, 2);
        index++;
    }
    wordsPlacement_->setGeometry({0, 10, 1850, 70 + height});
}

void DictionaryWindow::addWordSetIconToMenu(int wordset_id, const std::string& title){
    auto* action = new QAction(title.c_str(), this);
    wordSets_->addAction(action);
    QObject::connect(action, &QAction::triggered, this, [=]() { showWordSet(wordset_id);
    });
    wordSetIconForMenu_[wordset_id] = action;
}

void DictionaryWindow::deleteWordSetIconFromMenu(int wordset_id){
    wordSets_->removeAction(wordSetIconForMenu_[wordset_id]);
    wordSetIconForMenu_.erase(wordset_id);
}

DictionaryWindow::DictionaryWindow(QWidget *parent): QWidget(parent){

    QObject::connect(&mLogic_, &Dictionary::groupWasCreated, this,
                     &DictionaryWindow::addWordSetIconToMenu);
    QObject::connect(&mLogic_, &Dictionary::wordSetWasDeleted, this,
                     &DictionaryWindow::deleteWordSetIconFromMenu);

    wordSetsToolsBar_->addMenu(wordSets_);

    auto* all_groups = new QAction("Все слова", wordSets_);

    QObject::connect(all_groups, &QAction::triggered, this, [&]() { showWordSet(1);
    });

    wordSets_->addAction(all_groups);

    mLogic_.addAllWordsToWordSets();

    auto* addWordset = new QAction("Добавить группу", this);

    wordSetsToolsBar_->addAction(addWordset);

    QObject::connect(addWordset, &QAction::triggered, this, [&](){
        bool ok;
        QString wordsetName = QInputDialog::getText(this, tr("Новая группа"), tr("Название группы:"), QLineEdit::Normal, "", &ok);
        if (ok && !wordsetName.isEmpty())
            mLogic_.createWordSet(wordsetName.toStdString());
    });

    QObject::connect(&mLogic_, &Dictionary::wordWasAddedToDictionary, &mLogic_, &Dictionary::addWordAllGroups);

    int q = mLogic_.addWord("hello", "привет");
    int w = mLogic_.addWord("good", "хорошо");
    int z = mLogic_.addWord("word", "слово");

    int x = mLogic_.createWordSet("Гарри Поттер");

    mLogic_.addWordToWordSet(q, x);
    mLogic_.addWordToWordSet(w, x);

    int y = mLogic_.createWordSet("Портрет Дориана Грея");

    mLogic_.addWordToWordSet(z, y);

    wordsPlacement_->setGeometry({0, 10, 1850, 4000});
    wordsPlacement_->setLayout(layout_);
    
    std::ifstream file("dictionaryData.txt"); //файл лежит в cmake-build-debug
    std::string curString;
    getline(file, curString);
    if (!curString.empty()) {
        int counterWordId = std::stoi(curString);
        Word::setIdCounter(counterWordId);
        getline(file, curString);
        int wordsNumber = std::stoi(curString);
        for (int word = 0; word < wordsNumber; word++) {
            getline(file, curString);
            int id = std::stoi(curString);

            getline(file, curString);
            std::string original = curString;

            getline(file, curString);
            std::string translation = curString;

            mLogic_.addWord(original, translation, id);
        }
        getline(file, curString);
        int counterWordsetId = std::stoi(curString);
        WordSet::setIdCounter(counterWordsetId);

        getline(file, curString);
        int wordsetNumber = std::stoi(curString);

        for (int wordset = 0; wordset < wordsetNumber; wordset++) {
            getline(file, curString);
            int wordsetSize = std::stoi(curString);

            getline(file, curString);
            int wordsetId = std::stoi(curString);

            getline(file, curString);
            std::string wordsetName = curString;

            mLogic_.createWordSet(wordsetName, wordsetId);

            for (int word = 0; word < wordsetSize; word++) {
                getline(file, curString);
                int wordId = std::stoi(curString);
                mLogic_.addWordToWordSet(wordId, wordsetId);
            }
        }
        file.close();
    }
    else{
        file.close();
    }


}
