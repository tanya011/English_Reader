#include "include/collectionWindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QString>
#include <cstdio>
#include <iostream>
#include "include/bookRep.h"
#include "include/readNowWindow.h"
#include "include/serverProblemsException.h"

CollectionWindow::CollectionWindow(ConnectingWindow *parent, BookRep *bookRep)
    : QWidget(parent), bookRep_(bookRep), parent_(parent) {
    updateWindow(false);
    auto *syncBtn = new QPushButton(tr("Синхронизация"));
    syncBtn->setParent(this);
    syncBtn->setGeometry(30, 1370, 300, 70);
    syncBtn->show();
    QObject::connect(syncBtn, &QPushButton::clicked, this,
                     [=]() { synchronizationCollection(); });
}

void CollectionWindow::updateWindow(bool firstUpdate) {
    if (firstUpdate) {
        for (int i = 0; i < books_.size(); i++) {
            QLayoutItem *item;
            while ((item = layout_->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item->layout();
                delete item;
            }
        }
        titleLabels_.clear();
        readBtns_.clear();
        parent_->user->syncCollection();
    }

    books_ = bookRep_->getAllBooks();
    box_ = new QWidget;
    layout_ = new QGridLayout;
    box_->setLayout(layout_);

    titleLabels_ = std::vector<QLabel *>(books_.size());
    readBtns_ = std::vector<QPushButton *>(books_.size());
    deleteBtns_ = std::vector<QPushButton *>(books_.size());

    for (int i = 0; i < books_.size(); i++) {
        titleLabels_[i] = new QLabel(QString("Name: %1,   Author: %2")
                                         .arg(books_[i].getName().c_str(),
                                              books_[i].getAuthor().c_str()));
        layout_->addWidget(titleLabels_[i], i, 0);

        readBtns_[i] = new QPushButton(tr("Read"));
        layout_->addWidget(readBtns_[i], i, 1);

        deleteBtns_[i] = new QPushButton(tr("Delete"));
        layout_->addWidget(deleteBtns_[i], i, 2);

        readBtns_[i]->setFixedWidth(100);
        readBtns_[i]->setFixedHeight(50);
        deleteBtns_[i]->setFixedWidth(100);
        deleteBtns_[i]->setFixedHeight(50);
    }

    scrollArea_->setWidget(box_);

    for (int i = 0; i < books_.size(); i++) {
        QObject::connect(readBtns_[i], &QPushButton::clicked, this,
                         [=]() { connectWithReader(books_[i].getId()); });
    }

    for (int i = 0; i < books_.size(); i++) {
        QObject::connect(deleteBtns_[i], &QPushButton::clicked, this,
                         [=]() { deleteBook(books_[i].getId()); });
    }

    // Styles
    auto screen_width =
        QApplication::desktop()->screenGeometry().width() - 1000;
    auto screen_height =
        QApplication::desktop()->screenGeometry().height() - 200;
    this->setStyleSheet("QLabel{margin: 10px;}");
    box_->setFixedWidth(screen_width - 20);
    scrollArea_->setGeometry(400, 5, screen_width, screen_height - 3);
    // Styles
}

void CollectionWindow::connectWithReader(int bookId) {
    std::string text = bookRep_->getBookById(bookId).getText();
    std::string author = bookRep_->getBookById(bookId).getAuthor();
    std::string title = bookRep_->getBookById(bookId).getName();
    parent_->updateReadNow(title, author, text);
    parent_->showReadNow();
}

void CollectionWindow::deleteBook(int bookId) {
    while (true)
        try {
            parent_->user->deleteCollectionBook(bookId);
            try {
                std::remove(
                    bookRep_->getBookById(bookId).getFileName().c_str());
            } catch (std::exception &e) {
                std::cout << e.what() << std::endl;
            }
            bookRep_->deleteBookById(bookId);
            parent_->updateCollection();
            break;
        } catch (ServerProblemsExceptionReconnect &) {
            continue;
        } catch (ServerProblemsExceptionNotDeleteInCollection &) {
            break;
        }
}

void CollectionWindow::synchronizationCollection() {
    parent_->user->syncCollection();
    parent_->updateCollection();
}