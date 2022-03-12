#include "include/dictionary.h"
#include <QPushButton>
#include<iostream>
#include <QDebug>
#include <QInputDialog>


void Dictionary::show_group(int wordsetId){
    //qDebug() << wordsetId;
    for (auto &previousIcon: m_logic.groups[current_open_group].getWords()){
        QLayoutItem* item;
        while ( (item = layout->takeAt(0)) != nullptr){
            delete item->widget();
            delete item;
        }
        wordBtnsDeleteAll.clear();
        wordBtnsDeleteGroup.clear();
    }
    current_open_group = wordsetId;
    wordBtnsDeleteAll.resize(m_logic.groups[wordsetId].getWords().size());
    wordBtnsDeleteGroup.resize(m_logic.groups[wordsetId].getWords().size());
    int index = 0;
    int height = 0;
    for (auto &curWord: m_logic.groups[wordsetId].getWords()) {
        std::string fullWord = curWord.second->getOriginal() + " - " +  curWord.second->getTranslation();
        auto *word = new QTextEdit(fullWord.c_str());
        word->setMaximumHeight(100);
        height += word->height();
        layout->addWidget(word, index, 0);
        wordBtnsDeleteAll[index] = new QPushButton("Удалить из словаря");
        wordBtnsDeleteGroup[index] = new QPushButton("Удалить из группы");
        QObject::connect(wordBtnsDeleteAll[index], &QPushButton::clicked, &m_logic, [=](){
            m_logic.delete_word(curWord.first);
            show_group(wordsetId);
        });
        QObject::connect(wordBtnsDeleteGroup[index], &QPushButton::clicked, &m_logic, [=](){
            m_logic.delete_word_from_group(curWord.first, wordsetId);
            show_group(wordsetId);
        });
        layout->addWidget(wordBtnsDeleteAll[index], index, 1);
        layout->addWidget(wordBtnsDeleteGroup[index], index, 2);
        index++;
    }
    current_word_height = 70 * index;
    words_placement->setGeometry({0, 10, 1850, 70 + height});
}

void Dictionary::add_group_to_menu(int wordsetId, std::string title, std::map<int,Word*> wordset){
    QAction* action = new QAction(title.c_str(), this);
    groups->addAction(action);
    QObject::connect(action, &QAction::triggered, this, [=]() {
        show_group(wordsetId);
    });
    group_buttons[wordsetId] = action;
}

void Dictionary::delete_group_from_menu(int wordset_id){
    groups->removeAction(group_buttons[wordset_id]);
    group_buttons.erase(wordset_id);
}

Dictionary::Dictionary(QWidget *parent): QWidget(parent){

    QObject::connect(&m_logic, &DictionaryLogic::group_was_created, this, &Dictionary::add_group_to_menu);
    QObject::connect(&m_logic, &DictionaryLogic::wordset_was_deleted, this, &Dictionary::delete_group_from_menu);

    groups_manager->addMenu(groups);
    groups_manager->addMenu(sorts);


    QAction* all_groups = new QAction("Все слова", groups);

    QObject::connect(all_groups, &QAction::triggered, this, [&]() {
        show_group(1);
    });

    groups->addAction(all_groups);

    m_logic.add_allgroups_to_map();

    QAction* addWordset = new QAction("Добавить группу", this);

    groups_manager->addAction(addWordset);


    QObject::connect(addWordset, &QAction::triggered, this, [&](){
        bool ok;
        QString wordsetName = QInputDialog::getText(this, tr("Новая группа"), tr("Название группы:"), QLineEdit::Normal, "", &ok);
        if (ok && !wordsetName.isEmpty())
                m_logic.create_wordSet(wordsetName.toStdString());
    });

    QObject::connect(&m_logic, &DictionaryLogic::word_was_added_to_dictionary, &m_logic, &DictionaryLogic::add_word_allgroups);

    int q = m_logic.add_word("hello", "привет");
    int w = m_logic.add_word("good", "хорошо");
    int z = m_logic.add_word("word","слово");

    int x =  m_logic.create_wordSet("Гарри Поттер");

    m_logic.add_word_to_group(q, x);
    m_logic.add_word_to_group(w, x);

    int y = m_logic.create_wordSet("Портрет Дориана Грея");

    m_logic.add_word_to_group(z, y);






    words_placement->setGeometry({0, 10, 1850, 4000});
    words_placement->setLayout(layout);

    show_group(1);
}
