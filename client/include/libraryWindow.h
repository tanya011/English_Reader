#ifndef LIBRARY_WINDOW_H
#define LIBRARY_WINDOW_H

#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>
#include "include/connectingWindow.h"
#include "../include/book.h"


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
    LibraryWindow(ConnectingWindow *parent);

private slots:
    void updateWindow();

};

#endif  // LIBRARY_WINDOW_H