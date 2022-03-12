#include "include/sectionbase.h"
#include <QMenu>
#include <QMenuBar>
#include "ui_sectionbase.h"

SectionBase::SectionBase(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SectionBase), readNow(), libraryWindow() {
    ui->setupUi(this);

    //инициализация кнопок, если сделать класс для QActions - реализовать
    //иниициализацию

    library = new QAction("Библиотека", this);
    reading_now = new QAction("Читаю сейчас", this);
    //     collection = new QAction("Коллекция", parent);
    dictionary = new QAction("Словарь", this);
    //     cards = new QAction("Карточки", parent);
    //     settings = new QAction("Настройки", parent);
    //     entrance_exit = new QAction("Вход/Выход", parent);

    sections = menuBar()->addMenu("Меню");

    sections->addAction(library);
    sections->addAction(reading_now);
    //    sections->addAction(collection);
    sections->addAction(dictionary);
    //    sections->addAction(cards);
    //    sections->addAction(settings);
    //    sections->addAction(entrance_exit);

    this->setWindowTitle("Читаю сейчас");
    setCentralWidget(&readNow);

    QObject::connect(reading_now, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Читаю сейчас");
        takeCentralWidget();
        setCentralWidget(&readNow);
    });
    QObject::connect(library, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Библиотека");
        takeCentralWidget();
        setCentralWidget(&libraryWindow);
    });
    QObject::connect(dictionary, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Словарь");
        takeCentralWidget();
        setCentralWidget(&dictionaryWindow);
    });
}

SectionBase::~SectionBase() {
    delete ui;
}
