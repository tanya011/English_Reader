#include "include/dictionaryWindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include "include/serverProblemsException.h"

DictionaryWindow::DictionaryWindow(WordRep *wordRep,
                                   WordSetRep *wordSetRep,
                                   WordSetContentRep *wordSetContentRep,
                                   ConnectingWindow *parent)
    : QWidget(parent),
      wordRep_(wordRep),
      wordSetRep_(wordSetRep),
      wordSetContentRep_(wordSetContentRep),
      parent_(parent) {
    toolBar_ = new QMenuBar(this);
    layout_ = new QGridLayout();
    scrollArea = new QScrollArea(this);
    wordSets_ = new QMenu("Выбор группы", toolBar_);
    allWords_ = new QAction("Все группы", wordSets_);

    toolBar_->addMenu(wordSets_);
    wordSets_->addAction(allWords_);

    QObject::connect(allWords_, &QAction::triggered, this,
                     [=]() { showWordSet(1); });
    wordSetIconForMenu_[1] = allWords_;

    QObject::connect(wordRep_, &WordRep::wordAdded, wordSetContentRep_,
                     &WordSetContentRep::addWordToSetTable);

    dictSyncButtonConnect();
}

void DictionaryWindow::showWordSet(int wordSetId) {
    for (int i = 0; i < layout_->count(); i++) {
        layout_->itemAt(i)->widget()->deleteLater();
    }
    wordBtnsDeleteFromDictionary_.clear();
    wordBtnsDeleteFromWordSet_.clear();
    wordLabels_.clear();

    int curSize = wordSetContentRep_->getWordSetSize(wordSetId);
    wordBtnsDeleteFromDictionary_.resize(curSize);
    wordBtnsDeleteFromWordSet_.resize(curSize);
    std::vector<int> wordIds = wordSetContentRep_->getWordSetContent(wordSetId);

    box_ = new QWidget;
    layout_ = new QGridLayout;
    box_->setLayout(layout_);

    for (int i = 0; i < curSize; i++) {
        Word curWord = wordRep_->getWordById(wordIds[i]);
        int id = curWord.getId();
        auto word = new QLabel(curWord.getOriginal().c_str());
        auto translation = new QLabel(curWord.getTranslation().c_str());
        wordLabels_.emplace_back(word, translation);

        wordBtnsDeleteFromDictionary_[i] =
            new QPushButton("Удалить из словаря");
        wordBtnsDeleteFromWordSet_[i] = new QPushButton("Удалить из группы");
        QObject::connect(wordBtnsDeleteFromDictionary_[i],
                         &QPushButton::clicked, this, [=]() {
                             wordSetContentRep_->deleteWordFromAllSets(id);
                             wordRep_->deleteWordById(id);
                             showWordSet(wordSetId);
                         });
        QObject::connect(
            wordBtnsDeleteFromWordSet_[i], &QPushButton::clicked, this, [=]() {
                wordSetContentRep_->deleteWordFromSet(wordSetId, id);
                showWordSet(wordSetId);
            });
    }

    for (int i = 0; i < wordLabels_.size(); i++) {
        layout_->addWidget(wordLabels_[i].first, i, 0);
        layout_->addWidget(wordLabels_[i].second, i, 1);
        layout_->addWidget(wordBtnsDeleteFromDictionary_[i], i, 2);
        layout_->addWidget(wordBtnsDeleteFromWordSet_[i], i, 3);
    }

    scrollArea->setWidget(box_);

    // Styles
    auto screen_width =
        QApplication::desktop()->screenGeometry().width() - 1000;
    auto screen_height =
            QApplication::desktop()->screenGeometry().height() - 500;
    this->setStyleSheet("QLabel{margin: 10px;}");
    this->setStyleSheet("QPushButton{font-size: 20pt;}");
    box_->setFixedWidth(screen_width - 20);
    scrollArea->setGeometry(450, 50, screen_width, screen_height);
    // Styles
}

void DictionaryWindow::dictSyncButtonConnect() {
    serverSync_ = new QPushButton(this);
    serverSync_->setGeometry(950, 1200, 300, 100);
    serverSync_->setText("Синхронизация");
    serverSync_->show();
    QObject::connect(serverSync_, &QPushButton::clicked, this, [=]() {
        while (true) {
            try {
                updateDictionaryChanges();
                break;
            } catch (ServerProblemsExceptionReconnect &) {
                continue;
            } catch (ServerProblemsExceptionNotSaveDict &) {
                break;
            }
        }
    });
}

void DictionaryWindow::makeWordSetIcons() {
    std::vector<WordSet> wordSets = wordSetRep_->getWordSets();
    auto *allWordSets = new QAction("Все группы", wordSets_);
    wordSets_->addAction(allWordSets);
    QObject::connect(allWordSets, &QAction::triggered, this,
                     [=]() { showWordSet(1); });
    for (auto &wordSet : wordSets) {
        if (wordSet.getId() == 1) {
            continue;
        }
        auto *action = new QAction(wordSet.getTitle().c_str(), wordSets_);
        wordSets_->addAction(action);
        QObject::connect(action, &QAction::triggered, this,
                         [=]() { showWordSet(wordSet.getId()); });
    }
}

std::vector<WordSet> DictionaryWindow::getWordSets() {
    std::vector<WordSet> wordSets;
    WordSet allWordSets(1, "Все группы");
    std::vector<Word> allWords = wordRep_->getWords();
    for (auto &word : allWords) {
        allWordSets.addWord(word);
    }
    wordSets.push_back(allWordSets);

    for (auto &wordSet : wordSetRep_->getWordSets()) {
        std::vector<int> content =
            wordSetContentRep_->getWordSetContent(wordSet.getId());
        for (auto i : content) {
            Word word(wordRep_->getWordById(i));
            wordSet.addWord(word);
        }
        wordSets.push_back(wordSet);
    }
    return wordSets;
}

void DictionaryWindow::removeWordSetIcons() {
    wordSets_->clear();
}

void DictionaryWindow::executeRequestFromReadNow(
    const std::string &original,
    const std::string &translation,
    const std::string &wordSetTitle,
    const std::string &context) {
    int wordId = wordRep_->addWord(original, translation, context);
    int wordSetId = wordSetRep_->addWordSet(wordSetTitle);
    wordSetContentRep_->addWordToSetTable(wordSetId, wordId);
    wordSetContentRep_->saveHistoryAddWordToSet(wordSetId, wordId);
}

void DictionaryWindow::updateDictionaryChanges() {
    std::deque<HistoryChangeWordRep> wordRepHistory =
        wordRep_->getHistoryChanges();
    std::deque<HistoryChangeWordSetRep> wordSetRepHistory =
        wordSetRep_->getHistoryChanges();
    std::deque<HistoryChangeWordSetContentRep> wordSetContentRepHistory =
        wordSetContentRep_->getHistoryChanges();

    while (!wordRepHistory.empty()) {
        parent_->user->sendWordRepHistoryChange(wordRepHistory.back());
        wordRepHistory.pop_back();
    }
    while (!wordSetRepHistory.empty()) {
        parent_->user->sendWordSetRepHistoryChange(wordSetRepHistory.back());
        wordSetRepHistory.pop_back();
    }
    while (!wordSetContentRepHistory.empty()) {
        parent_->user->sendWordSetContentRepHistoryChange(
            wordSetContentRepHistory.back());
        wordSetContentRepHistory.pop_back();
    }

    wordRep_->clearHistory();
    wordSetRep_->clearHistory();
    wordSetContentRep_->clearHistory();
}