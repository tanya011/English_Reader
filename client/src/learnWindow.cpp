#include "include/learnWindow.h"

#include <utility>
#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <iostream>
#include <vector>
#include "../include/cardsDisplay.h"

LearnWindow::LearnWindow(ConnectingWindow *parent)
        : parent_(parent), QWidget(parent) {

    screen_width_ = QApplication::desktop()->screenGeometry().width();
    screen_height_ = QApplication::desktop()->screenGeometry().height();

    printHeading(); // Заголовок: "Выберите группы"
    printButton(); // Кнопка "Начать"

}

// Вызываем из ConnectingWindow, передавая WordSet'ы
void LearnWindow::setWordSets(std::vector<WordSet> ws) {
    WordSets_ = std::move(ws);
    printBox();
}

void LearnWindow::button_clicked() {
    if (!checkedWordSets_.empty())
        checkedWordSets_ = {};
    for (auto wordSetInCheck: m_checkBoxes_) {
        if (wordSetInCheck.checkBox_->isChecked()) {
            checkedWordSets_.push_back(wordSetInCheck.wordSet_);
        }
    }

    if (parent_->windowIndexes.cards != -1) {
        auto *old = dynamic_cast<CardsDisplay *>(
                parent_->allWindows.widget(parent_->windowIndexes.cards));
        parent_->allWindows.removeWidget(
                old);
        old->setParent(nullptr); // maybe its solution
        delete old;
    }
    auto cardsDisplay = new CardsDisplay(parent_);
    cardsDisplay->displayWords(checkedWordSets_);
    parent_->windowIndexes.cards = parent_->allWindows.addWidget(cardsDisplay);
    parent_->allWindows.setCurrentIndex(parent_->windowIndexes.cards);
}

void LearnWindow::printHeading() {
    auto *text = new QLabel(this);
    text->setText("Выберите группы");
    text->setGeometry(screen_width_ / 2 - 200, 10, 500, 100);
    this->setStyleSheet("QLabel{font-size: 11pt;}");
}

void LearnWindow::printBox() {
    m_checkBoxes_ = {}; // вектор checkBox + wordSet
    auto *scroll = new QScrollArea(this);
    auto *hbox = new QVBoxLayout(scroll);
    this->setLayout(hbox);
    for (auto &wordSet: WordSets_) {
        std::string wordSetTitle = wordSet.getTitle();
        auto *cb = new QCheckBox(wordSetTitle.c_str(), this);
        m_checkBoxes_.push_back({cb, wordSet});
        hbox->addWidget(cb);
    }
    hbox->addStretch(1);
    auto *widget = new QWidget();
    widget->setLayout(hbox);
    scroll->setWidget(widget);
    scroll->setGeometry(screen_width_ / 2 - 900, 100, 1700, screen_height_ - 700);
}

void LearnWindow::printButton() {
    auto *button = new QPushButton("Начать", this);
    button->setGeometry(screen_width_ / 2 - 150, screen_height_ - 500, 200, 60);
    connect(button, &QPushButton::clicked, this, &LearnWindow::button_clicked);
}
