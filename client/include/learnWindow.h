#ifndef YAFR_LEARNWINDOW_H
#define YAFR_LEARNWINDOW_H

#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QDesktopWidget>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <iostream>
#include <vector>
#include "cardsDisplay.h"
#include "connectingWindow.h"
#include "dictionary.h"
#include "wordset.h"

class LearnWindow : public QWidget {
public:
    explicit LearnWindow(ConnectingWindow *parent = nullptr);
    void setWordSets(std::vector<WordSet> ws);
    ConnectingWindow *parent;

private slots:
    void button_clicked();

private:
    int screen_width;
    int screen_height;

    struct WordSetsInCheckBox {
        QCheckBox *checkBox;
        WordSet wordSet;
    };
    std::vector<WordSetsInCheckBox> m_checkBoxVector;
    std::vector<WordSet> checkedWordSets;
    std::vector<WordSet> allWordSets;

    void printHeading();

    void printBox();

    void printButton();
};

#endif  // YAFR_LEARNWINDOW_H
