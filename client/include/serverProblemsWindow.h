#ifndef YAFR_SERVERPROBLEMSWINDOW_H
#define YAFR_SERVERPROBLEMSWINDOW_H

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
    ConnectingWindow *parent_;
    QWidget *box;
    QGridLayout *layout = new QGridLayout();

public:
    LibraryWindow(ConnectingWindow *parent) : parent_(parent) {
        box = new QWidget;
        layout = new QGridLayout;
        box->setLayout(layout);

        QPushButton *button = new QPushButton;
        button->setParent(this);
        button->setGeometry(30, 1370, 300, 70);
        button->setText("Test Update");
        button->show();

        // Styles
        auto screen_width =
                QApplication::desktop()->screenGeometry().width() - 1000;
        auto screen_height =
                QApplication::desktop()->screenGeometry().height() - 200;
        this->setStyleSheet("QLabel{font-size: 10pt; margin: 10px;}");
        box->setFixedWidth(screen_width - 20);
        // Styles

    }

};

#endif //YAFR_SERVERPROBLEMSWINDOW_H
