#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "wordset.h"
#include "sectionbase.h"


class Dictionary: public SectionBase{

    std::map<int, Word> words;

    std::map<int, WordSet> groups;

public:

    Dictionary(QWidget *parent = nullptr): SectionBase(parent){};

    void add_word(Word &word){
        words[word.getId()] = word;
        word.kill_word();
    }

    void delete_word(int wordId){
        for (auto &group: groups){
            group.second.deleteWordById(wordId);
        }

        words.erase(wordId);
    }

    void add_word_to_group(int wordId, int groupId){
        groups[groupId].addWord(words[wordId]);
    }

    void create_wordSet(std::string &title){
        WordSet new_wordSet(title);
        groups[new_wordSet.getId()] = new_wordSet;
        new_wordSet.kill_set();
    }

    void delete_wordSet(int setId){
        groups.erase(setId);
    }
};


#endif // DICTIONARY_H
