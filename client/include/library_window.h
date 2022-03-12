#ifndef LIBRARY_WINDOW_H
#define LIBRARY_WINDOW_H

#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QScrollArea>

class LibraryWindow : public QWidget {
public:
    explicit LibraryWindow(QWidget *parent = nullptr) : QWidget(parent){
        auto horizontalGroupBox = new QGroupBox("Библиотека", this);
        QHBoxLayout *layout = new QHBoxLayout;
        std::vector<QPushButton *> buttons(3);
        for (int i = 0; i < 3; ++i) {
            buttons[i] = new QPushButton(tr("Button %1").arg(i + 1));
            layout->addWidget(buttons[i]);
        }
        horizontalGroupBox->setLayout(layout);
    }
};

#endif  // LIBRARY_WINDOW_H
