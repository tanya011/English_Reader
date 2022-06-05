#ifndef LIBRARY_WINDOW_H
#define LIBRARY_WINDOW_H

#include <QApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include "../include/book.h"
#include "include/connectingWindow.h"

class LibraryWindow : public QWidget {
private:
    std::vector<Book> books_;
    std::vector<QLabel *> titleLabels_;
    std::vector<QPushButton *> addBtns_;
    ConnectingWindow *parent_;
    QWidget *box;
    QGridLayout *layout = new QGridLayout();
    QScrollArea *scrollArea = new QScrollArea(this);

    void connectWithCollection(int bookId);

public:
    explicit LibraryWindow(ConnectingWindow *parent);
    void updateWindow();
};

#endif  // LIBRARY_WINDOW_H