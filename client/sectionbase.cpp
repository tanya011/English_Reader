#include "include/sectionbase.h"
#include <QMenu>
#include <QMenuBar>
#include "ui_sectionbase.h"

SectionBase::SectionBase(DBManager& m, QWidget *parent)
    : dbManager(m), QMainWindow(parent), ui(new Ui::SectionBase), readNow(), libraryWindow(m) {
    ui->setupUi(this);


    //инициализация кнопок, если сделать класс для QActions - реализовать
    //иниициализацию

    library = new QAction("Библиотека", this);
    reading_now = new QAction("Читаю сейчас", this);
    //     collection = new QAction("Коллекция", parent);
    //     dictionary = new QAction("Словарь", parent);
    //     cards = new QAction("Карточки", parent);
    //     settings = new QAction("Настройки", parent);
    //     entrance_exit = new QAction("Вход/Выход", parent);

    menuBar()->addAction(library);
    menuBar()->addAction(reading_now);

    //    sections->addAction(collection);
    //    sections->addAction(dictionary);
    //    sections->addAction(cards);
    //    sections->addAction(settings);
    //    sections->addAction(entrance_exit);

    setCentralWidget(&readNow);
    this->setWindowTitle("Книга не выбрана");


    QObject::connect(reading_now, &QAction::triggered, this, [&]() {
        takeCentralWidget();
        setCentralWidget(&readNow);
    });
    QObject::connect(library, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Библиотека");
        takeCentralWidget();
        setCentralWidget(&libraryWindow);
        libraryWindow.connectWithReader(*this, readNow);
    });
}

SectionBase::~SectionBase() {
    delete ui;
}
