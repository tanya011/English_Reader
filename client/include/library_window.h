#ifndef LIBRARY_H
#define LIBRARY_H

#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>

#include "sectionbase.h"

class LibraryWindow : public SectionBase {
public:
    explicit LibraryWindow(QWidget *parent = nullptr) : SectionBase(parent) {
        auto horizontalGroupBox = new QGroupBox();
        QHBoxLayout *layout = new QHBoxLayout;
        std::vector<QPushButton*> buttons(3);
        for (int i = 0; i < 3; ++i) {
            buttons[i] = new QPushButton(tr("Button %1").arg(i + 1));
            layout->addWidget(buttons[i]);
        }
        horizontalGroupBox->setLayout(layout);
        setCentralWidget(horizontalGroupBox);

        auto horizontalGroupBox2 = new QGroupBox();
        QHBoxLayout *layout2 = new QHBoxLayout;
        std::vector<QPushButton*> buttons2(5);
        for (int i = 0; i < 5; ++i) {
            buttons2[i] = new QPushButton(tr("Button %1").arg(i + 1));
            layout2->addWidget(buttons2[i]);
        }
        horizontalGroupBox2->setLayout(layout2);
        setCentralWidget(horizontalGroupBox2);
    };
};

#endif  // LIBRARY_H
