#ifndef LIBRARY_H
#define LIBRARY_H

#include "sectionbase.h"

class Library:public SectionBase{
public:
    Library(QWidget* parent = nullptr): SectionBase(parent){};
};

#endif // LIBRARY_H
