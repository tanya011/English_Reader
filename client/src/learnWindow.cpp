#include "include/learnWindow.h"

LearnWindow::LearnWindow(ConnectingWindow *parent)
    : parent(parent), QWidget(parent) {
    screen_width = QApplication::desktop()->screenGeometry().width();
    screen_height = QApplication::desktop()->screenGeometry().height();

    printHeading();
    printButton();
}

void LearnWindow::setWordSets(std::vector<WordSet> ws) {
    allWordSets = ws;
    printBox();
}

void LearnWindow::button_clicked() {
    for (auto wordSetInCheck : m_checkBoxVector) {
        if (wordSetInCheck.checkBox->isChecked()) {
            checkedWordSets.push_back(wordSetInCheck.wordSet);
            // std::cout << wordSetInCheck.wordSet.getTitle() << " ";
        }
    }

    auto cardsDisplay = dynamic_cast<CardsDisplay *>(
        parent->allWindows.widget(parent->windowIndexes.cards));
    cardsDisplay->displayWords(checkedWordSets);
    parent->allWindows.setCurrentIndex(parent->windowIndexes.cards);
}

void LearnWindow::printHeading() {
    auto *text = new QLabel(this);
    text->setText("Выберите группы");
    text->setGeometry(screen_width / 2 - 200, 10, 500, 100);
    this->setStyleSheet("QLabel{font-size: 11pt;}");
}

void LearnWindow::printBox() {
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
    scroll->setGeometry(screen_width / 2 - 900, 100, 1700, screen_height - 700);
}

void LearnWindow::printButton() {
    auto *button = new QPushButton("Начать", this);
    button->setGeometry(screen_width / 2 - 150, screen_height - 500, 200, 60);
    connect(button, &QPushButton::clicked, this, &LearnWindow::button_clicked);
}
