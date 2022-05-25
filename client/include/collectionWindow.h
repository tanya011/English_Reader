#ifndef COLLECTION_WINDOW_H
#define COLLECTION_WINDOW_H

#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include "include/bookRep.h"
#include "include/connectingWindow.h"
#include "../include/book.h"


class CollectionWindow : public QWidget {
private:
    std::vector<Book> books;
    std::vector<QLabel *> titleLabels;
    std::vector<QPushButton *> readBtns;
    BookRep* bookRep;
    ConnectingWindow *parent;

public:
    CollectionWindow(ConnectingWindow *parent, BookRep* bookRep);

    void connectWithReader(int bookId);
};

#endif  // COLLECTION_WINDOW_H