#ifndef COLLECTION_WINDOW_H
#define COLLECTION_WINDOW_H

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include "../include/book.h"
#include "include/bookRep.h"
#include "include/connectingWindow.h"

class CollectionWindow : public QWidget {
private:
    BookRep *bookRep_;
    std::vector<Book> books_;

    std::vector<QLabel *> titleLabels_;
    std::vector<QPushButton *> readBtns_;
    std::vector<QPushButton *> deleteBtns_;
    ConnectingWindow *parent_;
    QScrollArea *scrollArea_ = new QScrollArea(this);
    QWidget *box_;
    QGridLayout *layout_ = new QGridLayout();

public:
    CollectionWindow(ConnectingWindow *parent, BookRep *bookRep);

    void connectWithReader(int bookId);

    void synchronizationCollection();

    void updateWindow(bool firstUpdate = true);

    void deleteBook(int bookId);
};

#endif  // COLLECTION_WINDOW_H