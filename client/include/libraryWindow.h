#ifndef LIBRARY_WINDOW_H
#define LIBRARY_WINDOW_H

#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include "include/book_rep.h"
#include "include/connectingWindow.h"
#include "include/library.h"

class LibraryWindow : public QWidget {
private:
    std::vector<BookPreview> bookPreviews;
    std::vector<QLabel *> titleLabels;
    std::vector<QPushButton *> readBtns;
    BookRep bookRep;
    ConnectingWindow *parent;

public:
    LibraryWindow(ConnectingWindow *parent, DBManager &dbManager);

    void connectWithReader(int bookId);
};

#endif  // LIBRARY_WINDOW_H