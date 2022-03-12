#ifndef LIBRARY_WINDOW_H
#define LIBRARY_WINDOW_H

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QString>
#include <QToolBar>
#include "include/book_rep.h"
#include "include/library.h"
#include "readnow.h"

class LibraryWindow : public QWidget {
private:
    std::vector<BookPreview> bookPreviews;
    std::vector<QLabel *> titleLabels;
    std::vector<QPushButton *> readBtns;
    BookRep bookRep;
    bool areBtnsConnected =
            false;  // Need this to avoid calling function connectWithreader(which
    // connects btns twice)

public:
    explicit LibraryWindow(DBManager &dbManager, QWidget *parent = nullptr)
            : QWidget(parent), bookRep(dbManager) {
        Library library(bookRep.getBookPreview());
        auto box = new QWidget;
        auto layout = new QGridLayout;
        bookPreviews = std::vector<BookPreview>(library.getBooks());
        titleLabels = std::vector<QLabel *>(bookPreviews.size());
        readBtns = std::vector<QPushButton *>(bookPreviews.size());
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
        scrollArea->setGeometry(1, 1, 300, 500);

    }

    void connectWithReader(QMainWindow &parent, ReadNow &readNow) {
        if (areBtnsConnected)
            return;
        for (int i = 0; i < bookPreviews.size(); i++) {
            QObject::connect(
                    readBtns[i], &QPushButton::clicked, &parent, [&, i]() {
                        parent.takeCentralWidget();
                        parent.setWindowTitle(QString::fromStdString(bookPreviews[i].getAuthor()) + " " +
                                              QString::fromStdString(bookPreviews[i].getName()));
                        parent.setCentralWidget(&readNow);
                        QString text = QString::fromStdString(
                                bookRep.getBookById(bookPreviews[i].getId()).getText());
                        readNow.printBook(text);
                        readNow.buttonsInLibraryConnectWithReader();
                    });
        }
        areBtnsConnected = true;
    }
};

#endif  // LIBRARY_WINDOW_H