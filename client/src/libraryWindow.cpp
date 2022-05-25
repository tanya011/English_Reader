#include "include/libraryWindow.h"
#include <QGridLayout>
#include <QScrollArea>

LibraryWindow::LibraryWindow(ConnectingWindow *parent) : parent_(parent) {
    updateWindow();
}
void LibraryWindow::updateWindow() {
    std::vector<Book> books = parent_->user->getLibraryBooks();

    auto box = new QWidget;
    auto layout = new QGridLayout;

    titleLabels_ = std::vector<QLabel *>(books.size());
    addBtns_ = std::vector<QPushButton *>(books.size());

    for (int i = 0; i < books.size(); i++) {
        titleLabels_[i] = new QLabel(
            QString("Name: %1,   Author: %2")
                .arg(books[i].getName().c_str(), books[i].getAuthor().c_str()));
        layout->addWidget(titleLabels_[i], i, 0);

        addBtns_[i] = new QPushButton(tr("Add"));
        layout->addWidget(addBtns_[i], i, 1);

        addBtns_[i]->setFixedWidth(100);
        addBtns_[i]->setFixedHeight(50);
    }

    box->setLayout(layout);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidget(box);

    for (int i = 0; i < books.size(); i++) {
        QObject::connect(addBtns_[i], &QPushButton::clicked, this,
                         [=]() { connectWithCollection(books[i].getId()); });
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
void LibraryWindow::connectWithCollection(int bookId) {
    parent_->user->addBookToCollection(bookId);
    parent_->updateCollection(); // TODO it don't updates
}
