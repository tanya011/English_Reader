#ifndef SECTIONBASE_H
#define SECTIONBASE_H

#include <QMainWindow>

namespace Ui {
class SectionBase;
}

class SectionBase : public QMainWindow
{
    Q_OBJECT

public:
    explicit SectionBase(QWidget *parent = nullptr);
    ~SectionBase();


    //кнопки на меню
    QAction* collection;
    QAction* library;
    QAction* reading_now;
    QAction* dictionary;
    QAction* cards;
    QAction* settings;
    QAction* entrance_exit;

private:
    QMenu* sections;

    Ui::SectionBase *ui;
};

#endif // SECTIONBASE_H
