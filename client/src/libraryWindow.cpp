#include "include/libraryWindow.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QString>
#include "include/book_rep.h"
#include "include/readNowWindow.h"

LibraryWindow::LibraryWindow(ConnectingWindow *parent/*, DBManager &dbManager*/)
    : QWidget(parent)/*, bookRep(dbManager),*/, parent(parent) {

        //////
    std::vector<BookPreview> bookPreviews;
    for(int i = 0; i < 20; i++){
        bookPreviews.emplace_back(i, &"Autor "[i], &"Title " [ i]);
    }

        ////
    //Library library(bookRep.getBookPreview());
    Library library(bookPreviews);

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

    box->setLayout(layout);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidget(box);

    for (int i = 0; i < bookPreviews.size(); i++) {
        QObject::connect(readBtns[i], &QPushButton::clicked, this,
                         [=]() { connectWithReader(bookPreviews[i].getId()); });
    }

    // Styles
    auto screen_width =
        QApplication::desktop()->screenGeometry().width() - 1000;
    auto screen_height =
        QApplication::desktop()->screenGeometry().height() - 200;
    this->setStyleSheet("QLabel{font-size: 10pt; margin: 10px;}");
    box->setFixedWidth(screen_width - 20);
    scrollArea->setGeometry(400, 5, screen_width, screen_height - 3);
    // Styles
}

void LibraryWindow::connectWithReader(int bookId) {
    std::string text = "Tfjksdkldsfldlmdsfmdsmkdfklfd";
    std::string author = bookPreviews[bookId].getAuthor();
    std::string title = bookPreviews[bookId].getName();
    parent->updateReadNow(title, author, text);
    parent->showReadNow();
}
