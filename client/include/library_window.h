#ifndef LIBRARY_H
#define LIBRARY_H

#include <QScrollArea>
#include "sectionbase.h"

class LibraryWindow : public SectionBase {
public:
    LibraryWindow(QWidget *parent = nullptr) : SectionBase(parent) {
        QScrollArea* scrollArea = new QScrollArea;
        scrollArea
    };
};

#endif  // LIBRARY_H
