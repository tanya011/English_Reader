#include "include/connectingWindow.h"
#include <QMenuBar>
#include "include/collectionWindow.h"
#include "include/dictionaryWindow.h"
#include "include/learnWindow.h"
#include "include/libraryWindow.h"
#include "include/readNowWindow.h"

ConnectingWindow::ConnectingWindow(QWidget *parent)
    : allWindows(new QStackedWidget), QMainWindow(parent) {
    addMenu();
}

void ConnectingWindow::setUser(User *u) {
    user = u;
}

void ConnectingWindow::addMenu() {
    collectionAction_ = new QAction("Коллекция", this);
    readNowAction_ = new QAction("Читаю сейчас", this);
    dictionaryAction_ = new QAction("Словарь", this);
    authorizationAction_ = new QAction("Войти", this);
    cardsAction_ = new QAction("Карточки", this);
    libraryAction_ = new QAction("Библиотека", this);

    menuBar()->addAction(libraryAction_);
    menuBar()->addAction(collectionAction_);
    menuBar()->addAction(readNowAction_);
    menuBar()->addAction(dictionaryAction_);
    menuBar()->addAction(cardsAction_);
    menuBar()->addAction(authorizationAction_);

    setCentralWidget(&allWindows);

    QObject::connect(readNowAction_, &QAction::triggered, this, [=]() {
        if (!user->isAuthorized())
            return;
        showReadNow();
    });

    QObject::connect(collectionAction_, &QAction::triggered, this, [=]() {
        if (!user->isAuthorized())
            return;
        showCollection();
    });

    QObject::connect(libraryAction_, &QAction::triggered, this, [=]() {
        if (!user->isAuthorized())
            return;
        updateLibrary();
        showLibrary();
    });

    QObject::connect(dictionaryAction_, &QAction::triggered, this, [=]() {
        if (!user->isAuthorized())
            return;
        updateDictionary();
        showDictionary();
    });

    QObject::connect(authorizationAction_, &QAction::triggered, this,
                     [=]() { showAuth(); });

    QObject::connect(cardsAction_, &QAction::triggered, this, [=]() {
        if (!user->isAuthorized())
            return;
        updateCards();
        showCards();
    });
}

void ConnectingWindow::showLibrary() {
    setWindowTitle("Библиотека");
    allWindows.setCurrentIndex(windowIndexes.library);
}

void ConnectingWindow::showCollection() {
    setWindowTitle("Коллекция");
    allWindows.setCurrentIndex(windowIndexes.collection);
}

void ConnectingWindow::showReadNow() {
    if (this->windowTitle() == "")
        this->setWindowTitle("Книга не выбрана");
    else
        this->setWindowTitle(dynamic_cast<ReadNowWindow *>(
                                 allWindows.widget(windowIndexes.readNow))
                                 ->getAuthorTitle());
    allWindows.setCurrentIndex(windowIndexes.readNow);
}

void ConnectingWindow::showDictionary() {
    setWindowTitle("Словарь");
    allWindows.setCurrentIndex(windowIndexes.dictionary);
    dynamic_cast<DictionaryWindow *>(
        allWindows.widget(windowIndexes.dictionary))
        ->showWordSet(1);
}

void ConnectingWindow::showCards() {
    setWindowTitle("Карточки");
    allWindows.setCurrentIndex(windowIndexes.learn);
}

void ConnectingWindow::showAuth() {
    setWindowTitle("Авторизация");
    allWindows.setCurrentIndex(windowIndexes.auth);
}

std::vector<WordSet> ConnectingWindow::getWordSets() const {
    return dynamic_cast<DictionaryWindow *>(
               allWindows.widget(windowIndexes.dictionary))
        ->getWordSets();
}

void ConnectingWindow::updateLibrary() const {
    dynamic_cast<LibraryWindow *>(allWindows.widget(windowIndexes.library))
        ->updateWindow();
}

void ConnectingWindow::updateReadNow(const std::string &title,
                                     const std::string &author,
                                     const std::string &text) const {
    auto readNow =
        dynamic_cast<ReadNowWindow *>(allWindows.widget(windowIndexes.readNow));
    readNow->printBook(QString::fromStdString(text),
                       QString::fromStdString(author),
                       QString::fromStdString(title));
    if (readNow->emptyTranslatedTextDisplay())
        readNow->cleanTranslatedTextDisplay();

    readNow->updateToolBar();
}

void ConnectingWindow::updateDictionary() const {
    auto dictionary = dynamic_cast<DictionaryWindow *>(
        allWindows.widget(windowIndexes.dictionary));
    dictionary->removeWordSetIcons();
    dictionary->makeWordSetIcons();
}

void ConnectingWindow::updateCards() {
    dynamic_cast<LearnWindow *>(allWindows.widget(windowIndexes.learn))
        ->setWordSets(getWordSets());
}

void ConnectingWindow::updateCollection() const {
    auto collection = dynamic_cast<CollectionWindow *>(
        allWindows.widget(windowIndexes.collection));
    collection->updateWindow();
}