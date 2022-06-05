#include "include/libraryWindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QString>
#include <exception>
#include "include/readNowWindow.h"
#include "include/serverProblemsException.h"

LibraryWindow::LibraryWindow(ConnectingWindow *parent) : parent_(parent) {
    box_ = new QWidget;
    layout_ = new QGridLayout;
    box_->setLayout(layout_);

    if (parent_->user->isAuthorized())
        updateWindow();

    auto *button = new QPushButton;
    button->setParent(this);
    button->setGeometry(30, 1370, 300, 70);
    button->setText("Test Update");
    button->show();
    connect(button, &QPushButton::clicked, this, &LibraryWindow::updateWindow);

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

void LibraryWindow::updateWindow() {
    std::vector<Book> old_books = books_;

    while (true) {
        try {
            books_ = parent_->user->getLibraryBooks();
            break;
        } catch (ServerProblemsExceptionReconnect &) {
            continue;
        } catch (ServerProblemsExceptionNotUploadLibrary &) {
            books_ = old_books;
            break;
        }
    }

    for (int i = 0; i < old_books.size(); i++) {
        QLayoutItem *item;
        while ((item = layout_->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item->layout();
            delete item;
        }
    }

    titleLabels_.clear();
    addBtns_.clear();

    titleLabels_ = std::vector<QLabel *>(books_.size());
    addBtns_ = std::vector<QPushButton *>(books_.size());

    for (int i = 0; i < books_.size(); i++) {
        titleLabels_[i] = new QLabel(QString("Name: %1,   Author: %2")
                                         .arg(books_[i].getName().c_str(),
                                              books_[i].getAuthor().c_str()));
        layout_->addWidget(titleLabels_[i], i, 0);

        addBtns_[i] = new QPushButton(tr("Add"));
        layout_->addWidget(addBtns_[i], i, 1);

        addBtns_[i]->setFixedWidth(100);
        addBtns_[i]->setFixedHeight(50);
    }

    scrollArea_->setWidget(box_);

    for (int i = 0; i < books_.size(); i++) {
        QObject::connect(addBtns_[i], &QPushButton::clicked, this,
                         [=]() { connectWithCollection(books_[i].getId()); });
    }
}

void LibraryWindow::connectWithCollection(int bookId) {
    while (true) {
        try {
            parent_->user->addBookToCollection(bookId);
            parent_->updateCollection();
            break;
        } catch (ServerProblemsExceptionReconnect &) {
            continue;
        } catch (ServerProblemsExceptionNotAddInCollection &) {
            break;
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
}