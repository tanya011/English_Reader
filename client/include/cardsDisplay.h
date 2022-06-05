#ifndef YAFR_CARDSDISPLAY_H
#define YAFR_CARDSDISPLAY_H

#include <QWidget>
#include "connectingWindow.h"
#include "../include/wordset.h"

class CardsDisplay : public QWidget {
private:
    std::vector<Word> words;
    int curWord = 0;
    bool translationShowed = false;

public:
    explicit CardsDisplay(ConnectingWindow *parent);

    void displayWords(std::vector<WordSet> wordSets);
};

#endif  // YAFR_CARDSDISPLAY_H
