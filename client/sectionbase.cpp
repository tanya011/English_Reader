#include "include/sectionbase.h"
#include <QMenu>
#include "ui_sectionbase.h"

SectionBase::SectionBase(DBManager &m, QWidget *parent)
        : dbManager_(m), QMainWindow(parent), ui(new Ui::SectionBase), readNowWindow_(), libraryWindow_(m),
          authorizationWindow_(), cardsWindow_(this),
          dictionaryWindow_() {

    ui->setupUi(this);
    creatMenu();
}

SectionBase::~SectionBase() {
    delete ui;
}

std::vector<WordSet> SectionBase::getWordSets() {
    return dictionaryWindow_.getWordSets();
}

void SectionBase::creatMenu() {
    // инициализация кнопок меню
    libraryAction_ = new QAction("Библиотека", this);
    readNowAction_ = new QAction("Читаю сейчас", this);
    dictionaryAction_ = new QAction("Словарь", this);
    authorizationAction_ = new QAction("Войти", this);
    cardsAction_ = new QAction("Карточки", this);

    menuBar()->addAction(libraryAction_);
    menuBar()->addAction(readNowAction_);
    menuBar()->addAction(dictionaryAction_);
    menuBar()->addAction(cardsAction_);
    menuBar()->addAction(authorizationAction_);

    setCentralWidget(&readNowWindow_);
    this->setWindowTitle("Книга не выбрана");

    QObject::connect(readNowAction_, &QAction::triggered, this, [&]() {
        if (this->windowTitle() == "") {
            this->setWindowTitle("Книга не выбрана");
        } else {
            this->setWindowTitle(readNowWindow_.getAuthorTitle());
        }
        takeCentralWidget();
        setCentralWidget(&readNowWindow_);
    });
    QObject::connect(libraryAction_, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Библиотека");
        takeCentralWidget();
        setCentralWidget(&libraryWindow_);
        libraryWindow_.connectWithReader(*this, readNowWindow_);
    });

    QObject::connect(dictionaryAction_, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Словарь");
        takeCentralWidget();
        setCentralWidget(&dictionaryWindow_);
        dictionaryWindow_.show_group(1);
    });
    QObject::connect(authorizationAction_, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Авторизация");
        takeCentralWidget();
        setCentralWidget(&authorizationWindow_);
    });

    QObject::connect(cardsAction_, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Карточки");
        takeCentralWidget();
        setCentralWidget(&cardsWindow_);
        cardsWindow_.setWordSets(getWordSets());
    });
}

/*
399
10
0


351
their greatest fear
их самый большой страх
352
for several years
на несколько лет
353
shudder
содрогаться
354
tawny
рыжевато-коричневый
355
on the corner
на углу
356
How exciting!
Как здорово!
357
in the yard
во дворе
358
nothing more, nothing less
ни больше ни меньше
359
forever
навсегда
378
4
10
1
Все группы
0
351
352
353
354
355
356
357
358
359
5
364
Joanne Katheline Rowling
 Harry Potter and the Philosopher's Stone
351
352
353
354
355
2
365
Nicole Irving Hachiko
356
357
2
366
Paul Shipton
 The Beatles
358
359
 */