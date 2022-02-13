#include "sectionbase.h"
#include "ui_sectionbase.h"
#include <QMenu>
#include <QMenuBar>


SectionBase::SectionBase(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SectionBase)
{
    ui->setupUi(this);

    //инициализация кнопок, если сделать класс для QActions - реализовать иниициализацию
    collection = new QAction("Коллекция", parent);
    library = new QAction("Каталог", parent);
    reading_now = new QAction("Читаю сейчас", parent);
    dictionary = new QAction("Словарь", parent);
    cards = new QAction("Карточки", parent);
    settings = new QAction("Настройки", parent);
    entrance_exit = new QAction("Вход/Выход", parent);

    sections = menuBar()->addMenu("Меню");

    sections->addAction(collection);
    sections->addAction(library);
    sections->addAction(reading_now);
    sections->addAction(dictionary);
    sections->addAction(cards);
    sections->addAction(settings);
    sections->addAction(entrance_exit);

}

SectionBase::~SectionBase()
{
    delete ui;
}
