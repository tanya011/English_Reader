#include "include/dictionaryWindow.h"
#include "include/serverProblemsException.h"

void DictionaryWindow::showWordSet(int wordSetId) {
    int prevSize = wordSetContentRep_->getWordSetSize(curOpenWordSetId_);
    for (int i = 0; i < prevSize; i++) {
        QLayoutItem *item;
        while ((item = layout_->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        wordBtnsDeleteFromDictionary_.clear();
        wordBtnsDeleteFromWordSet_.clear();
    }
    curOpenWordSetId_ = wordSetId;
    int curSize = wordSetContentRep_->getWordSetSize(wordSetId);
    wordBtnsDeleteFromDictionary_.resize(curSize);
    wordBtnsDeleteFromWordSet_.resize(curSize);
    int index = 0;
    int height = 0;
    std::vector<int> wordIds = wordSetContentRep_->getWordSetContent(wordSetId);
    for (int i = 0; i < curSize; i++) {
        Word curWord = wordRep_->getWordById(wordIds[i]);
        int id = curWord.getId();
        std::string fullWord = curWord.getOriginal() + " - " +
                               curWord.getTranslation();
        auto *word = new QTextEdit(fullWord.c_str());
        word->setReadOnly(true);
        word->setMaximumHeight(100);
        height += word->height();
        layout_->addWidget(word, index, 0);
        wordBtnsDeleteFromDictionary_[index] =
                new QPushButton("Удалить из словаря");
        wordBtnsDeleteFromWordSet_[index] =
                new QPushButton("Удалить из группы");
        QObject::connect(wordBtnsDeleteFromDictionary_[index],
                         &QPushButton::clicked, this, [=]() {
                    wordSetContentRep_->deleteWordFromAllSets(id);
                    wordRep_->deleteWordById(id);
                    showWordSet(wordSetId);
                });
        QObject::connect(wordBtnsDeleteFromWordSet_[index],
                         &QPushButton::clicked, this, [=]() {
                    wordSetContentRep_->deleteWordFromSet(wordSetId,
                                                          id);
                    showWordSet(wordSetId);
                });
        layout_->addWidget(wordBtnsDeleteFromDictionary_[index], index, 1);
        layout_->addWidget(wordBtnsDeleteFromWordSet_[index], index, 2);
        index++;
    }
    wordsPlacement_->setGeometry({0, 10, 1850, 70 + height});
}

void DictionaryWindow::addWordSetIconToMenu(int wordSetId,
                                            const std::string &title) {
    auto *action = new QAction(title.c_str(), this);
    wordSets_->addAction(action);
    std::cout << "icon made";
    QObject::connect(action, &QAction::triggered, this,
                     [=]() { showWordSet(wordSetId); });
    wordSetIconForMenu_[wordSetId] = action;
}

void DictionaryWindow::deleteWordSetIconFromMenu(int wordSetId) {
    wordSets_->removeAction(wordSetIconForMenu_[wordSetId]);
    wordSetIconForMenu_.erase(wordSetId);
}

DictionaryWindow::DictionaryWindow(WordRep *wordRep, WordSetRep *wordSetRep, WordSetContentRep *wordSetContentRep ,  ConnectingWindow *parent)
        : QWidget(parent), wordRep_(wordRep), wordSetRep_(wordSetRep), wordSetContentRep_(wordSetContentRep), parent_(parent) {

    wordSetsToolsBar_->addMenu(wordSets_);

    wordSets_->addAction(allWords_);

    QObject::connect(allWords_, &QAction::triggered, this,
                     [=]() { showWordSet(1); });
    wordSetIconForMenu_[1] = allWords_;

    QObject::connect(wordRep_, &WordRep::wordCreated,
                     wordSetContentRep_, &WordSetContentRep::addWordToSetTable);

    wordsPlacement_->setGeometry({0, 10, 1850, 4000});
    wordsPlacement_->setLayout(layout_);

    dictSyncButtonConnect();
}
std::vector<WordSet> DictionaryWindow::getWordSets() {
    std::vector<WordSet> wordSets;
    WordSet allWordSets(1, "Все группы");
    std::vector<Word> allWords = wordRep_->getWords();
    for (auto &word: allWords){
        allWordSets.addWord(word);
    }
    wordSets.push_back(allWordSets);

    for (auto &g : wordSetRep_->getWordSets()) {
        std::vector<int> content = wordSetContentRep_->getWordSetContent(g.getId());
        for (auto i : content){
            Word word(wordRep_->getWordById(i));
            g.addWord(word);
        }
        wordSets.push_back(g);
    }
    return wordSets;
}

void DictionaryWindow::dictSyncButtonConnect() {
    serverSync_ = new QPushButton(this);
    serverSync_->setGeometry(750, 850, 300, 100);
    serverSync_->setText("Синхронизация");
    serverSync_->show();
    QObject::connect(serverSync_, &QPushButton::clicked, this, [=](){
        while(true) {
            try {
                updateDictionaryChanges();
                break;
            } catch (ServerProblemsExceptionReconnect &){
                continue;
            } catch (ServerProblemsExceptionNotSaveDict &){
                break;
            }
        }
    });
}

void DictionaryWindow::clearTables() {
    wordRep_->clear();
    wordSetRep_->clear();
    wordSetContentRep_->clear();
}

void DictionaryWindow::executeRequestFromReadNow(const std::string& original, const std::string& translation,
                                                 const std::string& wordSetTitle, const std::string& context) {
    int wordId = wordRep_->addWord(original, translation, context);
    int wordSetId = wordSetRep_->addWordSet(wordSetTitle);
    wordSetContentRep_->addWordToSetTable( wordSetId,wordId);
    wordSetContentRep_->saveHistoryAddWordToSet(wordSetId, wordId);
}

void DictionaryWindow::makeIcons(){
    std::vector<WordSet> wordSets = wordSetRep_->getWordSets();
    auto * allWordSets = new QAction("Все группы", wordSets_);
    wordSets_->addAction(allWordSets);
    QObject::connect(allWordSets, &QAction::triggered, this, [=](){
        showWordSet(1);
    });
    for (auto &wordSet: wordSets){
        if (wordSet.getId() == 1){
            continue;
        }
        auto * action = new QAction(wordSet.getTitle().c_str(), wordSets_);
        wordSets_->addAction(action);
        QObject::connect(action, &QAction::triggered, this, [=](){
            showWordSet(wordSet.getId());
        });
    }
}

void DictionaryWindow::updateDictionaryChanges() {
    std::deque<HistoryChangeWordRep> wordRepHistory = wordRep_->getHistoryChanges();
    std::deque<HistoryChangeWordSetRep> wordSetRepHistory = wordSetRep_->getHistoryChanges();
    std::deque<HistoryChangeWordSetContentRep> wordSetContentRepHistory = wordSetContentRep_->getHistoryChanges();

    while (!wordRepHistory.empty()){
        parent_->user->sendWordRepHistoryChange(wordRepHistory.back());
        wordRepHistory.pop_back();
    }
    while (!wordSetRepHistory.empty()){
        parent_->user->sendWordSetRepHistoryChange(wordSetRepHistory.back());
        wordSetRepHistory.pop_back();
    }
    while (!wordSetContentRepHistory.empty()){
        parent_->user->sendWordSetContentRepHistoryChange(wordSetContentRepHistory.back());
        wordSetContentRepHistory.pop_back();
    }

    wordRep_->clearHistory();
    wordSetRep_->clearHistory();
    wordSetContentRep_->clearHistory();

}

void DictionaryWindow::removeIcons() {
    wordSets_->clear();
}