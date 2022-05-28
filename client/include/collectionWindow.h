#ifndef COLLECTION_WINDOW_H
#define COLLECTION_WINDOW_H

#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>
#include "include/bookRep.h"
#include "include/connectingWindow.h"
#include "../include/book.h"


class CollectionWindow : public QWidget {
private:


public:
    CollectionWindow(ConnectingWindow *parent, BookRep *bookRep);

    BookRep *bookRep_;

    void connectWithReader(int bookId);

    void synchronizationCollection();

    std::vector<Book> books_;
    std::vector<QLabel *> titleLabels_;
    std::vector<QPushButton *> readBtns_;
    std::vector<QPushButton *> deleteBtns_;
    ConnectingWindow *parent_;

    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *box;
    QGridLayout *layout = new QGridLayout();

    void deleteBook(int bookId);

};

#endif  // COLLECTION_WINDOW_H