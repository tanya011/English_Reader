#ifndef DICTIONARY_LOGIC_H
#define DICTIONARY_LOGIC_H

#include "wordset.h"


class DictionaryLogic{

    std::map<int, Word> words;

    std::map<int, WordSet> groups;

public:

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


#endif // DICTIONARY_LOGIC_H
