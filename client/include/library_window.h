#ifndef LIBRARY_H
#define LIBRARY_H

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QString>
#include "include/book_rep.h"
#include "include/library.h"

class LibraryWindow : public QWidget {
public:

    explicit LibraryWindow(QWidget *parent = nullptr) : QWidget(parent) {
    }
    explicit LibraryWindow(DBManager &dbManager, QWidget *parent = nullptr)
        : LibraryWindow(parent) {
        BookRep bookRep(dbManager);
        Library library(bookRep.getBookPreview());
        auto box = new QWidget;
        auto layout = new QGridLayout;
        std::vector<BookPreview> bookPreviews(library.getBooks());
        std::vector<QLabel *> titleLabels(bookPreviews.size());
        std::vector<QPushButton*> readBtns(bookPreviews.size());
        for (int i = 0; i < bookPreviews.size(); i++) {
            titleLabels[i] =
                new QLabel(QString("Name: %1 \n Author: %2")
                               .arg(bookPreviews[i].getName().c_str(),
                                    bookPreviews[i].getAuthor().c_str()));
            layout->addWidget(titleLabels[i], i, 0);
            readBtns[i] = new QPushButton(tr("Read"));
            layout->addWidget(readBtns[i], i, 1);
        }
        box->setLayout(layout);

        auto scrollArea = new QScrollArea(this);
        scrollArea->setWidget(box);
        // scrollArea->setWidgetResizable(false);
    }
};

#endif  // LIBRARY_H
