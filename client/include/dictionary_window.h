#ifndef DICTIONARY_WINDOW_H
#define DICTIONARY_WINDOW_H


#include "sectionbase.h"
#include "dictionary_logic.h"

class Dictionary: public QWidget{

	DictionaryLogic m_logic;

public:
    Dictionary(QWidget *parent = nullptr): QWidget(parent){};
};


#endif // DICTIONARY_WINDOW_H
