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
#include <QListWidget>
#include <QClipboard>

/*
class ReadNow : public SectionBase {
public:
    explicit ReadNow(QWidget *parent = nullptr) : SectionBase(parent) {
        //this->grabMouse(); //захватывает ввод мыши
    };
};
*/
namespace translate {
std::string translate(const std::string &text);
}
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

                                   printTextInBox();

                                   connect(this->textEdit, &QTextEdit::copyAvailable, this, &ReadNow::textSelected); // emmited when you select the text
                                   clipboard = QApplication::clipboard();

                                   text = new QTextEdit(this);
                                   text->setGeometry(width() + 50, 80, width(), height() - 50);
                               };
    ~ReadNow() override {
        delete ui;
    }

// todo передавать в эту функцию любую строку или любой текстовый файл
    void printTextInBox(){
        textEdit = new QTextEdit(this);

        textEdit->append("wo roads diverged in a yellow wood,\n"
            "And sorry I could not travel both\n"
            "And be one traveler, long I stood\n"
            "And looked down one as far as I could\n"
            "To where it bent in the undergrowth.\n"
            "\n"
            "Then took the other, as just as fair,\n"
            "And having perhaps the better claim,\n"
            "Because it was grassy and wanted wear;\n"
            "Though as for that the passing there\n"
            "Had worn them really about the same.\n"
            "\n"
            "And both that morning equally lay\n"
            "In leaves no step had trodden black.\n"
            "Oh, I kept the first for another day!\n"
            "Yet knowing how way leads on to way,\n"
            "I doubted if I should ever come back.\n"
            "\n"
            "I shall be telling this with a sigh\n"
            "Somewhere ages and ages hence:\n"
            "Two roads diverged in a wood, and I—\n"
            "I took the one less traveled by,\n"
            "And that has made all the difference.\n"
            );

        textEdit->setReadOnly(true);
        textEdit->setGeometry(10, 80, width() - 50, this->height() - 50);
    }
private slots:
    void textSelected(bool yes) // Slot called only when you select text in your field
    {
        if (yes){
            qDebug() << this->textEdit->toPlainText();
            connect(clipboard, &QClipboard::dataChanged, this, &ReadNow::changeTextCopiedToCB); // wait tor CTRL+C
        }
    }
    void changeTextCopiedToCB() // Once CTRL+C .. the data in clipboard changes..thats my data
    {
        text->clear();
        QString line = clipboard->text();
        std::string l = line.toStdString();
        text->append(translate::translate(l).c_str());
        disconnect(clipboard, &QClipboard::dataChanged, this, &ReadNow::changeTextCopiedToCB); // after copy from this field, leave clipboard alone!
    }

private:
    QClipboard *clipboard;
private:
    QTextEdit *textEdit;
    QTextEdit *text;
    QLabel *label;
    QString *currentFile;

    Ui::ReadNow *ui;
};

#endif  // READNOW_H
