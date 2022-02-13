#ifndef MENUCONNECTUONS_H
#define MENUCONNECTUONS_H


#include "collection.h"
#include "library.h"
#include "readnow.h"
#include "dictionary.h"
#include "cards.h"
#include "settings.h"
#include "entrance.h"

#include <vector>
#include <QApplication>
#include <QAction>
#include <QObject>



//Реализация коннектов для каждой кнопки

void reading_now_connect(SectionBase* previous_section, SectionBase* next_section){
    QObject::connect(previous_section->reading_now, &QAction::triggered, next_section, &QMainWindow::show);
    QObject::connect(previous_section->reading_now, &QAction::triggered, previous_section, &QMainWindow::close);
}

void collection_connect(SectionBase* previous_section, SectionBase* next_section){
    QObject::connect(previous_section->collection, &QAction::triggered, next_section, &QMainWindow::show);
    QObject::connect(previous_section->collection, &QAction::triggered, previous_section, &QMainWindow::close);
}

void library_connect(SectionBase* previous_section, SectionBase* next_section){
    QObject::connect(previous_section->library, &QAction::triggered, next_section, &QMainWindow::show);
    QObject::connect(previous_section->library, &QAction::triggered, previous_section, &QMainWindow::close);
}

void dictionary_connect(SectionBase* previous_section, SectionBase* next_section){
    QObject::connect(previous_section->dictionary, &QAction::triggered, next_section, &QMainWindow::show);
    QObject::connect(previous_section->dictionary, &QAction::triggered, previous_section, &QMainWindow::close);
}

void cards_connect(SectionBase* previous_section, SectionBase* next_section){
    QObject::connect(previous_section->cards, &QAction::triggered, next_section, &QMainWindow::show);
    QObject::connect(previous_section->cards, &QAction::triggered, previous_section, &QMainWindow::close);
}

void settings_connect(SectionBase* previous_section, SectionBase* next_section){
    QObject::connect(previous_section->settings, &QAction::triggered, next_section, &QMainWindow::show);
    QObject::connect(previous_section->settings, &QAction::triggered, previous_section, &QMainWindow::close);
}


void entrance_connect(SectionBase* previous_section, SectionBase* next_section){
    QObject::connect(previous_section->entrance_exit, &QAction::triggered, next_section, &QMainWindow::show);
    QObject::connect(previous_section->entrance_exit, &QAction::triggered, previous_section, &QMainWindow::close);
}

//Итоговая обертка над всеми коннектами,if чтобы не открывать окно само в себя

void all_connections(SectionBase* cll, SectionBase* lib, SectionBase* rdn, SectionBase* dic, SectionBase* crd, SectionBase* stt, SectionBase* ent){

    std::vector<SectionBase*> windows = {cll, lib, rdn, dic, crd, stt, ent};

    for (auto &window: windows){

        if (window != cll){
            collection_connect(window, cll);
        }

        if (window != lib){
            library_connect(window, lib);
        }

        if (window != rdn){
            reading_now_connect(window, rdn);
        }

        if (window != dic){
            dictionary_connect(window, dic);
        }

        if (window != crd){
            cards_connect(window, crd);
        }

        if (window != stt){
            settings_connect(window, stt);
        }

        if (window != ent){
            entrance_connect(window, ent);
        }
    }
}

#endif // MENUCONNECTUONS_H
