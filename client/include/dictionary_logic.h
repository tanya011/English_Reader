#ifndef DICTIONARY_LOGIC_H
#define DICTIONARY_LOGIC_H

#include "wordset.h"
#include <QObject>
#include <QTextEdit>

class DictionaryLogic : public QObject{

    Q_OBJECT



    std::map<int, Word> words;



public:
    WordSet all_words = WordSet("Все группы");
    std::map<int, WordSet> groups;

    void add_word(Word word){
        words[word.getId()] = word;
    }

    void delete_word(int wordId){
        for (auto &group: groups){
            delete_word_from_group(wordId, group.second.getId());
        }
        words.erase(wordId);
    }

    void add_word_to_group(int wordId, int groupId){
        groups[groupId].addWord(words[wordId]);
    }

    void delete_word_from_group(int wordId, int wordsetId){
        if(groups[wordsetId].checkWord(wordId)){
        groups[wordsetId].deleteWordById(wordId);
        }
    }

    int create_wordSet(std::string title){
        WordSet new_wordSet(title);
        groups[new_wordSet.getId()] = new_wordSet;
        return new_wordSet.getId();
     }

    void delete_wordSet(int wordset_Id){
        groups.erase(wordset_Id);
    }

    void add_allgroups_to_map(){
        groups[all_words.getId()] = all_words;
    }
};


#endif // DICTIONARY_LOGIC_H
