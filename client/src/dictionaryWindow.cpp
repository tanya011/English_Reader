#include <iostream>
#include "include/dictionaryWindow.h"


void DictionaryWindow::showWordSet(int wordSetId){
    for (auto &previousIcon: mLogic_.wordSets_[curOpenWordSetId_].getWords()){
        QLayoutItem* item;
        while ( (item = layout1->takeAt(0)) != nullptr){
            delete item->widget();
            delete item;
        }
        wordBtnsDeleteFromDictionary_.clear();
        wordBtnsDeleteFromWordSet_.clear();
    }
    curOpenWordSetId_ = wordSetId;
    wordBtnsDeleteFromDictionary_.resize(
        mLogic_.wordSets_[wordSetId].getWords().size());
    wordBtnsDeleteFromWordSet_.resize(
        mLogic_.wordSets_[wordSetId].getWords().size());
    int index = 0;
    int height = 0;
    for (auto &curWord: mLogic_.wordSets_[wordSetId].getWords()) {
        std::string fullWord = curWord.second->getOriginal() + " - " +  curWord.second->getTranslation();
        auto *word = new QTextEdit(fullWord.c_str());
        word->setMaximumHeight(100);
        height += word->height();
        layout1->addWidget(word, index, 0);
        wordBtnsDeleteFromDictionary_[index] = new QPushButton("Удалить из словаря");
        wordBtnsDeleteFromWordSet_[index] = new QPushButton("Удалить из группы");
        QObject::connect(wordBtnsDeleteFromDictionary_[index], &QPushButton::clicked, &mLogic_, [=](){
                             mLogic_.deleteWordFromDictionary(curWord.first);
                             showWordSet(wordSetId);
        });
        QObject::connect(wordBtnsDeleteFromWordSet_[index], &QPushButton::clicked, &mLogic_, [=](){
                             mLogic_.deleteWordFromWordSet(curWord.first,
                                                           wordSetId);
                showWordSet(wordSetId);
        });
        layout1->addWidget(wordBtnsDeleteFromDictionary_[index], index, 1);
        layout1->addWidget(wordBtnsDeleteFromWordSet_[index], index, 2);
        index++;
    }
   // wordsPlacement_->setGeometry({0, 10, 1850, 70 + height});
}

void DictionaryWindow::update(){

}

void DictionaryWindow::addWordSetIconToMenu(int wordSetId, const std::string& title){
    auto* action = new QAction(title.c_str(), this);
    wordSets_->addAction(action);
    QObject::connect(action, &QAction::triggered, this, [=]() { showWordSet(wordSetId);
    });
    wordSetIconForMenu_[wordSetId] = action;
}

void DictionaryWindow::deleteWordSetIconFromMenu(int wordSetId){
    wordSets_->removeAction(wordSetIconForMenu_[wordSetId]);
    wordSetIconForMenu_.erase(wordSetId);
}

DictionaryWindow::DictionaryWindow(ConnectingWindow *parent): QWidget(parent){

    button = new QPushButton;
    button->setParent(this);
    button->setGeometry(30, 1370, 300, 70);
    button->setText("Update");
    button->show();

    connect(button, &QPushButton::clicked, this,
            &DictionaryWindow::update);

    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollArea->setWidgetResizable( true );
    scrollArea->setGeometry( 10, 50, 2200, 1300 );


    QWidget *widget = new QWidget();
    scrollArea->setWidget( widget );

    QVBoxLayout *layout = new QVBoxLayout();
    widget->setLayout( layout1 );

    wordSetsToolsBar_->addMenu(wordSets_);

    auto* allWords = new QAction("Все слова", wordSets_);

    wordSets_->addAction(allWords);

    mLogic_.addAllWordsToWordSets();

    auto* addWordSet = new QAction("Добавить группу", this);

    wordSetsToolsBar_->addAction(addWordSet);

    QObject::connect(&mLogic_, &Dictionary::groupWasCreated, this,
                     &DictionaryWindow::addWordSetIconToMenu);
    QObject::connect(&mLogic_, &Dictionary::wordSetWasDeleted, this,
                     &DictionaryWindow::deleteWordSetIconFromMenu);

    QObject::connect(allWords, &QAction::triggered, this, [&]() { showWordSet(1);
    });

    QObject::connect(addWordSet, &QAction::triggered, this, [&](){
        bool ok;
        QString wordSetName = QInputDialog::getText(this, tr("Новая группа"), tr("Название группы:"), QLineEdit::Normal, "", &ok);
        if (ok && !wordSetName.isEmpty())
            mLogic_.createWordSet(wordSetName.toStdString());
    });

    QObject::connect(&mLogic_, &Dictionary::wordWasAddedToDictionary, &mLogic_, &Dictionary::addWordAllGroups);

    mLogic_.downloadDictionary();
}
std::vector<WordSet> DictionaryWindow::getWordSets() {
    std::vector<WordSet> wordSets;
    for(auto g: mLogic_.wordSets_){
        wordSets.push_back(g.second);
    }
    return wordSets;
}
