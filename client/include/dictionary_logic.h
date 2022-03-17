#ifndef DICTIONARY_LOGIC_H
#define DICTIONARY_LOGIC_H

#include "wordset.h"
#include <QObject>
#include <QTextEdit>
#include <QDebug>

class DictionaryLogic : public QObject{
    Q_OBJECT

    std::map<int, Word> words;

public:
    WordSet all_words = WordSet("Все группы");
    std::map<int, WordSet> groups;
    int add_word(std::string original, std::string translation);
    int add_word(std::string original, std::string translation, int id);
    void delete_word(int wordId);
    void add_word_to_group(int wordId, int groupId);
    void delete_word_from_group(int wordId, int wordsetId);
    int create_wordSet(std::string title);
    int create_wordSet(std::string title, int id);
    void delete_wordSet(int wordset_Id);
    void add_allgroups_to_map();
    ~DictionaryLogic();

signals:
    void group_was_created(int wordset_id, std::string title, std::map<int, Word*> new_wordset);
    void wordset_was_deleted(int wordset_id);
    void word_was_added_to_dictionary(int wordId);

public slots:
    void add_word_allgroups(int wordId);
};


#endif // DICTIONARY_LOGIC_H
