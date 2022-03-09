#include "include/readnow.h"
#include <QAction>
#include <QGroupBox>
#include <QIcon>
#include <QMainWindow>
#include <QToolBar>

ReadNow::ReadNow(QMainWindow *parent) : QMainWindow(parent) {
    auto *layout = new QHBoxLayout;

    screen_width = QApplication::desktop()->screenGeometry().width();
    screen_height = QApplication::desktop()->screenGeometry().height();

    printBook();  // выводим книгу на экран
    textEdit->setLayout(layout);
    printWindowWithTranslate();
    text->setLayout(layout);
    buttonPhraseToDict();
    createActions();   // создаем Actions
    createToolBars();  // создаем панель
};

void ReadNow::translateText() {
    if (text) {
        text->clear();
        QString trr = textEdit->textCursor().selectedText();
        QString answer =
            QString::fromStdString(translate::translate(trr.toStdString()));
        text->append(answer);
    }
}

void ReadNow::buttonPhraseToDict() {
    auto *button = new QPushButton;
    button->setParent(this);
    button->setGeometry(screen_width - 850, 660, 600, 50);
    button->setText("Добавить в словарь");
    button->show();
}

void ReadNow::printWindowWithTranslate() {
    text = new QTextEdit(this);
    text->setGeometry(screen_width - 900, 120, 700, 500);
}

void ReadNow::printBook(const QString &book) {
    textEdit = new QTextEdit(this);
    textEdit->append("1234");
    textEdit->setReadOnly(true);
    textEdit->setGeometry(40, 120, screen_width - 1000, screen_height - 300);
}

void ReadNow::createActions() {
    translateSelectedText = new QAction(tr("Translate"), this);
    translateSelectedText->setShortcut(tr("Ctrl+D"));
    translateSelectedText->setStatusTip(
        tr("Фраза будет переведена при нажатии"));
    connect(translateSelectedText, &QAction::triggered, this,
            &ReadNow::translateText);

    // TODO: works only when it is commented, so translate button is enabled
   // translateSelectedText->setEnabled(false);
  /*  connect(textEdit, SIGNAL(copyAvailable(bool)), translateSelectedText,
            SLOT(setEnabled(bool)));*/
}

void ReadNow::createToolBars() {
    fileToolBar = addToolBar("main toolbar");
    fileToolBar->addAction(translateSelectedText);
}
