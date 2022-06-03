#include "include/readNowWindow.h"
#include <QAction>
#include <QMainWindow>
#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <iostream>
#include <QDesktopWidget>


ReadNowWindow::ReadNowWindow(ConnectingWindow *parent) : QMainWindow(parent) {
    auto *layout = new QHBoxLayout;

    //parent->setWindowTitle("Читаю сейчас");

    screenWidth_ = QApplication::desktop()->screenGeometry().width();
    screenHeight_ = QApplication::desktop()->screenGeometry().height();

    printBook();  // выводим книгу на экран
    bookTextDisplay_->setLayout(layout);
    createWindowWithTranslate();
    translatedTextDisplay_->setLayout(layout);
    createButtonAddPhraseToDict();
    createActions();   // создаем Actions
    createToolBars();  // создаем панель
};

void ReadNowWindow::translateText() {
    if (translatedTextDisplay_) {
        translatedTextDisplay_->clear();
        selectedText_ = bookTextDisplay_->textCursor().selectedText();
        translatedText_ =
                QString::fromStdString(translate::translate(selectedText_.toStdString()));
        translatedTextDisplay_->append(translatedText_);
    }
}

void ReadNowWindow::createButtonAddPhraseToDict() {
    button_ = new QPushButton;
    button_->setParent(this);
    button_->setGeometry(screenWidth_ - 850, 660, 600, 50);
    button_->setText("Добавить в словарь");
    button_->show();
    connect(button_, &QPushButton::clicked, this,
            [=]() {
    });
}

void ReadNowWindow::createWindowWithTranslate() {
    translatedTextDisplay_ = new QTextEdit(this);
    translatedTextDisplay_->setGeometry(screenWidth_ - 900, 120, 700, 500);

    auto font = translatedTextDisplay_->font();
    //font.setBold(true);
    font.setPointSize(12);
    translatedTextDisplay_->setFont(font);
}

void ReadNowWindow::printBook(const QString &book, const QString &author, const QString &title) {
    bookTextDisplay_ = new QTextEdit(this);
    authorName_ = author;
    title_ = title;
    if (book == nullptr) {
        bookTextDisplay_->append("Select book");
    } else {
        bookTextDisplay_->append(book);
    }
    bookTextDisplay_->moveCursor(QTextCursor::Start);
    bookTextDisplay_->setReadOnly(true);
    bookTextDisplay_->setGeometry(40, 120, screenWidth_ - 1000, screenHeight_ - 350);

    auto font = bookTextDisplay_->font();
    //font.setBold(true);
    font.setPointSize(13);
    bookTextDisplay_->setFont(font);
}

void ReadNowWindow::createActions() {
    translateSelectedTextAction_ = new QAction(tr("Translate"), this);
    translateSelectedTextAction_->setShortcut(tr("Ctrl+D"));
    translateSelectedTextAction_->setStatusTip(
            tr("Фраза будет переведена при нажатии"));
    connect(translateSelectedTextAction_, &QAction::triggered, this,
            &ReadNowWindow::translateText);

    translateSelectedTextAction_->setEnabled(false);
    connect(bookTextDisplay_, &QTextEdit::copyAvailable, translateSelectedTextAction_,
            &QAction::setEnabled);

}

void ReadNowWindow::createToolBars() {
    toolBar_ = addToolBar("main toolbar");
    toolBar_->addAction(translateSelectedTextAction_);
}

[[nodiscard]] QSize ReadNowWindow::sizeHint() const {
    return {1000, 500};
}