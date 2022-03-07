#ifndef SECTIONBASE_H
#define SECTIONBASE_H

#include "library_window.h"
#include "readnow.h"
#include "cards.h"

#include <QMainWindow>

namespace Ui {
class SectionBase;
}

class SectionBase : public QMainWindow
{
    Q_OBJECT

private:
    LibraryWindow libraryWindow;
    ReadNow readNow;
    Cards cards;

public:
    explicit SectionBase(QWidget *parent = nullptr);
    ~SectionBase();


    //кнопки на меню
 //   QAction* collection;
    QAction* library;
    QAction* reading_now;

    //QAction* dictionary;
    QAction* cards_action;
    /*QAction* settings;
    QAction* entrance_exit;
*/
private:
    QMenu* sections;

    Ui::SectionBase *ui; // TODO: better scoped_ptr
};

#endif // SECTIONBASE_H
