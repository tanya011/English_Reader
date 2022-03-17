#ifndef DICTIONARY_H
#define DICTIONARY_H


#include <QWidget>
#include "dictionary_logic.h"
#include <QMenu>
#include <QMenuBar>
#include <QGroupBox>
#include <QTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <fstream>

class Dictionary: public QWidget {

    QMenuBar* groups_manager = new QMenuBar(this);

    QGroupBox* words_placement = new QGroupBox(this);

    QGridLayout *layout = new QGridLayout;

    int current_open_group = 1;

    int current_word_height = 1;

    std::map<int, QAction*> group_buttons;  // нужен, чтобы можно было при создании группы добавлялась её иконка в панель выбора групп


    QMenu* groups = new QMenu("Выбор группы", groups_manager);
    QMenu* sorts = new QMenu("По дате добавления", groups_manager);

    std::vector<QPushButton *> wordBtnsDeleteAll;
    std::vector<QPushButton *> wordBtnsDeleteGroup;

public:

    Dictionary(QWidget *parent = nullptr);

    void show_group(int wordsetId);

    DictionaryLogic m_logic;

public slots:
    void add_group_to_menu(int wordset_id, const std::string& title);

    void delete_group_from_menu(int wordset_id);

    void make_word_icon(int wordId, int wordsetId, std::string text_for_visualisation);

    void remove_word_icon(int wordId, int wordsetId);

    void remove_wordsets_icons(int wordsetId);

    void showContextMenuWord();

};


#endif // DICTIONARY_H
