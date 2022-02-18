#ifndef SETTINGS_H
#define SETTINGS_H

#include "include/sectionbase.h"

class Settings:public SectionBase{
public:
   Settings(QWidget* parent = nullptr): SectionBase(parent){};
};

#endif // SETTINGS_H
