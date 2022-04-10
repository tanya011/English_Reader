#include "include/dictionaryWindow.h"


void DictionaryWindow::showWordSet(int wordSetId){
    for (auto &previousIcon: mLogic_.wordSets_[curOpenWordSetId_].getWords()){
        QLayoutItem* item;
        while ( (item = layout_->takeAt(0)) != nullptr){
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
        layout_->addWidget(word, index, 0);
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
        layout_->addWidget(wordBtnsDeleteFromDictionary_[index], index, 1);
        layout_->addWidget(wordBtnsDeleteFromWordSet_[index], index, 2);
        index++;
    }
    wordsPlacement_->setGeometry({0, 10, 1850, 70 + height});
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

DictionaryWindow::DictionaryWindow(QWidget *parent): QWidget(parent){

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

    mLogic_.downloadDictionary();

}
