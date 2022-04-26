#include "include/libraryWindow.h"
#include "include/readNowWindow.h"
#include "include/book_rep.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QString>
#include <QToolBar>

LibraryWindow::LibraryWindow(ConnectingWindow *parent, DBManager &dbManager)
    : QWidget(parent), bookRep(dbManager), parent(parent) {
    Library library(bookRep.getBookPreview());
    auto box = new QWidget;
    auto layout = new QGridLayout;
    bookPreviews = std::vector<BookPreview>(library.getBooks());
    titleLabels = std::vector<QLabel *>(bookPreviews.size());
    readBtns = std::vector<QPushButton *>(bookPreviews.size());
    for (int i = 0; i < bookPreviews.size(); i++) {
        titleLabels[i] =
            new QLabel(QString("Name: %1,   Author: %2")
                           .arg(bookPreviews[i].getName().c_str(),
                                bookPreviews[i].getAuthor().c_str()));
        layout->addWidget(titleLabels[i], i, 0);
        readBtns[i] = new QPushButton(tr("Read"));
        layout->addWidget(readBtns[i], i, 1);

        readBtns[i]->setFixedWidth(100);
        readBtns[i]->setFixedHeight(50);
    }

    auto screen_width =
        QApplication::desktop()->screenGeometry().width() - 1000;
    auto screen_height =
        QApplication::desktop()->screenGeometry().height() - 200;

    box->setLayout(layout);
    this->setStyleSheet("QLabel{font-size: 10pt; margin: 10px;}");
    box->setFixedWidth(screen_width - 20);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidget(box);
    scrollArea->setGeometry(400, 5, screen_width, screen_height - 3);

    for (int i = 0; i < bookPreviews.size(); i++) {
        QObject::connect(readBtns[i], &QPushButton::clicked, this, [&, i]() {
            connectWithReader(bookPreviews[i].getId());
        });
    }
}

void LibraryWindow::connectWithReader(int bookId) {
    std::string text = bookRep.getBookById(bookId).getText();
    std::string author = bookRep.getBookById(bookId).getAuthor();
    std::string title = bookRep.getBookById(bookId).getName();
    parent->updateReadNow(title, author, text);
    parent->showReadNow();
}