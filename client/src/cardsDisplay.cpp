#include "include/cardsDisplay.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <utility>

CardsDisplay::CardsDisplay(ConnectingWindow *parent) : QWidget(parent) {
}

void CardsDisplay::displayWords(std::vector<WordSet> wordSets) {
    for (auto ws : wordSets)
        for (auto w : ws.getWords())
            words.push_back(w.second);

    auto box = new QWidget(this);
    auto layout = new QVBoxLayout;
    auto *card = new QLabel();

    box->setLayout(layout);

    auto font = card->font();
    font.setBold(true);
    font.setPointSize(50);
    card->setFont(font);
    card->setAlignment(Qt::AlignHCenter);
    box->setFixedSize(QApplication::desktop()->screenGeometry().width() - 150,
                      1200);
    card->setMargin(90);
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

    QObject::connect(
        btnPrev, &QPushButton::clicked, this, [&, card, btnShowTranslation]() {
            curWord--;
            curWord %= words.size();
            card->setText(
                QString::fromStdString(words[curWord]->getOriginal()));

            btnShowTranslation->setText("Translate");
        });

    QObject::connect(
        btnNext, &QPushButton::clicked, this, [&, card, btnShowTranslation]() {
            curWord++;
            curWord %= words.size();
            card->setText(
                QString::fromStdString(words[curWord]->getOriginal()));
            btnShowTranslation->setText("Translate");
        });
    QObject::connect(
        btnShowTranslation, &QPushButton::clicked, this,
        [&, card, btnShowTranslation]() {
            if (!translationShowed) {
                btnShowTranslation->setText("Hide translation");
                card->setText(QString::fromStdString(
                    words[curWord]->getOriginal() + "\n--------------\n" +
                    words[curWord]->getTranslation()));
            } else {
                btnShowTranslation->setText("Translate");
                card->setText(
                    QString::fromStdString(words[curWord]->getOriginal()));
            }
            translationShowed = !translationShowed;
        });

    if (!words.empty()) {
        card->setText(QString::fromStdString(words[curWord]->getOriginal()));
    } else {
        card->setText("No groups selected");
        btnNext->setEnabled(false);
        btnPrev->setEnabled(false);
        btnShowTranslation->setEnabled(false);
    }
}
