#ifndef DICTIONARY_H
#define DICTIONARY_H


#include "sectionbase.h"
#include "dictionary_logic.h"

class Dictionary: public SectionBase{

	DictionaryLogic m_logic;

public:
    Dictionary(QWidget *parent = nullptr): SectionBase(parent){};
};


#endif // DICTIONARY_H
