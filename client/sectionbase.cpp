#include "include/sectionbase.h"
#include <QMenu>
#include <QMenuBar>
#include "ui_sectionbase.h"

SectionBase::SectionBase(DBManager& m, QWidget *parent)
    : dbManager(m), QMainWindow(parent), ui(new Ui::SectionBase), readNow(), libraryWindow(m),
    auth_action(){
    ui->setupUi(this);


    //инициализация кнопок, если сделать класс для QActions - реализовать
    //иниициализацию

    library = new QAction("Библиотека", this);
    reading_now = new QAction("Читаю сейчас", this);
    //     collection = new QAction("Коллекция", parent);
    dictionary = new QAction("Словарь", this);
    //     cards = new QAction("Карточки", parent);
    //     settings = new QAction("Настройки", parent);
    auth_action = new QAction("Войти", parent);

    menuBar()->addAction(library);
    menuBar()->addAction(reading_now);
    menuBar()->addAction(dictionary);
    //    sections->addAction(collection);
    //    sections->addAction(dictionary);
    //    sections->addAction(cards);
    //    sections->addAction(settings);
    menuBar()->addAction(auth_action);

    setCentralWidget(&readNow);
    this->setWindowTitle("Книга не выбрана");

    readNow.buttonConnectWithDict(*this, dictionaryWindow);

    QObject::connect(reading_now, &QAction::triggered, this, [&]() {
        this->setWindowTitle("Читаю сейчас");
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
        takeCentralWidget();
        setCentralWidget(&auth);
    });
}

SectionBase::~SectionBase()
{
    delete ui;
}
