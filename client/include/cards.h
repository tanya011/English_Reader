#ifndef YAFR_CARDS_H
#define YAFR_CARDS_H

#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QDesktopWidget>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <iostream>
#include <vector>
#include "cards_display.h"
#include "dictionary.h"
#include "sectionbase.h"
#include "wordset.h"

class Cards : public QWidget {
public:
    explicit Cards(QWidget *parent = nullptr) : QWidget(parent) {
        screen_width = QApplication::desktop()->screenGeometry().width();
        screen_height = QApplication::desktop()->screenGeometry().height();

        printHeading();
        printButton();
    }
    void setWordSets(std::vector<WordSet> ws) {
        allWordSets = ws;
        printBox();
    }

private slots:
    void button_clicked() {
        for (auto wordSetInCheck : m_checkBoxVector) {
            if (wordSetInCheck.checkBox->isChecked()) {
                checkedWordSets.push_back(wordSetInCheck.wordSet);
                // std::cout << wordSetInCheck.wordSet.getTitle() << " ";
            }
        }
        auto parent = dynamic_cast<QMainWindow *>(parentWidget());
        parent->takeCentralWidget();
        auto cardsDisplay = new CardsDisplay(checkedWordSets, parent);
        parent->setCentralWidget(cardsDisplay);
    }

private:
    int screen_width;
    int screen_height;

    struct WordSetsInCheckBox {
        QCheckBox *checkBox;
        WordSet wordSet;
    };
    std::vector<WordSetsInCheckBox> m_checkBoxVector;
    std::vector<WordSet> checkedWordSets;
    std::vector<WordSet> allWordSets;

    void printHeading() {
        auto *text = new QLabel(this);
        text->setText("Выберите группы");
        text->setGeometry(screen_width / 2 - 200, 10, 500, 100);
        this->setStyleSheet("QLabel{font-size: 11pt;}");
    }

    void printBox() {
        auto *scroll = new QScrollArea(this);
        auto *hbox = new QVBoxLayout(scroll);
        this->setLayout(hbox);

        for (auto &wordSet : allWordSets) {
            std::string a = wordSet.getTitle();
            auto *cb = new QCheckBox(a.c_str(), this);
            m_checkBoxVector.push_back({cb, wordSet});
            hbox->addWidget(cb);
        }
        hbox->addStretch(1);
        auto *w = new QWidget();
        w->setLayout(hbox);
        scroll->setWidget(w);
        scroll->setGeometry(screen_width / 2 - 900, 100, 1700,
                            screen_height - 700);
    }

    void printButton() {
        auto *button = new QPushButton("Начать", this);
        button->setGeometry(screen_width / 2 - 150, screen_height - 500, 200,
                            60);
        connect(button, &QPushButton::clicked, this, &Cards::button_clicked);
    }
};

#endif  // YAFR_CARDS_H
