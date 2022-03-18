#include "include/sectionbase.h"
#include <QMenu>
#include "ui_sectionbase.h"

SectionBase::SectionBase(DBManager &m, QWidget *parent)
        : dbManager(m), QMainWindow(parent), ui(new Ui::SectionBase), readNow(), libraryWindow(m),
          auth_action(), cards(this),
          dictionaryWindow() {
    ui->setupUi(this);


    // инициализация кнопок меню
    library = new QAction("Библиотека", this);
    reading_now = new QAction("Читаю сейчас", this);
    dictionary = new QAction("Словарь", this);
    auth_action = new QAction("Войти", parent);
    cards_action = new QAction("Карточки", parent);


    menuBar()->addAction(library);
    menuBar()->addAction(reading_now);
    menuBar()->addAction(dictionary);
    menuBar()->addAction(cards_action);
    //menuBar()->addAction(auth_action);

    setCentralWidget(&readNow);
    this->setWindowTitle("Книга не выбрана");

    readNow.buttonConnectWithDict(*this, dictionaryWindow);

    QObject::connect(reading_now, &QAction::triggered, this, [&]() {
        if (this->windowTitle() == "") {
            this->setWindowTitle("Книга не выбрана");
        } else {
            this->setWindowTitle(readNow.getAuthorTitle());
        }
        takeCentralWidget();
        setCentralWidget(&readNow);
    });
    QObject::connect(library, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Библиотека");
        takeCentralWidget();
        setCentralWidget(&libraryWindow);
        libraryWindow.connectWithReader(*this, readNow);
    });
    QObject::connect(dictionary, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Словарь");
        takeCentralWidget();
        setCentralWidget(&dictionaryWindow);
        dictionaryWindow.show_group(1);
    });
    QObject::connect(auth_action, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Авторизация");
        takeCentralWidget();
        setCentralWidget(&auth);
    });

    QObject::connect(cards_action, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Карточки");
        takeCentralWidget();
        setCentralWidget(&cards);
        cards.setWordSets(getWordSets());
    });
}

SectionBase::~SectionBase() {
    delete ui;
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