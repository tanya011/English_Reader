#include "include/libraryWindow.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QString>
#include <exception>
#include "include/readNowWindow.h"

LibraryWindow::LibraryWindow(ConnectingWindow *parent) : parent_(parent) {
    box = new QWidget;
    layout = new QGridLayout;
    box->setLayout(layout);

    updateWindow();

    QPushButton *button = new QPushButton;
    button->setParent(this);
    button->setGeometry(30, 1370, 300, 70);
    button->setText("Test Update");
    button->show();
    connect(button, &QPushButton::clicked, this,
            &LibraryWindow::updateWindow);

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

void LibraryWindow::updateWindow() {

    std::vector<Book> old_books = books_;

    try {
        books_ = parent_->user->getLibraryBooks();

        for (int i = 0; i < old_books.size(); i++) {
            QLayoutItem *item;
            while ((item = layout->takeAt(0)) != nullptr) {
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
            titleLabels_[i] = new QLabel(
                    QString("Name: %1,   Author: %2")
                            .arg(books_[i].getName().c_str(), books_[i].getAuthor().c_str()));
            layout->addWidget(titleLabels_[i], i, 0);

            addBtns_[i] = new QPushButton(tr("Add"));
            layout->addWidget(addBtns_[i], i, 1);

            addBtns_[i]->setFixedWidth(100);
            addBtns_[i]->setFixedHeight(50);
        }

        scrollArea->setWidget(box);

        for (int i = 0; i < books_.size(); i++) {
            QObject::connect(addBtns_[i], &QPushButton::clicked, this,
                             [=]() { connectWithCollection(books_[i].getId()); });
        }
    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }
}


void LibraryWindow::connectWithCollection(int bookId) {
    if (parent_->user->addBookToCollection(bookId))
        parent_->updateCollection();
}