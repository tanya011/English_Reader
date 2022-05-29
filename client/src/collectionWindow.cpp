#include "include/collectionWindow.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QString>
#include <iostream>
#include <cstdio>
#include "include/bookRep.h"
#include "include/readNowWindow.h"

CollectionWindow::CollectionWindow(ConnectingWindow *parent, BookRep *bookRep)
        : QWidget(parent), bookRep_(bookRep), parent_(parent) {
    box = new QWidget;
    layout = new QGridLayout;
    box->setLayout(layout);

    books_ = bookRep->getAllBooks();
    titleLabels_ = std::vector<QLabel *>(books_.size());
    readBtns_ = std::vector<QPushButton *>(books_.size());
    deleteBtns_ = std::vector<QPushButton *> (books_.size());

    for (int i = 0; i < books_.size(); i++) {
        titleLabels_[i] = new QLabel(
                QString("Name: %1,   Author: %2")
                        .arg(books_[i].getName().c_str(), books_[i].getAuthor().c_str()));
        layout->addWidget(titleLabels_[i], i, 0);

        readBtns_[i] = new QPushButton(tr("Read"));
        layout->addWidget(readBtns_[i], i, 1);

        deleteBtns_[i] = new QPushButton(tr("Delete"));
        layout->addWidget(deleteBtns_[i], i, 2);

        readBtns_[i]->setFixedWidth(100);
        readBtns_[i]->setFixedHeight(50);

        deleteBtns_[i]->setFixedWidth(100);
        deleteBtns_[i]->setFixedHeight(50);
    }

    scrollArea->setWidget(box);

    for (int i = 0; i < books_.size(); i++) {
        QObject::connect(readBtns_[i], &QPushButton::clicked, this,
                         [=]() { connectWithReader(books_[i].getId()); });
    }

    for (int i = 0; i < books_.size(); i++) {
        QObject::connect(deleteBtns_[i], &QPushButton::clicked, this,
                         [=]() {
            deleteBook(books_[i].getId());
        });
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

    QPushButton *button = new QPushButton;
    button->setParent(this);
    button->setGeometry(30, 1370, 70, 70);
    button->setIcon(QIcon("files/sync.png"));
    button->show();
    QObject::connect(button, &QPushButton::clicked, this,
                     [=]() {
        synchronizationCollection();
                     });
}

void CollectionWindow::connectWithReader(int bookId) {
    std::string text = bookRep_->getBookById(bookId).getText();
    std::string author = bookRep_->getBookById(bookId).getAuthor();
    std::string title = bookRep_->getBookById(bookId).getName();
    parent_->updateReadNow(title, author, text);  // TODO path instead of text
    parent_->showReadNow();
}

void CollectionWindow::deleteBook(int bookId) {
    // 1. Удалить файлик из папки
    try{
        std::remove(bookRep_->getBookById(bookId).getFileName().c_str());
    }catch (...){
        std::cout << "No such file";
    }
    // 2. Удалить из локальной базы данных
    bookRep_->deleteBookById(bookId);
    // 3. Удалить с сервера
    parent_->user->deleteCollectionBook(bookId);
    // 4. обновляем окно
    parent_->updateCollection();
}

void CollectionWindow::synchronizationCollection() {
    parent_->user->sync_collect();
    parent_->updateCollection();
}