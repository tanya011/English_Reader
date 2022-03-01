#ifndef COLLECTION_H
#define COLLECTION_H

#include <QPushButton>
#include <QVBoxLayout>
#include "sectionbase.h"

class Collection: public SectionBase{
public:
    Collection(QWidget *parent = nullptr): SectionBase(parent){
        QPushButton *button1 = new QPushButton("One");
        QPushButton *button2 = new QPushButton("Two");
        QPushButton *button3 = new QPushButton("Three");
        QVBoxLayout *layout = new QVBoxLayout(parent);
        layout->addWidget(button1);
        layout->addWidget(button2);
        layout->addWidget(button3);

          };
};


#endif // COLLECTION_H
