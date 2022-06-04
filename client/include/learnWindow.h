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
#include "../include/wordset.h"

class LearnWindow : public QWidget {
public:
    explicit LearnWindow(ConnectingWindow *parent = nullptr);

    // через эту функцию передаются WordSet'ы пользователя, сохраняем в WordSets_
    void setWordSets(std::vector<WordSet> ws);

    ConnectingWindow *parent_;

private slots:
    void button_clicked(); // нажимаем на кнопку Начать

private:
    int screen_width_;
    int screen_height_;

    struct WordSetsInCheckBox_ {
        QCheckBox *checkBox_{};
        WordSet wordSet_;
    };

    std::vector<WordSetsInCheckBox_> m_checkBoxes_; // checkBox + wordSet
    std::vector<WordSet> checkedWordSets_; // выбранные
    std::vector<WordSet> WordSets_; // WordSet`ы пользователя

    void printHeading(); // Заголовок: "Выберите группы"
    void printBox(); // отрисовка checkBox`ов с названиями WordSet`ов
    void printButton(); // Кнопка : "Начать"
};

#endif  // YAFR_LEARNWINDOW_H
