#ifndef YAFR_CARDS_DISPLAY_H
#define YAFR_CARDS_DISPLAY_H

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <utility>
#include "wordset.h"

class CardsDisplay : public QWidget {
private:
    std::vector<Word*> words;
    int curWord=0;

public:
    explicit CardsDisplay(std::vector<WordSet> wordSets,
                          QWidget *parent = nullptr)
        : QWidget(parent) {
        for(auto ws:wordSets)
            for(auto w: ws.getWords())
                words.push_back(w.second);


        auto box = new QWidget(this);
        auto layout = new QVBoxLayout;
        auto card = new QLabel(QString::fromStdString(words[curWord]->getWord()));
        box->setLayout(layout);

        auto font = card->font();
        font.setBold(true);
        font.setPointSize(50);
        card->setFont(font);
        card->setAlignment(Qt::AlignHCenter);
        card->setFixedSize(1000, 500);
        card->setMargin(100);
        layout->addWidget(card);

        auto menu = new QWidget(this);
        auto btnLayout = new QHBoxLayout;
        menu->setLayout(btnLayout);
        layout->addWidget(menu);

        auto btnPrev = new QPushButton("Previous");
        auto btnNext = new QPushButton("Next");
        auto btnShowTranslation = new QPushButton("Translation");
        btnLayout->addWidget(btnPrev);
        btnLayout->addWidget(btnShowTranslation);
        btnLayout->addWidget(btnNext);
    }
};

#endif  // YAFR_CARDS_DISPLAY_H
