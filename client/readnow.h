#ifndef READNOW_H
#define READNOW_H

#include <iostream>
#include "include/sectionbase.h"
#include <QDebug>
#include <QMenu>
#include <QGraphicsScene>
#include <QFile>
#include "YAFR_autogen/include/ui_sectionbase.h"
#include <QTextStream>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QTextEdit>

/*
class ReadNow : public SectionBase {
public:
    explicit ReadNow(QWidget *parent = nullptr) : SectionBase(parent) {
        //this->grabMouse(); //захватывает ввод мыши
    };
};
*/

namespace Ui {
class ReadNow : public Ui::SectionBase {};
}

class ReadNow : public SectionBase {
    Q_OBJECT
public:
    explicit ReadNow(QWidget *parent = nullptr) : SectionBase(parent), ui(new Ui::ReadNow){
                                   ui->setupUi(this);

                                   // сделать функцию
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

                                   textEdit = new QTextEdit(this);
                                   textEdit->append("Я ненавижу свет\n"
                                       "Однообразных звезд.\n"
                                       "Здравствуй, мой давний бред, —\n"
                                       "Башни стрельчатой рост!\n"
                                       "\n"
                                       "Кружевом камень будь\n"
                                       "И паутиной стань:\n"
                                       "Неба пустую грудь\n"
                                       "Тонкой иглою рань.\n"
                                       "\n"
                                       "Будет и мой черед —\n"
                                       "Чую размах крыла.\n"
                                       "Так — но куда уйдет\n"
                                       "Мысли живой стрела?\n"
                                       "\n"
                                       "Или свой путь и срок\n"
                                       "Я, исчерпав, вернусь:\n"
                                       "Там — я любить не мог,\n"
                                       "Здесь — я любить боюсь…");

                                   textEdit->setReadOnly(true);
                                   textEdit->setGeometry(10, 80, width() - 50, this->height() - 50);

                               };
    ~ReadNow() override {
        delete ui;
    }


private:

    QLabel *label;
    QString *currentFile;
    QTextEdit *textEdit;
    Ui::ReadNow *ui;
};

#endif  // READNOW_H
