#ifndef YAFR_LIBRARYWINDOW_H
#define YAFR_LIBRARYWINDOW_H

#ifndef LIBRARY_WINDOW_H
#define LIBRARY_WINDOW_H

#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include "include/connectingWindow.h"
#include "../include/book.h"


class LibraryWindow : public QWidget {
private:
    std::vector<Book> books_;
    std::vector<QLabel *> titleLabels_;
    std::vector<QPushButton *> addBtns_;
    ConnectingWindow *parent_;


    void connectWithCollection(int bookId);

public:
    LibraryWindow(ConnectingWindow *parent);

    void updateWindow();


};

#endif  // LIBRARY_WINDOW_H


#endif  // YAFR_LIBRARYWINDOW_H
