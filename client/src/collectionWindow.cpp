#include "include/collectionWindow.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QString>
#include "include/bookRep.h"
#include "include/libraryWindow.h"
#include "include/readNowWindow.h"

CollectionWindow::CollectionWindow(ConnectingWindow *parent, BookRep *bookRep)
    : QWidget(parent), bookRep(bookRep), parent(parent) {
    auto box = new QWidget;
    auto layout = new QGridLayout;

    books = bookRep->getAllBooks();
    titleLabels = std::vector<QLabel *>(books.size());
    readBtns = std::vector<QPushButton *>(books.size());

    for (int i = 0; i < books.size(); i++) {
        titleLabels[i] = new QLabel(
            QString("Name: %1,   Author: %2")
                .arg(books[i].getName().c_str(), books[i].getAuthor().c_str()));
        layout->addWidget(titleLabels[i], i, 0);

        readBtns[i] = new QPushButton(tr("Read"));
        layout->addWidget(readBtns[i], i, 1);

        readBtns[i]->setFixedWidth(100);
        readBtns[i]->setFixedHeight(50);
    }

    box->setLayout(layout);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidget(box);

    for (int i = 0; i < books.size(); i++) {
        QObject::connect(readBtns[i], &QPushButton::clicked, this,
                         [=]() { connectWithReader(books[i].getId()); });
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

void CollectionWindow::connectWithReader(int bookId) {
    std::string text = bookRep->getBookById(bookId).getText();
    std::string author = bookRep->getBookById(bookId).getAuthor();
    std::string title = bookRep->getBookById(bookId).getName();
    parent->updateReadNow(title, author, text);  // TODO path instead of text

    parent->showReadNow();
}
