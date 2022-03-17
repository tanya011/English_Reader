#include "include/sectionbase.h"
#include <QMenu>
#include <QMenuBar>
#include "ui_sectionbase.h"

SectionBase::SectionBase(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::SectionBase),
      readNow(),
      libraryWindow(this),
      cards(this) {
    ui->setupUi(this);

    //инициализация кнопок, если сделать класс для QActions - реализовать
    //иниициализацию

    library = new QAction("Библиотека", this);
    reading_now = new QAction("Читаю сейчас", this);
    //     collection = new QAction("Коллекция", parent);
    //     dictionary = new QAction("Словарь", parent);
    cards_action = new QAction("Карточки", parent);
    //     settings = new QAction("Настройки", parent);
    //     entrance_exit = new QAction("Вход/Выход", parent);

    sections = menuBar()->addMenu("Меню");

    sections->addAction(library);
    sections->addAction(reading_now);
    //    sections->addAction(collection);
    //    sections->addAction(dictionary);
    sections->addAction(cards_action);
    //    sections->addAction(settings);
    //    sections->addAction(entrance_exit);

    setCentralWidget(&cards);

    QObject::connect(reading_now, &QAction::triggered, this, [&]() {
        takeCentralWidget();
        setCentralWidget(&readNow);
    });
    QObject::connect(library, &QAction::triggered, this, [&]() {
        takeCentralWidget();
        setCentralWidget(&libraryWindow);
    });
    QObject::connect(cards_action, &QAction::triggered, this, [&]() {
        takeCentralWidget();
        setCentralWidget(&cards);
    });
}

SectionBase::~SectionBase() {
    delete ui;
}
