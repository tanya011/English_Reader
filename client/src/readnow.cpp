#include "include/readnow.h"
#include <QAction>
#include <QIcon>
#include <QMainWindow>

ReadNow::ReadNow(QMainWindow *parent) : QMainWindow(parent) {
    auto *layout = new QHBoxLayout;

    //parent->setWindowTitle("Читаю сейчас");

    screen_width = QApplication::desktop()->screenGeometry().width();
    screen_height = QApplication::desktop()->screenGeometry().height();

    printBook();  // выводим книгу на экран
    bookText->setLayout(layout);
    printWindowWithTranslate();
    translatedText->setLayout(layout);
    buttonAddPhraseToDict();
    createActions();   // создаем Actions
    createToolBars();  // создаем панель
};

void ReadNow::translateText() {
    if (translatedText) {
        translatedText->clear();
        selectedText = bookText->textCursor().selectedText();
        QString answer =
                QString::fromStdString(translate::translate(selectedText.toStdString()));
        translatedText->append(answer);
    }
}

void ReadNow::buttonAddPhraseToDict() {
    auto *button = new QPushButton;
    button->setParent(this);
    button->setGeometry(screen_width - 850, 660, 600, 50);
    button->setText("Добавить в словарь");
    button->show();
}

void ReadNow::printWindowWithTranslate() {
    translatedText = new QTextEdit(this);
    translatedText->setGeometry(screen_width - 900, 120, 700, 500);
}

void ReadNow::printBook(const QString &book) {
    bookText = new QTextEdit(this);
    if (book == nullptr) {
        bookText->append("Select book");
    } else {
        bookText->append(book);
    }
    bookText->setReadOnly(true);
    bookText->setGeometry(40, 120, screen_width - 1000, screen_height - 300);
}

void ReadNow::createActions() {
    translateSelectedText = new QAction(tr("Translate"), this);
    translateSelectedText->setShortcut(tr("Ctrl+D"));
    translateSelectedText->setStatusTip(
            tr("Фраза будет переведена при нажатии"));
    connect(translateSelectedText, &QAction::triggered, this,
            &ReadNow::translateText);

    translateSelectedText->setEnabled(false);
    connect(bookText, &QTextEdit::copyAvailable, translateSelectedText,
            &QAction::setEnabled);

}

void ReadNow::createToolBars() {
    fileToolBar = addToolBar("main toolbar");
    fileToolBar->addAction(translateSelectedText);
}
